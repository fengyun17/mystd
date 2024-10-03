//
// Created by 果汁 on 2024/9/21.
//

#ifndef BBST_H
#define BBST_H
#include <cmath>



namespace ebstd{
typedef enum {red, black} color;

template <typename T> struct node{
	T data;
	node<T> *p, *l, *r;
	int h;
	color c;

	node() : p(nullptr), l(nullptr), r(nullptr), h(0), c(red){}
	explicit node(T e, node<T> *p=nullptr, node<T> *l=nullptr, node<T> *r=nullptr, int h=0, color c=red) : data(e), p(p), l(l), r(r), h(h), c(c){}

	node<T> *succ(){
		node<T> *cur = this;
		if (r){
			cur = r;
			while (cur->l){
				cur = cur->l;
			}
		} else {
			while (cur->p && cur==cur->p->r){
				cur = cur->p;
			}
			cur = cur->p;
		}
		return cur;
	}
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

template <typename T> static node<T> *del(node<T> *&x, node<T> *&hot){
	node<T> *cur = x;
	node<T> *s = nullptr;
	if (!(x->l)){
		s = x = x->r;
	} else if (!(x->r)){
		s = x = x->l;
	} else {
		cur = cur->succ();
		swap(x, cur);
		node<T> *t = cur->p;
		((t == x) ? t->r : t->l) = s = cur->r;
	}
	hot = cur->p;
	if (s){
		s->p = hot;
	}
	free(cur->data);
	free(cur);
	cur = nullptr;
	return s;
}

template <typename T> class tree{
protected:
	int cnt{};
	node<T> *root{};

	virtual int update(node<T> *x){
		return x->h = 1 + max(x->l ? x->l->h : -1, x->r ? x->r->h : -1);
	}

	void cascade(node<T> *x){
		while (x){
			update(x);
			x = x->p;
		}
	}

public:
	virtual ~tree() = default;

	[[nodiscard]] int size() const {
		return cnt;
	}

	bool empty(){
		return !root;
	}
};

template <typename T> class bbst : public tree<T>{
protected:
	node<T> *hot{};
	bool (*com)(T, T);
	bool (*eq)(T, T);

	node<T> *conn34(node<T> *a, node<T> *b, node<T> *c, node<T> *t0, node<T> *t1, node<T> *t2, node<T> *t3){
		a->l = t0;
		if (t0) t0->p = a;
		a->r = t1;
		if (t1) t1->p = a;
		update(a);

		c->l = t2;
		if (t2) t2->p = c;
		c->r = t3;
		if (t3) t3->p = c;
		update(c);

		b->l = a;
		a->p = b;
		b->r = c;
		c->p = b;
		update(b);
		return b;
	}

	node<T> *rotate(node<T> *v){
		node<T> *p = v->p;
		node<T> *g = p->p;
		if (p->p && p==p->p->l){ //zig
			if (v->p && v==v->p->l){ //zig-zig
				p->p = g->p; //旋转后的根向上连接
				return conn34(v, p, g, v->l, v->r, p->r, g->r);
			} else { //zig-zag
				v->p = g->p; //旋转后的根向上连接
				return conn34(p, v, g, p->l, v->l, v->r, g->r);
			}
		} else { //zag
			if (v->p && v==v->p->r){ //zag-zag
				p->p = g->p; //旋转后的根向上连接
				return conn34(g, p, v, g->l, p->l, v->l, v->r);
			} else { // zag-zig
				v->p = g->p; //旋转后的根向上连接
				return conn34(g, v, p, g->l, v->l, v->r, p->r);
			}
		}
	}

public:
	bbst(bool (*c)(T, T), bool (*e)(T, T)){
		com = c;
		eq = e;
	}

	//返回的是来自父节点的左孩子或右孩子的指针的地址(接下来可能要进行拓扑连接(必须是2级指针))
	node<T> *&find(node<T> *&v, const T &e, node<T> *&h){
		if (!v || eq(v->data, e)) return v;
		h = v;
		return find((com(v->data, e) ? v->l : v->r), e, h);
	}

	virtual node<T> *&find(const T &e){
		return find(this->root, e, hot=nullptr);
	}

	virtual node<T> *add(const T &e) = 0;
	virtual bool del(const T &e) = 0;
};

template <typename T> class spt : public bbst<T>{
protected:
	node<T> *splay(node<T> *v){ //将v伸展到根(经过zig-zig或zag-zag的链的高度都减半)
		if (!v) return nullptr;
		node<T> *p, *g;
		while (p==v->p && g==p->p){
			node<T> *gg = g->p;
			if (v->p && v==v->p->l){
				if (p->p && p==p->p->l){ //zig-zig
					g->l = p->r;
					if (p->r) p->r->p = g;
					p->l = v->r;
					if (v->r) v->r->p = p;
					p->r = g;
					g->p = p;
					v->r = p;
					p->p = v;
				} else { //zig-zag
					p->l = v->r;
					if (v->r) v->r->p = p;
					g->r = v->l;
					if (v->l) v->l->p = g;
					v->l = g;
					g->p = v;
					v->r = p;
					p->p = v;
				}
			} else if (p->p && p==p->p->r){ //zag-zag
				g->r = p->l;
				if (p->l) p->l->p = g;
				p->r = v->l;
				if (v->l) v->l->p = p;
				p->l = g;
				g->p = p;
				v->l = p;
				p->p = v;
			} else { // zag-zig
				p->r = v->l;
				if (v->l) v->l->p = p;
				g->l = v->r;
				if (v->r) v->r->p = g;
				v->r = g;
				g->p = v;
				v->l = p;
				p->p = v;
			}
			if (!gg){
				v->p = nullptr;
			} else {
				if (g == gg->l){
					gg->l = v;
					v->p = gg;
				} else {
					gg->r = v;
					v->p = gg;
				}
			}
			update(g);
			update(p);
			update(v);
		}
		if (p == v->p){ //需要最后再做一次单旋
			if (v == v->p->l){
				p->l = v->r;
				if (v->r) v->r->p = p;
				v->r = p;
				p->p = v;
			} else {
				p->r = v->l;
				if (v->l) v->l->p = p;
				v->l = p;
				p->p = v;
			}
			update(p);
			update(v);
		}
		v->p = nullptr;
		return v;
	}

public:
	using bbst<T>::bbst;

	node<T> *&find(const T &e) override {
		node<T> *p = this->find(this->root, e, this->hot=nullptr);
		this->root = splay(p ? p : this->hot);
		return this->root;
	}

	node<T> *add(const T &e) override {
		if (!this->root){
			++this->cnt;
			return this->root = new node<T>(e);
		}
		if (this->eq(find(e)->data, e)){ //关键码重复不再插入
			return this->root;
		}
		++this->cnt;
		node<T> *t = this->root;
		if (this->com(this->root->data, e)){
			t->p = this->root = new node<T>(e, nullptr, t, t->r);
			if (t->r){
				t->r->p = this->root;
				t->r = nullptr;
			}
		} else {
			t->p = this->root = new node<T>(e, nullptr, t->l, t);
			if (t->l){
				t->l->p = this->root;
				t->l = nullptr;
			}
		}
		cascade(t);
		return this->root;
	}

	bool del(const T &e) override {
		if (!this->root || !(eq(find(e)->data, e))) return false;
		node<T> *t = this->root;
		if (!(t->l)){
			this->root = this->root->r;
			if (this->root){
				this->root->p = nullptr;
			}
		} else if (!(t->r)){
			this->root = this->root->l;
			if (this->root){
				this->root->p = nullptr;
			}
		} else {
			node<T> *lt = this->root->l;
			lt->p = nullptr;
			this->root->l = nullptr;
			this->root = this->root->r;
			this->root->p = nullptr;
			find(t->data);
			this->root->l = lt;
			lt->p = this->root;
		}
		free(t->data);
		free(t);
		t = nullptr;
		--this->cnt;
		if (this->root){
			update(this->root);
		}
		return true;
	}
};

//一级指针p, 二级指针x
#define height(p) ((p) ? (p)->h : -1)
#define isRoot(x) (!((x).p))
#define isL(x) (!isRoot(x) && (&(x)==(x).p->l))
#define sibling(p) (isL(*(p)) ? (p)->p->r : (p)->p->l)
#define uncle(p) (isL(*((p)->p)) ? (p)->p->p->r : (p)->p->p->l)
#define fromP(x) (isRoot(x) ? this->root : (isL(x) ? (x).p->l : (x).p->r)) //返回的是来自父节点的左孩子或右孩子的指针的地址(接下来可能要进行拓扑连接(必须是2级指针))
#define isBlack(p) (!(p) || black==(p)->c)
#define isRed(p) (!isBlack(p))
#define balFac(x) (height((x).l)==height((x).r) && ((x).h==(isRed(&x) ? height((x).l) : height((x).l)+1)))

template <typename T> class rbt : public bbst<T>{
protected:
	int update(node<T> *x) override {
		x->h = max(height(x->l), height(x->r));
		return isBlack(x) ? x->h++ : x->h;
	}

	void doubleRed(node<T> *x){
		if (isRoot(*x)){
			this->root->c = black;
			++this->root->h;
			return;
		}
		node<T> *p = x->p;
		if (isBlack(p)) return;
		node<T> *g = p->p; //p是红，则g必然存在且是黑
		if (node<T> *u = uncle(x); isBlack(u)){
			if (isL(*x) == isL(*p)){ //zig-zig或zag-zag
				p->c = black;
			} else { //zig-zag或zag-zig
				x->c = black;
			}
			g->c = red;
			node<T> *gg = g->p;
			node<T> *t = fromP(*g) = rotate(x);
			t->p = gg;
		} else {
			p->c = black;
			++p->h;
			u->c = black;
			++u->h;
			if (!isRoot(*g)) g->c = red;
			doubleRed(g);
		}
	}

	void doubleBlack(node<T> *x){
		node<T> *p = x ? x->p : this->hot;
		if (!p) return;
		node<T> *s = sibling(x);
		if (isBlack(s)){
			node<T> *t = nullptr;
			if (isRed(s->r)) t = s->r;
			if (isRed(s->l)) t = s->l;
			if (t){ //2-1
				color old = p->c;
				node<T> *b = fromP(*p) = rotate(t);
				if (b->l){
					b->l->c = black;
					update(b->l);
				}
				if (b->r){
					b->r->c = black;
					update(b->r);
				}
				b->c = old;
				update(b);
			} else {
				s->c = red;
				--s->h;
				if (isRed(p)){ //2-2-1
					p->c = black; //高度抵消了(不变)
				} else { //2-2-2
					--p->h; //高度减小
					doubleBlack(p);
				}
			}
		} else { //2-3
			s->c = black;
			p->c = red;
			node<T> *t = isL(*s) ? s->l : s->r;
			this->hot = p;
			fromP(*p) = rotate(t);
			doubleBlack(x);
		}
	}

public:
	using bbst<T>::bbst;

	node<T> *add(const T &e) override {
		node<T> *&x = find(e);
		if (x) return x;
		x = new node<T>(e, this->hot, nullptr, nullptr, -1);
		++this->cnt;
		doubleRed(x);
		return x;
	}

	bool del(const T &e) override {
		node<T> *&x = find(e);
		if (!x) return false;
		node<T> *t = del(x, this->hot);
		if (!(--this->cnt)) return true;
		if (!this->hot){
			this->root->c = black;
			update(this->root);
			return true;
		}
		if (balFac(*this->hot)) return true; //同时也检测出了连续2个红(3)，并且x为红且t为黑一定满足
		if (isRed(t)){
			t->c = black;
			++t->h;
			return true;
		}
		//x和t都是黑
		doubleBlack(t);
		return true;
	}
};

}

#endif //BBST_H
