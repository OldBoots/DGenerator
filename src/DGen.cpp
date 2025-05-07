#include "DGen.hpp"

DGen::DGen()
{
}

DTable::DTable()
{
}

DColumn::DColumn()
{
}

DString::DString()
{
}

DSubStr::DSubStr()
{
    def_alph = "abcdefghijklmnopqrstuvwxyz0123456789";
    type = 0;
    len_min = 3;
    len_max = 10;
    duplicates = 0;
}

void DSubStr::set_len(unsigned short min, unsigned short max)
{
    len_min = min;
    len_max = max;
    if(len_min){
        AuxBin::set_bit(0, type, 5);
    } else {
        AuxBin::set_bit(1, type, 5);
    }
}

void DSubStr::set_len(unsigned short length)
{
    len_min = 0;
    len_max = length;
    AuxBin::set_bit(1, type, 5);
}

void DSubStr::set_alph(std::string pattern = "")
{
    if(pattern.size()){
        AuxBin::set_bit(0, type, 0);
        std::regex alph_pattern("[" + pattern + "]");
        auto begin = std::sregex_iterator(def_alph.begin(), def_alph.end(), alph_pattern);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            alphabet += it->str()[0];
        }
    } else {
        AuxBin::set_bit(1, type, 0);
        alphabet = def_alph;
    }
}

void DSubStr::set_separator(std::string sep)
{
    AuxBin::set_bit(1, type, 1);
    separator = sep;
}

void DSubStr::set_duplicates(float val)
{
    if(val < 0){
        std::cout << "DSubStr: Error: Incorrect value of duplicates.";
        return;
    }
    duplicates = val;
    if(duplicates < 1){
        AuxBin::set_bit(0, type, 4);
    } else {
        AuxBin::set_bit(1, type, 4);
    }
}

void DSubStr::set_static_str(std::string str)
{
    AuxBin::set_bit(1, type, 0);
    alphabet = str;
}

void DSubStr::use_CL()
{
    AuxBin::set_bit(1, type, 2);
}

void DSubStr::use_UR()
{
    AuxBin::set_bit(1, type, 3);
}

void DSubStr::show_alph()
{
    std::cout << "A:- \"" << alphabet << "\"" << std::endl;
}

std::string DSubStr::get_alph()
{
    return alphabet;
}

uint8_t DSubStr::get_type()
{
    return type;
}

DNum::DNum()
{
}

DFloat::DFloat()
{
}

DDate::DDate()
{
}

