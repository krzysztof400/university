println("\n--- Przedział [1, 2] ---")
delta_teoria_1 = 2.0^-52
delta_eksperyment_1 = eps(1.0)
println("Krok teoretyczny (2^-52): $delta_teoria_1")
println("Krok eksperymentalny (eps(1.0)): $delta_eksperyment_1")
println("1.0:         $(bitstring(1.0))")
println("nextfloat(1.0): $(bitstring(nextfloat(1.0)))")
println("prevfloat(2.0): $(bitstring(prevfloat(2.0)))")
println("2.0:         $(bitstring(2.0))")

println("\n--- Przedział [0.5, 1] ---")
delta_teoria_05 = 2.0^-53
delta_eksperyment_05 = eps(0.5)
println("Krok teoretyczny (2^-53): $delta_teoria_05")
println("Krok eksperymentalny (eps(0.5)): $delta_eksperyment_05")
println("0.5:         $(bitstring(0.5))")
println("prevfloat(1.0): $(bitstring(prevfloat(1.0)))")

println("\n--- Przedział [2, 4] ---")
delta_teoria_2 = 2.0^-51
delta_eksperyment_2 = eps(2.0)
println("Krok teoretyczny (2^-51): $delta_teoria_2")
println("Krok eksperymentalny (eps(2.0)): $delta_eksperyment_2")
println("2.0:         $(bitstring(2.0))")
println("nextfloat(2.0): $(bitstring(nextfloat(2.0)))")
println("prevfloat(4.0): $(bitstring(prevfloat(4.0)))")
