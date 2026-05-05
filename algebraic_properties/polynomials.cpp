#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <numeric>

using Monomial = std::vector<int>;

// Pad a monomial to length n on the right with zeros.
inline Monomial pad(Monomial m, size_t n) {
    m.resize(n, 0);
    return m;
}

// Component-wise addition of exponent vectors (= monomial product).
inline Monomial monomialAdd(const Monomial& a, const Monomial& b) {
    size_t n = std::max(a.size(), b.size());
    Monomial res(n, 0);
    for (size_t i = 0; i < a.size(); ++i) res[i] += a[i];
    for (size_t i = 0; i < b.size(); ++i) res[i] += b[i];
    return res;
}

// Total degree of a monomial.
inline int totalDeg(const Monomial& m) {
    return std::accumulate(m.begin(), m.end(), 0);
}

template <typename T>
class MvPolynomial {
private:
    std::map<Monomial, T> terms;   // monomial → coefficient
    int numVars;                   // number of variables

    static bool isZero(const T& v) { return v == T(0); }

    // Insert or add a term; erase if coefficient becomes zero.
    void addTerm(const Monomial& m, const T& c) {
        if (isZero(c)) return;
        auto [it, inserted] = terms.emplace(m, c);
        if (!inserted) {
            it->second = it->second + c;
            if (isZero(it->second)) terms.erase(it);
        }
    }

public:
    explicit MvPolynomial(int n = 1) : numVars(n) {}

    // Constant polynomial.
    MvPolynomial(T constant, int n)
        : numVars(n) {
        if (!isZero(constant))
            terms[Monomial(n, 0)] = constant;
    }

    // Single monomial: coefficient * x_0^e[0] * x_1^e[1] * ...
    MvPolynomial(T coeff, Monomial exponents)
        : numVars(static_cast<int>(exponents.size())) {
        if (!isZero(coeff))
            terms[std::move(exponents)] = coeff;
    }

    // Build from explicit map.
    MvPolynomial(std::map<Monomial, T> t, int n)
        : terms(std::move(t)), numVars(n) {}


    bool isZeroPoly() const { return terms.empty(); }
    int  variables()  const { return numVars; }

    // Leading monomial (last in lex order = highest).
    const Monomial& leadMonomial() const {
        if (isZeroPoly()) throw std::runtime_error("Zero polynomial");
        return terms.rbegin()->first;
    }
    const T& leadCoeff() const {
        if (isZeroPoly()) throw std::runtime_error("Zero polynomial");
        return terms.rbegin()->second;
    }

    int degree() const {
        int d = 0;
        for (auto& [m, _] : terms) d = std::max(d, totalDeg(m));
        return d;
    }

    MvPolynomial operator+(const MvPolynomial& other) const {
        MvPolynomial res = *this;
        for (auto& [m, c] : other.terms) res.addTerm(m, c);
        res.numVars = std::max(numVars, other.numVars);
        return res;
    }

    MvPolynomial operator-(const MvPolynomial& other) const {
        MvPolynomial res = *this;
        for (auto& [m, c] : other.terms) res.addTerm(m, T(0) - c);
        res.numVars = std::max(numVars, other.numVars);
        return res;
    }

    MvPolynomial operator-() const {
        MvPolynomial res(numVars);
        for (auto& [m, c] : terms) res.terms[m] = T(0) - c;
        return res;
    }

    MvPolynomial operator*(const MvPolynomial& other) const {
        MvPolynomial res(std::max(numVars, other.numVars));
        for (auto& [ma, ca] : terms)
            for (auto& [mb, cb] : other.terms)
                res.addTerm(monomialAdd(ma, mb), ca * cb);
        return res;
    }

    MvPolynomial operator*(const T& scalar) const {
        if (isZero(scalar)) return MvPolynomial(numVars);
        MvPolynomial res(numVars);
        for (auto& [m, c] : terms) res.terms[m] = c * scalar;
        return res;
    }

    MvPolynomial operator/(const T& scalar) const {
        if (isZero(scalar)) throw std::invalid_argument("Division by zero scalar");
        MvPolynomial res(numVars);
        for (auto& [m, c] : terms) res.terms[m] = c / scalar;
        return res;
    }

    std::pair<std::vector<MvPolynomial>, MvPolynomial>
    divmod(const std::vector<MvPolynomial>& divisors) const {
        int s = divisors.size();
        std::vector<MvPolynomial> quots(s, MvPolynomial(numVars));
        MvPolynomial remainder(numVars);
        MvPolynomial p = *this;

        while (!p.isZeroPoly()) {
            bool divided = false;
            const Monomial& lm_p = p.leadMonomial();
            const T&        lc_p = p.leadCoeff();

            for (int i = 0; i < s; ++i) {
                if (divisors[i].isZeroPoly())
                    throw std::invalid_argument("Division by zero polynomial");

                const Monomial& lm_d = divisors[i].leadMonomial();

                // Check if lm_d divides lm_p component-wise.
                size_t n = std::max(lm_p.size(), lm_d.size());
                Monomial lm_p_pad = pad(lm_p, n);
                Monomial lm_d_pad = pad(lm_d, n);
                bool divides = true;
                Monomial diff(n);
                for (size_t k = 0; k < n; ++k) {
                    diff[k] = lm_p_pad[k] - lm_d_pad[k];
                    if (diff[k] < 0) { divides = false; break; }
                }

                if (divides) {
                    T factor = lc_p / divisors[i].leadCoeff();
                    MvPolynomial term(factor, diff);
                    quots[i]  = quots[i]  + term;
                    p         = p         - term * divisors[i];
                    divided   = true;
                    break;
                }
            }

            if (!divided) {
                // Leading term goes to remainder.
                remainder.addTerm(lm_p, lc_p);
                // Remove leading term from p.
                MvPolynomial lt(lc_p, lm_p);
                p = p - lt;
            }
        }

        return {quots, remainder};
    }

    // Convenience: single divisor.
    MvPolynomial operator/(const MvPolynomial& d) const {
        return divmod({d}).first[0];
    }
    MvPolynomial operator%(const MvPolynomial& d) const {
        return divmod({d}).second;
    }

    T evaluate(const std::vector<T>& point) const {
        T result = T(0);
        for (auto& [m, c] : terms) {
            T term = c;
            for (size_t i = 0; i < m.size(); ++i) {
                T base = (i < point.size()) ? point[i] : T(0);
                for (int e = 0; e < m[i]; ++e) term = term * base;
            }
            result = result + term;
        }
        return result;
    }

    MvPolynomial partialDerivative(int k) const {
        MvPolynomial res(numVars);
        for (auto& [m, c] : terms) {
            int e = (k < static_cast<int>(m.size())) ? m[k] : 0;
            if (e == 0) continue;
            Monomial dm = m;
            dm[k]--;
            res.addTerm(dm, c * T(e));
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const MvPolynomial& p) {
        if (p.isZeroPoly()) { os << "0"; return os; }

        bool first = true;
        // Print from highest to lowest
        for (auto it = p.terms.rbegin(); it != p.terms.rend(); ++it) {
            auto& [m, c] = *it;
            if (!first) os << " + ";
            first = false;

            bool allZeroExp = std::all_of(m.begin(), m.end(),
                                          [](int e){ return e == 0; });
            if (allZeroExp || !(c == T(1))) os << c;

            for (size_t i = 0; i < m.size(); ++i) {
                if (m[i] == 0) continue;
                os << "x" << i;
                if (m[i] > 1) os << "^" << m[i];
            }
        }
        return os;
    }
};

template <typename T>
MvPolynomial<T> gcd(MvPolynomial<T> a, MvPolynomial<T> b) {
    while (!b.isZeroPoly()) {
        MvPolynomial<T> r = a % b;
        a = b;
        b = r;
    }
    if (!a.isZeroPoly()) a = a / a.leadCoeff();
    return a;
}

template <typename T>
MvPolynomial<T> lcm(const MvPolynomial<T>& a, const MvPolynomial<T>& b) {
    if (a.isZeroPoly() || b.isZeroPoly()) return MvPolynomial<T>(T(0), a.variables());
    return a / gcd(a, b) * b;
}

template <typename T>
MvPolynomial<T> ext_gcd(MvPolynomial<T> a, MvPolynomial<T> b, MvPolynomial<T>& x, MvPolynomial<T>& y) {
    int n = a.variables();
    MvPolynomial<T> x0(T(1), n), y0(T(0), n);
    MvPolynomial<T> x1(T(0), n), y1(T(1), n);

    while (!b.isZeroPoly()) {
        auto div_res = a.divmod({b});
        MvPolynomial<T> q = div_res.first[0];
        MvPolynomial<T> r = div_res.second;

        a = b;
        b = r;

        MvPolynomial<T> x2 = x0 - q * x1;
        MvPolynomial<T> y2 = y0 - q * y1;

        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }

    T lc = a.leadCoeff();
    x = x0 / lc;
    y = y0 / lc;
    return a / lc;
}

int main() {
    double c = 9.0;
    int a_val = 2;
    double b = 7.0;

    MvPolynomial<double> P1(1);
    P1 = P1 + MvPolynomial<double>(c, Monomial{a_val});
    P1 = P1 + MvPolynomial<double>(b, Monomial{0});

    MvPolynomial<double> D(1);
    D = D + MvPolynomial<double>(1.0, Monomial{1});
    D = D + MvPolynomial<double>(1.0, Monomial{0});

    std::cout << P1.degree() << "\n";

    auto div_res = P1.divmod({D});
    std::cout << div_res.first[0] << "\n";
    std::cout << div_res.second << "\n";

    MvPolynomial<double> V(1);
    V = V + MvPolynomial<double>(1.0, Monomial{3});
    V = V + MvPolynomial<double>(-2.0, Monomial{2});
    V = V + MvPolynomial<double>(-1.0, Monomial{1});
    V = V + MvPolynomial<double>(2.0, Monomial{0});

    MvPolynomial<double> W(1);
    W = W + MvPolynomial<double>(7.0, Monomial{3});
    W = W + MvPolynomial<double>(5.0, Monomial{2});
    W = W + MvPolynomial<double>(7.0, Monomial{1});

    double g = -W.evaluate({1.0});

    MvPolynomial<double> W_g = W + MvPolynomial<double>(g, Monomial{0});

    MvPolynomial<double> X(1), Y(1);
    MvPolynomial<double> nwd = ext_gcd(V, W_g, X, Y);

    std::cout << g << "\n";
    std::cout << nwd << "\n";
    std::cout << X << "\n";
    std::cout << Y << "\n";
    
    MvPolynomial<double> check = (V * X) + (W_g * Y);
    std::cout << check << "\n";
    
    std::cout << lcm(V, W_g) << "\n";

    return 0;
}