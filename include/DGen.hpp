#ifndef DGEN_H
#define DGEN_H

#include <iostream>
#include <concepts>
#include <cstdint>
#include <string>
#include <regex>
#include <vector>

template<typename T>
concept Registr = std::unsigned_integral<T>;

class AuxBin{
public:
    template<Registr T>
    static void set_bit(bool bit, T &reg, uint8_t index)
    {
        if(index >= sizeof(T) * 8){
            std::cout << "AuxBit: set_bit: Error: The index is larger than the register." << std::endl;
            return;
        }
        if(bit){
            reg |= static_cast<T>(1) << index;
        } else {
            reg &= ~static_cast<T>(1) << index;
        }
    }
    template<Registr T>
    static void set_reg(T &reg, T other_reg)
    {
        reg = other_reg;
    }
    template<Registr T>
    static bool get_bit(T reg, uint8_t index)
    {
        if(index >= sizeof(T) * 8){
            std::cout << "AuxBit: get_bit: Error: The index is larger than the register." << std::endl;
            return 0;
        }
        return (reg >> index) & 1;
    }
    template<Registr T>
    static void show_registr(T reg)
    {
        int reg_size = sizeof(T) * 8;
        for(int i = reg_size - 1; i >= 0 ; i--){
            std::cout << ((reg >> i) & static_cast<T>(1));
        }
        std::cout << std::endl;
    }
private:
};

class DGen{
public:
    DGen();
private:

};

class DString{
    public:
        DString();
    private:
                    
};
    
/*
 * DSubStr()
 * type:
 * 0 - Write a static string. Otherwise generate a string.
 * 1 - Use a separator.
 * 2 - Use a capital letter. Otherwise, everything is in lowercase (if !3).
 * 3 - Use an Up Register.
 * 4 - Static number of duplicates. Otherwise, the percentage of duplicates.
 * 5 - Uniqe strings. Otherwise, non check duplicate.
 * 6 - Static length. Otherwise, in the range (defoult 3-10).
 * 7 - 
 * duplicates:
 * if < 1 - count of duplicates
 * else if == 0 - unique strings
 * else - percent of duplicates
 * length:
 * if min ==  0 - strictly maximum length
 * alphabet:
 * defoult - a-z, 0-9;
*/
class DSubStr{
public:
    DSubStr();
    void setLen(unsigned short len_min, unsigned short len_max);
    void setLen(unsigned short length);
    void setAlph(std::string pattern);
    void setSeparator(std::string sep);
    void setDuplicates(float val);
    void setStaticStr(std::string str);
    void useCapitalLetters();
    void useUpperRegister();
    void showAlph();
    std::string getAlph();
    uint8_t getInfo();
private:
    unsigned short m_lenMin;
    unsigned short m_lenMax;
    uint8_t m_info;
    float m_duplicates;
    std::string m_separator;
    std::string m_alphabet;
    std::string m_defAlph;
};

class DNum{
    public:
    DNum();
    private:
                    
    };

class DFloat{
public:
    DFloat();
private:
                
};

class DDate{
public:
    DDate();
private:
                    
};

template<typename T>
concept DType = 
std::is_same_v<T, DString> ||
std::is_same_v<T, DSubStr> ||
std::is_same_v<T, DNum> ||
std::is_same_v<T, DFloat> ||
std::is_same_v<T, DDate>;

/*
 * DColumn();
 * type: Register of state.
 * 0 - string
 * 1 - number
 * 2 - float
 * 3 - date
 * 4
 * 5
 * 6
 * 7
 * 8 - Static number of duplicates. Otherwise, the percentage of duplicates.
 * 9 - Unique records.
 * 10-15 - Zero. 
 * num_rec: The number of entries in the column.
 * 
 * 
*/
template<DType T> 
class DColumn{
public:
    DColumn();
    void setName(std::string str);
    void setNumRec(uint32_t num);
    void setSize(uint32_t bytes);
    void setDuplicates(float val);
    void setDtype(T dtype);
    void toStr();
    std::string getName();
    uint32_t getNumRec();
    uint16_t getInfo();
    uint32_t getSize();
    float getDuplicates();
    T getDType();
private:
    std::string m_name;
    uint32_t m_numRec;
    uint16_t m_info;
    uint32_t m_size;
    float m_duplicates;
    T m_dType;
};

// template<DType T>
// class DTable{
// public:
//     DTable();
//     void set_name(std::string str);
//     void add_col(DColumn<T> col);
//     void show_name();
//     void show_cols();
//     std::vector <DColumn<T>> cols();
//     DColumn<T> col(int index);
//     DColumn<T> col(std::string name);
//     std::string name();

// private:
//     std::string name;
//     std::vector <DColumn<T>> cols;
// };
        
#endif // DGEN_H
