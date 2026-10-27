# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 6: pochodna f(x) = sin x + cos 3x w x0 = 1,
# h = 2^-n, n = 0..54 (Float64).

using Printf

f(x)  = sin(x) + cos(3x)
fp(x) = cos(x) - 3sin(3x)   # exact derivative

"""
    approx_derivative(f, x0, h)

Forward difference (f(x0+h) - f(x0)) / h.
Parameters: f - function, x0 - point, h - step.
"""
approx_derivative(f, x0::Float64, h::Float64) = (f(x0 + h) - f(x0)) / h

x0 = 1.0
exact = fp(x0)
@printf("exact f'(1) = %.16f\n\n", exact)
@printf("%3s %12s %20s %12s %20s\n", "n", "h", "approx", "error", "1+h")
for n in 0:54
    h = 2.0^(-n)
    d = approx_derivative(f, x0, h)
    @printf("%3d %12.4e %20.16f %12.4e %20.16f\n", n, h, d, abs(exact - d), 1.0 + h)
end
