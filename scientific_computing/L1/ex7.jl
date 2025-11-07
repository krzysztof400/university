# Krzysztof Zając, 279757

using Printf

f_z7(x) = sin(x) + cos(3x)
f_prime_exact(x) = cos(x) - 3 * sin(3x)

x0 = 1.0


exact_val = f_prime_exact(x0)
println("Wartość dokładna f'(1.0): $exact_val")

println("\n n |      h      |   f_approx'  |     Błąd     | 1+h == 1? ")
println("---|-------------|--------------|--------------|-----------")



for n in 0:54
    h = 2.0^-n
    
    
    f_approx = (f_z7(x0 + h) - f_z7(x0)) / h
    error = abs(exact_val - f_approx)
    
    check = (1.0 + h) == 1.0 
    
    @printf("%2d | %.5e | %.10f | %.10e | %s\n", n, h, f_approx, error, check)
end