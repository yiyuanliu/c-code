#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
    char    lable[10];
    int     priority;
    int     left;
    int     right;
};

struct node nodes[100000+1];

int cmp(const void *p1, const void *p2) {
    const struct node *n1 = (struct node*)p1, *n2 = (struct node*)p2;
    return (n2->priority - n1->priority);
}

static inline
int cmp_label(struct node *n1, struct node *n2) {
    return strcmp(n1->lable, n2->lable);
}

void dfs(int n) {
    if (n == 0)
        return;
    printf("(");
    dfs(nodes[n].left);
    printf("%s/%d", nodes[n].lable, nodes[n].priority);
    dfs(nodes[n].right);
    printf(")");
}

int main()
{
    int n = 0;
    while (scanf("%d", &n) && n != 0) {
        char buf[100];
        for (int i = 1; i <= n; i++) {
            scanf("%s", buf);
            sscanf(buf, "%[^/]/%d", nodes[i].lable, &nodes[i].priority);
            nodes[i].left = nodes[i].right = 0;
        }

        qsort(nodes+1, n, sizeof(struct node), cmp);
        for (int i = 2; i <= n; i++) {
            int p = 1;
            while (true) {
                if (cmp_label(&nodes[i], &nodes[p]) < 0) {
                    // set as left child
                    if (nodes[p].left == 0)     {nodes[p].left = i; break;}
                    p = nodes[p].left;
                } else {
                    // set as right child
                    if (nodes[p].right == 0)    {nodes[p].right = i;break;}
                    p = nodes[p].right;
                }
            }
        }

        dfs(1);
        printf("\n");
    }

    return 0;
}