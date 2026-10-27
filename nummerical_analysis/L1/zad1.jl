# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 1: macheps, eta i MAX dla Float16/32/64.

using Printf

"""
    my_macheps(T)

Machine epsilon of type `T`.
Parameters: T - floating-point type (Float16, Float32, Float64).
"""
function my_macheps(T::Type)
    e = one(T)
    while one(T) + e / T(2) > one(T)
        e = e / T(2)
    end
    return e
end

"""
    my_eta(T)

Smallest positive machine number of type `T`.
Parameters: T - floating-point type.
"""
function my_eta(T::Type)
    x = one(T)
    while x / T(2) > zero(T)
        x = x / T(2)
    end
    return x
end

"""
    my_max(T)

Largest finite number of type `T`.
Parameters: T - floating-point type.
"""
function my_max(T::Type)
    x = one(T)
    while !isinf(x * T(2))
        x = x * T(2)
    end
    m = x
    delta = x / T(2)
    while delta > zero(T) && !isinf(m + delta)
        m = m + delta
        delta = delta / T(2)
    end
    return m
end

# Values from float.h (Float16 has no C equivalent)
c_eps = Dict(Float16 => 9.765625e-4,
             Float32 => 1.19209290e-07,
             Float64 => 2.2204460492503131e-16)
c_max = Dict(Float16 => 65504.0,
             Float32 => 3.40282347e+38,
             Float64 => 1.7976931348623157e+308)

println("=== macheps ===")
for T in (Float16, Float32, Float64)
    @printf("%-8s iter=%-14.6e eps()=%-14.6e float.h=%-14.6e\n",
            T, my_macheps(T), eps(T), c_eps[T])
end

println("\n=== eta (MIN_sub) ===")
for T in (Float16, Float32, Float64)
    @printf("%-8s iter=%-14.6e nextfloat(0)=%-14.6e floatmin=%-14.6e\n",
            T, my_eta(T), nextfloat(zero(T)), floatmin(T))
end

println("\n=== MAX ===")
for T in (Float16, Float32, Float64)
    @printf("%-8s iter=%-14.6e floatmax()=%-14.6e float.h=%-14.6e\n",
            T, my_max(T), floatmax(T), c_max[T])
end
