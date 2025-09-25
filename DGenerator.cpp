#include "DGenerator.hpp"

//___________Column______________

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

Column::Column(size_t countRows, std::string name, bool flgShuffle, bool flgDebug) : m_name(name), 
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

//___________Int______________

Int::Int() : m_min(0), m_max(0)
{
}

Int::Int(std::string name, size_t min, size_t max, size_t countRows, float duplicates, bool flgShuffle, 
    bool flgDebug) : Column(name, countRows, duplicates, flgShuffle, flgDebug), m_min(min), m_max(max)
{
    if(m_flgDebug){
        showDebug();
    }
}

Int::Int(size_t min, size_t max, size_t countRows, std::string name, bool flgShuffle, 
    bool flgDebug) : Column(countRows, name, flgShuffle, flgDebug), m_min(min), m_max(max)
{
    if(m_flgDebug){
        showDebug();
    }
}

void Int::setRange(size_t min, size_t max)
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
        if(static_cast<size_t>(m_max - m_min) < m_countRows - maxCountDupl){
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
    std::uniform_int_distribution<int> distrVal(m_min, m_max);
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
            m_vecRows.push_back(std::to_string(distrVal(m_gen)));
        }
    }else{
        if(m_duplicates == 1){
            m_vecRows.resize(m_countRows, std::to_string(distrVal(m_gen)));
        }else{
            std::set<int> setUnique;
            while(setUnique.size() < m_countUniq){
                genValue(distrVal, setUnique, enumVal, miss, maxCountMiss);
                // std::cout << ".";
            }
            size_t stillDupl = m_countDupl;
            std::uniform_real_distribution<float> distrChange(0.0, 1.0);
            while(stillDupl > 0){
                if((distrChange(m_gen) > 0.7 || stillDupl == 1) && stillDupl != m_countDupl){
                    m_vecRows.push_back(m_vecRows.back());
                    stillDupl--;
                }else{
                    genValue(distrVal, setUnique, enumVal, miss, maxCountMiss);
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
