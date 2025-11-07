# Krzysztof Zając, 279757

x_min = nextfloat(1.0)
wynik = x_min * (1.0 / x_min)

println("Najmniejsza liczba x > 1: $x_min")
println("Bitstring x_min: $(bitstring(x_min))")
println("Wynik x * (1/x): $wynik")
println("Czy wynik == 1.0? $(wynik == 1.0)")
println("Poprzednik 1.0: $(prevfloat(1.0))")
