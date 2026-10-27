# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 5: f(x) = sqrt(x^2+1) - 1 vs g(x) = x^2/(sqrt(x^2+1)+1),
# x = 8^-k (Float64); BigFloat jako wartość odniesienia.

using Printf

f(x) = sqrt(x^2 + 1) - 1
g(x) = x^2 / (sqrt(x^2 + 1) + 1)

setprecision(BigFloat, 512)
fbig(x) = x^2 / (sqrt(x^2 + 1) + 1)   # reference value in 512-bit precision

@printf("%3s %12s %22s %22s %22s %10s %10s\n",
        "k", "x", "f(x)", "g(x)", "reference", "rel.f", "rel.g")
for k in 1:200
    x = 8.0^(-k)
    r = fbig(BigFloat(x))
    rf = abs((BigFloat(f(x)) - r) / r)
    rg = abs((BigFloat(g(x)) - r) / r)
    @printf("%3d %12.4e %22.15e %22.15e %22.15e %10.2e %10.2e\n",
            k, x, f(x), g(x), Float64(r), Float64(rf), Float64(rg))
    g(x) == 0.0 && break
end
