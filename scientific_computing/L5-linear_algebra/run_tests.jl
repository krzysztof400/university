include("matrixgen.jl")
include("blocksys.jl")

using .matrixgen
using .blocksys
using LinearAlgebra
using Statistics
using Printf
using Plots

function run_experiments()
    sizes_err = [100, 1000, 5000]
    ck_values = [1.0, 10.0, 1000.0]
    block_size = 4
    temp_file = "temp_matrix_A.txt"
    
    println("========================================================================")
    println("  CZEŚĆ 1: TESTY POPRAWNOŚCI (Błąd względny)")
    println("========================================================================")
    println(@sprintf("%-8s %-6s %-10s %-15s %-15s", "n", "l", "cond(Ak)", "Err Gauss", "Err LU"))
    println("------------------------------------------------------------------------")

    for n in sizes_err
        if n % block_size != 0 continue end
        for ck in ck_values
            try
                matrixgen.blockmat(n, block_size, Float64(ck), temp_file)
                A = blocksys.load_matrix(temp_file)
                
                x_exact = ones(Float64, n)
                b = blocksys.multiply_matrix_vector(A, x_exact)
                
                # Gauss
                x_gauss = blocksys.solve_gauss(A, b, pivot=true)
                err_gauss = norm(x_gauss - x_exact) / norm(x_exact)
                
                # LU
                # Wczytujemy ponownie, aby mieć czystą macierz
                A_lu = blocksys.load_matrix(temp_file)
                M_lu, pivots = blocksys.compute_lu(A_lu, pivot=true)
                x_lu = blocksys.solve_lu(M_lu, pivots, b)
                err_lu = norm(x_lu - x_exact) / norm(x_exact)
                
                println(@sprintf("%-8d %-6d %-10.1e %-15.4e %-15.4e", 
                        n, block_size, ck, err_gauss, err_lu))
            catch e
                println("Błąd dla n=$n, ck=$ck: $e")
            end
        end
    end

    println("\n========================================================================")
    println("  CZEŚĆ 2: TESTY WYDAJNOŚCI (Generowanie wykresu)")
    println("========================================================================")
    
    sizes_time = [1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000, 1024000]
    cond_time = 1.0

    num_repeats = 5

    times_gauss_mean = Float64[]
    times_gauss_std = Float64[]
    times_lu_mean = Float64[]
    times_lu_std = Float64[]
    valid_sizes = Int[]

    for n in sizes_time
        if n % block_size != 0 continue end

        println("Mierzenie czasu (n = $n), powtórzeń = $num_repeats ...")

        matrixgen.blockmat(n, block_size, cond_time, temp_file)

        gauss_runs = Float64[]
        lu_runs = Float64[]

        for rep in 1:num_repeats
            A = blocksys.load_matrix(temp_file)
            x_exact = ones(Float64, n)
            b = blocksys.multiply_matrix_vector(A, x_exact)

            GC.gc(); sleep(0.01)
            t_g = @elapsed blocksys.solve_gauss(A, b, pivot=true)
            push!(gauss_runs, t_g)

            A_lu = blocksys.load_matrix(temp_file)
            GC.gc(); sleep(0.01)
            t_l = @elapsed begin
                M, p = blocksys.compute_lu(A_lu, pivot=true)
                blocksys.solve_lu(M, p, b)
            end
            push!(lu_runs, t_l)
        end

        push!(valid_sizes, n)
        push!(times_gauss_mean, mean(gauss_runs))
        push!(times_gauss_std, std(gauss_runs))
        push!(times_lu_mean, mean(lu_runs))
        push!(times_lu_std, std(lu_runs))
        
        @printf("  n=%8d  Gauss mean=%.4fs std=%.4fs  LU mean=%.4fs std=%.4fs\n",
            n, times_gauss_mean[end], times_gauss_std[end], times_lu_mean[end], times_lu_std[end])
    end

    println("Generowanie wykresu 'time_complexity.png'...")

    p = plot(xlabel="Rozmiar macierzy n",
        ylabel="Czas wykonania [s]",
        title="Złożoność czasowa algorytmów (skala log-log)",
        xscale=:log10,
        yscale=:log10,
        marker=:circle,
        lw=2,
        legend=:topleft)

    plot!(p, valid_sizes, times_gauss_mean, yerror=times_gauss_std, label="Eliminacja Gaussa")
    plot!(p, valid_sizes, times_lu_mean, yerror=times_lu_std, label="Rozkład LU")

    # Dodanie linii odniesienia O(n) dla porównania - oparta na ostatnim pomiarze Gaussa
    if !isempty(times_gauss_mean)
        ref_x = valid_sizes
        ref_y = (times_gauss_mean[end] / valid_sizes[end]) .* valid_sizes
        plot!(p, ref_x, ref_y, label="O(n)", linestyle=:dash, color=:grey)
    end

    savefig(p, "time_complexity.png")
    println("Gotowe. Wykres zapisano jako 'time_complexity.png'.")
    
    rm(temp_file, force=true)
end

run_experiments()