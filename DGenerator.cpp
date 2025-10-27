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
    template <typename T>
    void showVec(const std::vector<T>& vec, bool inLine = true)
    {
        std::cout << "size = " << vec.size() << " -- ";
        for(const auto& val : vec){
            std::cout << (inLine ? " " : "\n") << val;
        }
        std::cout << std::endl;
    }
}

//______________Column______________

Column::Column() : m_flgUnRegDupl(false), m_flgShuffle(false), m_duplicates(0), m_countDupl(0), 
m_countRows(0), m_rd(), m_gen(m_rd())
{
}

Column::Column(size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
m_flgUnRegDupl(false), m_flgShuffle(flgShuffle), m_flgDebug(flgDebug), m_duplicates(duplicates), 
m_countRows(countRows), m_rd(), m_gen(m_rd())
{
    calcUniqAndDupl();
}

Column::Column(size_t countRows, bool flgShuffle, bool flgDebug) :  
m_flgUnRegDupl(true), m_flgShuffle(flgShuffle), m_flgDebug(flgDebug), m_duplicates(0), m_countRows(countRows), m_rd(), 
m_gen(m_rd())
{
    calcUniqAndDupl();
}

void Column::setConfig(std::string name, bool nullable, bool unique, bool primaryKey, size_t length)
{
    m_config.name = name;
    m_config.nullable = nullable;
    m_config.unique = unique;
    m_config.primaryKey = primaryKey;
    m_config.length = length;
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

const ColumnConfig &Column::getConfig() const
{
    return m_config;
}

std::string Column::getRow(size_t index) const
{
    if(index < m_vecRows.size() && index >= 0){
        return m_vecRows[index];
    }
    return "";
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

const std::vector<std::string> &Column::getRows() const
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

void Column::removeRow(size_t index)
{
    if(index < m_vecRows.size() && index >= 0){
        std::swap(m_vecRows[index], m_vecRows.back());
        m_vecRows.pop_back();
    }
}

bool Column::isValidDuplicate(std::string &errMsg)
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

const std::vector<std::string> &Column::genRows()
{
    std::cout << "The \"genRows()\" method of the \"Column\" base class was called!" << std::endl;
    return m_vecRows;
}

void Column::showGeneralInfo()
{
    std::cout << std::endl;
    outputDebugLine("Column name", 24, m_config.name);
    outputDebugLine("Column type", 24, m_config.type);
    outputDebugLine("Duplicates registration", 24, m_flgUnRegDupl, "NO", "YES");
    if(!m_flgUnRegDupl){
        outputDebugLine("Duplicates", 24, m_duplicates);
        outputDebugLine("Count duplicates", 24, m_countDupl);
        outputDebugLine("Count unique", 24, m_countUniq);
    }
    outputDebugLine("Count rows", 24, m_countRows);
    outputDebugLine("Shuffle rows", 24, m_flgShuffle, "YES", "NO");
}

template <typename T>
void Column::outputDebugLine(const std::string& lable, unsigned int gap, const T& value)
{
    std::cout << lable << "\r" << "\033[" + std::to_string(gap) + "C- " << value << std::endl;
}

template <typename T1, typename T2>
void Column::outputDebugLine(const std::string& lable, unsigned int gap, bool condition, const T1& tr, const T2& fls)
{
    std::cout << lable << "\r" << "\033[" + std::to_string(gap) + "C- " << (condition ? tr : fls) << std::endl;
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

GenInt::GenInt() : c_objId(s_nextId++)
{
}

GenInt::GenInt(int min, int max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(countRows, duplicates, flgShuffle, flgDebug), c_objId(s_nextId++), m_min(min), m_max(max)
{
    setDefaultConfig();
}

GenInt::GenInt(int min, int max, size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(countRows, flgShuffle, flgDebug), c_objId(s_nextId++), m_min(min), m_max(max)
{
    setDefaultConfig();
}

GenInt::GenInt(int start, int step, size_t countRows, bool flgDebug) : 
Column(countRows, false, flgDebug), c_objId(s_nextId++), m_min(start), m_max(start), m_flgSequence(true), m_step(step)
{
    setDefaultConfig();
}

void GenInt::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

void GenInt::setflgSequence(bool flg)
{
    m_flgSequence = flg;
}

bool GenInt::isSequence()
{
    return m_flgSequence;
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
    if(!m_flgUnRegDupl && !m_flgSequence){
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
    showGeneralInfo();
    outputDebugLine("Range values", 24, std::to_string(m_min) + " - " + std::to_string(m_max));
    outputDebugLine("Is Sequence", 24, m_flgSequence, "YES", "NO");
}

void GenInt::setDefaultConfig()
{
    m_config.name = "INT" + std::to_string(c_objId);
    m_config.type = "INT";
    m_config.nullable = true;
    m_config.primaryKey = false;
    m_config.unique = false;
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
        outputDebugLine("Generation", 24, "START");
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        if(m_flgSequence){
            for(size_t i = 0; i < m_countRows; i++){
                m_vecRows.push_back(std::to_string(m_min + i * m_step));
            }
        }else{
            for(size_t i = 0; i < m_countRows; i++){
                m_vecRows.push_back(std::to_string(distrValue(m_gen)));
            }
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
        outputDebugLine("Generation", 24, "FINISHED");
        outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
        outputDebugLine("Using an enumeration", 24, miss >= maxCountMiss, "YES", "NO");
    }
    return m_vecRows;
}

//______________Float______________

GenFloat::GenFloat() : c_objId(s_nextId++), m_min(0), m_max(0)
{
}

GenFloat::GenFloat(double min, double max, size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(countRows, duplicates, flgShuffle, flgDebug), c_objId(s_nextId++), m_min(min), m_max(max)
{
    setDefaultConfig();
}

GenFloat::GenFloat(double min, double max, size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(countRows, flgShuffle, flgDebug), c_objId(s_nextId++), m_min(min), m_max(max)
{
    setDefaultConfig();
}

GenFloat::GenFloat(double start, double step, size_t countRows, bool flgDebug) :
Column(countRows, false, flgDebug), c_objId(s_nextId++), m_min(start), m_max(start), m_flgSequence(true), m_step(step)
{
    setDefaultConfig();
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
        outputDebugLine("Generation", 24, "START");
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        if(m_flgSequence){
            for(size_t i = 0; i < m_countRows; i++){
                m_vecRows.push_back(std::to_string(m_min + i * m_step));
            }
        }else{
            for(size_t i = 0; i < m_countRows; i++){
                m_vecRows.push_back(std::to_string(distrValue(m_gen)));
            }
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
        outputDebugLine("Generation", 24, "FINISHED");
        outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
    }
    return m_vecRows;
}

void GenFloat::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
}

void GenFloat::setFlgSequence(bool flg)
{
    m_flgSequence = flg;
}

bool GenFloat::isSequence()
{
    return m_flgSequence;
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
    if(!m_flgUnRegDupl && !m_flgSequence){
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
    showGeneralInfo();
    outputDebugLine("Range values", 24, std::to_string(m_min) + " - " + std::to_string(m_max));
    outputDebugLine("Is sequence", 24, m_flgSequence, "YES", "NO");
}

void GenFloat::setDefaultConfig()
{
    m_config.name = "FLOAT" + std::to_string(c_objId);
    m_config.type = "FLOAT";
    m_config.nullable = true;
    m_config.primaryKey = false;
    m_config.unique = false;
}

//______________Word______________

GenWord::GenWord() : c_objId(s_nextId++), m_minLength(0), m_maxLength(0), m_capitalLetter(0), m_flgUpperCase(0)
{
}

GenWord::GenWord(size_t minLength, size_t maxLength, size_t countRows, double duplicates, 
    double capitalLetter, bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(countRows, duplicates, flgShuffle, flgDebug), c_objId(s_nextId++), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{
    setDefaultConfig();
}

GenWord::GenWord(size_t minLength, size_t maxLength, size_t countRows, double capitalLetter, 
    bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(countRows, flgShuffle, flgDebug), c_objId(s_nextId++), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{
    setDefaultConfig();
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

void GenWord::genValue(std::uniform_int_distribution<>& distrLetter, std::uniform_real_distribution<>& distrPrecent, 
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
    showGeneralInfo();
    outputDebugLine("Word length range", 24, std::to_string(m_minLength) + " - " + std::to_string(m_maxLength));
    outputDebugLine("Proportion of CL", 24, m_capitalLetter);
    outputDebugLine("Is upper case enable", 24, m_flgUpperCase, "YES", "NO");
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

void GenWord::setDefaultConfig()
{
    m_config.name = "WORD" + std::to_string(c_objId);
    m_config.type = "VARCHAR";
    m_config.nullable = true;
    m_config.primaryKey = false;
    m_config.unique = false;
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
        outputDebugLine("Generation", 24, "START");
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            genValue(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
        }
    }else{
        if(m_duplicates == 1){
            genValue(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
            m_vecRows.resize(m_countRows, m_vecRows[0]);
        }else{
            while(setUnique.size() < m_countUniq){
                genValue(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genValue(distrLetter, distrPercent, minLength, setUnique, miss, maxCountMiss);
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
        outputDebugLine("Generation", 24, "FINISHED");
        outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
    }
    return m_vecRows;
}

//______________Date______________

GenDateTime::GenDateTime() : c_objId(s_nextId++) 
{
    setFormat(DateFormat::DATETIME);
}

GenDateTime::GenDateTime(size_t countRows, DateFormat format, std::string begin, std::string end, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(countRows, duplicates, flgShuffle, flgDebug), c_objId(s_nextId++), m_format(format)
{
    setDefaultConfig();
    setFormat(format);
    setRange(begin, end);
}

GenDateTime::GenDateTime(size_t countRows, DateFormat format, std::string begin, std::string end, bool flgShuffle, bool flgDebug) : 
Column(countRows, flgShuffle, flgDebug), c_objId(s_nextId++), m_format(format)
{
    setDefaultConfig();
    setFormat(format);
    setRange(begin, end);
}

GenDateTime::GenDateTime(size_t countRows, DateFormat format, std::string begin, std::chrono::seconds step, bool flgDebug) : 
Column(countRows, false, flgDebug), c_objId(s_nextId++), m_step(step), m_flgSequence(true)
{
    setDefaultConfig();
    setFormat(format);
    setRange(begin, begin);
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

void GenDateTime::setFlgSequence(bool flg)
{
    m_flgSequence = flg;
}

bool GenDateTime::isSequence()
{
    return m_flgSequence;
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
    std::set<std::chrono::sys_seconds> setUnique;
    std::uniform_int_distribution<size_t> distDays(0, std::chrono::duration_cast<std::chrono::days>(m_end - m_begin).count());
    std::uniform_int_distribution<size_t> distSeconds(0, std::chrono::duration_cast<std::chrono::seconds>(m_end - m_begin).count());
    std::uniform_real_distribution<double> distrPercent(0.0, 1.0);
    if(m_flgDebug){
        showDebug();
        outputDebugLine("Generation", 24, "START");
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        if(m_flgSequence){
            for(size_t i = 0; i < m_countRows; i++){
                m_vecRows.push_back(dateToStr(m_begin + m_step * i));
            }
        }else{
            for(size_t i = 0; i < m_countRows; i++){
                genValue(distDays, distSeconds, setUnique);
            }
        }
        
    }else{
        if(m_duplicates == 1){
            genValue(distDays, distSeconds, setUnique);
            m_vecRows.resize(m_countRows, m_vecRows[0]);
        }else{
            while(setUnique.size() < m_countUniq){
                genValue(distDays, distSeconds, setUnique);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genValue(distDays, distSeconds, setUnique);
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
        outputDebugLine("Generation", 24, "FINISHED");
        outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
    }
    return m_vecRows;
}

void GenDateTime::genValue(std::uniform_int_distribution<size_t>& distDays, std::uniform_int_distribution<size_t>& distSeconds, std::set<std::chrono::sys_seconds>& set)
{
    std::chrono::sys_seconds date;
    while (true){
        if(m_format == DateFormat::DATE){
            date = m_begin + std::chrono::days(distDays(m_gen));
        }else{
            date = m_begin + std::chrono::seconds(distSeconds(m_gen));
        }
        if(!set.contains(date)){
            set.insert(date);
            break;
        }
    }
    m_vecRows.push_back(dateToStr(date));
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
    if(!m_flgUnRegDupl && !m_flgSequence){
        flg = isValidDuplicate(errMsg);
        size_t maxCountDupl = m_countDupl / 2;
        if(m_format == DateFormat::DATE){
            std::chrono::days range = std::chrono::duration_cast<std::chrono::days>(m_end - m_begin);
            if(range.count() < m_countUniq + maxCountDupl){
                errMsg += "\n\tThere are too few days between the dates.";
                flg = false;
            }
        }else{
            std::chrono::seconds range = std::chrono::duration_cast<std::chrono::seconds>(m_end - m_begin);
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

void GenDateTime::showDebug()
{
    showGeneralInfo();
    std::cout << "Date format\r"<< "\033[24C- " << (m_format == DateFormat::DATE ? "DATE" 
        : m_format == DateFormat::TIME ? "TIME" 
        : m_format == DateFormat::DATETIME ? "DATETIME" 
        : "Unknown") << std::endl;
    outputDebugLine("Date range", 24, dateToStr(m_begin) + " - " + dateToStr(m_end));
}

void GenDateTime::setDefaultConfig()
{
    m_config.name = "DATATIME" + std::to_string(c_objId);
    m_config.type = "DATATIME";
    m_config.nullable = true;
    m_config.primaryKey = false;
    m_config.unique = false;
}

//______________String______________

GenString::GenString() : c_objId(s_nextId++), m_maxCountRows(0)
{}

GenString::GenString(size_t countRows, double duplicates, bool flgShuffle, bool flgDebug) : 
Column(countRows, duplicates, flgShuffle, flgDebug), c_objId(s_nextId++)
{
    setDefaultConfig();
}

GenString::GenString(size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(countRows, flgShuffle, flgDebug), c_objId(s_nextId++)
{
    setDefaultConfig();
}

GenString::GenString(size_t countRows, bool flgDebug) : 
Column(countRows, false, flgDebug), m_flgSequence(true), c_objId(s_nextId++)
{
    setDefaultConfig();
}

void GenString::addColumn(Column* column, std::string prefix,  std::string suffix)
{
    size_t countRows = column->getCountRows();
    if(countRows > m_maxCountRows){
        m_maxCountRows = column->getCountRows();
    }
    m_vecColumns.push_back(column);
    m_vecPrefix.push_back(prefix);
    m_vecSuffix.push_back(suffix);
}

void GenString::setFlgSequence(bool flg)
{
    m_flgSequence = flg;
}

bool GenString::isSequence()
{
    return m_flgSequence;
}

void GenString::showConfig()
{
    std::cout << "String Config\r" << "\033[24C" << "- \"";
    for(int i = 0; i < m_vecColumns.size(); i++){
        std::cout << m_vecPrefix[i] << "<" << m_vecColumns[i]->m_config.name << ">"  << m_vecSuffix[i];
    }
    std::cout << "\"" << std::endl;
    std::cout << "- Name:" << std::endl;
    std::cout << "- Type:" << std::endl;
    std::cout << "- Count rows:" << std::endl;
    for(int i = 0; i < m_vecColumns.size(); i++){
        int shift = 16 * (i + 1);
        std::cout << "\033[3A";
        std::cout  << "\033[" + std::to_string(shift) + "C" << "\"" << m_vecColumns[i]->m_config.name << "\"" << std::endl;
        std::cout  << "\033[" + std::to_string(shift) + "C" << "\"" << m_vecColumns[i]->m_config.type << "\"" << std::endl;
        std::cout  << "\033[" + std::to_string(shift) + "C" << "\"" << m_vecColumns[i]->m_countRows << "\"" << std::endl;
    }
}

const std::vector<std::string> &GenString::genRows()
{
    for(int i = 0; i < m_vecColumns.size(); i++){
        if(m_flgDebug && !m_vecColumns[i]->m_flgDebug){
            outputDebugLine("Generation " + m_vecColumns[i]->m_config.type, 24, "START");
            auto start = std::chrono::high_resolution_clock::now();
            m_vecColumns[i]->genRows();
            auto finish = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
            outputDebugLine("Generation " + m_vecColumns[i]->m_config.type, 24, "FINISHED");
            outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
        }else{
            m_vecColumns[i]->genRows();
        }
    }
    joinColumns();
    return m_vecRows;
}

const std::vector<std::string> &GenString::joinColumns()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "GenString: joinColumns: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    if(m_flgDebug){
        showDebug();
    }
    std::vector<std::vector<std::string>> matValues;
    copyVectors(matValues);
    size_t count; 
    if(m_flgSequence){
        count = 0;
    }else{
        count = m_maxCountRows - 1;
    }
    std::uniform_real_distribution<double> distrPercent(0.0, 1.0);
    if(m_flgDebug){
        outputDebugLine("Gluing rows", 24, "START");
    }
    auto start = std::chrono::high_resolution_clock::now();
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            glueString(matValues, count);
        }
    }else{
        if(m_duplicates == 1){
            glueString(matValues, count);
            m_vecRows.resize(m_countRows, m_vecRows[0]);
        }else{
            while(m_vecRows.size() < m_countUniq){
                glueString(matValues, count);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    glueString(matValues, count);
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl -= 2;
                }
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    if(m_flgDebug){
        outputDebugLine("Gluing rows", 24, "FINISHED");
        outputDebugLine("Time", 24, std::to_string(time.count()) + " ms");
    }
    if(m_flgShuffle){
        shuffleRows();
    }
    return m_vecRows;
}

std::string GenString::glueString(std::vector<std::vector<std::string>>& matValues, size_t& count)
{
    std::string str;
    size_t index;
    if(m_flgSequence){
        index = count;
        count++;
    }else{
        std::uniform_int_distribution<size_t> distRow(0, count);
        index = distRow(m_gen);
        count--;
    }
    for(size_t i = 0; i < matValues.size(); i++){
        str += m_vecPrefix[i] + matValues[i][index] + m_vecSuffix[i];
        if(!m_flgSequence){
            matValues[i][index]; 
            std::swap(matValues[i][index], matValues[i].back());
        }
    }
    m_vecRows.push_back(str);
    return str;
}

void GenString::copyVectors(std::vector<std::vector<std::string>>& matValues)
{
    matValues.clear();
    for(int i = 0; i < m_vecColumns.size(); i++){
        matValues.push_back(m_vecColumns[i]->m_vecRows);
        if(matValues[i].size() < m_maxCountRows){
            matValues[i].resize(m_maxCountRows, "");
        }
    }
}

bool GenString::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
        size_t maxCountDupl = m_countDupl / 2;
        if(m_maxCountRows < m_countUniq + maxCountDupl){
            errMsg += "\n\tNot enough entries in side columns.";
            flg = false;
        }
    }
    return flg;
}

void GenString::setDefaultConfig()
{
    m_config.name = "STRING" + std::to_string(c_objId);
    m_config.type = "VARCHAR";
    m_config.nullable = true;
    m_config.primaryKey = false;
    m_config.unique = false;
}

void GenString::showDebug()
{
    showGeneralInfo();
    showConfig();
}

//______________Table______________

Table::Table()
{
}

Table::Table(std::string tableName) : m_tableName(tableName)
{
}

void Table::setTableName(std::string tableName)
{
    m_tableName = tableName;
}

std::string Table::getTableName()
{
    return m_tableName;
}

void Table::readColumn(Column *column)
{
    m_vecColConfig.push_back(column->getConfig());
    m_matValues.push_back(column->getRows());
    if(m_countRows < column->getCountRows()){
        m_countRows = column->getCountRows();
    }
    equalizeVectors();
}

void Table::showTable()
{
    for(int i = 0; i < m_matValues.size(); i++){
        std::string shift = std::to_string(32 * i);
        std::cout << m_vecColConfig[i].name << "\r\033[" + shift + "C";
    }
    std::cout << std::endl;
    for(int j = 0; j < m_countRows; j++){
        for(int i = 0; i < m_matValues.size(); i++){
            std::string shift = std::to_string(32 * i);
            std::cout << m_matValues[i][j] << "\r\033[" + shift + "C";
        }
    }
}

void Table::equalizeVectors()
{
    for(int i = 0; i < m_matValues.size(); i++){
        if(m_matValues[i].size() < m_countRows){
            m_matValues[i].resize(m_countRows, "");
        }
    }
}

//______________GenSqlScript______________

GenSqlScript::GenSqlScript()
{
}

GenSqlScript::GenSqlScript(std::string fileName) : m_fileName(fileName)
{
}

void GenSqlScript::createTable(Table table, bool overwriteFile, bool overwriteTable)
{
    if(overwriteFile){
        m_file.open(m_fileName, std::ios::trunc);
    }else{
        m_file.open(m_fileName, std::ios::app);
    }
    if(overwriteTable){
        m_file << "";
    }
}

