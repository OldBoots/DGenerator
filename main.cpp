#include <iostream>
#include <vector>
#include <string>

#include "DGenerator.hpp"

namespace
{
    template <typename T>
    void showVec(const std::vector<T>& vec, bool inLine = true)
    {
        std::cout << "size = " << vec.size() << " -- ";
        for(const auto& val : vec){
            std::cout << (inLine ? " " : "\n") << val;
        }
        std::cout << std::endl;
    }
}

int main()
{
    // Word a("Test", 3, 10, 10, 0, 0, 0, 0);
    // showVec(a.genRows(), 0);
    for(int i = 0; i <= 3; i++){
        float d = i / 100.0;
        Word a("Test", 2, 5, 1000000, 0, 0, 0, 0, 1);
        // std::cout << i << ") d = " << d << " cu = " << a.getCountUniq() << " cd = " << a.getCountDupl() << " sh = " <<  a.isShuffleEnabled() << " "; 
        a.genRows();
        // showVec();
        // std::vector<std::string> vec = a.genRows();
        // std::sort(vec.begin(), vec.end());
        // showVec(a.getVecRows());
        // std::cout << "size = " << a.getVecRows().size();
    }
    return 0;
}
