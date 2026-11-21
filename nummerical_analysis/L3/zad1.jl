# Author: Krzysztof Zając
# Zadanie 1: moduł RootFinding z metodami bisekcji, stycznych (Newtona) i siecznych
# do rozwiązywania równania f(x) = 0 oraz funkcjami pomocniczymi do wydruku wyników.

module RootFinding

using Printf

export mbisekcji, mstycznych, msiecznych
export refroot, resultrow, RESULT_COLS, printtable, convtable, lastvalid

# |f'(x)| or a difference quotient below this value is treated as zero (status code 2)
const DEGEN = 1e-30

"""
    mbisekcji(f, a, b, delta, epsilon, maxit; trace=nothing) -> (r, v, it, err)

Bisection method for f(x) = 0 on [a, b].

# Arguments
- `f`: function f(x).
- `a::Float64`, `b::Float64`: interval ends.
- `delta::Float64`: argument tolerance, stop when |b_n - a_n| <= delta.
- `epsilon::Float64`: value tolerance, stop when |f(c_n)| <= epsilon.
- `maxit::Int`: maximum number of iterations.
- `trace`: optional `Vector{Float64}`; every midpoint c_n is pushed to it.

# Returns
`(r, v, it, err)`: root approximation, f(r), number of iterations, status:
0 - accuracy reached, 1 - maxit exceeded, 2 - f does not change sign on [a, b].
"""
function mbisekcji(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64, maxit::Int;
                   trace::Union{Nothing,Vector{Float64}}=nothing)
    fa = f(a)
    fb = f(b)
    fa == 0.0 && return (a, fa, 0, 0)
    fb == 0.0 && return (b, fb, 0, 0)
    sign(fa) == sign(fb) && return (a, fa, 0, 2)
    c, fc = a, fa
    for it in 1:maxit
        c = a + (b - a) / 2
        fc = f(c)
        trace === nothing || push!(trace, c)
        if sign(fc) == sign(fa)
            a, fa = c, fc
        else
            b = c
        end
        if abs(fc) <= epsilon || abs(b - a) <= delta
            return (c, fc, it, 0)
        end
    end
    return (c, fc, maxit, 1)
end

"""
    mstycznych(f, pf, x0, delta, epsilon, maxit; trace=nothing) -> (r, v, it, err)

Newton's method for f(x) = 0.

# Arguments
- `f`: function f(x).
- `pf`: derivative f'(x).
- `x0::Float64`: starting point.
- `delta::Float64`: argument tolerance, stop when |x_{n+1} - x_n| <= delta.
- `epsilon::Float64`: value tolerance, stop when |f(x_{n+1})| <= epsilon.
- `maxit::Int`: maximum number of iterations.
- `trace`: optional `Vector{Float64}`; x_0 and every iterate are pushed to it.

# Returns
`(r, v, it, err)`: root approximation, f(r), number of iterations, status:
0 - accuracy reached, 1 - maxit exceeded, 2 - f'(x) ~ 0 or non-finite values.
"""
function mstycznych(f, pf, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int;
                    trace::Union{Nothing,Vector{Float64}}=nothing)
    x = x0
    fx = f(x)
    trace === nothing || push!(trace, x)
    abs(fx) <= epsilon && return (x, fx, 0, 0)
    for it in 1:maxit
        d = pf(x)
        if !isfinite(d) || !isfinite(fx) || abs(d) < DEGEN
            return (x, fx, it - 1, 2)
        end
        xn = x - fx / d
        fn = f(xn)
        trace === nothing || push!(trace, xn)
        step = abs(xn - x)
        x, fx = xn, fn
        if step <= delta || abs(fx) <= epsilon
            return (x, fx, it, 0)
        end
    end
    return (x, fx, maxit, 1)
end

"""
    msiecznych(f, x0, x1, delta, epsilon, maxit; trace=nothing) -> (r, v, it, err)

Secant method for f(x) = 0.

# Arguments
- `f`: function f(x).
- `x0::Float64`, `x1::Float64`: starting approximations.
- `delta::Float64`: argument tolerance, stop when |x_{n+1} - x_n| <= delta.
- `epsilon::Float64`: value tolerance, stop when |f(x_{n+1})| <= epsilon.
- `maxit::Int`: maximum number of iterations.
- `trace`: optional `Vector{Float64}`; x_0, x_1 and every iterate are pushed to it.

# Returns
`(r, v, it, err)`: root approximation, f(r), number of iterations, status:
0 - accuracy reached, 1 - maxit exceeded, 2 - difference quotient ~ 0 or non-finite.
"""
function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int;
                    trace::Union{Nothing,Vector{Float64}}=nothing)
    xp, xc = x0, x1
    fp, fc = f(xp), f(xc)
    if trace !== nothing
        push!(trace, xp)
        push!(trace, xc)
    end
    abs(fc) <= epsilon && return (xc, fc, 0, 0)
    for it in 1:maxit
        q = (fc - fp) / (xc - xp)
        if !isfinite(q) || abs(q) < DEGEN
            return (xc, fc, it - 1, 2)
        end
        xn = xc - fc / q
        fn = f(xn)
        trace === nothing || push!(trace, xn)
        step = abs(xn - xc)
        xp, fp = xc, fc
        xc, fc = xn, fn
        if step <= delta || abs(fc) <= epsilon
            return (xc, fc, it, 0)
        end
    end
    return (xc, fc, maxit, 1)
end

"""
    refroot(f, pf, x0; iters=30) -> BigFloat

Reference root: Newton iterations carried out in BigFloat (global precision, set by the caller
with `setprecision(BigFloat, ...)`). `f` and `pf` must accept BigFloat.

# Arguments
- `f`, `pf`: function and its derivative.
- `x0::Float64`: starting point close to the root.
- `iters::Int`: number of iterations.
"""
function refroot(f, pf, x0::Float64; iters::Int=30)
    x = BigFloat(x0)
    for _ in 1:iters
        x -= f(x) / pf(x)
    end
    return x
end

const RESULT_COLS = ["r", "r_ref", "v", "v_ref", "it", "err", "abs_err_r", "abs_err_v"]

"""
    resultrow(res, f, rref) -> Vector{Any}

Table row (columns `RESULT_COLS`) for a solver result: Float64 result next to BigFloat reference
values and the absolute errors in separate columns.

# Arguments
- `res`: tuple `(r, v, it, err)` returned by a solver.
- `f`: function f(x), must accept BigFloat (gives the reference value f(r) in BigFloat).
- `rref::BigFloat`: reference root.
"""
function resultrow(res, f, rref::BigFloat)
    r, v, it, err = res
    vref = f(BigFloat(r))
    return Any[r, rref, v, vref, it, err,
               Float64(abs(BigFloat(r) - rref)), Float64(abs(BigFloat(v) - vref))]
end

fmt(x::Float64) = @sprintf("%.17e", x)
fmt(x::BigFloat) = @sprintf("%.30e", x)
fmt(x) = string(x)

"""
    printtable(title, header, rows)

Print a table: line `# title`, comma-separated header, comma-separated rows, blank line.
Float64 is printed with 17 digits, BigFloat with 30.

# Arguments
- `title::String`: table name.
- `header::Vector{String}`: column names.
- `rows`: vector of row vectors.
"""
function printtable(title::String, header::Vector{String}, rows)
    println("# ", title)
    println(join(header, ","))
    for row in rows
        println(join(map(fmt, row), ","))
    end
    println()
end

"""
    convtable(title, trace, rref, ps, labels)

Print the error table of a convergence study: n, x_n, e_n = x_n - rref (BigFloat difference),
and ratios |e_n| / |e_{n-1}|^p for every p in `ps` (NaN for n = 0 or e_{n-1} = 0).

# Arguments
- `title::String`: table name.
- `trace::Vector{Float64}`: iterates x_n.
- `rref::BigFloat`: reference root.
- `ps::Vector{Float64}`: convergence orders p.
- `labels::Vector{String}`: ratio column names, one per p.
"""
function convtable(title::String, trace::Vector{Float64}, rref::BigFloat,
                   ps::Vector{Float64}, labels::Vector{String})
    e = [BigFloat(x) - rref for x in trace]
    rows = Any[]
    for n in eachindex(e)
        rat = Any[(n == 1 || iszero(e[n-1])) ? NaN : Float64(abs(e[n]) / abs(e[n-1])^p) for p in ps]
        push!(rows, vcat(Any[n - 1, trace[n], Float64(e[n])], rat))
    end
    printtable(title, vcat(["n", "x", "e"], labels), rows)
end

"""
    lastvalid(trace, rref, p; thr=1e-10) -> (n, ratio)

Last ratio |e_n| / |e_{n-1}|^p computed while |e_{n-1}| > `thr` (later errors are dominated by
Float64 rounding of x_n). Returns `(n, ratio)`, or `(-1, NaN)` if there is none.

# Arguments
- `trace::Vector{Float64}`: iterates x_n.
- `rref::BigFloat`: reference root.
- `p::Float64`: convergence order.
- `thr::Float64`: error threshold.
"""
function lastvalid(trace::Vector{Float64}, rref::BigFloat, p::Float64; thr::Float64=1e-10)
    e = [BigFloat(x) - rref for x in trace]
    nl, rl = -1, NaN
    for n in 2:length(e)
        if abs(e[n-1]) > thr
            nl = n - 1
            rl = Float64(abs(e[n]) / abs(e[n-1])^p)
        end
    end
    return (nl, rl)
end

end # module

using .RootFinding

"""
    selftest()

Check the three solvers on x^2 - 2 and verify all status codes.
"""
function selftest()
    setprecision(BigFloat, 256)
    f(x) = x^2 - 2
    pf(x) = 2x
    rref = sqrt(BigFloat(2))
    rows = Any[]
    push!(rows, vcat(Any["bisection"], resultrow(mbisekcji(f, 1.0, 2.0, 1e-12, 1e-12, 100), f, rref)))
    push!(rows, vcat(Any["newton"], resultrow(mstycznych(f, pf, 1.0, 1e-12, 1e-12, 100), f, rref)))
    push!(rows, vcat(Any["secant"], resultrow(msiecznych(f, 1.0, 2.0, 1e-12, 1e-12, 100), f, rref)))
    printtable("zad1_selftest_results", vcat(["method"], RESULT_COLS), rows)

    status = Any[
        ["bisection_ok", 0, mbisekcji(f, 1.0, 2.0, 1e-12, 1e-12, 100)[4]],
        ["bisection_maxit", 1, mbisekcji(f, 1.0, 2.0, 1e-12, 1e-12, 3)[4]],
        ["bisection_no_sign_change", 2, mbisekcji(f, 2.0, 3.0, 1e-12, 1e-12, 100)[4]],
        ["newton_ok", 0, mstycznych(f, pf, 1.0, 1e-12, 1e-12, 100)[4]],
        ["newton_maxit", 1, mstycznych(f, pf, 1.0, 1e-12, 1e-12, 2)[4]],
        ["newton_zero_derivative", 2, mstycznych(f, pf, 0.0, 1e-12, 1e-12, 100)[4]],
        ["secant_ok", 0, msiecznych(f, 1.0, 2.0, 1e-12, 1e-12, 100)[4]],
        ["secant_maxit", 1, msiecznych(f, 1.0, 2.0, 1e-12, 1e-12, 2)[4]],
        ["secant_zero_quotient", 2, msiecznych(f, 1.0, 1.0, 1e-12, 1e-12, 100)[4]],
    ]
    printtable("zad1_selftest_status", ["case", "expected", "got"], status)
end

if abspath(PROGRAM_FILE) == @__FILE__
    selftest()
end