# Author: Krzysztof Zając
# Zadanie 5: (a) bisekcja dla x - 10^8 - pi = 0 i błąd rzeczywisty w BigFloat vs delta;
# (b) ilorazy asymptotyczne błędów dla równania z zad. 2 (bisekcja, sieczne, Newton).

include(joinpath(@__DIR__, "zad1.jl"))
using .RootFinding

setprecision(BigFloat, 256)

# ---- (a)
f5(x) = x - 1.0e8 - pi
delta5 = 1e-13
rref5 = BigFloat(10)^8 + big(pi)
res5 = mbisekcji(f5, 99999999.0, 100000005.0, delta5, delta5, 100)
row = resultrow(res5, f5, rref5)
printtable("zad5a_bisection",
           vcat(RESULT_COLS, ["delta", "epsilon", "ulp_r", "abs_err_over_delta"]),
           [vcat(row, Any[delta5, delta5, eps(res5[1]), row[7] / delta5])])

# ---- (b)
const R_GIVEN = 1.9337537628270213
const PHI = (1 + sqrt(5)) / 2
f(x) = sin(x) - (x / 2)^2
pf(x) = cos(x) - x / 2
f2(x) = -sin(x) - 1 / 2          # f''
rbig = refroot(f, pf, R_GIVEN)

# asymptotic constant |f''(r) / (2 f'(r))|; secant: this value to the power 1/phi
cnewton = Float64(abs(f2(rbig) / (2 * pf(rbig))))
csecant = cnewton^(1 / PHI)

summary = Any[]
for tol in (1e-7, 1e-14)
    tb, tn, ts = Float64[], Float64[], Float64[]
    rb = mbisekcji(f, 1.5, 2.0, tol, tol, 100; trace=tb)
    rn = mstycznych(f, pf, 1.5, tol, tol, 100; trace=tn)
    rs = msiecznych(f, 1.0, 2.0, tol, tol, 100; trace=ts)
    convtable("zad5b_bisection_tol=$(tol)", tb, rbig, [1.0], ["ratio_p1"])
    convtable("zad5b_secant_tol=$(tol)", ts, rbig, [PHI], ["ratio_phi"])
    convtable("zad5b_newton_tol=$(tol)", tn, rbig, [2.0], ["ratio_p2"])
    for (name, tr, p, ct, res) in [("bisection", tb, 1.0, 0.5, rb),
                                   ("secant", ts, PHI, csecant, rs),
                                   ("newton", tn, 2.0, cnewton, rn)]
        nl, rl = lastvalid(tr, rbig, p)
        push!(summary, Any[name, tol, p, ct, res[3], res[4], nl, rl])
    end
end
printtable("zad5b_summary",
           ["method", "tol", "p", "theory_const", "it", "err", "n_last", "ratio_last"], summary)