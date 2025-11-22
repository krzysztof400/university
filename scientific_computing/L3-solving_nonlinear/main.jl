# Krzysztof Zając, 279757

include("solvers.jl")
using .Solvers

println("--- ZADANIE 4 ---")
f4(x) = sin(x) - (0.5 * x)^2
pf4(x) = cos(x) - 0.5 * x

delta = 0.5 * 10^-5
epsilon = 0.5 * 10^-5

res_bis = mbisekcji(f4, 1.5, 2.0, delta, epsilon)
println("Bisekcja: ", res_bis)

res_newt = mstycznych(f4, pf4, 1.5, delta, epsilon, 100)
println("Newton: ", res_newt)

res_siecz = msiecznych(f4, 1.0, 2.0, delta, epsilon, 100)
println("Sieczne: ", res_siecz)

println("\n--- ZADANIE 5 ---")
f5(x) = 3x - exp(x)
delta5 = 10^-4
epsilon5 = 10^-4

res_5a = mbisekcji(f5, 0.0, 1.0, delta5, epsilon5)
println("Przecięcie 1 (0, 1]: ", res_5a)

res_5b = mbisekcji(f5, 1.0, 2.0, delta5, epsilon5)
println("Przecięcie 2 [1, 2]: ", res_5b)

println("\n--- ZADANIE 6 ---")
f6a(x) = exp(1 - x) - 1
pf6a(x) = -exp(1 - x)
f6b(x) = x * exp(-x)
pf6b(x) = exp(-x) - x * exp(-x)

delta6 = 10^-5
epsilon6 = 10^-5

println("f1(x) = e^(1-x) - 1")
println("Bisekcja [-0.5, 1.5]: ", mbisekcji(f6a, -0.5, 1.5, delta6, epsilon6))
println("Newton x0=0.5: ", mstycznych(f6a, pf6a, 0.5, delta6, epsilon6, 20))
println("Newton x0=5.0: ", mstycznych(f6a, pf6a, 5.0, delta6, epsilon6, 20))
println("Sieczne x0=0, x1=2: ", msiecznych(f6a, 0.0, 2.0, delta6, epsilon6, 20))

println("\nf2(x) = x*e^(-x)")
println("Bisekcja [-0.5, 0.5]: ", mbisekcji(f6b, -0.5, 0.5, delta6, epsilon6))
println("Newton x0=0.5: ", mstycznych(f6b, pf6b, 0.5, delta6, epsilon6, 20))
println("Newton x0=1.0: ", mstycznych(f6b, pf6b, 1.0, delta6, epsilon6, 20))
println("Sieczne x0=-0.5, x1=0.5: ", msiecznych(f6b, -0.5, 0.5, delta6, epsilon6, 20))