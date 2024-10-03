//
// Created by 果汁 on 2024/9/22.
//

#ifndef BITMAP_H
#define BITMAP_H
#include <algorithm>

namespace ebstd{
class bitmap{
private:
    int *f{};
    int *t{};
    int cnt;
    int top;

protected:
    [[nodiscard]] bool valid(const int r) const{
        return (0<=r) && (r<top);
    }
    [[nodiscard]] bool del(const int k) const {
        return valid(f[k]) && !(t[f[k]]+1+k);
    }

public:
    explicit bitmap(const int n = 64){
        cnt = n;
		f = static_cast<int *>(malloc(cnt));
		t = static_cast<int *>(malloc(cnt));
        top = 0;
    }

    explicit bitmap(const char *file){
	    FILE *fl = fopen(file, "w+");
		fseek(fl, 0, SEEK_SET);
	    const long begin = ftell(fl);
		fseek(fl, 0, SEEK_END);
	    const long end = ftell(fl);
	    const long filesize = end - begin;
		fseek(fl, 0, SEEK_SET);
		auto *p = static_cast<int *>(malloc(filesize * sizeof(int)));
		fread(p, sizeof(int), filesize, fl);
	    constexpr int n = sizeof(p) / sizeof(p[0]);
		int m = INT_MIN;
		for (int i=0; i<n; i++){
			if (p[i]>=0){
				if (p[i]>m) m = p[i];
			} else {
				m = std::max(m, (int)(-1 - p[i]));
            }
		}
		top = n;
		cnt = std::max(m, top) >> 1;
        f = static_cast<int *>(malloc(cnt));
        t = static_cast<int *>(malloc(cnt));
		for (int i=0; i<top; i++){
			t[i] = p[i];
			if (t[i]>=0){
				f[t[i]] = i;
			} else {
				f[-1 - t[i]] = i;
			}
		}
		delete [] p;
    }

    ~bitmap(){
        delete [] f;
        delete [] t;
    }

	void expand(const int k){
        if (k < cnt / 2) return;
        if (top < 1) return;
        const int *of = f;
        const int *ot = t;
        constexpr int n = sizeof(of) / sizeof(of[0]);
        constexpr int m = sizeof(ot) / sizeof(ot[0]);
		cnt = std::max(m, n) >> 1;
		top = m;
		f = static_cast<int *>(realloc(f, cnt));
		t = static_cast<int *>(realloc(t, cnt));
        delete [] of;
        delete [] ot;
    }

    bool test(const int k){
        expand(k);
        return valid(f[k]) && (k == t[f[k]]);
    }

    void set(const int k){
        expand(k);
        if (test(k)) return;
        if (!del(k)){
            f[k] = top++;
        }
        t[f[k]] = k;
    }

    void clear(const int k){
        expand(k);
        if (test(k)){
            t[f[k]] = -1 - k; //k = -1 - T[i]
        }
    }

    char* b2s(const int n){
        expand(n-1);
        const auto s = new char[n+1];
        s[n] = '\0';
        for (int i=0; i<n; i++){
            s[i] = test(i) ? '1' : '0';
        }
        return s;
    }

    void dump(const char *file) const {
        FILE *fp = fopen(file, "w");
        fwrite(t, sizeof(int), top, fp);
        fclose(fp);
    }
};

}
#endif //BITMAP_H
