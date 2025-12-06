# Krzysztof Zając, 279757
export naturalna

function naturalna(x::Vector{Float64}, fx::Vector{Float64})
    n = length(fx)
    a = zeros(Float64, n)
    a[n] = fx[n]

    for k = (n-1):-1:1
        for i = n:-1:2
            a[i] = a[i-1] - x[k] * a[i]
        end
        a[1] = fx[k] - x[k] * a[1]
    end
    
    return a
end