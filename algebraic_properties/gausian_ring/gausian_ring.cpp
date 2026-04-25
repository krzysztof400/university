#include <iostream>
#include <cmath>

class Gaussian {
    private:
        long long real;
        long long imaginary;
    
    public:
        Gaussian(long long real, long long imaginary) {
            this->real = real;
            this->imaginary = imaginary;
        }
        Gaussian operator+(const Gaussian& other) const {
            return Gaussian(this->real + other.real, this->imaginary + other.imaginary);
        }
        Gaussian operator-(const Gaussian& other) const {
            return Gaussian(this->real - other.real, this->imaginary - other.imaginary);
        }
        Gaussian operator*(const Gaussian& other) const {
            return Gaussian(this->real * other.real, this->imaginary * other.imaginary);
        }
        Gaussian operator%(const Gaussian& other) const {
            long logdiv_imaginary
            if (other.real != 0) {
                long long div_real = std::round(this->real / other.real);
                long long rest_real = this->real - (div_real * other.real);
            } if (other.imaginary != 0) {
                long long div_imaginary = std::round(this->imaginary / other.imaginary);
                long long rest_imaginary = this->imaginary - div_imaginary * other.imaginary;
            }

            return Gaussian(rest_real, rest_imaginary);
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Gaussian& z) {
            os << z.real;
            if(z.imaginary != 0) {
                os << "+" << z.imaginary << "i";
            }
            return os;
        }

        long long norm() const {
            return (std::pow(this->real, 2) + std::pow(this->real, 2));
        }
};

// Gaussian gcd (Gaussian num1, Gaussian num2){

// }

// Gaussian lcm (Gaussian num1, Gaussian num2){

// }

double abs (const Gaussian& z) {
    return std::sqrt(z.norm());
}

int main() {
    Gaussian z1 = Gaussian(10, 0);
    Gaussian z2 = Gaussian(-2, 0);

    std::cout << z1 % z2<< std::endl;
    std::cout << abs(z1)<< std::endl;

    return 0;
}