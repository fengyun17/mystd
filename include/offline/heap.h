//
// Created by 果汁 on 2024/9/22.
//

#ifndef HEAP_H
#define HEAP_H
#include "vector.h"

namespace ebstd{
template <typename T> struct heap {
    virtual ~heap() = default;

    virtual void add(T) = 0;
    virtual T& get() = 0;
    virtual T del() = 0;
};

template <typename T> class apq : public heap<T>, public vector<T>{
	template<class K> friend std::ostream& operator<<(std::ostream &out, apq<K> const &o);

protected:
	size_t search(size_t l, size_t r, size_t i) {
        while (l < r){
            if (const size_t m = l + ((r - l) >> 1); this->com(this->data[((i + 1) >> m) - 1], this->data[i])){
                l = m + 1;
            } else {
                r = m;
            }
        }
        return --l;
    }

	void up(size_t i){
        T t = this->data[i];
        const int h = static_cast<size_t>(log(this->size) / log(2)) + 1;
        const int a = search(0, h-1, i);
        const int b = (static_cast<int>(i + 1) >> a) - 1;
        while (i > b){
            this->data[i] = this->data[(i-1)/2];
            i = (i - 1) / 2;
        }
        this->data[i] = this->data[(i-1)/2];
        i = (i - 1) / 2;
        this->data[i] = t;
    }

	void down(size_t i){
        T t = this->data[i];
        i = i * 2 + 1;
        while(i < this->size){
            if (i+1<this->size && this->com(this->data[i], this->data[i+1])) i++;
            if (com(this->data[i], t)){
                break;
            } else {
                this->data[(i-1)/2] = this->data[i];
                i = i * 2 + 1;
            }
        }
        this->data[(i-1)/2] = t;
    }

public:
	using vector<T>::vector;

	explicit apq(bool (*c)(T, T)){
		this->com = c;
	}

	~apq() override {
        free(this->data);
    }

    [[noreturn]] void create(bool (*c)(T, T), size_t n, T *t){
        copy(t, 0, n);
		this->com = c;
		for (int i=this->size/2-1; i>=0; --i){
            down(i);
        }
    }

	void add(T&& t) override {
        this->add(this->size, std::forward<T>(t));
		up(this->size-1);
    }

	T del() override {
        T res = this->data[0];
        this->data[0] = this->data[this->size - 1];
        this->del(this->size-1, this->size);
        down(0);
        return res;
    }

	T& get() override {
        return this->data[0];
    }

	void sort(){
        for(int i=this->size-1; i>0; --i){
            swap(this->data[0], this->data[i]);
            --this->size;
            down(0);
        }
    }
};

template<class T> std::ostream& operator<<(std::ostream &out, apq<T> &o){
    for (size_t i=0; i<o.size; i++){
        out << o[i] << " ";
    }
    out << std::endl;
    return out;
}

}

#endif //HEAP_H
