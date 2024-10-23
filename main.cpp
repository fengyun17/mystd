#include <iostream>
#include <map>
#include <unordered_set>
#include <unordered_map>

struct desc {
    std::string title;
    std::vector<std::pair<std::string, std::string>> examples;
    std::string url;
};

struct word {
    std::map<std::string, std::map<std::string, std::map<std::string, desc>>> data;
};

template <typename T> struct node{
    bool leaf;
    node<T> *parent;
    node<T> *skip;
    T data;
    std::deque<node<T>*> chs;

    explicit node(T&& d=nullptr){
        data = std::move(d);
        leaf = false;
        parent = nullptr;
        skip = nullptr;
    }
};

template <typename T, typename U> struct edge;

template <typename T, typename U> struct vertex : public node<T>{
    int id;
    int in;
    int out;
    std::deque<edge<T, U>*> adj;
    explicit vertex(int i) : id(i), in(0), out(0){}
};

template <typename T, typename U> struct edge{
    U data;
    vertex<T, U> *f;
    vertex<T, U> *t;
    edge(U&& d, vertex<T, U> *from, vertex<T, U> *to) : f(from), t(to){
        data = std::move(d);
    }
};

template <typename T> class ToT{
public:
    int cnt;
    bool (*eq)(T, T);
    bool (*com)(T, T);
    node<T> *root;
    node<T> *hot;
    ToT(bool (*e)(T, T), bool (*c)(T, T)) : cnt(0), hot(nullptr) {
        eq = e;
        com = c;
        root = new node<T>;
    }
    ~ToT(){
        delete root; root = nullptr;
    }
    [[nodiscard]] int size() const {return cnt;}
    [[nodiscard]] bool empty() const {return !root ? false : true;}
};

template <typename T, typename U> class graph{
public:
    std::unordered_map<int, vertex<T, U>*> ns;
    std::unordered_set<edge<T, U>*> es;
};

int main() {
    std::cout << "Hello, World! yes" << std::endl;
    const char *s = "test";
    std::cout << std::string{s} << std::endl;
    std::string str = "hello";
    char *p = const_cast<char*>(str.c_str());
    std::cout << p << std::endl;
    std::map<std::string, long> dict;
    dict.insert_or_assign("field", new word);
    return 0;
}
