# Author: Krzysztof Zając
# Zadanie 2: pierwiastek równania sin x - (x/2)^2 = 0 metodami bisekcji, Newtona i siecznych
# (delta = epsilon = 0.5e-5); porównanie liczby iteracji i błędów.

include(joinpath(@__DIR__, "zad1.jl"))
using .RootFinding

setprecision(BigFloat, 256)

const R_GIVEN = 1.9337537628270213  # root given in the task
const TOL = 0.5e-5
const MAXIT = 100

f(x) = sin(x) - (x / 2)^2
pf(x) = cos(x) - x / 2

rbig = refroot(f, pf, R_GIVEN)
printtable("zad2_reference", ["r_given", "r_big", "abs_diff"],
           [Any[R_GIVEN, rbig, Float64(abs(BigFloat(R_GIVEN) - rbig))]])

results = [
    ("bisection", mbisekcji(f, 1.5, 2.0, TOL, TOL, MAXIT)),
    ("newton", mstycznych(f, pf, 1.5, TOL, TOL, MAXIT)),
    ("secant", msiecznych(f, 1.0, 2.0, TOL, TOL, MAXIT)),
]
rows = Any[]
for (name, res) in results
    push!(rows, vcat(Any[name], resultrow(res, f, rbig), Any[abs(res[1] - R_GIVEN)]))
end
printtable("zad2_results", vcat(["method"], RESULT_COLS, ["abs_err_given"]), rows)