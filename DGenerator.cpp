#include "DGenerator.hpp"

//______________Column______________

Column::Column() : m_name(""), m_flgUnRegDupl(false), m_flgShuffle(false), m_duplicates(0), m_countDupl(0), 
m_countRows(0), m_rd(), m_gen(m_rd())
{
}

Column::Column(std::string name, size_t countRows, float duplicates, bool flgShuffle, bool flgDebug) : 
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

void Column::setDuplicate(float val)
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

float Column::getDuplicate()
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

Int::Int() : m_min(0), m_max(0)
{
}

Int::Int(std::string name, int min, int max, size_t countRows, float duplicates, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, duplicates, flgShuffle, flgDebug), m_min(min), m_max(max)
{
    if(m_flgDebug){
        showDebug();
    }
}

Int::Int(std::string name, int min, int max, size_t countRows, bool flgShuffle, bool flgDebug) : 
Column(name, countRows, flgShuffle, flgDebug), m_min(min), m_max(max)
{
    if(m_flgDebug){
        showDebug();
    }
}

void Int::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

int Int::getMin()
{
    return m_min;
}

int Int::getMax()
{
    return m_max;
}

bool Int::isValidProperties()
{
    std::string errMsg = "\nIncorrect properties: ";
    bool flg = true;
    if(!m_flgUnRegDupl){
        flg = isValidDuplicate(errMsg);
        size_t maxCountDupl = m_countDupl / 2;
        if(m_max < m_min){
            errMsg += "\n\tIncorrect range.";
            flg = false;
        }
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

void Int::genValue(std::uniform_int_distribution<>& distr, std::set<int>& set, size_t& enumeration, size_t& miss, size_t maxCountMiss)
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

void Int::showDebug()
{
    std::cout << std::endl << std::endl;
    std::cout << "Column type\t\t- INT" << std::endl;
    showGeneralInfo();
    std::cout << "Range values\t\t- " << m_min << " - " << m_max << std::endl;
}

const std::vector<std::string>& Int::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "Int: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    std::uniform_int_distribution<int> distrValue(m_min, m_max);
    std::uniform_real_distribution<float> distrPercent(0.0, 1.0);
    size_t miss = 0;
    size_t maxCountMiss = 10000;
    size_t enumVal = m_min;
    if(m_flgDebug){
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

//______________Word______________

Word::Word() : Column(), m_minLength(0), m_maxLength(0), m_capitalLetter(0), m_flgUpperCase(0)
{
}

Word::Word(std::string name, size_t minLength, size_t maxLength, size_t countRows, float duplicates, 
    float capitalLetter, bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(name, countRows, duplicates, flgShuffle, flgDebug), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{    
}

Word::Word(std::string name, size_t minLength, size_t maxLength, size_t countRows, float capitalLetter, 
    bool flgUpperCase, bool flgShuffle, bool flgDebug) : 
    Column(name, countRows, flgShuffle, flgDebug), m_minLength(minLength), m_maxLength(maxLength), 
    m_capitalLetter(capitalLetter), m_flgUpperCase(flgUpperCase)
{
}

void Word::setRange(size_t minLength, size_t maxLength)
{
    m_minLength = minLength;
    m_maxLength = maxLength;
}

void Word::setLength(size_t length)
{
    m_minLength = m_maxLength = length;
}

void Word::setUpperCase(bool flg)
{
    m_flgUpperCase = flg;
}

void Word::setCapitalLetter(float capitalLetter)
{
    m_capitalLetter = capitalLetter;
}

size_t Word::getMinLength()
{
    return m_minLength;
}

size_t Word::getMaxLength()
{
    return m_maxLength;
}

float Word::getCapitalLetter()
{
    return m_capitalLetter;
}

size_t Word::isUpperCase()
{
    return m_flgUpperCase;
}

void Word::genWord(std::uniform_int_distribution<>& distrLetter, std::uniform_int_distribution<>& distrLength, 
    std::uniform_real_distribution<>& distrPrecent, std::set<std::string>& set)
{
    int numShift = (m_flgUpperCase ? 65 : 97);
    size_t length = distrLength(m_gen);
    std::string word;
    word.resize(length);
    int numChar;
    while(true){
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
            break;
        }
    }
    m_vecRows.push_back(word);
}

bool Word::isValidProperties()
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
            if(pow(26, m_maxLength - m_minLength)  < m_countRows - maxCountDupl){
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

const std::vector<std::string>& Word::genRows()
{
    m_errMesage.clear();
    if(!isValidProperties()){
        std::cout << "Word: genRows: Error: " << m_errMesage << std::endl;
        return m_vecRows;
    }
    m_vecRows.clear();
    std::uniform_int_distribution<> distrLetter(0, 25);
    std::uniform_real_distribution<> distrPercent(0.0, 1.0);
    std::uniform_int_distribution<> distrLength(m_minLength, m_maxLength);
    std::set<std::string> setUnique;
    if(m_flgUnRegDupl){
        for(size_t i = 0; i < m_countRows; i++){
            genWord(distrLetter, distrLength, distrPercent, setUnique);
        }
    }else{
        if(m_duplicates == 1){
            genWord(distrLetter, distrLength, distrPercent, setUnique);
            m_vecRows.resize(m_countRows, m_vecRows[0]);
        }else{
            while(setUnique.size() < m_countUniq){
                genWord(distrLetter, distrLength, distrPercent, setUnique);
            }
            size_t stillDupl = m_countDupl;
            while(stillDupl > 0){
                if((distrPercent(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genWord(distrLetter, distrLength, distrPercent, setUnique);
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl -= 2;
                }
            }
        }
    }
    return m_vecRows;
}