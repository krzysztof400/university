# Krzysztof Zając, 279757
# Używałem Gemini Pro w celu wizulizacji danych w terminalu

using Printf

function quadratic_map(c::Float64, x0::Float64, steps::Int)
    x = x0
    println("Test: c = $c, x0 = $x0")
    @printf "  n= 0: %.16f\n" x
    for n in 1:steps
        x = x*x + c
        if n <= 5 || n == steps
            @printf "  n=%2d: %.16f\n" n x
        elseif n == 6
             println("  ...")
        end
    end
end

cases = [
    (-2.0, 1.0),
    (-2.0, 2.0),
    (-2.0, 1.999999999999999),
    (-1.0, 1.0),
    (-1.0, -1.0),
    (-1.0, 0.75),
    (-1.0, 0.25)
]

for (c, x0) in cases
    quadratic_map(c, x0, 40)
end
