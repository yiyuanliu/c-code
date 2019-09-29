#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
    int start;
    int idx;
} ship;

struct node {
    float time;
    float pos;
    int i, j;

    friend bool operator < (const node &a, const node &b)
    {
        return (a.time < b.time) || ((a.time == b.time) && (a.pos < b.pos));
    }
};

ship ships[100][250000];
int ship_v[250000+1];
int cnt_q[100];
int cnt_p[100];

int main()
{
    int n;  scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int x, v;
        scanf("%d %d", &x, &v);
        ship s;
        s.idx = i; s.start = x;
        ships[v][cnt_q[v]] = s;
        ship_v[i] = v;
        cnt_q[v]++;
    }

    vector<node> heap;
    heap.reserve(10000+10);
    make_heap(heap.begin(), heap.end());

    for (int i = 1; i <= n; i++) {
        int v = ship_v[i];
        ship ship1 = ships[v][cnt_p[v]];
        int v1 = v;
        cnt_p[v]++;
        
        while (--v) {
            for (int j = cnt_p[v]; j < cnt_q[v]; j++) {
                ship ship2 = ships[v][j];
                int v2 = v;
                // ship2 start in front of ship1, but has lower speed
                // so ship1 can pass ship2
                node n;
                n.time = ((float)ship2.start - ship1.start) / (v1 - v2);
                n.pos = n.time * v1 + ship1.start;
                n.i = ship1.idx;
                n.j = ship2.idx;
                if (heap.size() >= 10000) {
                    node top = *(heap.begin());
                    if (n < top) {
                        pop_heap(heap.begin(), heap.end());
                        heap.pop_back();
                        heap.push_back(n);
                        push_heap(heap.begin(), heap.end());
                    }
                } else {
                    heap.push_back(n);
                    push_heap(heap.begin(), heap.end());
                }
            }
        }
    }

    sort_heap(heap.begin(), heap.end());
    printf("%ld\n", heap.size());
    vector<node>::iterator itor = heap.begin();
    while (itor != heap.end()) {
        node n = *itor;
        printf("%d %d\n", n.i, n.j);
        itor++;
    }
}