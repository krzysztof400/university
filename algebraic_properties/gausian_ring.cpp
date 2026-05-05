#include <iostream>
#include <cmath>

class GaussianInt {
    private:
        long long real;
        long long imaginary;
    
    public:
        GaussianInt(long long r, long long i) : real(r), imaginary(i) {}
        GaussianInt operator+(const GaussianInt& other) const {
            return GaussianInt(this->real + other.real, this->imaginary + other.imaginary);
        }
        GaussianInt operator-(const GaussianInt& other) const {
            return GaussianInt(this->real - other.real, this->imaginary - other.imaginary);
        }
        GaussianInt operator*(const GaussianInt& other) const {
            // (a+bi)(c+di) = ac - bd + i(ad + bc)
            return GaussianInt(
                this->real * other.real - this->imaginary * other.imaginary, 
                this->real * other.imaginary + this->imaginary * other.real
            );
        }
        GaussianInt operator%(const GaussianInt& other) const {
            long long n = other.norm();

            if (n == 0) {
                throw std::invalid_argument("Dividing by zero");
            }

            double exact_real = (double)(this->real * other.real + this->imaginary * other.imaginary) / n;
            double exact_imag = (double)(this->imaginary * other.real - this->real * other.imaginary) / n;

            long long q_real = std::round(exact_real);
            long long q_imag = std::round(exact_imag);

            GaussianInt q(q_real, q_imag);

            return *this - (other * q);
        }
        
        friend std::ostream& operator<<(std::ostream& os, const GaussianInt& z) {
            os << z.real;
            if(z.imaginary > 0) {
                os << "+" << z.imaginary << "i";
            }
            else if (z.imaginary < 0) {
                os << z.imaginary << "i";
            }
            return os;
        }

        long long norm() const {
            return (this->real * this->real + this->imaginary * this->imaginary);
        }

        GaussianInt exactDivide(const GaussianInt& other) const {
            long long n = other.norm();
            
            if (n == 0) {
                throw std::invalid_argument("Dividing by zero");
            }

            long long r = (this->real * other.real + this->imaginary * other.imaginary) / n;
            long long i = (this->imaginary * other.real - this->real * other.imaginary) / n;
            
            return GaussianInt(r, i);
        }
};

GaussianInt gcd (GaussianInt z1, GaussianInt z2){
    while (z2.norm() != 0) {
        GaussianInt rest = z1 % z2;
        z1 = z2;
        z2 = rest;
    }
    return z1;
}

GaussianInt lcm (GaussianInt num1, GaussianInt num2){
    if (num1.norm() == 0 || num2.norm() == 0) {
        return GaussianInt(0,0);
    }
    GaussianInt my_gcd = gcd(num1, num2);

    return num1.exactDivide(my_gcd) * num2;
}

#include <vector>

GaussianInt gcd(const std::vector<GaussianInt>& list) {
    if (list.empty()) return GaussianInt(0, 0);
    GaussianInt result = list[0];
    for (size_t i = 1; i < list.size(); ++i) {
        result = gcd(result, list[i]);
    }
    return result;
}

GaussianInt lcm(const std::vector<GaussianInt>& list) {
    if (list.empty()) return GaussianInt(1, 0);
    GaussianInt result = list[0];
    for (size_t i = 1; i < list.size(); ++i) {
        result = lcm(result, list[i]);
    }
    return result;
}

double abs (const GaussianInt& z) {
    return std::sqrt(z.norm());
}

int main() {
    // Twoje zmienne
    long long a = 2, b = 7, c = 9, d = 7, e = 5, f = 7;

    std::cout << "========== ZADANIE 1b ==========\n";
    // 1. Wywołanie dla N(a + bi)
    GaussianInt z1(a, b);
    std::cout << "Liczba z1 = a + bi = " << z1 << "\n";
    std::cout << "Norma N(z1) = " << z1.norm() << "\n\n";

    // 2. Dzielenie z resztą: (c+a) + (d+b)i przez e + fi
    GaussianInt dividend(c + a, d + b); // Dzielna
    GaussianInt divisor(e, f);          // Dzielnik
    
    GaussianInt remainder = dividend % divisor;
    
    std::cout << "Dzielenie (" << dividend << ") przez (" << divisor << ")\n";
    std::cout << "Reszta z dzielenia = " << remainder << "\n";
    std::cout << "--------------------------------\n";

    std::cout << "========== ZADANIE 1c ==========\n";
    // Trójka liczb: a + bi, c + di, e + di
    GaussianInt num1(a, b);
    GaussianInt num2(c, d);
    GaussianInt num3(e, d); // Uwaga: w poleceniu jest e+di, a nie e+fi

    std::vector<GaussianInt> lista = {num1, num2, num3};
    
    std::cout << "Lista liczb:\n";
    std::cout << "1: " << num1 << "\n";
    std::cout << "2: " << num2 << "\n";
    std::cout << "3: " << num3 << "\n\n";

    GaussianInt my_gcd = gcd(lista);
    GaussianInt my_lcm = lcm(lista);

    std::cout << "NWD dla calej listy = " << my_gcd << "\n";
    std::cout << "NWW dla calej listy = " << my_lcm << "\n\n";

    std::cout << "Test przypadkow brzegowych:\n";
    std::vector<GaussianInt> pusta_lista;
    std::vector<GaussianInt> jedno_elementowa = {num1};

    std::cout << "NWD pustej listy = " << gcd(pusta_lista) << "\n";
    std::cout << "NWW pustej listy = " << lcm(pusta_lista) << "\n";
    std::cout << "NWD 1-elementowej (" << num1 << ") = " << gcd(jedno_elementowa) << "\n";
    std::cout << "NWW 1-elementowej (" << num1 << ") = " << lcm(jedno_elementowa) << "\n";

    return 0;
}