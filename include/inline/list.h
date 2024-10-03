//
// Created by 果汁 on 2024/9/21.
//

#ifndef LIST_H
#define LIST_H
namespace ebstd{
template <typename T> struct node{
	T data{};
	node<T> *p{};
	node<T> *s{};
	
	node()= default;
	explicit node(T e, node<T> *p=nullptr, node<T> *s=nullptr) : data(e), p(p), s(s){}
	
	
	node<T>* ap(T const &e){
		auto *cur = new node(e, p, this);
		p->s = cur;
		p = cur;
		return cur;
	}
	
	node<T>* as(T const &e){
		auto *cur = new node(e, this, s);
		s->p = cur;
		s = cur;
		return cur;
	}
};

template<typename T> class list{
private:
	int cnt{};
	node<T> *h;
	node<T> *t;

public:
	void init(){
		h = new node<T>();
		t = new node<T>();
		h->s = t;
		h->p = nullptr;
		t->s = nullptr;
		t->p = h;
		cnt = 0;
	}
	
	node<T>* ah(T const &e){
		cnt++;
		return h->as(e);
	}
	
	node<T>* at(T const &e){
		cnt++;
		return t->ap(e);
	}
	
	node<T>* as(node<T> *cur, T const &e){
		cnt++;
		return cur->as(e);
	}
	
	node<T>* ap(node<T> *cur, T const &e){
		cnt++;
		return cur->ap(e);
	}
	
	T del(node<T> *cur){
		T e = cur->data;
		cur->p->s = cur->s;
		cur->s->p = cur->p;
		delete cur;
		cnt--;
		return e;
	}
	
	T dh(){
		return del(h->s);
	}
	
	T dt(){
		return del(t->p);
	}
	
	[[nodiscard]] int size() const{
		return cnt;
	}
	
	[[nodiscard]] bool empty() const{
		return cnt==0;
	}
	
	node<T>* find(T const &e){
		int n = cnt;
		node<T> *cur = t;
		while (n--){
			if (e == (cur=cur->p)->data) return cur;
		}
		return nullptr;
	}
	
	void copy(node<T> *cur, int n){
		init();
		while (n--){
			at(cur->data);
			cur = cur->s;
		}
	}
	
	T& operator[](int n) const{
		node<T> *cur = h->s;
		while (n--){
			cur = cur->s;
		}
		return cur->data;
	}
	
	list(){
		init();
	}
	
	list(list<T> const &other){
		copy(other.h->s, other.size());
	}
	
	list(list<T> const &other, int l, int r){
		int n = r - l;
		copy(other[l], n);
	}
	
	int clear(){
		int res = cnt;
		while (cnt){
			del(h->s);
		}
		return res;
	}
	
	~list(){
		clear();
		delete h;
		delete t;
	}
};
}
#endif //LIST_H
