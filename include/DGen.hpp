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

class DTable{
public:
    DTable();
private:
        
};

class DColumn{
public:
    DColumn();
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
 * 5 - Static length. Otherwise, in the range (defoult 3-10).
 * 6 - 
 * 7 - 
 * duplicates:
 * if < 1 - count of duplicates
 * else - percent of duplicates
 * length:
 * if min ==  0 - strictly maximum length
 * alphabet:
 * defoult - a-z, 0-9;
*/
class DSubStr{
public:
    DSubStr();
    void set_len(unsigned short len_min, unsigned short len_max);
    void set_len(unsigned short length);
    void set_alph(std::string pattern);
    void set_separator(std::string sep);
    void set_duplicates(float val);
    void set_static_str(std::string str);
    void use_CL();
    void use_UR();
    void show_alph();
    std::string get_alph();
    uint8_t get_type();
private:
private:
    unsigned short len_min;
    unsigned short len_max;
    uint8_t type;
    float duplicates;
    std::string separator;
    std::string alphabet;
    std::string def_alph;
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
        
#endif // DGEN_H
