# Krzysztof Zając, 279757
# Używałem Gemini Pro w celu wizulizacji danych w terminalu

using Printf

r = Float32(3.0)
p0 = Float32(0.01)
N = 40

function logistic_map(p_start::T, r_val::T, steps::Int; truncate_at::Int=-1, truncate_val::T=T(0.0)) where T<:AbstractFloat
    p = p_start
    history = T[p]
    for n in 1:steps
        if n == truncate_at + 1
            p = truncate_val
        end
        
        p = p + r_val * p * (T(1.0) - p)
        push!(history, p)
    end
    return history
end

p_10_target_val = Float32(0.722)

trajectory_f32 = logistic_map(p0, r, N)
trajectory_f32_trunc = logistic_map(p0, r, N, truncate_at=10, truncate_val=p_10_target_val)

println("\n(1) Float32 vs Float32 z obcięciem p_10 = 0.722")
@printf "%-4s | %-15s | %-15s\n" "n" "p_n (F32)" "p_n (F32 Obcięte)"
println("-"^40)
@printf "%-4d | %-15.8f | %-15.8f\n" 0 trajectory_f32[1] trajectory_f32_trunc[1]
@printf "%-4d | %-15.8f | %-15.8f\n" 10 trajectory_f32[11] trajectory_f32[11]
@printf "%-4d | %-15.8f | %-15.8f\n" 11 trajectory_f32[12] trajectory_f32_trunc[12]
@printf "%-4d | %-15.8f | %-15.8f\n" N trajectory_f32[N+1] trajectory_f32_trunc[N+1]

trajectory_f64 = logistic_map(Float64(p0), Float64(r), N)

println("\n(2) Float32 vs Float64")
@printf "%-4s | %-15s | %-15s\n" "n" "p_n (F32)" "p_n (F64)"
println("-"^40)
for n in [0, 10, 20, 30, N]
    @printf "%-4d | %-15.8f | %-15.8f\n" n trajectory_f32[n+1] trajectory_f64[n+1]
end
