# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 3: x z (1,2) takie, że x*(1/x) != 1.

"""
    find_bad_x(start, stop)

Walks through consecutive Float64 numbers from `start` (exclusive) up to `stop`
and returns the first x with x*(1/x) != 1.0, or nothing.
Parameters: start, stop - Float64 interval bounds.
"""
function find_bad_x(start::Float64, stop::Float64)
    x = nextfloat(start)
    while x < stop
        if x * (1.0 / x) != 1.0
            return x
        end
        x = nextfloat(x)
    end
    return nothing
end

# Scanning upward from 1 gives the smallest such x, which also answers (a)
x = find_bad_x(1.0, 2.0)
println("smallest x in (1,2) with x*(1/x) != 1: ", x)
println("bits: ", bitstring(x))
println("x*(1/x) = ", x * (1.0 / x), "   (1 - that = ", 1.0 - x * (1.0 / x), ")")
