#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <numeric>

using Monomial = std::vector<int>;

template <typename T>
struct ReduceResult;

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

enum class OrderType {
    LEX,
    GRADED_LEX
};

struct MonomialCmp {
    OrderType type;
    std::vector<int> permutation; 

    explicit MonomialCmp(OrderType t = OrderType::LEX, std::vector<int> perm = {}) 
        : type(t), permutation(std::move(perm)) {}

    bool operator()(Monomial a, Monomial b) const {
        size_t n = std::max(a.size(), b.size());
        a.resize(n, 0);
        b.resize(n, 0);

        if (type == OrderType::GRADED_LEX) {
            int degA = std::accumulate(a.begin(), a.end(), 0);
            int degB = std::accumulate(b.begin(), b.end(), 0);
            if (degA != degB) {
                return degA < degB;
            }
        }

        if (permutation.empty()) {
            for (size_t i = 0; i < n; ++i) {
                if (a[i] != b[i]) return a[i] < b[i];
            }
        } else {
            for (int var_idx : permutation) {
                if (var_idx < static_cast<int>(n)) {
                    if (a[var_idx] != b[var_idx]) return a[var_idx] < b[var_idx];
                }
            }
        }
        return false;
    }
};

template <typename T>
class MvPolynomial {
private:
    std::map<Monomial, T, MonomialCmp> terms;   // monomial → coefficient
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
    explicit MvPolynomial(int n = 1, MonomialCmp cmp = MonomialCmp()) 
        : terms(cmp), numVars(n) {}

    MvPolynomial(T constant, int n, MonomialCmp cmp = MonomialCmp())
        : terms(cmp), numVars(n) {
        if (!isZero(constant))
            terms[Monomial(n, 0)] = constant;
    }

    MvPolynomial(T coeff, Monomial exponents, MonomialCmp cmp = MonomialCmp())
        : terms(cmp), numVars(static_cast<int>(exponents.size())) {
        if (!isZero(coeff))
            terms[std::move(exponents)] = coeff;
    }

    MvPolynomial(std::map<Monomial, T, MonomialCmp> t, int n)
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
        // Używamy terms.key_comp() aby pobrać komparator z obecnego wielomianu
        MvPolynomial res(std::max(numVars, other.numVars), terms.key_comp());
        for (auto& [m, c] : terms) res.addTerm(m, c);
        for (auto& [m, c] : other.terms) res.addTerm(m, c);
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
        MvPolynomial res(std::max(numVars, other.numVars), terms.key_comp());
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

    ReduceResult<T> PolynomialReduce(const std::vector<MvPolynomial>& G) const {
        int n = G.size();
        std::vector<MvPolynomial> alphas(n, MvPolynomial(numVars, terms.key_comp()));
        MvPolynomial r(numVars, terms.key_comp());
        MvPolynomial f = *this;

        while (!f.isZeroPoly()) {
            bool division_occurred = false;
            const Monomial& lm_f = f.leadMonomial();
            const T&        lc_f = f.leadCoeff();

            for (int i = 0; i < n; ++i) {
                if (G[i].isZeroPoly())
                    throw std::invalid_argument("Division by zero polynomial in G");

                const Monomial& lm_g = G[i].leadMonomial();

                size_t max_len = std::max(lm_f.size(), lm_g.size());
                Monomial lm_f_pad = pad(lm_f, max_len);
                Monomial lm_g_pad = pad(lm_g, max_len);
                bool divides = true;
                Monomial diff(max_len);
                
                for (size_t k = 0; k < max_len; ++k) {
                    diff[k] = lm_f_pad[k] - lm_g_pad[k];
                    if (diff[k] < 0) { divides = false; break; }
                }

                if (divides) {
                    T factor_coeff = lc_f / G[i].leadCoeff();
                    MvPolynomial term(factor_coeff, diff, terms.key_comp());
                    
                    alphas[i] = alphas[i] + term;
                    f = f - (term * G[i]);
                    division_occurred = true;
                    break;
                }
            }

            if (!division_occurred) {
                MvPolynomial lt_f(lc_f, lm_f, terms.key_comp());
                r = r + lt_f;
                f = f - lt_f;
            }
        }
        return {alphas, r};
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

template <typename T>
struct ReduceResult {
    std::vector<MvPolynomial<T>> alphas; // Ciąg współczynników
    MvPolynomial<T> r;                   // Reszta
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <1|2>\n";
        return 1;
    }

    std::string flag = argv[1];

    if (flag == "1") {
        std::cout << "--- List 1 (Exercise 37) ---\n";
        
        MonomialCmp grlex(OrderType::GRADED_LEX);
        
        MvPolynomial<double> f(3, grlex);
        f = f + MvPolynomial<double>(1.0, {3, 0, 0}, grlex)
              - MvPolynomial<double>(1.0, {2, 1, 0}, grlex)
              - MvPolynomial<double>(1.0, {2, 0, 1}, grlex);

        MvPolynomial<double> g1(3, grlex);
        g1 = g1 + MvPolynomial<double>(1.0, {2, 1, 0}, grlex)
                - MvPolynomial<double>(1.0, {0, 0, 1}, grlex);

        MvPolynomial<double> g2(3, grlex);
        g2 = g2 + MvPolynomial<double>(1.0, {1, 1, 0}, grlex)
                - MvPolynomial<double>(1.0, {0, 0, 0}, grlex);

        std::vector<MvPolynomial<double>> G1 = {g1, g2};
        std::vector<MvPolynomial<double>> G2 = {g2, g1};

        auto res1 = f.PolynomialReduce(G1);
        auto res2 = f.PolynomialReduce(G2);

        std::cout << "Polynomial f: " << f << "\n";
        std::cout << "r1 (G = g1, g2): " << res1.r << "\n";
        std::cout << "r2 (G = g2, g1): " << res2.r << "\n";

        MvPolynomial<double> diff = res1.r - res2.r;
        auto res_diff = diff.PolynomialReduce(G1);
        
        std::cout << "Is r1 - r2 in <g1, g2>? ";
        if (res_diff.r.isZeroPoly()) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }

    } else if (flag == "2") {
        std::cout << "--- List 2 (Index Exercise) ---\n";
        std::cout << "Index: 279757 (a=2, b=7, c=9, d=7, e=5, f=7)\n";

        std::vector<std::vector<int>> perms = {
            {0, 1, 2},
            {1, 2, 0}, 
            {2, 0, 1}
        };

        for (size_t i = 0; i < perms.size(); ++i) {
            MonomialCmp lex(OrderType::LEX, perms[i]);
            
            MvPolynomial<double> h(3, lex);
            h = h + MvPolynomial<double>(1.0, {2, 7, 0}, lex)
                  - MvPolynomial<double>(1.0, {0, 9, 7}, lex)
                  + MvPolynomial<double>(1.0, {5, 0, 7}, lex);

            MvPolynomial<double> g1(3, lex);
            g1 = g1 + MvPolynomial<double>(1.0, {2, 0, 0}, lex)
                    - MvPolynomial<double>(1.0, {0, 1, 0}, lex);

            MvPolynomial<double> g2(3, lex);
            g2 = g2 + MvPolynomial<double>(1.0, {0, 7, 0}, lex)
                    - MvPolynomial<double>(1.0, {0, 0, 1}, lex);

            std::vector<MvPolynomial<double>> G = {g1, g2};
            auto res = h.PolynomialReduce(G);

            std::cout << "\nPermutation " << i + 1 << " (";
            for (size_t j = 0; j < perms[i].size(); ++j) {
                std::cout << "x_" << perms[i][j] << (j < 2 ? " > " : "");
            }
            std::cout << "):\n";
            std::cout << "h(x,y,z) = " << h << "\n";
            std::cout << "Remainder r: " << res.r << "\n";
        }

    } else {
        std::cout << "Invalid flag. Use 1 for List 1, or 2 for List 2.\n";
        return 1;
    }

    return 0;
}