# Author: Krzysztof Zając
# Zadanie 3: punkty przecięcia y = 3x oraz y = e^x, czyli pierwiastki e^x - 3x = 0,
# metodami bisekcji i Newtona (delta = epsilon = 1e-4).

include(joinpath(@__DIR__, "zad1.jl"))
using .RootFinding

setprecision(BigFloat, 256)

const TOL = 1e-4
const MAXIT = 100

f(x) = exp(x) - 3x
pf(x) = exp(x) - 3

# (root id, bisection interval, Newton start, guess for the BigFloat reference).
# f(0) > 0 > f(1) and f(1) < 0 < f(2); f' = 0 at ln 3, which lies between the roots.
specs = [(1, (0.0, 1.0), 0.0, 0.6), (2, (1.0, 2.0), 2.0, 1.5)]

rows_b = Any[]
rows_n = Any[]
for (id, (a, b), x0, guess) in specs
    rref = refroot(f, pf, guess)
    rb = mbisekcji(f, a, b, TOL, TOL, MAXIT)
    rn = mstycznych(f, pf, x0, TOL, TOL, MAXIT)
    push!(rows_b, vcat(Any[id, a, b], resultrow(rb, f, rref), Any[3 * rb[1]]))
    push!(rows_n, vcat(Any[id, x0], resultrow(rn, f, rref), Any[3 * rn[1]]))
end
printtable("zad3_bisection", vcat(["root", "a", "b"], RESULT_COLS, ["y"]), rows_b)
printtable("zad3_newton", vcat(["root", "x0"], RESULT_COLS, ["y"]), rows_n)