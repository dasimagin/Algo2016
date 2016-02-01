#include <iostream>
#include <vector>

using namespace std;

int hanoi2 (int n, vector<int>& i, vector<int>& j, vector<int>& k, size_t t) {
    if (n > 0) {
        t = hanoi2 (n - 1, i, k, j, t);
        j.push_back(i[i.size() - 1]);
        i.pop_back();
        cout << t << ":\n[";
        for (int r = 1; r < 4; ++r) {
            if (i[0] == r) {
                for (int e = 1; e < i.size(); ++e) {
                    if (e != 1) {
                        cout << ", ";
                    }
                    cout << i[e];
                }
            }
            if (j[0] == r) {
                for (int e = 1; e < j.size(); ++e) {
                    if (e != 1) {
                        cout << ", ";
                    }
                    cout << j[e];
                }
            }
            if (k[0] == r) {
                for (int e = 1; e < k.size(); ++e) {
                    if (e != 1) {
                        cout << ", ";
                    }
                    cout << k[e];
                }
            }
            cout << "]\n";
            if (r != 3) {
                cout << '[';
            }
        }
        ++t;
        t = hanoi2(n - 1, k, j, i, t);
    }
    return t;
}

int main()
{
    int n = 5;
    vector<int> i;
    vector<int> j;
    vector<int> k;
    i.push_back(1);
    j.push_back(2);
    k.push_back(3);
    for (int t = n; t > 0; --t) {
        i.push_back(t);
    }
    hanoi2(n, i, j, k, 1);
    return 0;
}
