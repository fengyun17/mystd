#include <iostream>
#include <map>

struct word {
    std::map<std::string, std::map<std::string, std::map<char16_t[], std::string>>> data;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    const char *s = "test";
    std::cout << std::string{s} << std::endl;
    std::string str = "hello";
    char *p = const_cast<char*>(str.c_str());
    std::cout << p << std::endl;
    std::map<std::string, long> dict;
    dict.insert_or_assign("field", new word);
    return 0;
}
