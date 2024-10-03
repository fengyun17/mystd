//
// Created by 果汁 on 2024/9/21.
//

#ifndef HEAP_H
#define HEAP_H
namespace ebstd{
template <typename T> struct node{
    T data;
    node<T>* p, l, r;
    node() : p(nullptr), l(nullptr), r(nullptr){}
    explicit node(T e, node<T>* pa=nullptr, node<T>* lc=nullptr, node<T>* rc=nullptr) : data(e), p(pa), l(lc), r(rc){}
};

template <typename T> static void swap(node<T> *cur, node<T> *p){//先其拓扑，再其本身，每个拓扑连接都是2个(双向)
	if (cur== nullptr || p == nullptr) return;
	node<T> pp = p->p;
	cur->p = pp;
	if (pp != nullptr){
		if (p == pp->l){
			pp->l = cur;
		}else{
			pp->r = cur;
		}
	}
	pp = cur;
	if (cur->l != nullptr){
		cur->l->p = p;
	}
	if (cur->r != nullptr){
		cur->r->p = p;
	}
	if (cur == p->l){
		cur->l = p;
		cur->r = p->r;
		if (p->r != nullptr){
			p->r->p = cur;
		}
	}else{
		cur->l = p->l;
		cur->r = p;
		if (p->l != nullptr){
			p->l->p = cur;
		}
	}
	p->l = cur->l;
	p->r = cur->r;
}

template <typename T> struct heap {
	virtual ~heap() = default;

	virtual void add(T) = 0;
	virtual T& get() = 0;
	virtual T del() = 0;
};

template <typename T> class bpq : public heap<T>{
protected:
	node<T> *head;
	node<T> *last;
	int cnt{};
	bool (*com)(T, T);

	void heapify(){
		node<T> *cur = last;
		node<T> *p = cur->p;
		if (p!=nullptr && com(cur->data, p->data)) last = p;
		while (p!=nullptr && com(cur->data, p->data)){
			swap(cur, p);
			p = cur->p;
		}
		if (head->p != nullptr){
			head = head->p;
		}
	}

	void heapify(node<T> *cur){
		node<T> *l = cur->l;
		node<T> *r = cur->r;
		node<T> *now = cur;
		while (l != nullptr){
			if (com(l->data, now->data)) now = l;
			if (r!=nullptr && com(r->data, now->data)) now = r;
			if (now != cur){
				swap(now, cur);
			}else{
				break;
			}
			l = cur->l;
			r = cur->r;
			now = cur;
		}
		if (cur->p == last) last = cur;
		while (cur->p != nullptr){
			cur = cur->p;
		}
		head = cur;
	}

	static node<T>* mostLeft(node<T> *cur){
		while (cur->l != nullptr){
			cur = cur->l;
		}
		return cur;
	}

	static node<T>* mostRight(node<T> *cur){
		while (cur->r != nullptr){
			cur = cur->r;
		}
		return cur;
	}

	node<T>* setPreLast(){
		node<T> *cur = last;
		node<T> *p = cur->p;
		while (p!=nullptr && cur!=p->r){
			cur = p;
			p = cur->p;
		}
		if (p == nullptr){
			cur = last;
			p = cur->p;
			cur->p = nullptr;
			if (cur == p->l){
				p->l = nullptr;
			}else{
				p->r = nullptr;
			}
			last = mostRight(head);
		}else{
			node<T> *now = mostRight(p->l);
			cur = last;
			p = cur->p;
			cur->p = nullptr;
			if (cur == p->l){
				p->l = nullptr;
			}else{
				p->r = nullptr;
			}
			last = now;
		}
		cnt--;
		return cur;
	}

public:
	bpq()= default;

    explicit bpq(bool (*c)(T, T)){
        size = 0;
		com = c;
		head = nullptr;
		last = nullptr;
	}

	T& get() override {
        return head == nullptr ? nullptr : head->data;
    }

	[[nodiscard]] int size() const {
		return cnt;
	}
	[[nodiscard]] bool empty() const {
		return cnt == 0 ? true : false;
	}

	void add(T e) override {
		auto *now = new node<T>(e);
		if (cnt == 0){
			head = now;
			last = now;
			cnt++;
			return;
		}
		node<T> *cur = last;
		node<T> *p = cur->p;
		while (p!=nullptr && cur!=p->l){
			cur = p;
			p = cur->p;
		}
		node<T> *conn = nullptr;
		if (p == nullptr){
			conn = mostLeft(head);
			conn->l = now;
			now->p = conn;
		} else if (p->r == nullptr){
			p->r = now;
			now->p = p;
		} else {
			conn = mostLeft(p->r);
			conn->l = now;
			now->p = conn;
		}
		last = now;
		heapify();
		cnt++;
	}

	T del() override {
		if (cnt == 0) return nullptr;
		node<T> *res = head;
		if (cnt == 1){
			head = nullptr;
			last = nullptr;
			cnt--;
			return res->data;
		}
		node<T> *old = setPreLast();
		if (cnt == 1){
			head = old;
			last = old;
			return res->data;
		}
		old->l = res->l;
		if (res->l != nullptr){
			res->l->p = old;
		}
		old->r = res->r;
		if (res->r != nullptr){
			res->r->p = old;
		}
		res->l = nullptr;
		res->r = nullptr;
		head = old;
		heapify(old);
		return res->data;
	}
};

}

#endif //HEAP_H
