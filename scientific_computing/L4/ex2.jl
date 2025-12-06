# Krzysztof Zając, 279757
export warNewton

function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
    n = length(fx)
    nt = fx[n] # c_n

    for k = (n-1):-1:1
        nt = fx[k] + (t - x[k]) * nt
    end
    
    return nt
end