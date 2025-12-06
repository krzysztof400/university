# Krzysztof Zając, 279757
using Plots
include("ex.jl")
include("ex2.jl")
export rysujNnfx


function rysujNnfx(f, a::Float64, b::Float64, n::Int; wezly::Symbol=:rownoodlegle)
    x_nodes = Vector{Float64}(undef, n+1)
    if wezly == :rownoodlegle
        h = (b - a) / n
        for k = 0:n
            x_nodes[k+1] = a + k * h
        end
    elseif wezly == :czebyszew
        for k = 1:(n+1)
            x_nodes[k] = 0.5 * (a + b) + 0.5 * (b - a) * cos((2*k - 1) * pi / (2 * (n + 1)))
        end
    else
        error("Błędny typ węzłów")
    end
    
    y_nodes = f.(x_nodes)
    diffs = ilorazyRoznicowe(x_nodes, y_nodes)
    
    plot_x = range(a, b, length=1000)
    plot_f = f.(plot_x)
    plot_poly = [warNewton(x_nodes, diffs, val) for val in plot_x]
    
    plt = plot(plot_x, plot_f, label="f(x)", lw=2, title="n=$n ($wezly)")
    plot!(plt, plot_x, plot_poly, label="N_n(x)", linestyle=:dash, lw=2)
    scatter!(plt, x_nodes, y_nodes, label="Węzły", mc=:red, ms=3)
    
    return plt
end