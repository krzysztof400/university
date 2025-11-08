# Krzysztof Zając

using Plots

f(x) = exp(x) * log(1.0 + exp(-x))

xs = 0.0:0.1:40.0
ys = f.(xs)

for x in 30.0:0.1:40.0
    val = f(x)
    if val == 0.0
        println("f(x) staje się 0.0 dla x ~= $x")
        break
    end
end
for x in 20.0:0.1:40.0
    val = f(x)
    if abs(val - 1) > 0.01
        println("f(x) traci stabilność dla x ~= $x")
        break
    end
end

p = plot(xs, ys, 
    label="f(x) = exp(x) * log(1 + exp(-x))", 
    xlabel="x", 
    ylabel="f(x)",
    title="Zad. 2: Błąd anulowania (Float64)",
    legend=:bottomleft
)
hline!([1.0], label="Granica analityczna y = 1", linestyle=:dash, color=:red)

savefig(p, "ex2_plot_jl.png")