#include "DGen.hpp"

DGen::DGen()
{
}

// template <DType T>
// DTable::DTable()
// {
// }

// void DTable::set_name(std::string str)
// {
//     std::regex multiple_spaces("[,.\\s-]+");
//     str = std::regex_replace(str, multiple_spaces, "_");
//     std::regex spec_char("[^0-9a-z_]", std::regex_constants::icase);
//     str = std::regex_replace(str, spec_char, "");
//     std::regex number("^[0-9]");
//     std::smatch m;
//     std::regex_search(str, m, number);
//     str = std::regex_replace(str, number, "_" + m[0].str());
//     std::regex true_char("[0-9a-z]", std::regex_constants::icase);
//     if(std::regex_search(str, true_char)){
//         name = str;
//     }
// }

template <DType T>
DColumn<T>::DColumn()
: m_name()
, m_numRec()
, m_info()
, m_size()
, m_duplicates()
, m_dType()
{
}

template <DType T>
void DColumn<T>::setName(std::string str)
{
    // заменить знаки припинания и пробелы на "_"
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
    // содержит хотя бы цифры или буквы
    std::regex true_char("[0-9a-z]", std::regex_constants::icase);
    if(std::regex_search(str, true_char)){
        m_name = str;
    }
}

template <DType T>
void DColumn<T>::setNumRec(uint32_t num)
{
    m_numRec = num;
}

template <DType T>
void DColumn<T>::setSize(uint32_t bytes)
{
    m_size = bytes;
}

template <DType T>
void DColumn<T>::setDuplicates(float val)
{
    if(val < 0){
        return;
    }
    duplicates_ = val;
    if(duplicates_ < 1){
        if(duplicates_ == 0){
            AuxBin::set_bit(1, m_info, 9);    
        }
        AuxBin::set_bit(0, m_info, 8);
    } else {
        AuxBin::set_bit(1, m_info, 8);
    }
}

template <DType T>
void DColumn<T>::setDtype(T type)
{
    dType_ = type;
    if constexpr (std::is_same_v<T, DString> || std::is_same_v<T, DSubStr>) {
        AuxBin::set_bit(1, m_info, 0);
    } else if constexpr (std::is_same_v<T, DNum>) {
        AuxBin::set_bit(1, m_info, 1);
    } else if constexpr (std::is_same_v<T, DFloat>) {
        AuxBin::set_bit(1, m_info, 2);
    } else if constexpr (std::is_same_v<T, DDate>) {
        AuxBin::set_bit(1, m_info, 3);
    }
}

template <DType T>
void DColumn<T>::toStr()
{
    AuxBin::set_bit(1, m_info, 0);
}

template <DType T>
std::string DColumn<T>::getName()
{
    return m_name;
}

template <DType T>
uint32_t DColumn<T>::getNumRec()
{
    return m_numRec;
}

template <DType T>
uint16_t DColumn<T>::getInfo()
{
    return m_info;
}

template <DType T>
uint32_t DColumn<T>::getSize()
{
    return m_size;
}

template <DType T>
float DColumn<T>::getDuplicates()
{
    return m_duplicates;
}

template <DType T>
T DColumn<T>::getDType()
{
    return m_dType;
}

DString::DString()
{
}

DSubStr::DSubStr()
: m_lenMin(3)
, m_lenMax(10)
, m_info(0)
, m_duplicates(0)
, m_separator("")
, m_alphabet("abcdefghijklmnopqrstuvwxyz")
, m_defAlph("abcdefghijklmnopqrstuvwxyz0123456789")
{
}

void DSubStr::setLen(unsigned short min, unsigned short max)
{
    m_lenMin = min;
    m_lenMax = max;
    if(m_lenMin){
        AuxBin::set_bit(0, m_info, 6);
    } else {
        AuxBin::set_bit(1, m_info, 6);
    }
}

void DSubStr::setLen(unsigned short length)
{
    m_lenMin = 0;
    m_lenMax = length;
    AuxBin::set_bit(1, m_info, 5);
}

void DSubStr::setAlph(std::string pattern = "")
{
    if(pattern.size()){
        AuxBin::set_bit(0, m_info, 0);
        std::regex alphPattern("[^" + pattern + "]");
        m_alphabet = std::regex_replace(m_defAlph, alphPattern, "");
    } else {
        AuxBin::set_bit(1, m_info, 0);
        m_alphabet = m_defAlph;
    }
}

void DSubStr::setSeparator(std::string sep)
{
    AuxBin::set_bit(1, m_info, 1);
    m_separator = sep;
}

void DSubStr::setDuplicates(float val)
{
    if(val < 0){
        std::cout << "DSubStr: Error: Incorrect value of duplicates.";
        return;
    }
    m_duplicates = val;
    if(m_duplicates < 1){
        if(m_duplicates == 0){
            AuxBin::set_bit(1, m_info, 5);
        }
        AuxBin::set_bit(0, m_info, 4);
    } else {
        AuxBin::set_bit(1, m_info, 4);
    }
}

void DSubStr::setStaticStr(std::string str)
{
    AuxBin::set_bit(1, m_info, 0);
    m_alphabet = str;
}

void DSubStr::useCapitalLetters()
{
    AuxBin::set_bit(1, m_info, 2);
}

void DSubStr::useUpperRegister()
{
    AuxBin::set_bit(1, m_info, 3);
}

void DSubStr::showAlph()
{
    std::cout << "A:- \"" << m_alphabet << "\"" << std::endl;
}

std::string DSubStr::getAlph()
{
    return m_alphabet;
}

uint8_t DSubStr::getInfo()
{
    return m_info;
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