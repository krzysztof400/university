function calculate_machine_epsilon(T::Type{<:AbstractFloat})
    epsilon = one(T)
    while one(T) + epsilon/2 > one(T)
        epsilon /= 2
    end
    return epsilon
end

function calculate_smallest_positive_number(T::Type{<:AbstractFloat})
    eta = one(T)
    while eta/2 > zero(T)
        eta /= 2
    end

    eta_start = one(T)
    while eta_start > zero(T)
        next_eta = eta_start / 2
        if next_eta == zero(T)
            return eta_start # The last non-zero number before underflow
        end
        eta_start = next_eta
    end
    return eta_start
end

function calculate_float_max(T::Type{<:AbstractFloat})
    max_pow_2 = one(T)
    while !isinf(max_pow_2 * 2)
        max_pow_2 *= 2
    end
    
    # The largest finite number has the maximum exponent and a mantissa full of 1s (2 - 2*eps(T))
    return max_pow_2 * (2 - eps(T))
end

float_types = [Float16, Float32, Float64]

println("--- Machine Epsilon (macheps) ---")
for T in float_types
    iter_epsilon = calculate_machine_epsilon(T)
    builtin_epsilon = eps(T)

    println("Type: $(T)")
    println("  Iterative ε: $(iter_epsilon)")
    println("  Built-in eps(): $(builtin_epsilon)")
    println("  Equality: $(iter_epsilon == builtin_epsilon)")
end

println("\n--- Smallest Positive Subnormal Number (η / MINsub) ---")
for T in float_types
    builtin_eta = nextfloat(zero(T))
    
    iter_eta = calculate_smallest_positive_number(T)
    
    println("Type: $(T)")
    println("  Iterative η: $(iter_eta)")
    println("  Built-in nextfloat(0.0): $(builtin_eta)")
    println("  Equality: $(iter_eta == builtin_eta)")
end

println("\n--- Maximum Representable Number (MAX) ---")
for T in float_types
    builtin_max = floatmax(T)
    
    iter_max = calculate_float_max(T)
    
    println("Type: $(T)")
    println("  Iterative MAX: $(iter_max)")
    println("  Built-in floatmax(): $(builtin_max)")
    println("  Equality: $(iter_max == builtin_max)")
end