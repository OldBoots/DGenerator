#ifndef DGENERATOR_HPP
#define DGENERATOR_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>

struct DateComponents
{
    int year;
    unsigned month;
    unsigned day;
    int hour;
    int minute;
    int second;
};

class Column
{
public:
    Column();
    Column(std::string type, std::string name, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    Column(std::string type, std::string name, size_t countRows, bool flgShuffle, bool flgDebug);
    void setName(std::string name);
    void setCountRows(size_t count);
    void setDuplicate(double val);
    void setFlgShuffle(bool flg);
    void setFlgUnRegDupl(bool flg);
    void setFlgDebug(bool flg);
    std::string getName();
    std::string getTypeName();
    size_t getCountRows();
    double getDuplicate();
    bool isShuffleEnabled();
    bool isDuplRegDisabled();
    const std::vector<std::string>& getRows() const;
    std::string getRow(size_t index) const;
    size_t getCountDupl();
    size_t getCountUniq();
    void shuffleRows();
    void clearVecRows();
    virtual const std::vector<std::string>& genRows();
    virtual ~Column() = default;
protected:
    void removeRow(size_t index);
    bool isValidDuplicate(std::string& errMsg);
    void showGeneralInfo();
    template <typename T>
    void outputDebugLine(const std::string& lable, unsigned int gap, const T& value);
    template <typename T1, typename T2>
    void outputDebugLine(const std::string& lable, unsigned int gap, bool condition, const T1& tr, const T2& fls);
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
    std::string m_type;
    friend class GenString;
private:
    void calcUniqAndDupl();
};

class GenInt : public Column
{
public:
    GenInt();
    GenInt(std::string name, int min, int max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    GenInt(std::string name, int min, int max, size_t countRows, bool flgShuffle, bool flgDebug);
    GenInt(std::string name, int start, int step, size_t countRows, bool flgDebug);
    const std::vector<std::string>& genRows() override;
    void setRange(int min, int max);
    void setflgSequence(bool flg);
    bool isSequence();
    int getMin();
    int getMax();
private:
    bool isValidProperties();
    void genValue(std::uniform_int_distribution<>& distr, std::set<int>& set, size_t& enumeration, size_t& miss, size_t maxCountMiss);
    void showDebug();
private:
    int m_min;
    int m_max;
    bool m_flgSequence;
    int m_step;
};

class GenFloat : public Column
{
public:
    GenFloat();
    GenFloat(std::string name, double min, double max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    GenFloat(std::string name, double min, double max, size_t countRows, bool flgShuffle, bool flgDebug);
    GenFloat(std::string name, double start, double step, size_t countRows, bool flgDebug);
    const std::vector<std::string>& genRows() override;
    void setRange(double min, double max);
    void setFlgSequence(bool flg);
    bool isSequence();
    double getMin();
    double getMax();
private:
    bool isValidProperties();
    void genValue(std::uniform_real_distribution<>& distr, std::set<double>& set);
    void showDebug();
private:
    double m_min;
    double m_max;
    bool m_flgSequence;
    double m_step;
};

class GenWord : public Column
{
public:
    GenWord();
    GenWord(std::string name, size_t minLength, size_t maxLength, size_t countRows, double duplicates, double capitalLetter, bool m_flgUpperCase, bool flgShuffle, bool flgDebug);
    GenWord(std::string name, size_t minLength, size_t maxLength, size_t countRows, double capitalLetter, bool m_flgUpperCase, bool flgShuffle, bool flgDebug);
    void setRange(size_t minLength, size_t maxLength);
    void setLength(size_t length);
    void setUpperCase(bool flg);
    void setCapitalLetter(double capitalLetter);
    size_t getMinLength();
    size_t getMaxLength();
    double getCapitalLetter();
    size_t isUpperCase();
    const std::vector<std::string>& genRows() override;
private:
    void genValue(std::uniform_int_distribution<>& distrLetter, std::uniform_real_distribution<>& distrPrecent, 
        size_t& minLength, std::set<std::string>& set, size_t& miss, size_t& maxCountMiss);
    void showDebug();
    bool isValidProperties();
private:
    size_t m_minLength;
    size_t m_maxLength;
    double m_capitalLetter;
    bool m_flgUpperCase;
};

class GenDateTime : public Column
{
public:
    enum class DateFormat{DATE, TIME, DATETIME};
    GenDateTime();
    GenDateTime(std::string name, size_t countRows, DateFormat format, std::string begin, std::string end, double duplicates, bool flgShuffle, bool flgDebug);
    GenDateTime(std::string name, size_t countRows, DateFormat format, std::string begin, std::string end, bool flgShuffle, bool flgDebug);
    void setRange(std::string begin, std::string end);
    void setFormat(DateFormat format);
    std::chrono::sys_seconds getBegin();
    std::chrono::sys_seconds getEnd();
    GenDateTime::DateFormat getFormat();
    const std::vector<std::string>& genRows() override;
private:
    void genValue(std::uniform_int_distribution<size_t>& distDays, std::uniform_int_distribution<size_t>& distSeconds, std::set<std::chrono::sys_seconds>& set);
    std::string dateToStr(const std::chrono::sys_seconds& dateTime);
    std::chrono::sys_seconds strToDate(const std::string& dateTime);
    bool isValidProperties();
    void showDebug();
private:
    DateFormat m_format;
    std::string m_DateFormatGet;
    std::string m_DateFormatGive;
    std::chrono::sys_seconds m_begin;
    std::chrono::sys_seconds m_end;
};

class GenString : public Column
{
public:
    GenString();
    GenString(std::string name, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug);
    GenString(std::string name, size_t countRows, bool flgShuffle, bool flgDebug);
    GenString(std::string name, size_t countRows, bool flgDebug);
    void addColumn(Column* column, std::string prefix = "",  std::string suffix = "");
    void setFlgSequence(bool flg);
    bool isSequence();
    void showConfig();
    const std::vector<std::string>& genRows() override;
private:
    void equalizeVec();
    void showDebug();
    std::string glueString(size_t& stillRows);
    bool isValidProperties();
private:
    std::vector<Column*> m_vecColumns;
    std::vector<std::string> m_vecSuffix;
    std::vector<std::string> m_vecPrefix;
    size_t m_maxCountRows;
    bool m_flgSequence;
};

#endif