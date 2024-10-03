//
// Created by 果汁 on 2024/9/22.
//

#ifndef LTREE_H
#define LTREE_H
namespace ebstd{
template<typename T> class ltree{
protected:
	int max;
	int n;
	int lowext{};
	int offset{};
	int *B;
	T *L;
	int (*win)(T*, int, int);
	int (*lose)(T*, int, int);
	
	void play(int p, int l, int r){
		B[p] = lose(L, l, r);
		int w1 = win(L, l, r);
		while (p>1 && p%2){ //处理内部右节点
			const int w2 = win(L, w1, B[p / 2]); //来到内部右节点(B[奇数编号])时保证了内部左节点已经处理完并把胜者送上了父节点(左->右的顺序)
			B[p/2] = lose(L, w1, B[p/2]);
			w1 = w2;
			p /= 2;
		}
		B[p/2] = w1; //处理内部左节点并把胜者送上了父节点
	}
	
public:
	ltree(const int size, int (*winner)(T*, int, int), int (*loser)(T*, int, int)): L(nullptr) {
		max = size;
		B = new int[size];
		n = 0;
		win = winner;
		lose = loser;
	}

	~ltree(){
		delete []B;
	}
	
	[[nodiscard]] int get() const {
		return n ? B[0] : 0;
	}
	
	void create(T arr*, const int k){
		if (k>max || k<2) return;
		n = k;
		L = arr;
		int i, s;
		for(s=1; 2*s<=n-1; s+=s) {}
		lowext = 2 * (n - s);
		offset = 2 * s - 1;
		for (i=2; i<=lowext; i+=2){
			play((i+offset)/2, i-1, i);
		}
		if (n % 2){
			play(n/2, B[n-1], lowext+1);
			i = lowext + 3;
		} else {
			i = lowext + 2;
		}
		for (; i<=n; i+=2){
			play((i-lowext+n-1)/2, i-1, i);
		}
	}
	
	void replay(const int i){
		if (i<=0 || i>n) return;
		int p;
		if (i <= lowext){
			p = (i + offset) / 2;
		} else {
			p = (i - lowext + n - 1) / 2;
		}
		B[0] = win(L, i, B[p]);
		B[p] = lose(L, i, B[p]);
		for(; (p/2)>=1; p/=2){
			//临时存放赢者的索引
			const int w = win(L, B[p / 2], B[0]);
			B[p/2] = lose(L, B[p/2], B[0]);	
			B[0] = w;
		}
	}
};

}

#endif //LTREE_H
