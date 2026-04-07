#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9 + 7;

struct Point {
    int x, y, id;
};

int N;
vector<Point> pts;
vector<int> y_vals;

struct Element {
    int y, val;
};

int bit_count[200005];

void add_count(int idx, int val) {
    for (; idx <= N; idx += idx & -idx)
        bit_count[idx] += val;
}

int query_count(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx)
        sum += bit_count[idx];
    return sum;
}

int bit_min[200005];

void add_min(int y, int val) {
    int idx = N - y + 1;
    for (; idx <= N; idx += idx & -idx)
        bit_min[idx] = min(bit_min[idx], val);
}

void clear_min(int y) {
    int idx = N - y + 1;
    for (; idx <= N; idx += idx & -idx)
        bit_min[idx] = INF;
}

int query_min(int y) {
    int idx = N - y;
    int res = INF;
    for (; idx > 0; idx -= idx & -idx)
        res = min(res, bit_min[idx]);
    return res;
}

int bit_max[200005];

void add_max(int y, int val) {
    int idx = y;
    for (; idx <= N; idx += idx & -idx)
        bit_max[idx] = max(bit_max[idx], val);
}

void clear_max(int y) {
    int idx = y;
    for (; idx <= N; idx += idx & -idx)
        bit_max[idx] = 0;
}

int query_max(int y) {
    int idx = y - 1;
    int res = 0;
    for (; idx > 0; idx -= idx & -idx)
        res = max(res, bit_max[idx]);
    return res;
}

long long solve(int l, int r) {
    if (l == r) return 0;
    int mid = l + (r - l) / 2;
    long long ans = solve(l, mid) + solve(mid + 1, r);

    vector<Element> L, R;
    
    for (int i = mid; i >= l; --i) {
        int ymax = query_min(pts[i].y);
        L.push_back({pts[i].y, ymax});
        add_min(pts[i].y, pts[i].y);
    }
    for (int i = mid; i >= l; --i) {
        clear_min(pts[i].y);
    }

    for (int i = mid + 1; i <= r; ++i) {
        int ymin = query_max(pts[i].y);
        R.push_back({pts[i].y, ymin});
        add_max(pts[i].y, pts[i].y);
    }
    for (int i = mid + 1; i <= r; ++i) {
        clear_max(pts[i].y);
    }

    sort(L.begin(), L.end(), [](const Element& a, const Element& b) {
        return a.val > b.val; // sort by ymax descending
    });
    sort(R.begin(), R.end(), [](const Element& a, const Element& b) {
        return a.y > b.y; // sort by y descending
    });

    int j = 0;
    for (const auto& B : R) {
        while (j < L.size() && L[j].val > B.y) {
            add_count(L[j].y, 1);
            j++;
        }
        ans += query_count(B.y - 1) - query_count(B.val);
    }

    for (int k = 0; k < j; ++k) {
        add_count(L[k].y, -1);
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    pts.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> pts[i].x >> pts[i].y;
        pts[i].id = i;
        y_vals.push_back(pts[i].y);
    }

    sort(y_vals.begin(), y_vals.end());
    y_vals.erase(unique(y_vals.begin(), y_vals.end()), y_vals.end());

    for (int i = 0; i < N; ++i) {
        pts[i].y = lower_bound(y_vals.begin(), y_vals.end(), pts[i].y) - y_vals.begin() + 1;
    }

    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    for (int i = 1; i <= N; ++i) {
        bit_min[i] = INF;
        bit_max[i] = 0;
        bit_count[i] = 0;
    }

    long long total_rectangles = solve(0, N - 1);
    cout << total_rectangles << "\n";

    return 0;
}
