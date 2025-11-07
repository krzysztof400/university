# Krzysztof Zając, 279757

using Printf

x_vec = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
y_vec = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]
exact_val = -1.00657107000000e-11

function dot_forward(x, y)
    S = zero(eltype(x))
    for i in 1:length(x)
        S += x[i] * y[i]
    end
    return S
end

function dot_backward(x, y)
    S = zero(eltype(x))
    for i in length(x):-1:1
        S += x[i] * y[i]
    end
    return S
end

function dot_sorted(x, y; rev=false)
    prods = x .* y
    sort!(prods, rev=rev)
    return sum(prods)
end

println("\n--- Float64 ---")
prods64 = x_vec .* y_vec
println(" (a) W przód:    $(dot_forward(x_vec, y_vec))")
println(" (b) W tył:      $(dot_backward(x_vec, y_vec))")
println(" (c) Sort najw->najmn: $(dot_sorted(x_vec, y_vec, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x_vec, y_vec, rev=false))")

println("\n--- Float32 ---")
x32 = Float32.(x_vec)
y32 = Float32.(y_vec)
prods32 = x32 .* y32
println(" (a) W przód:    $(dot_forward(x32, y32))")
println(" (b) W tył:      $(dot_backward(x32, y32))")
println(" (c) Sort najw->najmn: $(dot_sorted(x32, y32, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x32, y32, rev=false))")

println("\n Wartość dokładna: $exact_val")
