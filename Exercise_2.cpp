#include <cstring>
#include <iostream>
#include <memory>


template <typename T>
class Sequence 
{
public:
    Sequence() = delete;

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
    {
        std::cout << size;
    } 
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
int main()
{}
