include("ex1.jl")
include("ex3.jl")

vec1 = [-1.0,0.0,1.0,2.0]
vec2 = [-1.0,0.0,-1.0,2.0]

a = ilorazyRoznicowe(vec1, vec2)
b = naturalna(vec1, a)
println(b)
