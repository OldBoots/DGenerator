#include <iostream>
#include <clocale>
#include <DGen.hpp>

void set_name(std::string str)
{
    std::cout << "----------------" << std::endl;
    std::cout << str << std::endl;
    // заменить знаки припинания и пробелы на "_"
    // std::regex multiple_spaces("\\s+");
    std::regex multiple_spaces("[,.\\s-]+");
    str = std::regex_replace(str, multiple_spaces, "_");
    // убрать все спец символы
    // убрать кириллицу
    std::regex spec_char("[^0-9a-z_]", std::regex_constants::icase);
    str = std::regex_replace(str, spec_char, "");
    // если начинается с цифры, поставить "_"
    std::regex number("^[0-9]");
    std::smatch m;
    std::regex_search(str, m, number);
    str = std::regex_replace(str, number, "_" + m[0].str());
    // содержит хоть бы цифры или буквы
    std::regex true_char("[0-9a-z]", std::regex_constants::icase);
    if(std::regex_search(str, true_char)){
        std::cout << str << std::endl;
    }
}

int main(){
    return 0;
}