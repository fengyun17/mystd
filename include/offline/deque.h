//
// Created by 果汁 on 2024/9/22.
//

#ifndef DEQUE_H
#define DEQUE_H
#include <cassert>

#include "vector.h"

namespace ebstd{
template<class T> class deque{
    template<class K> friend std::ostream& operator<<(std::ostream &out, deque<K> const &o);

protected:
	vector<T> a;
	vector<T> b;

public:
	deque()= default;
	explicit deque(bool (*eq)(T, T)){
		a(eq);
		b(eq);
	}

	~deque(){
		~a();
		~b();
	}

	deque(deque const &other){
		a(other.a);
		b(other.b);
	}

	deque &operator=(deque const &other){
		a = other.a;
		b = other.b;
		return *this;
	}

	deque(deque &&other) noexcept {
		a(other.a);
		b(other.b);
	}

	deque &operator=(deque &&other) noexcept {
		a = other.a;
		b = other.b;
		return *this;
	}

	T &operator[](size_t r){
		if (r < b.count()){
			return b[b.count()-r-1];
		} else {
			return a[r-b.count()];
		}
	}

	[[nodiscard]] bool empty() const{
        return a.empty() && b.empty();
    }

    [[nodiscard]] size_t count() const{
        return a.count() + b.count();
    }

    void clear(){
        a.clear();
		b.clear();
    }

	void copy(T const *t, size_t l, size_t r){ //r == n
		size_t m = l + ((r - l) >> 1);
		a.copy(t, m, r);
		b.copy(t, l, m);
		b.reverse(l, m-1);
	}

	void reverse(size_t l, size_t r){ //r == n-1
		assert(l<=r);
		if (l==0 && r==count()-1){
			a.reverse();
			swap(a, b);
		} else {
			if (l >= b.count()){
				a.reverse(l-b.count(), r-b.count());
			} else if (r < b.count()){
				b.reverse(b.count()-r-1, b.count()-l-1);
			} else {
				size_t m = l + ((r - l) >> 1);
				for (size_t i=l; i<=m; i++){
					size_t j = r - i;
					if (i<b.count() && j<b.count()){
						i = b.count()-i-1;
						j = b.count()-j-1;
						swap(b[i], b[j]);
					} else if (i<b.count() && j>=b.count()){
						i = b.count()-i-1;
						j = j-b.count();
						swap(b[i], a[j]);
					} else if (i>=b.count() && j>=b.count()){
						i = i-b.count();
						j = j-b.count();
						swap(a[i], a[j]);
					}
				}
			}
		}
	}

	size_t al(T&& t){
        return b.add(std::forward<T>(t));
    }

	size_t ar(T&& t){
        return a.add(std::forward<T>(t));
    }

	size_t add(size_t k, T&& t){
        if (k < b.count()){
			k = b.count() - k - 1;
			return b.add(k, std::forward<T>(t));
		} else {
			k = k - b.count();
			return a.add(k, std::forward<T>(t));
		}
    }

	size_t dl(){
        return b.del();
    }

	size_t dr(){
        return a.del();
    }

	T del(size_t k){
		if (k < b.count()){
			k = b.count() - k - 1;
			return b.del(k);
		} else {
			k = k - b.count();
			return a.del(k);
		}
	}

	T& gl(){
		return b.get();
	}

	T& gr(){
		return a.get();
	}

	size_t find(T &&t) const{
        size_t idx = a.find(std::forward<T>(t));
		if (idx == -1){
			idx = b.find(std::forward<T>(t));
			if (idx == -1) return -1;
			return idx - b.count() - 1;
		} else {
			return idx + b.count();
		}
    }
};

template<class T> std::ostream& operator<<(std::ostream &out, deque<T> &o){
    for (size_t i=0; i<o.count(); i++){
        out << o[i] << " ";
    }
    out << std::endl;
    return out;
}

}


#endif //DEQUE_H
