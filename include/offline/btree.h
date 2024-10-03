//
// Created by 果汁 on 2024/9/22.
//

#ifndef BTREE_H
#define BTREE_H
/**
https://www.cnblogs.com/newhand-liu/archive/2012/09/15/2793266.html
https://www.cnblogs.com/coreLeo/p/17296876.html
https://www.jianshu.com/p/c4633978f54f
*/
//分裂(加->上->只加key)，合并(减->下->删key)
//因为分裂(添加)向上->树向上生长->兄弟节点要么都是叶节点，要么都不是叶节点
#include "vector.h"
namespace ebstd{
template <typename T, typename U> struct node{
	bool leaf;
	bool (*eq)(T, T);
	bool (*com)(T, T);
    node<T, U> *parent;
	node<T, U> *left;
	node<T, U> *right;
    vector<T> key;
	vector<U> data;
    vector<node<T, U>*> child;
    node(bool (*e)(T, T), bool (*c)(T, T)){
		eq = e;
		com = c;
		leaf = false;
		parent = nullptr;
		left = nullptr;
		right = nullptr;
		child.add(0, nullptr);
	}
};

template <typename T, typename U> class btree{
protected:
    int cnt;
    int ord;
	bool (*eq)(T, T);
	bool (*com)(T, T);
    node<T, U> *root;
    node<T, U> *hot;
    void overflow(node<T, U>*);
    void add(node<T, U>*);
    void underflow(node<T, U>*);
    bool find2l(const T &e);

public:
    btree(bool (*e)(T, T), bool (*c)(T, T), const int order=16) : cnt(0), ord(order), hot(nullptr) {
	    eq = e;
	    com = c;
	    root = new node<T, U>(eq, com);
    }

    ~btree(){
        delete root; root = nullptr;
    }

    [[nodiscard]] int order() const {return ord;}
    [[nodiscard]] int size() const {return cnt;}
    [[nodiscard]] bool empty() const {return !root ? false : true;}
    node<T, U>* find(const T &e);
	vector<U> find(const T &l, const T &r);
    bool add(const T &e, const U &d);
    bool del(const T &e);
};

template <typename T, typename U> node<T, U>* btree<T, U>::find(const T &e){
    node<T, U> *v = root;
    hot = nullptr;
    while (v){
        int r = v->key.bs(e);
        if ((0<=r) && eq(e, v->key[r])) return v;
        hot = v;
        v = v->child[r];
    }
    return nullptr;
}

template <typename T, typename U> bool btree<T, U>::add(const T &e, const U &d=0){
	if (find(e)) return false;
    const int r = hot->key.bs(e);
	hot->leaf = true;
    hot->key.add(r+1, e);
	hot->data.add(r+1, d);
    hot->child.add(r+1, nullptr);
    cnt++;
    add(hot);
    return true;
}

template <typename T, typename U> void btree<T, U>::add(node<T, U> *v){
    if (order >= v->key.size()) return;
    if (!v->parent){
        auto *u = new node<T, U>(eq, com);
        int s = v->key.size() / 2;
        const int k = v->key.size();
		if (v->leaf){
			v->right = u;
			u->left = v;
			for (int j=0, i=s; j<v->key.size()-s; j++){
				u->key.add(j, v->key[i]);
				u->data.add(j, v->data[i]);
				u->child.add(j, v->child[i++]);
			}
			for (int j=0; j<k-s; j++){
				v->key.del(v->key.size()-1);
				v->data.del(v->data.size()-1);
				v->child.del(v->child.size()-1);
			}

		} else {
			for (int j=0, i=s; j<v->key.size()-s; j++){
				u->key.add(j, v->key[i]);
				u->child.add(j, v->child[i++]);
			}
			for (int j=0; j<k-s; j++){
				v->key.del(v->key.size()-1);
				v->child.del(v->child.size()-1);
			}
		}
        auto p = new node<T, U>(eq, com);
    	root = p;
        p->child[0] = v;
        v->parent = p;
        p->child.add(1, u);
        u->parent = p;
        p->key.add(0, v->key[0]);
        p->key.add(1, u->key[0]);
        return;
    }
    overflow(v);
}

template <typename T, typename U> void btree<T, U>::overflow(node<T, U> *v){
    node<T, U> *p = v->parent;
    int r = 0;
    while (p->child[r] != v){
        r++;
    }
    if (0 < r){
        node<T, U> *ls = p->child[r-1];
        if (ord > ls->key.size()){
            ls->key.add(v->key.del(0));
			if (v->leaf){
				ls->data.add(v->data.del(0));
				if (!ls->right || !v->left){
					ls->right = v;
					v->left = ls;
				}
			}
            ls->child.add(v->child.del(0));
            ls->child[ls->child.size()-1]->parent = ls;
            p->key[r] = v->key[0];
            return;
        }
    }

    if (p->key.size()-1 > r){
        node<T, U> *rs = p->child[r+1];
        if (ord > rs->key.size()){
            rs->key.add(0, v->key.del(v->key.size()-1));
			if (v->leaf){
				rs->data.add(0, v->data.del(v->data.size()-1));
				if (!rs->left || !v->right){
					rs->left = v;
					v->right = rs;
				}
			}
            rs->child.add(0, v->child.del(v->child.size()-1));
            rs->child[0]->parent = rs;
            p->key[r] = rs->key[0];
            return;
        }
    }

    if (0 < r){
        node<T, U> *ls = p->child[r-1];
        p->key.add(r+1, v->key[ord-1]);
        p->child.add(r+1, new node<T, U>(eq, com));
        const int k = v->key.size();
		if (v->leaf){
			v->right = p->child[r+1];
			p->child[r+1]->left = v;
			for (int i=ord-1, j=0; i<v->key.size(); i++){
				p->child[r+1]->key.add(v->key[i]);
				p->child[r+1]->data.add(v->data[i]);
				p->child[r+1]->child.add(v->child[i]);
				p->child[r+1]->child[j++]->parent = p->child[r+1];
			}
			for (int i = ord-1; i<k; i++){
				v->key.del(v->key.size()-1);
				v->data.del(v->data.size()-1);
				v->child.del(v->child.size()-1);
			}
		} else {
			for (int i=ord-1, j=0; i<v->key.size(); i++){
				p->child[r+1]->key.add(v->key[i]);
				p->child[r+1]->child.add(v->child[i]);
				p->child[r+1]->child[j++]->parent = p->child[r+1];
			}
			for (int i = ord-1; i<k; i++){
				v->key.del(v->key.size()-1);
				v->child.del(v->child.size()-1);
			}
		}
		v->key.add(0, ls->key.del(ls->key.size()-1));
		v->child.add(0, ls->child.del(ls->child.size()-1));
        v->child[0]->parent = v;
        p->key[r] = v->key[0];
		if (v->leaf){
			v->data.add(0, ls->data.del(ls->data.size()-1));
			if (!v->left || !ls->right){
				v->left = ls;
				ls->right = v;
			}
		}
    } else {
        node<T, U> *rs = p->child[r+1];
        p->key.add(r+2, rs->key[ord-2]);
        p->child.add(r+2, new node<T, U>(eq, com));
		int k = rs->key.size();
		if (rs->leaf){
			rs->right = p->child[r+2];
			p->child[r+2]->left = rs;
			for (int i=ord-2, j=0; i<rs->key.size(); i++){
				p->child[r+2]->key.add(rs->key[i]);
				p->child[r+2]->data.add(rs->data[i]);
				p->child[r+2]->child.add(rs->child[i]);
				p->child[r+2]->child[j++]->parent = p->child[r+2];
			}
			for (int i=ord-2; i<k; i++){
				rs->key.del(rs->key.size()-1);
				rs->data.del(rs->data.size()-1);
				rs->child.del(rs->child.size()-1);
			}
		} else {
			for (int i=ord-2, j=0; i<rs->key.size(); i++){
				p->child[r+2]->key.add(rs->key[i]);
				p->child[r+2]->child.add(rs->child[i]);
				p->child[r+2]->child[j++]->parent = p->child[r+2];
			}
			for (int i=ord-2; i<k; i++){
				rs->key.del(rs->key.size()-1);
				rs->child.del(rs->child.size()-1);
			}
		}
        rs->key.add(0, v->key.del(v->key.size()-1));
        rs->child.add(0, v->child.del(v->child.size()-1));
        rs->child[0]->parent = rs;
        p->key[r+1] = rs->key[0];
		if (v->leaf){
			rs->data.add(0, v->data.del(v->data.size()-1));
			if (!v->right || !rs->left){
				v->right = rs;
				rs->left = v;
			}
		}
    }
    add(p);
}

template <typename T, typename U> bool btree<T, U>::find2l(const T &e){
    node<T, U> *v = root;
    hot = nullptr;
    bool flag = false;
    while (v){
        int r = v->key.bs(e);
        if ((0<=r) && eq(e, v->key[r])) flag = true;
        hot = v;
        v = v->child[r];
    }
    return flag;
}

template <typename T, typename U> bool btree<T, U>::del(const T &e){
    if (!find2l(e)) return false;
    int r = hot->key.bs(e);
    hot->key.del(r);
	hot->data.del(r);
    hot->child.del(r);
    cnt--;
    underflow(hot);
    return true;
}

template <typename T, typename U> void btree<T, U>::underflow(node<T, U> *v){
    if ((ord + 1) / 2 <= v->key.size()) return;
    if (v->parent && !(v->parent->parent)){
        node<T, U> *p = v->parent;
        int r = 0;
        while (p->child[r] != v){
            r++;
        }
        if (p->key.size() == 2){
            if (r > 0){
                node<T, U> *ls = p->child[r-1];
                if ((ord + 1) / 2 < ls->key.size()){
                    v->key.add(0, ls->key.del(ls->key.size()-1));
                    v->child.add(0, ls->child.del(ls->child.size()-1));
                    v->child[0]->parent = v;
                    p->key[r] = v->key[0];
					if (v->leaf){
						v->data.add(0, ls->data.del(ls->data.size()-1));
					}
                    return;
                } else {
                    for (int i=0, j=ls->child.size(); i<v->key.size(); i++){
                        ls->key.add(v->key[i]);
                        ls->child.add(v->child[i]);
                        ls->child[j++]->parent = ls;
                    }
                    int k = v->key.size();
                    for (int i = 0; i<k; i++){
                        v->key.del(v->key.size()-1);
                        v->child.del(v->child.size()-1);
                    }
					if (v->leaf){
						for (int i=0; i<v->key.size(); i++){
							ls->data.add(v->data[i]);
						}
						for (int i = 0; i<k; i++){
							v->data.del(v->data.size()-1);
						}
						ls->right = v->right;
					}
                    v->parent = nullptr;
					v->left = nullptr;
					v->right = nullptr;
					p->key.del(1);
                    p->key.del(0);
                    delete v;
                    p->child[1] = nullptr;
                    root = ls;
                    root->parent = nullptr;
                    return;
                }
            } else {
	            if (node<T, U> *rs = p->child[r+1]; (ord + 1) / 2 < rs->key.size()){
                    v->key.add(rs->key.del(0));
                    v->child.add(rs->child.del(0));
                    v->child[v->child.size()-1]->parent = v;
                    p->key[r+1] = rs->key[0];
					if (v->leaf){
						v->data.add(rs->data.del(0));
					}
                    return;
                } else {
                    for (int i=0, j=v->child.size(); i<rs->key.size(); i++){
                        v->key.add(rs->key[i]);
                        v->child.add(rs->child[i]);
                        v->child[j++]->parent = v;
                    }
                    int k = rs->key.size();
                    for (int i = 0; i<k; i++){
                        rs->key.del(rs->key.size()-1);
                        rs->child.del(rs->child.size()-1);
                    }
					if (v->leaf){
						for (int i=0; i<rs->key.size(); i++){
							v->data.add(rs->data[i]);
						}
						for (int i = 0; i<k; i++){
							rs->data.del(rs->data.size()-1);
						}
						v->right = rs->right;
					}
                    rs->parent = nullptr;
					rs->left = nullptr;
					rs->right = nullptr;
					p->key.del(1);
                    p->key.del(0);
                    delete rs;
                    p->child[1] = nullptr;
                    root = v;
                    root->parent = nullptr;
                    return;
                }
            }
        } else {
            if (r==0){
	            if (node<T, U> *rs = p->child[r+1]; (ord + 1) / 2 < rs->key.size()){
                    v->key.add(rs->key.del(0));
                    v->child.add(rs->child.del(0));
                    v->child[v->child.size()-1]->parent = v;
                    p->key[r+1] = rs->key[0];
					if (v->leaf){
						v->data.add(rs->data.del(0));
					}
                    return;
                } else {
                    for (int i=0, j=v->child.size(); i<rs->key.size(); i++){
                        v->key.add(rs->key[i]);
                        v->child.add(rs->child[i]);
                        v->child[j++]->parent = v;
                    }
                    int k = rs->key.size();
                    for (int i = 0; i<k; i++){
                        rs->key.del(rs->key.size()-1);
                        rs->child.del(rs->child.size()-1);
                    }
					if (v->leaf){
						for (int i=0; i<rs->key.size(); i++){
							v->data.add(rs->data[i]);
						}
						for (int i = 0; i<k; i++){
							rs->data.del(rs->data.size()-1);
						}
						v->right = rs->right;
					}
                    p->key.del(1);
                    rs->parent = nullptr;
					rs->left = nullptr;
					rs->right = nullptr;
                    p->child.del(1);
                    delete rs;
                    return;
                }
            } else if (r==p->key.size()-1){
                node<T, U> *ls = p->child[r-1];
                if ((ord + 1) / 2 < ls->key.size()){
                    v->key.add(0, ls->key.del(ls->key.size()-1));
                    v->child.add(0, ls->child.del(ls->child.size()-1));
                    v->child[0]->parent = v;
                    p->key[r] = v->key[0];
					if (v->leaf){
						v->data.add(0, ls->data.del(ls->data.size()-1));
					}
                    return;
                } else {
                    for (int i=0, j=ls->child.size(); i<v->key.size(); i++){
                        ls->key.add(v->key[i]);
                        ls->child.add(v->child[i]);
                        ls->child[j++]->parent = ls;
                    }
                    int k = v->key.size();
                    for (int i = 0; i<k; i++){
                        v->key.del(v->key.size()-1);
                        v->child.del(v->child.size()-1);
                    }
					if (v->leaf){
						for (int i=0; i<v->key.size(); i++){
							ls->data.add(v->data[i]);
						}
						for (int i = 0; i<k; i++){
							v->data.del(v->data.size()-1);
						}
						ls->right = v->right;
					}
                    p->key.del(r);
                    v->parent = nullptr;
					v->left = nullptr;
					v->right = nullptr;
					p->child.del(r);
                    delete v;
                    return;
                }
            } else {
                node<T, U> *rs = p->child[r+1];
                node<T, U> *ls = p->child[r-1];
                if ((ord + 1) / 2 < rs->key.size()){
                    v->key.add(rs->key.del(0));
                    v->child.add(rs->child.del(0));
                    v->child[v->child.size()-1]->parent = v;
                    p->key[r+1] = rs->key[0];
					if (v->leaf){
						v->data.add(rs->data.del(0));
					}
                    return;
                } else if ((ord + 1) / 2 < ls->key.size()){
                    v->key.add(0, ls->key.del(ls->key.size()-1));
                    v->child.add(0, ls->child.del(ls->child.size()-1));
                    v->child[0]->parent = v;
                    p->key[r] = v->key[0];
					if (v->leaf){
						v->data.add(0, ls->data.del(ls->data.size()-1));
					}
                    return;
                } else {
                    int m = (3 * ((ord + 1) / 2) - 1) / 2 - (ord + 1) / 2;
                    for (int i=0, j=ls->child.size(); i<m; i++){
                        ls->key.add(v->key[i]);
                        ls->child.add(v->child[i]);
                        ls->child[j++]->parent = ls;
                    }
                    v->key.reverse();
                    v->child.reverse();
                    for (int i=0; i<m; i++){
                        v->key.del(v->key.size()-1);
                        v->child.del(v->child.size()-1);
                    }
                    v->key.reverse();
                    v->child.reverse();
                    p->key[r] = v->key[0];
					if (v->leaf){
						for (int i=0; i<m; i++){
							ls->data.add(v->data[i]);
						}
						v->data.reverse();
						for (int i=0; i<m; i++){
							v->data.del(v->data.size()-1);
						}
						v->data.reverse();
					}

                    for (int i=0, j=v->child.size(); i<rs->key.size(); i++){
                        v->key.add(rs->key[i]);
                        v->child.add(rs->child[i]);
                        v->child[j++]->parent = v;
                    }
                    int k = rs->key.size();
                    for (int i=0; i<k; i++){
                        rs->key.del(rs->key.size()-1);
                        rs->child.del(rs->child.size()-1);
                    }
					if (v->leaf){
						for (int i=0; i<rs->key.size(); i++){
							v->data.add(rs->data[i]);
						}
						for (int i=0; i<k; i++){
							rs->data.del(rs->data.size()-1);
						}
						v->right = rs->right;
					}
                    p->key.del(r+1);
                    rs->parent = nullptr;
					rs->left = nullptr;
					rs->right = nullptr;
					p->child.del(r+1);
                    delete rs;
                    return;
                }
            }
        }
    } else {
        node<T, U> *p = v->parent;
        int r = 0;
        while (p->child[r] != v){
            r++;
        }
        if (r==0){
	        if (node<T, U> *rs = p->child[r+1]; (ord + 1) / 2 < rs->key.size()){
                v->key.add(rs->key.del(0));
                v->child.add(rs->child.del(0));
                v->child[v->child.size()-1]->parent = v;
                p->key[r+1] = rs->key[0];
				if (v->leaf){
					v->data.add(rs->data.del(0));
				}
                return;
            } else {
                for (int i=0, j=v->child.size(); i<rs->key.size(); i++){
                    v->key.add(rs->key[i]);
                    v->child.add(rs->child[i]);
                    v->child[j++]->parent = v;
                }
                int k = rs->key.size();
                for (int i = 0; i<k; i++){
                    rs->key.del(rs->key.size()-1);
                    rs->child.del(rs->child.size()-1);
                }
				if (v->leaf){
					for (int i=0; i<rs->key.size(); i++){
						v->data.add(rs->data[i]);
					}
					for (int i = 0; i<k; i++){
						rs->data.del(rs->data.size()-1);
					}
					v->right = rs->right;
				}
                p->key.del(1);
                rs->parent = nullptr;
				rs->left = nullptr;
				rs->right = nullptr;
                p->child.del(1);
                delete rs;
                underflow(p);
            }
        } else if (r==p->key.size()-1){
            node<T, U> *ls = p->child[r-1];
            if ((ord + 1) / 2 < ls->key.size()){
                v->key.add(0, ls->key.del(ls->key.size()-1));
                v->child.add(0, ls->child.del(ls->child.size()-1));
                v->child[0]->parent = v;
                p->key[r] = v->key[0];
				if (v->leaf){
					v->data.add(0, ls->data.del(ls->data.size()-1));
				}
                return;
            } else {
                for (int i=0, j=ls->child.size(); i<v->key.size(); i++){
                    ls->key.add(v->key[i]);
                    ls->child.add(v->child[i]);
                    ls->child[j++]->parent = ls;
                }
                int k = v->key.size();
                for (int i = 0; i<k; i++){
                    v->key.del(v->key.size()-1);
                    v->child.del(v->child.size()-1);
                }
				if (v->leaf){
					for (int i=0; i<v->key.size(); i++){
						ls->data.add(v->data[i]);
					}
					for (int i = 0; i<k; i++){
						v->data.del(v->data.size()-1);
					}
					ls->right = v->right;
				}
                p->key.del(r);
                v->parent = nullptr;
				v->left = nullptr;
				v->right = nullptr;
				p->child.del(r);
                delete v;
                underflow(p);
            }
        } else {
            node<T, U> *rs = p->child[r+1];
            node<T, U> *ls = p->child[r-1];
            if ((ord + 1) / 2 < rs->key.size()){
                v->key.add(rs->key.del(0));
                v->child.add(rs->child.del(0));
                v->child[v->child.size()-1]->parent = v;
                p->key[r+1] = rs->key[0];
				if (v->leaf){
					v->data.add(rs->data.del(0));
				}
                return;
            } else if ((ord + 1) / 2 < ls->key.size()){
                v->key.add(0, ls->key.del(ls->key.size()-1));
                v->child.add(0, ls->child.del(ls->child.size()-1));
                v->child[0]->parent = v;
                p->key[r] = v->key[0];
				if (v->leaf){
					v->data.add(0, ls->data.del(ls->data.size()-1));
				}
                return;
            } else {
                int m = (3 * ((ord + 1) / 2) - 1) / 2 - (ord + 1) / 2;
                for (int i=0, j=ls->child.size(); i<m; i++){
                    ls->key.add(v->key[i]);
                    ls->child.add(v->child[i]);
                    ls->child[j++]->parent = ls;
                }
                v->key.reverse();
                v->child.reverse();
                for (int i=0; i<m; i++){
                    v->key.del(v->key.size()-1);
                    v->child.del(v->child.size()-1);
                }
                v->key.reverse();
                v->child.reverse();
                p->key[r] = v->key[0];
				if (v->leaf){
					for (int i=0; i<m; i++){
						ls->data.add(v->data[i]);
					}
					v->data.reverse();
					for (int i=0; i<m; i++){
						v->data.del(v->data.size()-1);
					}
					v->data.reverse();
				}

                for (int i=0, j=v->child.size(); i<rs->key.size(); i++){
                    v->key.add(rs->key[i]);
                    v->child.add(rs->child[i]);
                    v->child[j++]->parent = v;
                }
                int k = rs->key.size();
                for (int i=0; i<k; i++){
                    rs->key.del(rs->key.size()-1);
                    rs->child.del(rs->child.size()-1);
                }
				if (v->leaf){
					for (int i=0; i<rs->key.size(); i++){
						v->data.add(rs->data[i]);
					}
					for (int i=0; i<k; i++){
						rs->data.del(rs->data.size()-1);
					}
					v->right = rs->right;
				}
                p->key.del(r+1);
                rs->parent = nullptr;
				rs->left = nullptr;
				rs->right = nullptr;
				p->child.del(r+1);
                delete rs;
                underflow(p);
            }
        }
    }
}

template <typename T, typename U> vector<U> btree<T, U>::find(const T &l, const T &r){
	node<T, U>* st = find(l);
	node<T, U>* end = find(r);
	if (st==nullptr && end==nullptr) return nullptr;
	vector<U> res = new vector<U>();
	int i = 0;
	if (st == nullptr){
		while (end){
			vector<U> tmp;
			int k = 0;
			for (int j=0; j<end->data.size(); j++){
				tmp[k++] = end->data[j];
			}
			tmp.reverse();
			for (int j=0; j<tmp.size(); j++){
				res[i++] = tmp[j];
			}
			end = end->left;
		}
		res.reverse();
		return res;
	} else if (end == nullptr){
		while (st){
			for (int j=0; j<st->data.size(); j++){
				res[i++] = st->data[j];
			}
			st = st->right;
		}
		return res;
	} else {
		while (st != end){
			for (int j=0; j<st->data.size(); j++){
				res[i++] = st->data[j];
			}
			st = st->right;
		}
		for (int j=0; j<end->data.size(); j++){
			res[i++] = end->data[j];
		}
		return res;
	}
}

}

#endif //BTREE_H
