//
// Created by 果汁 on 2024/9/21.
//

#ifndef VECTOR_H
#define VECTOR_H
#include <utility>
#include <stdexcept>
#include <iostream>

namespace ebstd{
template <typename T> struct allocator{
    template <typename... Ts> void construct(T *t, Ts&& ...ts){
        try {
            new (t) T(std::forward<Ts>(ts)...);
        } catch(const std::runtime_error &e){
            throw std::runtime_error(e.what());
        }
    }

    static void destroy(T *t){
        t->~T();
    }
};

//type_list
template <typename... Ts> class type_list;
template <typename H, typename... T> class type_list<H, T...>;

class any{};
template<typename T> struct len;
template<> struct len<any>{
    enum{v = 0};
};
template<typename T, typename U>
struct len<type_list<T..., U>>{
    enum{v = 1 + len<U>::v};
};

template<typename T, int i> struct idx;
template <typename H, typename... T>
struct idx<type_list<H, T...>, 0>{
    using res = H;
};
template<typename H, typename... T, int i>
struct idx<type_list<H, T...>, i>{
    using res = typename idx<type_list<T...>, i-1>::res;
};

template<typename H, typename T> struct idxOf;
template<typename H, typename... T>
struct idxOf<type_list<H, T...>, H>
{
    static constexpr int res = 0;
};
template<typename T>
struct idxOf<type_list<>, T>
{
    static constexpr int res = -1;
};
template<typename H, typename... T, typename K>
struct idxOf<type_list<H, T...>, K>{
    const static int tmp = idxOf<type_list<T...>, K>::res;
    const static int res = tmp == -1 ?  - 1 : tmp + 1;
};

template<typename H, typename T> struct add;
template<typename... H, typename T>
struct add<type_list<H...>, T>{
    using res = type_list<H..., T>;
};

template<typename T> using tmp_t = std::remove_cv_t<std::remove_reference_t<T>>;
template<typename T> using remove_cv_ref_t = tmp_t<std::remove_reference_t<T>>;
using original_t = type_list<char, unsigned char, short, unsigned short, int, unsigned int, long,
unsigned long, long long, unsigned long long, float, double, long double>;

//is_type_in_list
template <typename T>
constexpr bool is_type_in_list(){
    return false;
}

template <typename T, typename U, typename... Ts>
constexpr bool is_type_in_list(){
    return std::is_same_v<T, U> || is_type_in_list<U, Ts...>();
}

template<class T, class A=allocator<T>> class vector{
    template<class K> friend std::ostream& operator<<(std::ostream &out, vector<K> const &o);
    template<class V> friend void swap(vector<V> &l, vector<V> &r);

protected:
    size_t size{};
    size_t cap{};
    T *data;
    T *last;
    bool (*eq)(const T&, const T&);
    bool (*com)(const T&, const T&);
    A all;

    void exp(){
        if (size < cap) return;
        cap = 2 * cap + 10;
        data = static_cast<T *>(realloc(data, cap * sizeof(T)));
    }

    void shr(){
        if ((size<<2) > cap) return;
        cap >>= 1;
        data = static_cast<T *>(realloc(data, cap * sizeof(T)));
    }

    size_t insert(size_t k, T&& t){
        exp();
        for (size_t i=size; i>k; i--){
            data[i] = data[i-1];
        }
        data[k] = std::move(t);
        size++;
        ++last;
        return k;
    }

    size_t insert(size_t k, const T &t){
        exp();
        for (size_t i=size; i>k; i--){
            data[i] = data[i-1];
        }
        data[k] = t;
        size++;
        ++last;
        return k;
    }

    template <typename... Ts> size_t insert(size_t k, Ts&& ...ts){
        exp();
        for (size_t i=size; i>k; i--){
            data[i] = data[i-1];
        }
        data[k](std::forward<Ts>(ts)...);
        size++;
        ++last;
        return k;
    }

public:
    explicit vector(bool (*e)(const T&, const T&)=nullptr,
                    bool (*c)(const T&, const T&)=nullptr,
                    const size_t capacity=5) : size(0), cap(capacity), eq(e), com(c){
        data = cap == 0 ? static_cast<T *>(malloc(5 * sizeof(T))) : static_cast<T *>(malloc(cap * sizeof(T)));
        last = data;
    }

    virtual ~vector(){
        for (T *t=data; t!=last; ++t){
            all.destroy(t);
        }
        free(data);
        data = last = nullptr;
    }

    explicit vector(T const *t) : eq(t->eq), com(t->com){
        constexpr size_t l = 0;
        const size_t r = sizeof(t) / sizeof(t[0]);
        copy(t, l, r);
    }

    vector(vector const &other) : size(other.size), cap(other.cap), data(cap==0 ? static_cast<T *>(malloc(5 * sizeof(T))) : static_cast<T *>(malloc(cap * sizeof(T)))), eq(other.eq),
                                  com(other.com){
        memcpy(this->data, other.data, size * sizeof(T));
        last = data + size;
    }

    vector(vector &&other) noexcept : vector(nullptr, nullptr, 0){
        swap(other, *this);
        last = data + size;
    }

    vector &operator=(vector other){
        swap(other, *this);
        last = data + size;
        return *this;
    }

    T &operator[](size_t r){
        return data[r];
    }

    [[nodiscard]] bool empty() const{
        return !size;
    }

    [[nodiscard]] size_t count() const{
        return size;
    }

    void clear(){
        memset(data, 0, size * sizeof(T));
        size = 0;
    }

    void copy(T const *t, size_t l, size_t r){ //r == n
        if (r <= size){
            for (T *it=data+r; it!=data+size; ++it){
                all.destroy(it);
            }
        }
        cap = 2 * (r - l);
        size = 0;
        data = static_cast<T *>(realloc(data, cap * sizeof(T)));
        while (l < r){
            data[size++] = t[l++];
        }
        last = data + size;
    }

    void reverse(size_t l=0, size_t r=count()-1){ //r == n-1
        if (l > r){
            const size_t t = l;
            l = r;
            r = t;
        }
        while (l < r) swap(data[l++], data[r--]);
    }

    template <typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t add(size_t k, T&& t){
        return insert(k, std::forward<T>(t));
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t add(size_t k, T t){
        exp();
        for (size_t i=size; i>k; i--){
            data[i] = data[i-1];
        }
        data[k] = t;
        size++;
        ++last;
        return k;
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t add(T t){
        return add(size, t);
    }

    template <typename... Ts, typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<Ts>, original_t>()>> size_t add(Ts&& ...ts){
        exp();
        try {
            all.construct(last, std::forward<Ts>(ts)...);
        } catch(const std::runtime_error &e){
            return insert(size, std::forward<Ts>(ts)...);
        }
        ++last;
        return size++;
    }

    size_t del(size_t l, size_t r){ //[...)
        if (l == r) return 0;
        while (r < size){
            data[l++] = data[r++];
        }
        size = l;
        for (T *t=data+l; t!=data+r; ++t){
            all.destroy(t);
        }
        last = data + size;
        shr();
        return r - l;
    }

    T del(size_t k){
        T t = data[k];
        del(k, k+1);
        return t;
    }

    virtual size_t del(){
        return del(size-1, size);
    }

    template <typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t find(const T &t, size_t l, size_t r){
        while ((l < r--) && (!eq(t, data[r]))) {}
        return r;
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t find(T t, size_t l, size_t r) const{
        while ((l < r--) && (!eq(t, data[r]))) {}
        return r;
    }

    template <typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t find(const T &t){
        return find(t, 0, size);
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t find(T t) const{
        return find(t, 0, size);
    }

    template <typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t bs(const T &t, size_t l, size_t r){
        while (l < r){
            size_t m = (l + r) >> 1;
            (com(t, data[m])) ? r = m : l = m+1;
        }
        return --l;
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t bs(T t, size_t l, size_t r) const{
        while (l < r){
            size_t m = (l + r) >> 1;
            (com(t, data[m])) ? r = m : l = m+1;
        }
        return --l;
    }

    template <typename = std::enable_if_t<!is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t bs(const T &t){
        return bs(t, 0, size);
    }

    template <typename = std::enable_if_t<is_type_in_list<remove_cv_ref_t<T>, original_t>()>> size_t bs(T t) const{
        return bs(t, 0, size);
    }

    virtual T& get(){
        return data[size-1];
    }

    //迭代器(单链表(迭代器链))
    class iterator{
    private:
        T *pt;
        vector<T, A> *pv;

    public:
        friend vector<T, A>;
        explicit iterator(vector<T, A> *v=nullptr, T *t=nullptr) : pt(t), pv(v){}

        void operator++(){
            if (pv == nullptr) throw "无效";
            ++pt;
        }

        T &operator*(){
            if (pv == nullptr) throw "无效";
            return *pt;
        }

        const T &operator*() const{
            if (pv == nullptr) throw "无效";
            return *pt;
        }

        bool operator!=(const iterator &it) const{
            if (pv==nullptr || pv!=it.pv) throw "无效";
            return pt != it.pt;
        }
    };

    iterator begin(){
        return iterator(this, data);
    }

    iterator end(){
        return iterator(this, last);
    }
};

template<class T> std::ostream& operator<<(std::ostream &out, vector<T> &o){
    for (size_t i=0; i<o.size; i++){
        out << o[i] << " ";
    }
    out << std::endl;
    return out;
}

template<class T> void swap(vector<T> &l, vector<T> &r){
    using std::swap;
    swap(l.data, r.data);
    swap(l.cap, r.cap);
    swap(l.size, r.size);
    swap(l.eq, r.eq);
    swap(l.com, r.com);
}

}

#endif //VECTOR_H
