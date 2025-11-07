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

bool is_suffix_of(const string &pattern, int k, const string &s) {
    if ((int)s.size() < k) return false;
    int offset_s = (int)s.size() - k;
    for (int i = 0; i < k; ++i) {
        if (pattern[i] != s[offset_s + i]) return false;
    }
    return true;
}

vector<vector<int>> buildTransition(const string &pattern) {
    int m = (int)pattern.size();
    const int SIGMA = 256;
    vector<vector<int>> TF(m + 1, vector<int>(SIGMA, 0));

    for (int q = 0; q <= m; ++q) {
        for (int a = 0; a < SIGMA; ++a) {
            string s;
            if (q > 0) s.append(pattern, 0, q);
            s.push_back((char)a);

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

    int q = 0;
    long long total = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        q = TF[q][ch];
        if (q == m) {
            cout << (i - m + 1) << "\n";
            ++total;
        }
    }
    cout << "Total: " << total << "\n";
    return 0;
}
