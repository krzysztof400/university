# Author: Krzysztof Zając
# Zadanie 4: miejsca zerowe f1(x) = e^(1-x) - 1 oraz f2(x) = x e^(-x) (delta = epsilon = 1e-5);
# zachowanie metody Newtona dla x0 > 1 (f1, f2) oraz x0 = 1 (f2); dane do wykresów.

include(joinpath(@__DIR__, "zad1.jl"))
using .RootFinding

setprecision(BigFloat, 256)

const TOL = 1e-5
const MAXIT = 1000

f1(x) = exp(1 - x) - 1
pf1(x) = -exp(1 - x)
f2(x) = x * exp(-x)
pf2(x) = (1 - x) * exp(-x)

# (name, f, f', exact root, bisection interval, Newton x0, secant starts)
cases = [
    ("f1", f1, pf1, BigFloat(1), (0.0, 3.0), 0.5, (0.0, 0.5)),
    ("f2", f2, pf2, BigFloat(0), (-1.0, 1.0), -0.5, (-1.0, -0.5)),
]
rows = Any[]
for (name, f, pf, rref, (a, b), x0, (s0, s1)) in cases
    push!(rows, vcat(Any[name, "bisection", a, b],
                     resultrow(mbisekcji(f, a, b, TOL, TOL, MAXIT), f, rref)))
    push!(rows, vcat(Any[name, "newton", x0, NaN],
                     resultrow(mstycznych(f, pf, x0, TOL, TOL, MAXIT), f, rref)))
    push!(rows, vcat(Any[name, "secant", s0, s1],
                     resultrow(msiecznych(f, s0, s1, TOL, TOL, MAXIT), f, rref)))
end
printtable("zad4_main", vcat(["func", "method", "s1", "s2"], RESULT_COLS), rows)

"""
    newton_scan(title, f, pf, rref, starts)

Print a table of Newton runs from every starting point in `starts` (column x1 = first iterate).

# Arguments
- `title::String`: table name.
- `f`, `pf`: function and derivative.
- `rref::BigFloat`: exact root.
- `starts`: starting points x0.
"""
function newton_scan(title, f, pf, rref::BigFloat, starts)
    rows = Any[]
    for x0 in starts
        tr = Float64[]
        res = mstycznych(f, pf, x0, TOL, TOL, MAXIT; trace=tr)
        x1 = length(tr) >= 2 ? tr[2] : NaN
        push!(rows, vcat(Any[x0, x1], resultrow(res, f, rref)))
    end
    printtable(title, vcat(["x0", "x1"], RESULT_COLS), rows)
end

newton_scan("zad4a_newton_f1", f1, pf1, BigFloat(1), [0.5, 1.1, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 20.0, 40.0])
newton_scan("zad4b_newton_f2", f2, pf2, BigFloat(0), [0.5, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0, 20.0])

rows = Any[]
for (name, f, pf, x0) in [("f1", f1, pf1, 3.0), ("f2", f2, pf2, 2.0)]
    tr = Float64[]
    mstycznych(f, pf, x0, TOL, TOL, 12; trace=tr)
    for (n, x) in enumerate(tr)
        push!(rows, Any[name, x0, n - 1, x, f(x)])
    end
end
printtable("zad4_trace", ["func", "x0", "n", "x", "fx"], rows)

rows = Any[[x, f1(x), f2(x)] for x in -1.0:0.05:6.0]
printtable("zad4_plotdata", ["x", "f1", "f2"], rows)