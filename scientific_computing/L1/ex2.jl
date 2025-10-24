function kahan_macheps(T::Type{<:AbstractFloat})
    macheps = T(3.0)*(T(4.0/3.0) - T(1.0)) - T(1.0)
    return macheps
end

println("\n--- Kahan's Machine Epsilon ---")
float_types = [Float16, Float32, Float64]
for T in float_types
    kahan_epsilon = kahan_macheps(T)
    builtin_epsilon = eps(T)
    println("Type: $(T)")
    println("  Kahan ε: $(kahan_epsilon)")
    println("  Built-in eps(): $(builtin_epsilon)")
    println("  Equality: $(kahan_epsilon == builtin_epsilon)")
end


println("\n--- Binary Representation of nextfloat(0.0) ---")
for T in float_types
    nf = nextfloat(nextfloat(zero(T)))
    println("Type: $(T)")
    println("  nextfloat(0.0): $(nf)")
    println("  Binary: ", bitstring(nf))
end