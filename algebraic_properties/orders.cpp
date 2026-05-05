#include <iostream>
#include <vector>

using namespace std;

bool is_less_or_equal(const vector<int>& x, const vector<int>& y) {
    for (size_t i = 0; i < x.size(); ++i) {
        if (x[i] > y[i]) return false;
    }
    return true;
}

vector<vector<int>> find_minimal_elements(const vector<vector<int>>& A) {
    vector<vector<int>> minimal_elements;

    for (const auto& candidate : A) {
        bool is_minimal = true;
        
        for (const auto& other : A) {
            if (candidate != other && is_less_or_equal(other, candidate)) {
                is_minimal = false;
                break;
            }
        }
        
        if (is_minimal) {
            minimal_elements.push_back(candidate);
        }
    }
    return minimal_elements;
}

void print_elements(const vector<vector<int>>& elems) {
    for (const auto& v : elems) {
        cout << "(";
        for (size_t i = 0; i < v.size(); ++i) {
            cout << v[i] << (i + 1 == v.size() ? "" : ", ");
        }
        cout << ")\n";
    }
}

int main() {
    int a = 2, b = 7, c = 9, d = 7, e = 5, f = 7;

    vector<vector<int>> set_A;
    for (int x = a - 3; x <= a + 3; ++x) {
        for (int y = b - 3; y <= b + 3; ++y) {
            if (x >= 0 && y >= 0) {
                if ((x - a) * (x - a) + (y - b) * (y - b) < 5) {
                    set_A.push_back({x, y});
                }
            }
        }
    }

    cout << "Elementy minimalne w A:\n";
    vector<vector<int>> min_A = find_minimal_elements(set_A);
    print_elements(min_A);

    vector<vector<int>> set_B;
    for (int x1 = 0; x1 <= 15; ++x1) {
        for (int x2 = 0; x2 <= 15; ++x2) {
            for (int x3 = 0; x3 <= 15; ++x3) {
                for (int x4 = 0; x4 <= 15; ++x4) {
                    int dist_sq = (x1 - c)*(x1 - c) + (x2 - d)*(x2 - d) + 
                                  (x3 - e)*(x3 - e) + (x4 - f)*(x4 - f);
                    if (dist_sq > 224) {
                        set_B.push_back({x1, x2, x3, x4});
                    }
                }
            }
        }
    }

    cout << "\nElementy minimalne w B:\n";
    vector<vector<int>> min_B = find_minimal_elements(set_B);
    print_elements(min_B);

    return 0;
}