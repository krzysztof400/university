# Krzysztof Zając, 279757
# Używałem Gemini Pro w celu wizualizacji danych w terminalu

using LinearAlgebra
using Printf

function hilb(n::Int)
# Function generates the Hilbert matrix  A of size n,
#  A (i, j) = 1 / (i + j - 1)
# Inputs:
#	n: size of matrix A, n>=1
#
#
# Usage: hilb(10)
#
# Pawel Zielinski
        if n < 1
         error("size n should be >= 1")
        end
        return [1.0 / (i + j - 1) for i in 1:n, j in 1:n]
end

function matcond(n::Int, c::Float64)
# Function generates a random square matrix A of size n with
# a given condition number c.
# Inputs:
#	n: size of matrix A, n>1
#	c: condition of matrix A, c>= 1.0
#
# Usage: matcond(10, 100.0)
#
# Pawel Zielinski
        if n < 2
         error("size n should be > 1")
        end
        if c < 1.0
         error("condition number  c of a matrix  should be >= 1.0")
        end
        (U,S,V)=svd(randn(n,n)) # Używamy randn dla lepszej losowości
        return U*diagm(0 =>[LinRange(1.0,c,n);])*V'
end

function run_experiment(A::Matrix{Float64})
    n = size(A, 1)
    x_exact = ones(Float64, n)
    b = A * x_exact
    
    # Metoda 1: Eliminacja Gaussa (stabilna)
    x_gauss = A \ b
    err_gauss = norm(x_gauss - x_exact) / norm(x_exact)
    
    # Metoda 2: Macierz odwrotna (niestabilna)
    x_inv = inv(A) * b
    err_inv = norm(x_inv - x_exact) / norm(x_exact)
    
    return cond(A), rank(A), err_gauss, err_inv
end

println("\n(a) Macierze Hilberta H_n")
@printf "%-4s | %-10s | %-5s | %-12s | %-12s\n" "n" "cond(A)" "rank" "Błąd (A\\b)" "Błąd (inv(A)*b)"
println("-"^50)
for n in 2:2:20
    H = hilb(n)
    c, r, err_g, err_i = run_experiment(H)
    @printf "%-4d | %-10.3e | %-5d | %-12.5e | %-12.5e\n" n c r err_g err_i
end

println("\n(b) Macierze losowe R_n o zadanym 'c'")
conds = [1.0, 10.0, 1e3, 1e7, 1e12, 1e16]
ns = [5, 10, 20]

@printf "%-4s | %-10s | %-10s | %-5s | %-12s | %-12s\n" "n" "c (zadane)" "c (realne)" "rank" "Błąd (A\\b)" "Błąd (inv(A)*b)"
println("-"^60)
for n in ns
    for c_target in conds
        A = matcond(n, c_target) 
        
        c_real, r, err_g, err_i = run_experiment(A)
        @printf "%-4d | %-10.1e | %-10.3e | %-5d | %-12.5e | %-12.5e\n" n c_target c_real r err_g err_i
    end
end