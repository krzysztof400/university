# Author: Krzysztof Zając
# Zadanie 6: f(x) = (x-1)^3 cos x, pierwiastek 3-krotny r = 1; klasyczna metoda Newtona
# (zbieżność liniowa, C = 2/3) oraz zmodyfikowana (x - m f/f', zbieżność kwadratowa).

include(joinpath(@__DIR__, "zad1.jl"))
using .RootFinding

setprecision(BigFloat, 256)

const M = 3
const X0 = 1.2
const DELTA = 1e-10
const EPSIL = 1e-40
const MAXIT = 200
const RREF = BigFloat(1)

f(x) = (x - 1)^3 * cos(x)
pf(x) = 3 * (x - 1)^2 * cos(x) - (x - 1)^3 * sin(x)

"""
    mstycznych_mod(f, pf, x0, m, delta, epsilon, maxit; trace=nothing) -> (r, v, it, err)

Modified Newton method x_{n+1} = x_n - m f(x_n) / f'(x_n) for a root of known multiplicity m.

# Arguments
- `f`, `pf`: function and derivative.
- `x0::Float64`: starting point.
- `m::Int`: root multiplicity.
- `delta::Float64`: stop when |x_{n+1} - x_n| <= delta.
- `epsilon::Float64`: stop when |f(x_{n+1})| <= epsilon.
- `maxit::Int`: maximum number of iterations.
- `trace`: optional `Vector{Float64}`; x_0 and every iterate are pushed to it.

# Returns
`(r, v, it, err)` as in `mstycznych`.
"""
function mstycznych_mod(f, pf, x0::Float64, m::Int, delta::Float64, epsilon::Float64, maxit::Int;
                        trace::Union{Nothing,Vector{Float64}}=nothing)
    x = x0
    fx = f(x)
    trace === nothing || push!(trace, x)
    abs(fx) <= epsilon && return (x, fx, 0, 0)
    for it in 1:maxit
        d = pf(x)
        if !isfinite(d) || !isfinite(fx) || abs(d) < 1e-30
            return (x, fx, it - 1, 2)
        end
        xn = x - m * fx / d
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

tc, tm = Float64[], Float64[]
rc = mstycznych(f, pf, X0, DELTA, EPSIL, MAXIT; trace=tc)
rm = mstycznych_mod(f, pf, X0, M, DELTA, EPSIL, MAXIT; trace=tm)

labels = ["ratio_p1", "ratio_p2"]
convtable("zad6a_newton_classic", tc, RREF, [1.0, 2.0], labels)
convtable("zad6b_newton_modified", tm, RREF, [1.0, 2.0], labels)

c_classic = 1 - 1 / M
c_mod = Float64(abs(-sin(RREF) / (M * cos(RREF))))   # |g'(r) / (m g(r))|, g = cos
n1, r1 = lastvalid(tc, RREF, 1.0)
n2, r2 = lastvalid(tm, RREF, 2.0)
printtable("zad6_summary", ["method", "p", "theory_const", "it", "err", "n_last", "ratio_last"],
           [Any["classic", 1.0, c_classic, rc[3], rc[4], n1, r1],
            Any["modified", 2.0, c_mod, rm[3], rm[4], n2, r2]])