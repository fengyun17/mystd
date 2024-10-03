//
// Created by 果汁 on 2024/9/22.
//

#ifndef HASH_H
#define HASH_H
#include "bitmap.h"

namespace ebstd{
template <typename K, typename V> struct dict
{
	virtual ~dict() = default;

	[[nodiscard]] virtual int size() const = 0;
    virtual bool put(K, V) = 0;
    virtual V* get(K k) = 0;
    virtual bool del(K k) = 0;
};

template <typename K, typename V> struct entry{
    K key;
    V value;
    explicit entry(K k=K(), V v=V()) : key(k), value(v){};
    entry(entry<K, V> const &e) : key(e.key), value(e.value){};
    bool operator<(entry<K, V> const &e) {return key < e.key;}
    bool operator>(entry<K, V> const &e) {return key > e.key;}
    bool operator==(entry<K, V> const &e) {return key == e.key;}
    bool operator!=(entry<K, V> const &e) {return key != e.key;}
};

static int prime(const int end, const int st=2){
	bitmap b(end);
	b.set(0);
	b.set(1);
	int res = 1;
	for (int i=st; i*i<end; i++){
		if (!b.test(i)){
			for (int j=i*i; j<end; j+=i){
				b.set(j);
			}
		} else {
			if (i % 4 == 3){
				res = i;
				return res;
			}
		}
	}
	return res;
}

static int log(const int n){
	int res = 1;
	for (; res<=n; res+=res) {}
	return res;
}

static bool f(const int st, const int end){
	constexpr int A = 50;
	constexpr int B = 100;
	return A * end / log(end) > B * st / log(st);
}

static int get(const int end, const int st=2){
	int res = 1;
	do {
		int tmp = end;
		if (!f(st, tmp*tmp+11)){
			tmp *= 2;
			continue;
		}
		res = prime(tmp * tmp + 11, st);
		tmp *= 2;
	}while(res != 1);
	return res;
}

static int power(int a, int b){
	int res = 1;
	while (b > 0){
		if ((b & 1) == 1){
			res *= a;
		}
		a *= a;
		b >>= 1;
	}
	return res;
}

static size_t hashCode(const char c){return static_cast<size_t>(c);}
static size_t hashCode(const int k){return static_cast<size_t>(k);}
static size_t hashCode(const long long i){return (size_t)((i >> 32) + static_cast<int>(i));}
static size_t hashCode(char s[]){
    int h = 0;
    for (size_t n=strlen(s), i=0; i<n; i++){
        h = (h << 5) | (h >> 27);
        h += static_cast<int>(s[i]);
    }
    return static_cast<size_t>(h);
}

static size_t hashCode(const int s[]){
    int h = 0;
    for (size_t n=sizeof(s), i=0; i<n; i++){
        h = (h << 5) | (h >> 27);
        h += s[i];
    }
    return static_cast<size_t>(h);
}

template <typename K, typename V> class hash : public dict<K, V>{
private:
    entry<K, V> **ht;
    int m;
    int cnt;
    bitmap *lazy;

protected:
    int findHit(const K &k){
        int res = hashCode(k) % m;
        const int tmp = res;
        int i = 1, j = 1;
        while((ht[res] && (k != ht[res]->key)) || (!ht[res] && lazy->test(res))){
            res = (tmp + power(-1, j-1) * i * i) % m;
            if (j % 2 == 0){
                i++;
            }
            j++;
        }
        return res;
    }

    int findFree(const K &k){
        int res = hashCode(k) % m;
        const int tmp = res;
        int i = 1, j = 1;
        while(ht[res]){
            res = (tmp + power(-1, j-1) * i * i) % m;
            if (j % 2 == 0){
                i++;
            }
            j++;
        }
        return res;
    }

    void rehash(){
	    const int om = m;
        entry<K, V> **oht = ht;
        m = get(m, m);
        cnt = 0;
        ht = new entry<K, V>*[m];
        memset(ht, 0, sizeof(entry<K, V>*) * m);
        delete lazy;
		lazy = nullptr;
        lazy = new bitmap(m);
        for (int i=0; i<om; i++){
            if (oht[i]){
                put(oht[i]->key, oht[i]->value);
            }
        }
        delete[] oht;
        //oht = nullptr;
    }

public:
	explicit hash(int c){
		m = get(c);
		cnt = 0;
		ht = new entry<K, V>*[m];
		memset(ht, 0, sizeof(entry<K, V>*) * m);
		lazy = new bitmap(m);
	}

	~hash() override {
		delete[] ht;
        ht = nullptr;
		delete lazy;
		lazy = nullptr;
	}

	[[nodiscard]] int size() const override {
		return cnt;
	}

    V* get(K k) override {
        int r = findHit(k);
        return ht[r] ? (ht[r]->value) : nullptr;
    }

    bool del(K k) override {
        int r = findHit(k);
        if (!ht[r]) return false;
        delete ht[r];
        ht[r] = nullptr;
        lazy->set(r);
        cnt--;
        return true;
    }

    bool put(K k, V v) override {
        if (ht[findHit(k)]) return false;
        int r = findFree(k);
        ht[r] = new entry<K, V>(k, v);
        cnt++;
        if (2 * cnt > m) rehash();
        return true;
    }
};

}

#endif //HASH_H
