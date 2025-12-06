# Krysztof Zając, 279757
export ilorazyRoznicowe

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    n = length(f)
    fx = copy(f)
    
    for j = 2:n
        for i = n:-1:j
            fx[i] = (fx[i] - fx[i-1]) / (x[i] - x[i-j+1])
        end
    end
    return fx
end