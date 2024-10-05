#include <iostream>
#include <map>

struct word {
    std::string type;
    std::map<std::vector<std::string>, std::deque<char16_t[]>> transfer;
    std::string desc;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
