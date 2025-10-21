// fa.cpp
// Budowa automatu skończonego wg. Cormen (rozdz. 34.3) i wyszukiwanie wszystkich wystąpień wzorca.
// Wywołanie: FA <pattern> <filename>
// Wypisuje pozycje (0-based) startu każdego dopasowania, po jednej na linię,
// a na końcu "Total: N".

#include <bits/stdc++.h>
using namespace std;

string readFile(const string &filename) {
    ifstream in(filename, ios::binary);
    if (!in) {
        throw runtime_error("Cannot open file: " + filename);
    }
    string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    return contents;
}

// Sprawdza, czy pattern[0..k-1] jest sufiksem tekstu t (ktory ma dlugosc q i ma doklejony znak a na koncu)
// Implementacja pomocnicza przy budowie przejść (prostym sposobem: porównania znak-po-znaku)
bool is_suffix_of(const string &pattern, int k, const string &s) {
    // k - dlugosc prefiksu patternu
    // s - string, ktorego suffix porownujemy (moze byc pattern[0..q-1] + a)
    if ((int)s.size() < k) return false;
    int offset_s = (int)s.size() - k;
    for (int i = 0; i < k; ++i) {
        if (pattern[i] != s[offset_s + i]) return false;
    }
    return true;
}

vector<vector<int>> buildTransition(const string &pattern) {
    int m = (int)pattern.size();
    const int SIGMA = 256; // wszystkie bajty (bezpieczne)
    vector<vector<int>> TF(m + 1, vector<int>(SIGMA, 0));

    // Dla kazdego stanu q (0..m) i kazdego znaku a z alfabetu oblicz k
    for (int q = 0; q <= m; ++q) {
        for (int a = 0; a < SIGMA; ++a) {
            // s = pattern[0..q-1] + char(a)
            string s;
            if (q > 0) s.append(pattern, 0, q);
            s.push_back((char)a);

            // znalezienie maksymalnego k <= m takiego, ze pattern[0..k-1] jest sufiksem s
            int k = min(m, q + 1);
            for (; k >= 0; --k) {
                if (is_suffix_of(pattern, k, s)) break;
            }
            TF[q][a] = k;
        }
    }
    return TF;
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (argc != 3) {
        cerr << "Usage: FA <pattern> <filename>\n";
        return 1;
    }
    string pattern = argv[1];
    string filename = argv[2];

    if (pattern.empty()) {
        cerr << "Empty pattern provided. Aborting.\n";
        return 1;
    }

    string text;
    try {
        text = readFile(filename);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return 1;
    }

    int m = (int)pattern.size();
    auto TF = buildTransition(pattern);

    // Przejscia automatu
    int q = 0;
    long long total = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        q = TF[q][ch];
        if (q == m) {
            // wystapienie zaczyna sie na i-m+1
            cout << (i - m + 1) << "\n";
            ++total;
            // kontynuujemy - automat juz ma stan m; w klasycznej wersji mozna przejsc dalej
            // (nasz TF jest kompletny i pozwala to robić bez dodatkowych kroków).
        }
    }
    cout << "Total: " << total << "\n";
    return 0;
}
