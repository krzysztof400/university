include("blocksys.jl")

function print_blocks(M::blocksys.BlockMatrix)
    println("\nAk")
    for (i, mat) in enumerate(M.Ak)
        println("A[$i]:")
        display(mat)
    end
    
    if !isempty(M.Bk)
        println("\nBk")
        for (i, mat) in enumerate(M.Bk)
            println("B[$i]:")
            display(mat)
        end
    end

    if !isempty(M.Ck)
        println("\nCk")
        for (i, mat) in enumerate(M.Ck)
            println("C[$i]:")
            display(mat)
        end
    end
end

function main()
    A = blocksys.load_matrix("A.txt")
    b = blocksys.load_vector("b.txt")

    LU, pivot = blocksys.compute_lu(A)

    print_blocks(LU)

    
end

main()