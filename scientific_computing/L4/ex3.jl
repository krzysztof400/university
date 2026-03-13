# Krzysztof Zając, 279757
export naturalna

function naturalna(x::Vector{Float64}, c::Vector{Float64})
    n = length(c)
    a = zeros(Float64, n)
    a[n] = c[n]

    for k = (n-1):-1:1
        a[k] = c[k]
        for i = k:(n-1)
            a[i] = a[i] - x[k] * a[i+1]
        end
    end
    return a
end