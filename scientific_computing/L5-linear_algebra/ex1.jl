include("blocksys.jl")
import .blocksys

function main()
    if length(ARGS) < 2
        println("Usage: julia task1_solve.jl <matrix_file> <vector_file_or_generate> [output_file]")
        return
    end

    matrix_file = ARGS[1]
    b_arg = ARGS[2]
    out_file = length(ARGS) >= 3 ? ARGS[3] : "result_task1.txt"

    println("Loading matrix A...")
    A = blocksys.load_matrix(matrix_file)
    n = A.n

    local b
    if b_arg == "gen"
        # Generate b = A * ones(n)
        println("Generating right-hand side b...")
        x_ones = ones(Float64, n)
        b = blocksys.multiply_matrix_vector(A, x_ones)
    else
        println("Loading vector b...")
        b = blocksys.load_vector(b_arg)
    end

    println("Solving Ax = b using Gaussian Elimination (with pivoting)...")
    # Using partial pivoting (true) as requested for robustness
    @time x = blocksys.solve_gauss(A, b, pivot=true)

    # Calculate error if exact solution known (ones)
    err = 0.0
    if b_arg == "gen"
        err = norm(x - ones(n)) / norm(ones(n))
        println("Relative Error: $err")
    end

    println("Saving solution to $out_file...")
    blocksys.write_solution(out_file, x, err)
end

main()