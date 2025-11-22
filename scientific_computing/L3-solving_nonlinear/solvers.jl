# Krzysztof Zając, 279757

module Solvers

export mbisekcji, mstycznych, msiecznych

function mbisekcji(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
    fa = f(a)
    fb = f(b)
    if sign(fa) == sign(fb)
        return (0.0, 0.0, 0, 1)
    end
    it = 0
    c = a
    while (b - a) > delta
        it += 1
        c = (a + b) / 2.0
        fc = f(c)
        if abs(fc) < epsilon || abs(b - a) < delta
            return (c, fc, it, 0)
        end
        if sign(fc) != sign(fa)
            b = c
            fb = fc
        else
            a = c
            fa = fc
        end
    end
    return (c, f(c), it, 0)
end

function mstycznych(f, pf, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    it = 0
    x = x0
    v = f(x)
    while it < maxit
        it += 1
        der = pf(x)
        if abs(der) < eps(Float64)
            return (x, v, it, 2)
        end
        x_next = x - v / der
        v_next = f(x_next)
        if abs(x_next - x) < delta || abs(v_next) < epsilon
            return (x_next, v_next, it, 0)
        end
        x = x_next
        v = v_next
    end
    return (x, v, it, 1)
end

function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    it = 0
    fa = f(x0)
    fb = f(x1)
    a = x0
    b = x1
    while it < maxit
        it += 1
        if abs(fa) > abs(fb)
            a, b = b, a
            fa, fb = fb, fa
        end
        s = (b - a) / (fb - fa)
        b = a
        fb = fa
        a = a - fa * s
        fa = f(a)
        if abs(fa) < epsilon || abs(b - a) < delta
            return (a, fa, it, 0)
        end
    end
    return (a, fa, it, 1)
end

end