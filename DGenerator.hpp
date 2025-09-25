#ifndef DGENERATOR_HPP
#define DGENERATOR_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>

class Column
{
public:
    Column();
    Column(std::string name, size_t countRows, float duplicates, bool flgShuffle = false, bool flgDebug = false);
    Column(size_t countRows, std::string name, bool flgShuffle = false, bool flgDebug = false);
    void setName(std::string name);
    void setCountRows(size_t count);
    void setDuplicate(float val);
    void setFlgShuffle(bool flg);
    void setFlgUnRegDupl(bool flg);
    void setFlgDebug(bool flg);
    std::string getName();
    size_t getCountRows();
    float getDuplicate();
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
    float m_duplicates;
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
    Int(std::string name, size_t min, size_t max, size_t countRows, float duplicates, bool flgShuffle = false, bool flgDebug = false);
    Int(size_t min, size_t max, size_t countRows, std::string name, bool flgShuffle = false, bool flgDebug = false);
    const std::vector<std::string>& genRows();
    void setRange(size_t min, size_t max);
    int getMin();
    int getMax();
private:
    bool isValidProperties();
    void genValue(std::uniform_int_distribution<>& distr, std::set<int>& set, size_t& enumeration, size_t& miss, size_t maxCountMiss);
    void showDebug();
    int m_min;
    int m_max;
};

#endif