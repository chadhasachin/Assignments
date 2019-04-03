#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <iostream>
#include <set>
#include <vector>

//! Add elements in odd position, with start point, generic function and generic iterator
template <typename Iterator, typename T, typename Func>
T add_odd_elements(Iterator begin, Iterator end, const T& start_point, Func adder)
{
    std::vector<T> odd_elements;
    size_t pos = 0;
    T sum{start_point};

    std::copy_if(begin, end, std::back_inserter(odd_elements),
                        [&pos] (const T& elm) 
                        {
                           return !(pos++ % 2);
                        });
   
    for(const auto& v:odd_elements)
    {
        sum = adder(sum, v);
    }
    return sum;  
}
//! Add elements in odd position, with start point and generic function
template <typename T, typename Func>
T add_odd_elements(const std::vector<T>& elements, const T& start_point, Func adder)
{
    return add_odd_elements(elements.begin(), elements.end(), start_point, adder);
}

//! Add elements in odd position, with start point
template <typename T>
T add_odd_elements(const std::vector<T>& elements, const T& start_point)
{
    return add_odd_elements(elements, start_point, std::plus<>{});
}

//! Add elements in odd position
template <typename T>
auto add_odd_elements(const std::vector<T>& elements)
{
    return add_odd_elements<T>(elements,{});
}


bool validate_odd_element_summation()
{

    bool ret = true;

    const std::vector<std::uint32_t> unsigned_data_odd_count{1,2,3,4,5,6,7};
    ret &= (add_odd_elements(unsigned_data_odd_count) == 16);

    const std::vector<std::uint32_t> unsigned_data_even_count{1,2,3,4,5,6,7,8};
    ret &= (add_odd_elements(unsigned_data_even_count) == 16);

    const std::vector<std::int32_t> signed_data_odd_count{-1,2,-3,4,5,6,-7};
    ret &= (add_odd_elements(signed_data_odd_count) == -6);

    const std::vector<std::int32_t> signed_data_even_count{-1,2,-3,4,5,6,-7,8};
    ret &= (add_odd_elements(signed_data_even_count) == -6);

    const std::vector<float> float_data_even_count{1.0f, 2.0f, 3.9f};
    ret &= ( (4.9f - add_odd_elements(float_data_even_count)) <  std::numeric_limits<float>::epsilon());
 
    const std::vector<std::string> string_data{"Hello", "Cruel", " world"};
    ret &= (add_odd_elements(string_data) == "Hello world");

    const std::vector<char> char_data{'0', '1', '2'};
    ret &= (add_odd_elements(char_data) == ('0' + '2'));

    return ret;
}

bool validate_start_point_element_summation()
{

    bool ret = true;

    const std::vector<std::uint32_t> unsigned_data_odd_count{1,2,3,4,5,6,7};
    ret &= (add_odd_elements(unsigned_data_odd_count, 2u) == 18);

    const std::vector<std::int32_t> signed_data_even_count{-1,2,-3,4,5,6,-7,8};
    ret &= (add_odd_elements(signed_data_even_count, 1) == -5);

    const std::vector<std::int32_t> empty_list;
    ret &= (add_odd_elements(empty_list, 1) == 1);

    return ret;
}

bool validate_custom_function_element_summation()
{

    bool ret = true;

    const std::vector<std::uint32_t> unsigned_data_odd_count{1,2,3,4,5,6,7};
    ret &= (add_odd_elements(unsigned_data_odd_count, 2u, [](std::uint32_t partial_sum, std::uint32_t elm) {return partial_sum + elm;}) == 18);

    const std::vector<std::int32_t> signed_data_even_count{-1,2,-3,4,5,6,-7,8};
    ret &= (add_odd_elements(signed_data_even_count, 1,   [](std::int32_t partial_sum, std::int32_t elm) {return partial_sum + elm;}) == -5);

    const std::vector<std::int32_t> empty_list;
    ret &= (add_odd_elements(empty_list, 1, [](std::int32_t partial_sum, std::int32_t elm) {return partial_sum + elm;}) == 1);

    const std::vector<std::string> string_data{"Hello", "Cruel", " world"};
    ret &= (add_odd_elements(string_data, {}, [](std::string partial_sum, std::string elm) {return partial_sum + elm;}) == "Hello world");

    return ret;
}

bool validate_custom_iterator_element_summation()
{

    bool ret = true;

    const std::vector<std::uint32_t> unsigned_data_odd_count{1,2,3,4,5,6,7};
    ret &= (add_odd_elements(unsigned_data_odd_count.begin(), unsigned_data_odd_count.end(), 2u, [](std::uint32_t partial_sum, std::uint32_t elm) {return partial_sum + elm;}) == 18);

    const std::set<std::int32_t> signed_data_even_count{-1,2,3,4,5,6,7};
    ret &= (add_odd_elements(signed_data_even_count.begin(), signed_data_even_count.end(), 1,   [](std::int32_t partial_sum, std::int32_t elm) {return partial_sum + elm;}) == 15);

    return ret;
}

int main()
{
    std::cout << std::boolalpha;
    auto ret = validate_odd_element_summation() && validate_start_point_element_summation() && validate_custom_function_element_summation() && validate_custom_iterator_element_summation();
    std::cout << ret << std::endl;
    return ret;

    
}
