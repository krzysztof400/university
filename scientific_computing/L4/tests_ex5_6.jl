# Krzysztof Zając, 279757
include("ex4.jl")

if !isdir("plots") mkdir("plots") end
println("Generowanie wykresów...")

# ex5
f_exp(x) = exp(x)
f_sin(x) = x^2 * sin(x)

for n in [5, 10, 15]
    savefig(rysujNnfx(f_exp, 0.0, 1.0, n), "plots/z5_exp_n$n.png")
    savefig(rysujNnfx(f_sin, -1.0, 1.0, n), "plots/z5_sin_n$n.png")
end

# ex 6
f_abs(x) = abs(x)
f_runge(x) = 1.0 / (1.0 + x^2)

for n in [5, 10, 15]
    # (a) |x|
    savefig(rysujNnfx(f_abs, -1.0, 1.0, n, wezly=:rownoodlegle), "plots/z6_abs_row_n$n.png")
    savefig(rysujNnfx(f_abs, -1.0, 1.0, n, wezly=:czebyszew), "plots/z6_abs_cheb_n$n.png")
    
    # (b) Runge
    savefig(rysujNnfx(f_runge, -5.0, 5.0, n, wezly=:rownoodlegle), "plots/z6_runge_row_n$n.png")
    savefig(rysujNnfx(f_runge, -5.0, 5.0, n, wezly=:czebyszew), "plots/z6_runge_cheb_n$n.png")
end

println("Zakończono. Wykresy w folderze /plots.")