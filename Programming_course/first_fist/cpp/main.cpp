#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class MathUtil {
public:
    static bool isPrime(int number) {
        if (number <= 1) {
            return false;
        }
        for (int i = 2; i <= sqrt(number); i++) {
            if (number % i == 0) {
                return false;
            }
        }
        return true;
    }
};

class Primes {
private:
    vector<int> primes;

public:
    Primes(int n) {
        primes = genPrimes(n);
    }

    vector<int> genPrimes(int n) {
        vector<int> primeList;
        for (int i = 2; i <= n; i++) {
            if (MathUtil::isPrime(i)) {
                primeList.push_back(i);
            }
        }
        return primeList;
    }

    int getNumber(int m) {
        if (m < 0 || m >= primes.size()) {
            return -1;
        }
        return primes[m];
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./program <limit> [<index1> <index2> ...]" << endl;
        return 1;
    }

    int n;
    try {
        n = stoi(argv[1]);
        if (n < 2) {
            cout << "Invalid range. Please provide a number greater than 1." << endl;
            return 1;
        }

        Primes board(n);
        for (int i = 2; i < argc; i++) {
            try {
                int m = stoi(argv[i]);
                int prime = board.getNumber(m - 1);
                if (prime != -1) {
                    cout << m << " - " << prime << endl;
                } else {
                    cout << m << " - Index out of range" << endl;
                }
            } catch (const invalid_argument& ex) {
                cout << argv[i] << " - Invalid input" << endl;
            }
        }
    } catch (const invalid_argument& ex) {
        cout << argv[1] << " - Invalid input" << endl;
        return 1;
    }

    return 0;
}