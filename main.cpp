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
            std::cout << val << (inLine ? " " : "\n");
        }
        std::cout << std::endl;
    }
}

int main()
{
    for(int i = 0; i <= 10; i++){
        float d = i / 100.0;
        Int a("fff", 0, 1000, 1000, d, 0, 1);
        a.genRows();
        // std::vector<std::string> vec = a.genRows();
        // std::sort(vec.begin(), vec.end());
        // std::cout << i << ") d = " << d << " cu = " << a.getCountUniq() << " cd = " << a.getCountDupl() << " sh = " <<  a.isShuffleEnabled() << " "; 
        // showVec(a.getVecRows());
        // std::cout << "size = " << a.getVecRows().size();
    }
    return 0;
}
