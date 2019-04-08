#include <cstring>
#include <iostream>
#include <memory>


template <typename T>
class Sequence 
{
public:
    Sequence() = delete;

    explicit Sequence(std::size_t count) : size_(count), memory_(new T[count])
    {}

    explicit Sequence(const Sequence& rhs) : size_(rhs.size_)
    {
        memory_ = new T[size_];
        std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_, &memory_[0]);
    }

    ~Sequence() 
    {
        delete [] memory_;
    }

    Sequence& operator=(const Sequence& rhs) 
    {
        if(&rhs == this)
            return *this;

        
        if(size_ < rhs.size_)
        {
            T * temp; 
            temp = (new T[rhs.size_]); // Maintain the exception gurantee, only change state if new succeeds.
            std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_, &temp[0]);            

            delete[] memory_;
            size_ = rhs.size_;
            memory_= temp;
        }
        else
        {
            std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_, &memory_[0]);
        }


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

    T at(std::size_t index) const
    {
        if(index > size_)
            return 0;
            
        return memory_[index];
    }

    std::size_t get_size() const
    {
        return size_;
    }

private:
    T* memory_;
    std::size_t size_;   
};

class IntegerSequence 
{
public:

    using sequnce_data_type = std::int32_t;
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

    sequnce_data_type& operator[](std::size_t index) const
    {
        return s_[index];
    }

    sequnce_data_type at(std::size_t index) const
    {
        return s_.at(index);
    }

    std::size_t get_size() const
    {
        return s_.get_size();
    }

private:
    Sequence<sequnce_data_type> s_;
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

    std::cout << std::boolalpha;
    std::cout << ret << std::endl;

}
