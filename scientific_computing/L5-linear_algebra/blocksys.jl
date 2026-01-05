module blocksys

export load_matrix, load_vector, solve_gauss, compute_lu, solve_lu, write_solution, multiply_matrix_vector

using LinearAlgebra

"""
Structure to hold the Block Sparse Matrix A.
A is composed of dense diagonal blocks Ak, sparse sub-diagonal blocks Bk,
and diagonal super-diagonal blocks Ck.
"""
mutable struct BlockMatrix
    n::Int
    l::Int
    v::Int
    Ak::Vector{Matrix{Float64}}
    Bk::Vector{Matrix{Float64}}
    Ck::Vector{Matrix{Float64}}
end

"""
Load matrix A from file.
"""
function load_matrix(filename::String)
    open(filename, "r") do io
        line = split(readline(io))
        n = parse(Int, line[1])
        l = parse(Int, line[2])
        v = div(n, l)

        Ak = [zeros(l, l) for _ in 1:v]
        Bk = [zeros(l, l) for _ in 1:v]
        Ck = [zeros(l, l) for _ in 1:v]

        while !eof(io)
            line_str = readline(io)
            if isempty(strip(line_str))
                continue
            end
            vals = split(line_str)
            i = parse(Int, vals[1])
            j = parse(Int, vals[2])
            val = parse(Float64, vals[3])

            bi = div(i - 1, l) + 1
            bj = div(j - 1, l) + 1
            li = (i - 1) % l + 1
            lj = (j - 1) % l + 1

            if bi == bj
                Ak[bi][li, lj] = val
            elseif bi == bj + 1
                Bk[bi][li, lj] = val
            elseif bi == bj - 1
                Ck[bi][li, lj] = val
            end
        end
        return BlockMatrix(n, l, v, Ak, Bk, Ck)
    end
end

"""
Load vector b from file.
"""
function load_vector(filename::String)
    open(filename, "r") do io
        n = parse(Int, readline(io))
        b = zeros(Float64, n)
        idx = 1
        while !eof(io) && idx <= n
            line = strip(readline(io))
            if !isempty(line)
                b[idx] = parse(Float64, line)
                idx += 1
            end
        end
        return b
    end
end

"""
Write solution x to file.
"""
function write_solution(filename::String, x::Vector{Float64}, relative_error::Float64=0.0)
    open(filename, "w") do io
        if relative_error > 0
            println(io, relative_error)
        end
        for val in x
            println(io, val)
        end
    end
end

"""
Multiplies A * x taking sparsity into account.
"""
function multiply_matrix_vector(M::BlockMatrix, x::Vector{Float64})
    n = M.n
    l = M.l
    b = zeros(Float64, n)

    for k in 1:M.v
        start_idx = (k-1)*l + 1
        end_idx = k*l
        x_local = x[start_idx:end_idx]

        b[start_idx:end_idx] += M.Ak[k] * x_local

        if k < M.v
            x_next = x[end_idx+1 : end_idx+l]
            b[start_idx:end_idx] += M.Ck[k] * x_next
        end

        if k > 1
            x_prev = x[start_idx-l : start_idx-1]
            b[start_idx:end_idx] += M.Bk[k] * x_prev
        end
    end
    return b
end

"""
Solves Ax=b using Gaussian Elimination adapted for the block structure.
"""
function solve_gauss(M_in::BlockMatrix, b_in::Vector{Float64}; pivot::Bool=true)
    M = deepcopy(M_in)
    b = copy(b_in)
    n = M.n
    l = M.l
    v = M.v

    for k in 1:v
        offset = (k-1)*l

        for i in 1:l
            if pivot
                max_val = abs(M.Ak[k][i, i])
                max_row = i
                for r in i+1:l
                    if abs(M.Ak[k][r, i]) > max_val
                        max_val = abs(M.Ak[k][r, i])
                        max_row = r
                    end
                end

                if max_row != i
                    M.Ak[k][i, :], M.Ak[k][max_row, :] = M.Ak[k][max_row, :], M.Ak[k][i, :]
                    if k < v
                        M.Ck[k][i, :], M.Ck[k][max_row, :] = M.Ck[k][max_row, :], M.Ck[k][i, :]
                    end
                    b[offset+i], b[offset+max_row] = b[offset+max_row], b[offset+i]
                end
            end

            pivot_val = M.Ak[k][i, i]
            if abs(pivot_val) < 1e-14
                error("Singular matrix encountered.")
            end

            for r in i+1:l
                factor = M.Ak[k][r, i] / pivot_val
                M.Ak[k][r, i] = 0.0
                for c in i+1:l
                    M.Ak[k][r, c] -= factor * M.Ak[k][i, c]
                end
                if k < v
                    for c in 1:l
                        M.Ck[k][r, c] -= factor * M.Ck[k][i, c]
                    end
                end
                b[offset+r] -= factor * b[offset+i]
            end
        end

        if k < v
            for j in 1:l
                pivot_val = M.Ak[k][j, j]
                for r in 1:l
                    if M.Bk[k+1][r, j] != 0
                        factor = M.Bk[k+1][r, j] / pivot_val
                        M.Bk[k+1][r, j] = 0
                        for c in j+1:l
                            M.Bk[k+1][r, c] -= factor * M.Ak[k][j, c]
                        end
                        for c in 1:l
                            M.Ak[k+1][r, c] -= factor * M.Ck[k][j, c]
                        end
                        b[k*l + r] -= factor * b[(k-1)*l + j]
                    end
                end
            end
        end
    end

    x = zeros(Float64, n)
    x[n] = b[n] / M.Ak[v][l, l]

    for i in l-1:-1:1
        sum_val = 0.0
        for j in i+1:l
            sum_val += M.Ak[v][i, j] * x[(v-1)*l + j]
        end
        x[(v-1)*l + i] = (b[(v-1)*l + i] - sum_val) / M.Ak[v][i, i]
    end

    for k in v-1:-1:1
        offset = (k-1)*l
        for i in l:-1:1
            sum_C = 0.0
            for j in 1:l
                sum_C += M.Ck[k][i, j] * x[k*l + j]
            end

            sum_A = 0.0
            for j in i+1:l
                sum_A += M.Ak[k][i, j] * x[offset + j]
            end

            x[offset + i] = (b[offset + i] - sum_A - sum_C) / M.Ak[k][i, i]
        end
    end

    return x
end

"""
Computes LU decomposition in-place within the structure.
"""
function compute_lu(M_in::BlockMatrix; pivot::Bool=true)
    M = deepcopy(M_in)
    n = M.n
    l = M.l
    v = M.v
    pivots = Vector{Vector{Int}}(undef, v)

    for k in 1:v
        pivots[k] = collect(1:l)

        for i in 1:l
            if pivot
                max_val = abs(M.Ak[k][i, i])
                max_row = i
                for r in i+1:l
                    if abs(M.Ak[k][r, i]) > max_val
                        max_val = abs(M.Ak[k][r, i])
                        max_row = r
                    end
                end

                if max_row != i
                    M.Ak[k][i, :], M.Ak[k][max_row, :] = M.Ak[k][max_row, :], M.Ak[k][i, :]
                    if k < v
                        M.Ck[k][i, :], M.Ck[k][max_row, :] = M.Ck[k][max_row, :], M.Ck[k][i, :]
                    end
                    pivots[k][i], pivots[k][max_row] = pivots[k][max_row], pivots[k][i]
                end
            end

            pivot_val = M.Ak[k][i, i]
            for r in i+1:l
                factor = M.Ak[k][r, i] / pivot_val
                M.Ak[k][r, i] = factor
                for c in i+1:l
                    M.Ak[k][r, c] -= factor * M.Ak[k][i, c]
                end
                if k < v
                    for c in 1:l
                        M.Ck[k][r, c] -= factor * M.Ck[k][i, c]
                    end
                end
            end
        end

        if k < v
            for j in 1:l
                pivot_u = M.Ak[k][j, j]
                for r in 1:l
                    if M.Bk[k+1][r, j] != 0
                        factor = M.Bk[k+1][r, j] / pivot_u
                        M.Bk[k+1][r, j] = factor
                        for c in j+1:l
                            M.Bk[k+1][r, c] -= factor * M.Ak[k][j, c]
                        end
                        for c in 1:l
                            M.Ak[k+1][r, c] -= factor * M.Ck[k][j, c]
                        end
                    end
                end
            end
        end
    end

    return M, pivots
end

"""
Solves Ax=b using precomputed LU.
"""
function solve_lu(M::BlockMatrix, pivots::Vector{Vector{Int}}, b_in::Vector{Float64})
    b = copy(b_in)
    n = M.n
    l = M.l
    v = M.v

    for k in 1:v
        offset = (k-1)*l
        p = pivots[k]
        b_temp = copy(b[offset+1:offset+l])
        for i in 1:l
            b[offset+i] = b_temp[p[i]]
        end

        for i in 1:l
            for j in 1:i-1
                b[offset+i] -= M.Ak[k][i, j] * b[offset+j]
            end
        end

        if k < v
            for r in 1:l
                sum_val = 0.0
                for c in 1:l
                    sum_val += M.Bk[k+1][r, c] * b[offset+c]
                end
                b[k*l + r] -= sum_val
            end
        end
    end

    x = zeros(Float64, n)

    for i in l:-1:1
        sum_val = 0.0
        for j in i+1:l
            sum_val += M.Ak[v][i, j] * x[(v-1)*l + j]
        end
        x[(v-1)*l + i] = (b[(v-1)*l + i] - sum_val) / M.Ak[v][i, i]
    end

    for k in v-1:-1:1
        offset = (k-1)*l
        for i in l:-1:1
            sum_C = 0.0
            for j in 1:l
                sum_C += M.Ck[k][i, j] * x[k*l + j]
            end

            sum_A = 0.0
            for j in i+1:l
                sum_A += M.Ak[k][i, j] * x[offset + j]
            end

            x[offset + i] = (b[offset + i] - sum_A - sum_C) / M.Ak[k][i, i]
        end
    end

    return x
end

end