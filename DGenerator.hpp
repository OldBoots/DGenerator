#ifndef DGENERATOR_HPP
#define DGENERATOR_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>
#include <cmath>

class Column
{
public:
    Column();
    Column(std::string name, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    Column(std::string name, size_t countRows, bool flgShuffle, bool flgDebug);
    void setName(std::string name);
    void setCountRows(size_t count);
    void setDuplicate(double val);
    void setFlgShuffle(bool flg);
    void setFlgUnRegDupl(bool flg);
    void setFlgDebug(bool flg);
    std::string getName();
    size_t getCountRows();
    double getDuplicate();
    bool isShuffleEnabled();
    bool isDuplRegDisabled();
    const std::vector<std::string>& getVecRows() const;
    std::string getRow(size_t index) const;
    size_t getCountDupl();
    size_t getCountUniq();
    void shuffleRows();
    void clearVecRows();
protected:
    bool isValidDuplicate(std::string& errMsg);
    void showGeneralInfo();
protected:
    std::string m_name;
    std::vector<std::string> m_vecRows;
    bool m_flgUnRegDupl;
    bool m_flgShuffle;
    bool m_flgDebug;
    double m_duplicates;
    size_t m_countDupl;
    size_t m_countUniq;
    size_t m_countRows;
    std::string m_errMesage;
    std::random_device m_rd;
    std::mt19937 m_gen;
private:
    void calcUniqAndDupl();
};

class Int : public Column
{
public:
    Int();
    Int(std::string name, int min, int max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    Int(std::string name, int min, int max, size_t countRows, bool flgShuffle, bool flgDebug);
    const std::vector<std::string>& genRows();
    void setRange(int min, int max);
    int getMin();
    int getMax();
private:
    bool isValidProperties();
    void genValue(std::uniform_int_distribution<>& distr, std::set<int>& set, size_t& enumeration, size_t& miss, size_t maxCountMiss);
    void showDebug();
private:
    int m_min;
    int m_max;
};

class Float : public Column
{
public:
    Float();
    Float(std::string name, double min, double max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    Float(std::string name, double min, double max, size_t countRows, bool flgShuffle, bool flgDebug);
    const std::vector<std::string>& genRows();
    void setRange(double min, double max);
    double getMin();
    double getMax();
private:
    bool isValidProperties();
    void genValue(std::uniform_real_distribution<>& distr, std::set<double>& set);
    void showDebug();
private:
    double m_min;
    double m_max;
};

class Word : public Column
{
public:
    Word();
    Word(std::string name, size_t minLength, size_t maxLength, size_t countRows, double duplicates, double capitalLetter, bool m_flgUpperCase, bool flgShuffle, bool flgDebug);
    Word(std::string name, size_t minLength, size_t maxLength, size_t countRows, double capitalLetter, bool m_flgUpperCase, bool flgShuffle, bool flgDebug);
    void setRange(size_t minLength, size_t maxLength);
    void setLength(size_t length);
    void setUpperCase(bool flg);
    void setCapitalLetter(double capitalLetter);
    size_t getMinLength();
    size_t getMaxLength();
    double getCapitalLetter();
    size_t isUpperCase();
    const std::vector<std::string>& genRows();
private:
    void genWord(std::uniform_int_distribution<>& distrLetter, std::uniform_real_distribution<>& distrPrecent, 
        size_t& minLength, std::set<std::string>& set, size_t& miss, size_t& maxCountMiss);
    void showDebug();
    bool isValidProperties();
private:
    size_t m_minLength;
    size_t m_maxLength;
    double m_capitalLetter;
    bool m_flgUpperCase;
};

#endif