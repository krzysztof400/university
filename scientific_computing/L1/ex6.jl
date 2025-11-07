# Krzysztof Zając, 279757

using Printf
f(x) = sqrt(x^2 + 1) - 1

g(x) = x^2 / (sqrt(x^2 + 1) + 1)

println(" n |     x     |       f(x)       |       g(x)       ")
println("---|-------------|------------------|------------------")
for n in 1:12
    x = 8.0^-n
    @printf("%2d | %.3e | %.12e | %.12e\n", n, x, f(x), g(x))
end
