#include <cstring>
#include <iostream>
#include <memory>


template <typename T>
class Sequence 
{
public:
    Sequence() = delete;
    //! using std::size_t as point 1 says the that the count is non negative. If this needs to be enforces
    //! I would prefer Strong type, similar to Exercise 1.
    explicit Sequence(std::size_t count) : size_(count), memory_(std::make_unique<T[]>(count))
    {}

    explicit Sequence(const Sequence& rhs) : size_(rhs.size_)
    {
        memory_ = std::make_unique<T[]>(size_);
        std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_, &memory_[0]);
    }

    Sequence& operator=(const Sequence& rhs)
    {
        if(&rhs == this)
            return *this;

        if(size_ < rhs.size_)
        {
            memory_.reset(new T[rhs.size_]);
            size_ = rhs.size_;
        }

        std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_, &memory_[0]);
        return *this;
    }

    bool operator==( const Sequence& rhs ) const
    {
        if(&rhs == this)
            return true;
        
        return (std::equal(&rhs.memory_[0], &rhs.memory_[0] + size_, &memory_[0]) && size_ == rhs.size_);
    }

    T& operator[](std::size_t index) const
    {
        return memory_[index];
    }

    T& at(std::size_t index) const
    {
        static T ret = 0;

        if(index > size_)
            return ret;
            
        return memory_[index];
    }

    ~Sequence() = default; // Unique pointer will be destroyed

    std::size_t get_size() const
    {
        return size_;
    }

private:
    std::unique_ptr<T[]> memory_;
    std::size_t size_;   
};

class IntegerSequence 
{
public:

    explicit IntegerSequence(std::size_t size) 
    try : s_(size)
    {} 
    catch (std::bad_array_new_length&)
    {
        throw;
    }

    IntegerSequence(const IntegerSequence& rhs) : s_(rhs.s_)
    {}

    IntegerSequence& operator=(const IntegerSequence& rhs) = default;

    bool operator==( const IntegerSequence& rhs ) const
    {
        if(&rhs == this)
            return true;    

        return (s_ == rhs.s_);
    }

    std::uint32_t& operator[](std::size_t index) const
    {
        return s_[index];
    }

    std::uint32_t& at(std::size_t index) const
    {
        return s_.at(index);
    }

    std::size_t get_size() const
    {
        return s_.get_size();
    }

private:
    Sequence<std::uint32_t> s_;
};

IntegerSequence operator+ (const IntegerSequence& lhs, const IntegerSequence& rhs)
{
    IntegerSequence ret( lhs.get_size() + rhs.get_size() );

    ret = lhs;
    std::copy(&rhs[0], &rhs[0] + rhs.get_size(), &ret[lhs.get_size()]);

    return ret;
}

// Create a sequence of consecutive numbers.
IntegerSequence create_sequence(std::size_t size, std::uint32_t start_value)
{
    IntegerSequence s(size);
    for(auto idx = 0; idx < size; idx++)
        s[idx] = start_value + idx;

    return s;
}

template <typename T>
bool validate_assignment_operation()
{
    std::size_t size = 10;

    T sequence_a = create_sequence(size, 0);
    auto sequence_b = sequence_a;

    //Positive test case, content and length equal
    if(!(sequence_a  == sequence_b))
    {
        return false;
    }

    sequence_b[0] = 1;

    // Negative test case, content unequal 
    if((sequence_a  == sequence_b))
    {
        return false;
    }

    T sequence_c = create_sequence(size * 2, 0);
    // Negative test case, unequal length, contents
    if((sequence_a  == sequence_b))
    {
        return false;
    }

    return true;
}

int main()
{
    auto ret = true;
    
    ret &= validate_assignment_operation<IntegerSequence>();           // const objects.
    ret &= validate_assignment_operation<const IntegerSequence>();     // Non const objects.

    // Validate + operator.
    auto sequence_a = create_sequence(10, 0);   // 10 elements with start value 0
    auto sequence_b = create_sequence(20, 10);  // 20 elements with start value 10
    auto sequence_c = create_sequence(30, 0);   // 30 elements with start value 0
    
    auto sequence_d = sequence_a + sequence_b;  // 30 elements with contents of a followed by b

    ret &= (sequence_c == sequence_d);

    // Strong exception gurantee will be held due to use of unique pointers. Try allocating a very big array.

    try 
    {
        auto sequence_too_big = create_sequence(UINT64_MAX, 0);
    } 
    catch(std::bad_array_new_length& e)
    {
        std::cout << "Caught: " << e.what() << std::endl;
        ret &= (sequence_c == sequence_d);
    }

    std::cout << std::boolalpha;
    std::cout << ret << std::endl;

}
