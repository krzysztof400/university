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

vector<int> computePrefixFunction(const string &pattern) {
    int m = (int)pattern.size();
    vector<int> pi(m);
    pi[0] = 0;
    int k = 0;
    for (int q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) k = pi[k-1];
        if (pattern[k] == pattern[q]) ++k;
        pi[q] = k;
    }
    return pi;
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (argc != 3) {
        cerr << "Usage: KMP <pattern> <filename>\n";
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
    int n = (int)text.size();
    vector<int> pi = computePrefixFunction(pattern);

    int q = 0;
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        while (q > 0 && pattern[q] != text[i]) q = pi[q-1];
        if (pattern[q] == text[i]) ++q;
        if (q == m) {
            cout << (i - m + 1) << "\n";
            ++total;
            q = pi[q-1];
        }
    }
    cout << "Total: " << total << "\n";
    return 0;
}
