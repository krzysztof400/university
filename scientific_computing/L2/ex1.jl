# Krzysztof Zając, 279757

using Printf

x1_vec = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
y1_vec = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]
exact_val = -1.00657107000000e-11

x2_vec = [2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995]
y2_vec = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]

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


println("\n Stare Dane")

println("\n--- Float64 ---")
prods64 = x1_vec .* y1_vec
println(" (a) W przód:    $(dot_forward(x1_vec, y1_vec))")
println(" (b) W tył:      $(dot_backward(x1_vec, y1_vec))")
println(" (c) Sort najw->najmn: $(dot_sorted(x1_vec, y1_vec, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x1_vec, y1_vec, rev=false))")

println("\n--- Float32 ---")
x32 = Float32.(x1_vec)
y32 = Float32.(y1_vec)
prods32 = x32 .* y32
println(" (a) W przód:    $(dot_forward(x32, y32))")
println(" (b) W tył:      $(dot_backward(x32, y32))")
println(" (c) Sort najw->najmn: $(dot_sorted(x32, y32, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x32, y32, rev=false))")

println("\n Wartość dokładna: $exact_val")


println("\n Nowe dane")

println("\n--- Float64 ---")
prods64 = x2_vec .* y2_vec
println(" (a) W przód:    $(dot_forward(x2_vec, y2_vec))")
println(" (b) W tył:      $(dot_backward(x2_vec, y2_vec))")
println(" (c) Sort najw->najmn: $(dot_sorted(x2_vec, y2_vec, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x2_vec, y2_vec, rev=false))")

println("\n--- Float32 ---")
x32 = Float32.(x2_vec)
y32 = Float32.(y2_vec)
prods32 = x32 .* y32
println(" (a) W przód:    $(dot_forward(x32, y32))")
println(" (b) W tył:      $(dot_backward(x32, y32))")
println(" (c) Sort najw->najmn: $(dot_sorted(x32, y32, rev=true))")
println(" (d) Sort najmn->najw: $(dot_sorted(x32, y32, rev=false))")
