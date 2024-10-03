//
// Created by 果汁 on 2024/9/23.
//

#ifndef FIBHEAP_H
#define FIBHEAP_H
#include <climits>
#include <cmath>

namespace ebstd{
template <typename T> struct node{
    T data{};
    int key{};
	int d{};
	bool f{};
	node *p;
	node *c;
	node *l;
	node *r;
    node()= default;
    explicit node(T e, int k, int d, bool f, node<T> *p=nullptr, node<T> *c=nullptr, node<T> *l=nullptr, node<T> *r=nullptr) : data(e), key(k), d(d), f(f), p(p), c(c), l(l), r(r){}
};

template <typename T> node<T>* make_node(){
	node<T> *x = nullptr;
	x = static_cast<node<T> *>(malloc(sizeof(node<T>)));
	if (nullptr == x) exit(1);
	memset(x, 0, sizeof(node<T>));
	x->l = x->r = x;
	return x;
}

template <typename T> struct heap{
	int num;
	node<T> *min;
	int md;
	node<T> **arr;
};

template <typename T> heap<T>* make_heap(){
	heap<T> *x = nullptr;
	x = static_cast<heap<T> *>(malloc(sizeof(heap<T>)));
	if (nullptr == x) exit(1);
	memset(x, 0, sizeof(heap<T>));
	return x;
}

//双链表相关操作
//从双链表中删除x
template <typename T> void del(node<T> *x){
	x->l->r = x->r;
	x->r->l = x->l;
}

//将x插入到在双链表中的y前面
template <typename T> void add(node<T> *x, node<T> *y){
	x->l = y->l;
	y->l->r = x;
	x->r = y;
	y->l = x;
}

template <typename T> void insert(heap<T> *pq, node<T> *x){
	if (pq->num == 0){
		pq->min = x;
	} else {
		add(x, pq->min);
		x->p = nullptr;
		if (x->key < pq->min->key){
			pq->min = x;
		}
	}
	++pq->num;
}

template <typename T> static void cut(heap<T> *pq, node<T> *x, node<T> *y){
	del(x);
	--y->d;
	if (x == x->r){
		y->c = nullptr;
	} else {
		y->c = x->r;
	}
	x->p = nullptr;
	x->l = x->r = x;
	x->f = false;
	add(x, pq->min);
}

template <typename T> static void cascadingCut(heap<T> *pq, node<T> *y){
	node<T> *z = y->p;
	if (z != nullptr){
		if (y->f == false){
			y->f = true;
		} else {
			cut(pq, y, z);
			cascadingCut(pq, z);
		}
	}
}

template <typename T> void decKey(heap<T> *pq, node<T> *x, int k){
    if (x->key < k) return;
    x->key = k;
    node<T> *y = x->p;
    if (y!=nullptr && x->key<y->key){
        cut(pq, x, y);
        cascadingCut(pq, y);
    }
    if (x->key < pq->min->key){
        pq->min = x;
    }
}

template <typename T> node<T>* extractMin(heap<T> *pq){
	node<T> *t = pq->min;
	node<T> *x = nullptr;
	if (t != nullptr){
		while (t->c != nullptr){
			x = t->c;
			del(x);
			if (x->r == x){
				t->c = nullptr;
			} else {
				t->c = x->r;
			}
			add(x, t);
			x->p = nullptr;
		}
		del(t);
		if (t->r == t){
			pq->min = nullptr;
		} else {
			pq->min = t->r;
			consolidate(pq);
		}
		--pq->num;
	}
	return t;
}

template <typename T> static void make_arr(heap<T> *pq){
	int old = pq->md;
	pq->md = static_cast<int>(log(pq->num * 1.0) / log(1.618)) + 1;
	if (old < pq->md){
		pq->arr = static_cast<node<T> **>(realloc(pq->arr, sizeof(node<T> *) * (pq->md + 1)));
		if (nullptr == pq->arr) exit(1);
	}
}

template <typename T> static node<T>* removeMin(heap<T> *pq){
	node<T> *t = pq->min;
	if (pq->min == t->r){
		pq->min = nullptr;
	} else {
		del(t);
		pq->min = t->r;
	}
	t->l = t->r = t;
	return t;
}

template <typename T> void link(heap<T> *pq, node<T> *y, node<T> *x){
    del(y);
    if (x->c == nullptr){
        x->c = y;
    } else {
        add(y, x->c);
    }
    y->p = x;
    ++x->d;
    y->f = false;
}

template <typename T> void consolidate(heap<T> *pq){
	node<T> *x = nullptr, *y = nullptr;
	make_arr(pq);
	const int D = pq->md + 1;
	for (int i=0; i<D; i++){
		*(pq->arr + i) = nullptr;
	}
	while (pq->min != nullptr){
		x = removeMin(pq);
		int d = x->d;
		while (*(pq->arr+d) != nullptr){
			y = *(pq->arr + d);
			if (x->key > y->key){
				swap(x, y);
			}
			link(pq, y, x);
			*(pq->arr + d) = nullptr;
			++d;
		}
		*(pq->arr + d) = x;
	}

	for (int i=0; i<D; i++){
		if (*(pq->arr+i) != nullptr){
			if (pq->min == nullptr){
				pq->min = *(pq->arr + i);
			} else {
				add(*(pq->arr + i), pq->min);
				if (*(pq->arr + i)->key < pq->min->key){
					pq->min = *(pq->arr + i);
				}
			}
		}
	}
}

template <typename T> void remove(heap<T> *pq, node<T> *x){
	decKey(pq, x, INT_MIN);
	extractMin(pq);
}

template <typename T> node<T>* search(heap<T> *pq, int k){
	return dfs(pq->min, k);
}

template <typename T> static node<T>* dfs(node<T> *x, int k){
	node<T> *t = x, *y = nullptr;
	if (x != nullptr){
		do {
			if (t->key == k){
				y = t;
				break;
			} else if (nullptr != (y = dfs(t->c, k))){
				break;
			}
			t = t->r;
		} while (t != x);
	}
	return y;
}

template <typename T> heap<T>* create(const int keys[], T datas[]){
	constexpr int n = sizeof(keys) / sizeof(int);
	heap<T> *pq = nullptr;
	pq = make_heap<T>();
	node<T> *x = nullptr;
	for (int i=0; i<n; i++){
		x = make_node<T>();
		x->key = keys[i];
        x->data = datas[i];
		insert(pq, x);
	}
	return pq;
}

template <typename T> static void unionNode(node<T> *a, node<T> *b){
	node<T> *t = a->r;
    a->r = b->r;
    b->r->l = a;
    b->r = t;
    t->l = b;
}

//合并2个斐波那契堆
template <typename T> heap<T>* unionHeap(heap<T> *a, heap<T> *b){
	if (a == nullptr || a->min==nullptr){
		free(a->arr);
		free(a);
		return b;
	}
	if (b == nullptr || b->min==nullptr){
		free(b->arr);
		free(b);
		return a;
	}
	unionNode(a->min, b->min);
	if (a->min->key > b->min->key){
		a->min = b->min;
	}
	a->num += b->num;
	free(b->arr);
	free(b);
	return a;
}

template <typename T> static void dfs2(node<T> *x){
	node<T> *t = x, y = nullptr;
	while (t != nullptr){
		dfs2(t->c);
		y = t;
		if (t->l == x){
			t = nullptr;
		} else {
			t = t->l;
		}
		free(y->r);
	}
}

template <typename T> void destory(heap<T> *pq){
    dfs2(pq->min);
    free(pq->arr);
    free(pq);
    //pq = NULL;
}
}
#endif //FIBHEAP_H
