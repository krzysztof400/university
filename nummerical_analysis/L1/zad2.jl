# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 2: rozmieszczenie liczb Float64.

"""
    check_spacing(a, steps, delta)

Checks that the `steps` successors of `a` are spaced by `delta` and equal a + k*delta.
Parameters: a - start (Float64), steps - number of checked points, delta - expected step.
"""
function check_spacing(a::Float64, steps::Int, delta::Float64)
    x = a
    for k in 1:steps
        y = nextfloat(x)
        y - x == delta || return false
        y == a + k * delta || return false
        x = y
    end
    return true
end

delta = 2.0^-52

println("[1,2],   delta=2^-52 (first 10^6 numbers): ", check_spacing(1.0, 10^6, delta))
println("[1,2],   last numbers: ",
        check_spacing(prevfloat(2.0, 1000), 999, delta))
println("[1/2,1], delta=2^-53: ", check_spacing(0.5, 10^6, 2.0^-53))
println("[2,4],   delta=2^-51: ", check_spacing(2.0, 10^6, 2.0^-51))

# Random samples from [1,2): x = 1 + k*delta, integer k in 1..2^52-1
let ok = true
    for _ in 1:10^5
        x = 1 + rand()
        k = (x - 1) / delta
        ok &= (k == round(k)) && 1 <= k <= 2^52 - 1
    end
    println("random x in [1,2) has x = 1 + k*delta, integer k: ", ok)
end

# Bit layout: sign | exponent (11) | mantissa (52)
println("\nbitstrings:")
for x in (0.5, nextfloat(0.5), 1.0, nextfloat(1.0), 1.0 + 2 * delta, prevfloat(2.0), 2.0, nextfloat(2.0), 4.0)
    s = bitstring(x)
    println(rpad(string(x), 24), s[1], " ", s[2:12], " ", s[13:end])
end
