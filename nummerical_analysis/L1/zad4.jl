# Author: Krzysztof Zając
# Obliczenia naukowe, Lista 1, zad. 4: iloczyn skalarny w czterech kolejnościach.

using Printf

x64 = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
y64 = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]
exact = -1.00657107000000e-11

"""
Forward summation: sum_{i=1}^{n} x_i*y_i. T - element type; x, y - vectors.
"""
function dot_forward(x::Vector{T}, y::Vector{T}) where T
    S = zero(T)
    for i in 1:length(x)
        S += x[i] * y[i]
    end
    return S
end

"""
Backward summation: i = n downto 1.
"""
function dot_backward(x::Vector{T}, y::Vector{T}) where T
    S = zero(T)
    for i in length(x):-1:1
        S += x[i] * y[i]
    end
    return S
end

"""
Positives added from largest to smallest, negatives from most negative to largest,
then the two partial sums are added.
"""
function dot_largest_first(x::Vector{T}, y::Vector{T}) where T
    p = x .* y
    pos = sort(filter(>(0), p), rev=true)
    neg = sort(filter(<(0), p))
    sp = zero(T); for v in pos; sp += v; end
    sn = zero(T); for v in neg; sn += v; end
    return sp + sn
end

"""
Opposite order of dot_largest_first.
"""
function dot_smallest_first(x::Vector{T}, y::Vector{T}) where T
    p = x .* y
    pos = sort(filter(>(0), p))
    neg = sort(filter(<(0), p), rev=true)
    sp = zero(T); for v in pos; sp += v; end
    sn = zero(T); for v in neg; sn += v; end
    return sp + sn
end

methods = [("forward", dot_forward), ("backward", dot_backward),
           ("largest->smallest", dot_largest_first),
           ("smallest->largest", dot_smallest_first)]

@printf("exact = %.14e\n\n", exact)
for T in (Float32, Float64)
    xs = T.(x64); ys = T.(y64)
    println(T)
    for (name, f) in methods
        r = f(xs, ys)
        @printf("  %-18s %+.10e   abs.err = %.3e\n", name, r, abs(Float64(r) - exact))
    end
end
