# Krzysztof Zając, 279757
# Używałem Gemini Pro w celu wizulizacji danych w terminalu

using Polynomials
using Printf

p=[1, -210.0, 20615.0,-1256850.0,
      53327946.0,-1672280820.0, 40171771630.0, -756111184500.0,          
      11310276995381.0, -135585182899530.0,
      1307535010540395.0,     -10142299865511450.0,
      63030812099294896.0,     -311333643161390640.0,
      1206647803780373360.0,     -3599979517947607200.0,
      8037811822645051776.0,      -12870931245150988800.0,
      13803759753640704000.0,      -8752948036761600000.0,
      2432902008176640000.0]

P = Polynomial(reverse(p))

println("\n(a) Zera wielomianu P(x) z postaci naturalnej:")
roots_P = roots(P)
sort!(roots_P, by=real)

p_prod(x) = prod(k -> (x - k), 1:20)

@printf "%-4s | %-25s | %-12s | %-12s | %-12s\n" "k" "z_k" "|z_k - k|" "|P(z_k)|" "|p(z_k)|"
println("-"^70)
for k in 1:20
    z_k = roots_P[k]
    err = abs(z_k - k)
    val_P = abs(P(z_k))
    val_p = abs(p_prod(z_k))
    @printf "%-4d | %-25s | %-12.3e | %-12.3e | %-12.3e\n" k z_k err val_P val_p
end
println("Wnioski (a): Pierwiastki 1-8 OK, 9-20 bardzo niedokładne, wiele zespolonych.")


println("\n(b) Zera wielomianu P(x) z zaburzonym a_19 = -210 - 2^-23:")
p_perturbed = copy(p)

p_perturbed[2] = -210.0 - 2.0^(-23)

P_perturbed = Polynomial(reverse(p_perturbed))
roots_P_pert = roots(P_perturbed)
sort!(roots_P_pert, by=real)

@printf "%-4s | %-25s | %-12s\n" "k" "z_k (zaburzone)" "|z_k - k|"
println("-"^50)
for k in 1:20
    z_k = roots_P_pert[k]
    err = abs(z_k - k)
    @printf "%-4d | %-25s | %-12.3e\n" k z_k err
end