#include "DGenerator.hpp"

namespace
{
DateComponents extractDateComponents(const std::chrono::sys_seconds &dateTime)
{
    std::chrono::year_month_day ymd = floor<std::chrono::days>(dateTime);
    std::chrono::hh_mm_ss hms(dateTime - std::chrono::sys_days(ymd));
    return {
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()),
        static_cast<int>(hms.hours().count()),
        static_cast<int>(hms.minutes().count()),
        static_cast<int>(hms.seconds().count())
    };
}
}

//______________Column______________

Column::Column() : m_name(""), m_flgUnRegDupl(false), m_flgShuffle(false), m_duplicates(0), m_countDupl(0), 
m_countRows(0), m_rd(), m_gen(m_rd())
{
}

Column::Column(std::string name, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
m_name(name), m_flgUnRegDupl(false), m_flgShuffle(flgShuffle), m_flgDebug(flgDebug), m_duplicates(duplicates), 
m_countRows(countRows), m_rd(), m_gen(m_rd())
{
    calcUniqAndDupl();
}

Column::Column(std::string name, size_t countRows, bool flgShuffle, bool flgDebug) : m_name(name), 
m_flgUnRegDupl(true), m_flgShuffle(flgShuffle), m_flgDebug(flgDebug), m_duplicates(0), m_countRows(countRows), m_rd(), 
m_gen(m_rd())
{
    calcUniqAndDupl();
}

void Column::setName(std::string name)
{
    m_name = name;
}

void Column::shuffleRows()
{
    if(!m_vecRows.empty()){
        std::shuffle(m_vecRows.begin(), m_vecRows.end(), m_gen);
    }
}

void Column::setCountRows(size_t count)
{
    m_countRows = count;
}

void Column::setDuplicate(double val)
{
    m_duplicates = val;
}

void Column::setFlgShuffle(bool flg)
{
    m_flgShuffle = flg;
}

void Column::setFlgUnRegDupl(bool flg)
{
    m_flgUnRegDupl = flg;
}

void Column::setFlgDebug(bool flg)
{
    m_flgDebug = flg;
}

std::string Column::getRow(size_t index) const
{
    if(index >= m_vecRows.size()){
        return "";
    }
    return m_vecRows[index];
    
}

std::string Column::getName()
{
    return m_name;
}

size_t Column::getCountRows()
{
    return m_countRows;
}

bool Column::isShuffleEnabled()
{
    return m_flgShuffle;
}

bool Column::isDuplRegDisabled()
{
    return m_flgUnRegDupl;
}

const std::vector<std::string> &Column::getVecRows() const
{
    return m_vecRows;
}

double Column::getDuplicate()
{
    return m_duplicates;
}

size_t Column::getCountDupl()
{
    return m_countDupl;
}

size_t Column::getCountUniq()
{
    return m_countUniq;
}

bool Column::isValidDuplicate(std::string& errMsg)
{
    bool flg = true;
    if(m_duplicates == 1 || m_duplicates == 0){
        return true;
    }
    if(m_countDupl < 2){
        errMsg += "\n\tThere are too few duplicates specified.";
        flg = false;
    }
    if(m_countDupl > m_countRows){
        errMsg += "\n\tThere are too many duplicates specified.";
        flg = false;
    }
    return flg;
}

void Column::clearVecRows()
{
    m_vecRows.clear();
}

void Column::showGeneralInfo()
{
    if(m_flgDebug){
        std::cout << "Column name\t\t- \"" << m_name << "\"" << std::endl;
        std::cout << "Duplicates registration\t- " << (m_flgUnRegDupl ? "NO" : "YES") << std::endl;
        if(!m_flgUnRegDupl){
            std::cout << "Duplicates\t\t- " << m_duplicates << std::endl;
            std::cout << "Count duplicates\t- " << m_countDupl << std::endl;
            std::cout << "Count unique\t\t- " << m_countUniq << std::endl;
        }
        std::cout << "Count rows\t\t- " << m_countRows << std::endl;
        std::cout << "Shuffle rows\t\t- " << (m_flgShuffle ? "YES" : "NO") << std::endl;
    }
}

void Column::calcUniqAndDupl()
{
    if(m_duplicates == 1){
        m_countDupl = m_countRows;
    }else if(m_duplicates < 1){
        m_countDupl =  m_countRows * m_duplicates;
    } else{
        m_countDupl = m_duplicates;
    }
    m_countUniq = m_countRows - m_countDupl;
}

//______________Int______________

GenInt::GenInt() : Column(), m_min(0), m_max(0)
{
}

GenInt::GenInt(std::string name, int min, int max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, duplicates, flgShuffle, flgDebug), m_min(min), m_max(max)
{
}

GenInt::GenInt(std::string name, int min, int max, size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, flgShuffle, flgDebug), m_min(min), m_max(max)
{
}

void GenInt::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

int GenInt::getMin()
{
    return m_min;
}

int GenInt::getMax()
{
    return m_max;
}

bool GenInt::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(m_max < m_min){
            errMsg += "\n\tIncorrect range.";
            flg = false;
        }
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
        size_t maxCountDupl = m_countDupl / 2;
        int range = m_max - m_min;
        if(range < 0){
            range = -range;
        }
        if(static_cast<size_t>(range) < m_countRows - maxCountDupl){
            errMsg += "\n\tThe number of unique set options is not enough.";
            flg = false;
        }
    }
    if(!flg){
        m_errMesage += errMsg;
    }
    return flg;
}

void GenInt::genValue(std::uniform_int_distribution<>& distr, std::set<int>& set, size_t& enumeration, size_t& miss, size_t maxCountMiss)
{
    
    int value;
    if(miss < maxCountMiss){
        while(true){
            value = distr(m_gen);
            if(set.contains(value)){
                miss++;
            }else{
                set.insert(value);
                break;
            }
        }
    }else{
        while(true){
            value = enumeration;
            if(!set.contains(value)){
                set.insert(value);
                break;
            }else{
                enumeration++;
            }
        }
    }
    m_vecRows.push_back(std::to_string(value));
}

void GenInt::showDebug()
{
    std::cout << std::endl << std::endl;
    std::cout << "Column type\t\t- INT" << std::endl;
    showGeneralInfo();
    std::cout << "Range values\t\t- " << m_min << " - " << m_max << std::endl;
}

const std::vector<std::string>& GenInt::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "GenInt: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    std::uniform_int_distribution<int> distrValue(m_min, m_max);
    std::uniform_real_distribution<double> distrPercent(0.0, 1.0);
    size_t miss = 0;
    size_t maxCountMiss = 10000;
    size_t enumVal = m_min;
    if(m_flgDebug){
        showDebug();
        std::cout << "Properties validation\t- OK" << std::endl;
        std::cout << "Generation\t\t- START" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            m_vecRows.push_back(std::to_string(distrValue(m_gen)));
        }
    }else{
        if(m_duplicates == 1){
            m_vecRows.resize(m_countRows, std::to_string(distrValue(m_gen)));
        }else{
            std::set<int> setUnique;
            while(setUnique.size() < m_countUniq){
                genValue(distrValue, setUnique, enumVal, miss, maxCountMiss);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genValue(distrValue, setUnique, enumVal, miss, maxCountMiss);
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl -= 2;
                }
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    if(m_flgShuffle){
        shuffleRows();
    }
    if(m_flgDebug){
        std::cout << "Generation\t\t- FINISHED" << std::endl;
        std::cout << "Time\t\t\t- " << time.count() << " ms" << std::endl;
        std::cout << "Using an enumeration\t- " << (miss >= maxCountMiss ? "YES" : "NO") << std::endl;
    }
    return m_vecRows;
}

//______________Float______________

GenFloat::GenFloat() : Column(), m_min(0), m_max(0)
{
}

GenFloat::GenFloat(std::string name, double min, double max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, duplicates, flgShuffle, flgDebug), m_min(min), m_max(max)
{
}

GenFloat::GenFloat(std::string name, double min, double max, size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, flgShuffle, flgDebug), m_min(min), m_max(max)
{
}

const std::vector<std::string> &GenFloat::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "GenFloat: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    std::uniform_real_distribution<double> distrValue(m_min, m_max);
    std::uniform_real_distribution<double> distrPercent(0.0, 1.0);
    if(m_flgDebug){
        showDebug();
        std::cout << "Properties validation\t- OK" << std::endl;
        std::cout << "Generation\t\t- START" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            m_vecRows.push_back(std::to_string(distrValue(m_gen)));
        }
    }else{
        if(m_duplicates == 1){
            m_vecRows.resize(m_countRows, std::to_string(distrValue(m_gen)));
        }else{
            std::set<double> setUnique;
            while(setUnique.size() < m_countUniq){
                genValue(distrValue, setUnique);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genValue(distrValue, setUnique);
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl -= 2;
                }
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    if(m_flgShuffle){
        shuffleRows();
    }
    if(m_flgDebug){
        std::cout << "Generation\t\t- FINISHED" << std::endl;
        std::cout << "Time\t\t\t- " << time.count() << " ms" << std::endl;
    }
    return m_vecRows;
}

void GenFloat::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
}

double GenFloat::getMin()
{
    return m_min;
}

double GenFloat::getMax()
{
    return m_max;
}

bool GenFloat::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(m_max < m_min){
        errMsg += "\n\tIncorrect range.";
        flg = false;
    }
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
    }
    if(!flg){
        m_errMesage += errMsg;
    }
    return flg;
}

void GenFloat::genValue(std::uniform_real_distribution<double>& distr, std::set<double>& set)
{
    double value;
    while (true)
    {
        value = distr(m_gen);
        if(!set.contains(value)){
            set.insert(value);
            break;
        }
    }
    m_vecRows.push_back(std::to_string(value));
}

void GenFloat::showDebug()
{
    std::cout << std::endl << std::endl;
    std::cout << "Column type\t\t- FLOAT" << std::endl;
    showGeneralInfo();
    std::cout << "Range values\t\t- " << m_min << " - " << m_max << std::endl;
}

//______________Word______________

GenWord::GenWord() : Column(), m_minLength(0), m_maxLength(0), m_capitalLetter(0), m_flgUpperCase(0)
{
}

GenWord::GenWord(std::string name, size_t minLength, size_t maxLength, size_t countRows, double duplicates, 
    double capitalLetter, bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(name, countRows, duplicates, flgShuffle, flgDebug), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{    
}

GenWord::GenWord(std::string name, size_t minLength, size_t maxLength, size_t countRows, double capitalLetter, 
    bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(name, countRows, flgShuffle, flgDebug), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{
}

void GenWord::setRange(size_t minLength, size_t maxLength)
{
    m_minLength = minLength;
    m_maxLength = maxLength;
}

void GenWord::setLength(size_t length)
{
    m_minLength = m_maxLength = length;
}

void GenWord::setUpperCase(bool flg)
{
    m_flgUpperCase = flg;
}

void GenWord::setCapitalLetter(double capitalLetter)
{
    m_capitalLetter = capitalLetter;
}

size_t GenWord::getMinLength()
{
    return m_minLength;
}

size_t GenWord::getMaxLength()
{
    return m_maxLength;
}

double GenWord::getCapitalLetter()
{
    return m_capitalLetter;
}

size_t GenWord::isUpperCase()
{
    return m_flgUpperCase;
}

void GenWord::genWord(std::uniform_int_distribution<>& distrLetter, std::uniform_real_distribution<>& distrPrecent, 
        size_t& minLength, std::set<std::string>& set, size_t& miss, size_t& maxCountMiss)
{
    int numShift = (m_flgUpperCase ? 65 : 97);
    size_t length;
    int numChar;
    while(true){
        std::uniform_int_distribution<> distrLength(minLength, m_maxLength);
        length = distrLength(m_gen);
        std::string word;
        word.resize(length);
        if(miss >= maxCountMiss){
            miss = 0;
            if(minLength != m_maxLength){
                minLength++;
            }
        }
        numChar = distrLetter(m_gen) + numShift;
        if(!m_flgUpperCase && m_capitalLetter != 0 && distrPrecent(m_gen) > (1 - m_capitalLetter)){
            numChar -= 32;
        }
        word[0] = static_cast<char>(numChar);
        for(size_t i = 1; i < length; i++){
            numChar = distrLetter(m_gen) + numShift;
            word[i] = static_cast<char>(numChar);
        }
        if(!set.contains(word)){
            set.insert(word);
            m_vecRows.push_back(word);
            break;
        }else{
            miss++;
        }
    }
}

void GenWord::showDebug()
{
    std::cout << std::endl << std::endl;
    std::cout << "Column type\t\t- WORD" << std::endl;
    showGeneralInfo();
    std::cout << "GenWord length range\t- " << m_minLength << " - " << m_maxLength << std::endl;
    std::cout << "Proportion of CL\t- " << m_capitalLetter << std::endl;
    std::cout << "Is upper case enable\t- " << (m_flgUpperCase ? "YES" : "NO") << std::endl;
}

bool GenWord::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(m_maxLength < m_minLength || m_maxLength == 0){
        errMsg += "\n\tIncorrect range.";
        flg = false;
    }
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
        if(m_duplicates != 1){
            size_t maxCountDupl = m_countDupl / 2;
            if(pow(26, m_maxLength)  < m_countRows - maxCountDupl){
                errMsg += "\n\tThe number of possible options is less than necessary to generate this number of rows.";
                flg = false;
            }
        }
    }
    if(!flg){
        m_errMesage += errMsg;
    }
    return flg;
}

const std::vector<std::string>& GenWord::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "GenWord: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    std::uniform_int_distribution<> distrLetter(0, 25);
    std::uniform_real_distribution<> distrPercent(0.0, 1.0);
    std::set<std::string> setUnique;
    size_t miss = 0;
    size_t maxCountMiss = 500000;
    size_t minLength = m_minLength;
    if(m_flgDebug){
        showDebug();
        std::cout << "Properties validation\t- OK" << std::endl;
        std::cout << "Generation\t\t- START" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            genWord(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
        }
    }else{
        if(m_duplicates == 1){
            genWord(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
            m_vecRows.resize(m_countRows, m_vecRows[0]);
        }else{
            while(setUnique.size() < m_countUniq){
                genWord(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genWord(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl -= 2;
                }
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    if(m_flgShuffle){
        shuffleRows();
    }
    if(m_flgDebug){
        std::cout << "Generation\t\t- FINISHED" << std::endl;
        std::cout << "Time\t\t\t- " << time.count() << " ms" << std::endl;
    }
    return m_vecRows;
}

//______________Date______________

GenDateTime::GenDateTime()
{
    setFormat(GenDateTime::DateFormat::DATETIME);
}

GenDateTime::GenDateTime(GenDateTime::DateFormat format)
{
    setFormat(format);
}

void GenDateTime::setRange(std::string begin, std::string end)
{
    m_begin = strToDate(begin);
    m_end = strToDate(end);
}

void GenDateTime::setFormat(DateFormat format)
{
    m_format = format;
    switch (m_format)
    {
    case DateFormat::DATE:
        m_DateFormatGet = "%Y-%m-%d";
        m_DateFormatGive = "%d-%d-%d";
        break;
    case DateFormat::TIME:
        m_DateFormatGet = "%H:%M:%S";
        m_DateFormatGive = "%d:%d:%d";
        break;
    case DateFormat::DATETIME:
        m_DateFormatGet = "%Y-%m-%d %H:%M:%S";
        m_DateFormatGive = "%d-%d-%d %d:%d:%d";
        break;
    }
}

std::chrono::sys_seconds GenDateTime::getBegin()
{
    return m_begin;
}

std::chrono::sys_seconds GenDateTime::getEnd()
{
    return m_end;
}

GenDateTime::DateFormat GenDateTime::getFormat()
{
    return m_format;
}

const std::vector<std::string> &GenDateTime::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "GenDateTime: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();

    // DGeneration rows

    return m_vecRows;
}

std::string GenDateTime::dateToStr(const std::chrono::sys_seconds &dateTime)
{
    DateComponents dc = extractDateComponents(dateTime);
    char str[100];
    switch (m_format){
    case DateFormat::DATE:
        std::snprintf(str, sizeof(str), m_DateFormatGive.c_str(), dc.year, dc.month, dc.day);
        break;
    case DateFormat::TIME:
        std::snprintf(str, sizeof(str), m_DateFormatGive.c_str(), dc.hour, dc.minute, dc.second);
        break;
    case DateFormat::DATETIME:
        std::snprintf(str, sizeof(str), m_DateFormatGive.c_str(), dc.year, dc.month, dc.day, dc.hour, dc.minute, dc.second);
        break;
    }
    return str;
}

std::chrono::sys_seconds GenDateTime::strToDate(const std::string &dateTime)
{
    std::tm tm = {};
    std::istringstream ss(dateTime);
    ss >> std::get_time(&tm, m_DateFormatGet.c_str());
    if (ss.fail()) {
        std::cout << "The entered date is incorrect: " << dateTime << std::endl;
        return std::chrono::sys_seconds();
    }
    if(tm.tm_mday == 0){
        tm.tm_mday++;
    }
    return std::chrono::sys_days(std::chrono::year(tm.tm_year + 1900)/
    std::chrono::month(tm.tm_mon + 1)/std::chrono::day(tm.tm_mday)) + std::chrono::hours(tm.tm_hour) + 
    std::chrono::minutes(tm.tm_min) + std::chrono::seconds(tm.tm_sec);
}

bool GenDateTime::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(m_end < m_begin){
        errMsg += "\n\tIncorrect range.";
        flg = false;
    }
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
        auto range = m_end - m_begin;
        size_t maxCountDupl = m_countDupl / 2;
        if(m_format == GenDateTime::DateFormat::DATE){
            if(range.count() / 86400  < m_countUniq + maxCountDupl){
                errMsg += "\n\tThere are too few days between the dates.";
                flg = false;
            }
        }else{
            if(range.count()  < m_countUniq + maxCountDupl){
                errMsg += "\n\tThere are too few seconds between the dates.";
                flg = false;
            }
        }
    }
    if(!flg){
        m_errMesage += errMsg;
    }
    return flg;
}
