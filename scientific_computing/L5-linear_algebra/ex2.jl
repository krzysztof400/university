include("blocksys.jl")
import .blocksys

function main()
    if length(ARGS) < 2
        println("Usage: julia task2_lu.jl <matrix_file> <vector_file_or_generate> [output_file]")
        return
    end

    matrix_file = ARGS[1]
    b_arg = ARGS[2]
    out_file = length(ARGS) >= 3 ? ARGS[3] : "result_task2.txt"

    println("Loading matrix A...")
    A = blocksys.load_matrix(matrix_file)
    n = A.n

    local b
    if b_arg == "gen"
        println("Generating right-hand side b...")
        x_ones = ones(Float64, n)
        b = blocksys.multiply_matrix_vector(A, x_ones)
    else
        println("Loading vector b...")
        b = blocksys.load_vector(b_arg)
    end

    println("Computing LU Decomposition...")
    @time LU, pivots = blocksys.compute_lu(A, pivot=true)

    println("Solving Ax = b using LU factors...")
    @time x = blocksys.solve_lu(LU, pivots, b)

    err = 0.0
    if b_arg == "gen"
        err = norm(x - ones(n)) / norm(ones(n))
        println("Relative Error: $err")
    end

    println("Saving solution to $out_file...")
    blocksys.write_solution(out_file, x, err)
end

main()