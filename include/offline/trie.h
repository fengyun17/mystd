//
// Created by 果汁 on 2024/10/5.
//

#ifndef TRIE_H
#define TRIE_H
#include <string>
#include <__fwd/string.h>

namespace ebstd {
struct node {
    int tree[][] = new int[100001][26];
    int *p = new int[100001];
    std::string *e = new std::string[100001];
    int cnt{};

    void build(std::string ws[]) {
        cnt = 1;
        for (auto w: ws) {
            int cur = 1;
            p[cur]++;
            for (int i = 0, path; i < w.size(); ++i) {
                path = w[i] - 'a';
                if (tree[cur][path] == 0) {
                    tree[cur][path] = ++cnt;
                }
                cur = tree[cur][path];
                p[cur]++;
            }
            e[cur] = w;
        }
    }

    void clear(){
        for (int i = 1; i <= cnt; ++i) {
            memset(tree[i], 0, sizeof(int) * 26);
            e[i].clear();
            p[i] = 0;
        }
    }

    [[nodiscard]] std::string find(const std::string &word) const {
        int cur = 1;
        for (int i = 0, path; i < word.size(); ++i) {
            path = word[i] - 'a';
            if (tree[cur][path] == 0) return "";
            cur = tree[cur][path];
        }
        return e[cur];
    }

    [[nodiscard]] int search(const std::string &pre) const {
        int cur = 1;
        for (int i = 0, path; i < pre.size(); ++i) {
            path = pre[i] - 'a';
            if (tree[cur][path] == 0) return 0;
            cur = tree[cur][path];
        }
        return p[cur];
    }

    void del(const std::string &word) {
        if (!find(word).empty()) {
            int cur = 1;
            for (int i = 0, path; i < word.size(); ++i) {
                path = word[i] - 'a';
                if (--p[tree[cur][path]] == 0) {
                    tree[cur][path] = 0;
                    return;
                }
                cur = tree[cur][path];
            }
            e[cur].clear();
        }
    }
};
}
#endif //TRIE_H
