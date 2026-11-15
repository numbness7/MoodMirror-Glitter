#ifndef VARIABLE_MANIPULATION_H
#define VARIABLE_MANIPULATION_H
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <assert.h>
template<typename ARRAY> ARRAY substring(const ARRAY str, const unsigned int startInclusive, const unsigned int endExclusive){
	ARRAY sub;
	for(unsigned int i = startInclusive; i < endExclusive; i++)
		sub.push_back(str[i]);
	return sub;
}

template<typename ARRAY> bool contains(const ARRAY str, const ARRAY subStr)
{
    if(str.size() < subStr.size())
		return false;
    for(unsigned int i = 0; i <= str.size() - subStr.size(); i++)
        if(substring(str, i, i + subStr.size()) == subStr)
            return true;
    return false;
}

template <typename NUMBER, typename ITERABLE> NUMBER mean(ITERABLE iterable);
template <typename NUMBER, typename ITERABLE> NUMBER sum(ITERABLE iterable);
template <typename CONTAINER> CONTAINER::value_type  maxM(const CONTAINER c);
template <typename CONTAINER> CONTAINER::value_type  minM(const CONTAINER c);
template <typename KEY, typename NUMBER> std::tuple<KEY, NUMBER> maxM(std::map<KEY,NUMBER> mapping);
// #Variable Manipulation

template <typename T> void mySwap(T& t1, T& t2){
    T temp = t1;
    t1 = t2;
    t2 = temp;
}


/// @brief Average the columns.
/// @tparam V
/// @param vec Multiple rows.
/// @return The average of each individual column.
template <typename V> std::vector<V> average(std::vector<std::vector<V>> vec);

/// @brief Get a vector of the given size for which every entry is initialized to the given element
/// 
/// Don't use push_back when adding items with the expectation it will add items before the filling
/// @tparam T 
/// @param size given size
/// @param element given size
/// @return the vector
template <typename T> std::vector<T> getFilledVector(unsigned int size, T element);


/// @brief 
/// @tparam T 
/// @param arrayCopyTo array to copy to.
/// @param arrayCopyFrom array to copy from.
/// @param sizeCopyTo size of array to copy to.
/// @param sizeCopyFrom size of array to copy from.
/// @return True if size of arrays are equal, False otherwise.
template <typename T> bool doCopy(T arrayCopyTo[], const T arrayCopyFrom[], const unsigned int sizeCopyTo, const unsigned int sizeCopyFrom){
    for(unsigned int i = 0; i < sizeCopyTo && i < sizeCopyFrom; i++)
        arrayCopyTo[i] = arrayCopyFrom[i];
    return sizeCopyTo == sizeCopyFrom;
}

/// @brief Create a copy of arrayCopyFrom.
/// @tparam T 
/// @param arrayCopyFrom 
/// @param size 
/// @return The copy.
template <typename T> T* copyInitialize(const T arrayCopyFrom[], const unsigned int size){
    T* aCopy = (T*) malloc(sizeof(T)*size);
    doCopy(aCopy, arrayCopyFrom, size, size);  
    return aCopy;
}
// #Simple Math Operations ----------------------------------------
template <typename NUMBER, typename ITERABLE> NUMBER sum(ITERABLE iterable){
    NUMBER summation = 0;
    for(auto const number : iterable) summation += number;
    return summation;
}
template <typename CONTAINER> CONTAINER::value_type  maxM(const CONTAINER c)
{
    typename CONTAINER::value_type maxi = c[0];
    for(auto v : c){
        if(v > maxi){
            maxi = v;
        }
    }
    return maxi;
}
template <typename CONTAINER> CONTAINER::value_type  minM(const CONTAINER c)
{
    typename CONTAINER::value_type mini = c[0];
    for(auto v : c){
        if(v < mini){
            mini = v;
        }
    }
    return mini;
}
template <typename KEY, typename NUMBER> std::tuple<KEY, NUMBER> maxM(std::map<KEY, NUMBER> mapping)
{
    auto [tMXKey, mxVal] = *mapping.begin();
    KEY mxKey = tMXKey;
    for(auto it = mapping.begin(); it != mapping.end(); ++it){
        auto [cKey, cVal] = *it;
        if(cVal > mxVal){
            mxVal = cVal;
            mxKey = cKey;
        }
    }
    return std::tuple<KEY, NUMBER>{mxKey, mxVal};
}
template <typename NUMBER, typename ITERABLE>
NUMBER mean(ITERABLE iterable)
{
    NUMBER summation = ssum<NUMBER>(iterable);
    return summation/iterable.size();
}
// #Variable Manipulation
template <typename V> std::vector<V> average(std::vector<std::vector<V>> vec){
    std::vector<V> averages{};
    const int rowSize = vec.size();
    assert(rowSize > 0);
    const int colSize = vec[0].size();
    for(int col = 0; col < colSize; col++){
        averages.push_back(0);
        for(int row = 0; row < rowSize; row++){
            assert(vec[row].size() == colSize);
            averages[col] += vec[row][col];
        }
        averages[col] /= rowSize;
    }
    return averages;
}

template <typename T>
std::vector<T> getFilledVector(unsigned int size, T element)
{
    std::vector<T> vec;
    vec.reserve(size);
    for(unsigned int i = 0; i < size; i++)
        vec.push_back(element);
    return vec;
}

template <typename T>
std::string vec2str(T vec, unsigned int size)
{
    std::string s = "(";
    assert(size > 0);
    unsigned char part;
    for(part=0; part < size - 1; part++){
        s += std::to_string(vec[part]) + ',';
    }
    s += std::to_string(vec[part])  + ')';
    return s;
}
#endif