#include <cstring>
#include <iostream>
#include <memory>

template <typename T, typename P>
class NumericType 
{
public:
    explicit NumericType(const T& value) : value_{value}
    {
        if(std::is_arithmetic<T>::value && value_ < 0)
            throw std::range_error("Only arithmetic non negative values are supported");
    }

    explicit NumericType(T&& value)      : value_{std::move(value)}
    {
        if(std::is_arithmetic<T>::value && value_ < 0)
            throw std::range_error("Only arithmetic non negative values are supported");
    }

    NumericType(const NumericType& rhs) : value_{rhs.value_}
    {}

    NumericType& operator=(const NumericType& rhs)
    {
        if(&rhs == this)
            return *this;

        value_ = rhs.value_;
        return *this; 
    }

    bool operator==( const NumericType& rhs ) const
    {
        if(&rhs == this)
            return true;
        return (value_ == rhs.value_);        
    }

    T& get() { return value_; }
    const T & get() const {return value_; }

private:
    T value_;    
};

template <typename T>
class Sequence 
{
public:
    using sequence_size = NumericType<T, struct sequence_size>;

    Sequence() = delete;
    explicit Sequence(sequence_size count) : size_(count), memory_(std::make_unique<T[]>(count.get()))
    {}

    explicit Sequence(const Sequence& rhs) : size_(rhs.size_)
    {
        memory_ = std::make_unique<T[]>(size_.get());
        std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_.get(), &memory_[0]);
    }

    Sequence& operator=(const Sequence& rhs)
    {
        if(&rhs == this)
            return *this;

        if(size_.get() != rhs.size_.get())
        {
            memory_.reset(new T[rhs.size_.get()]);
            size_ = rhs.size_;
        }

        std::copy(&rhs.memory_[0], &rhs.memory_[0] + size_.get(), &memory_[0]);
        return *this;
    }

    bool operator==( const Sequence& rhs ) const
    {
        if(&rhs == this)
            return true;
        
        return (std::equal(&rhs.memory_[0], &rhs.memory_[0] + size_.get(), &memory_[0]) && size_ == rhs.size_);
    }

    T operator[](std::size_t index) const
    {
        return memory_[index];
    }

    T at(std::size_t index) const
    {
        if(index < 0 || index > size_.get())
            return 0;
            
        return memory_[index];
    }
    ~Sequence() = default; // Unique pointer will be destroyed

private:
    std::unique_ptr<T[]> memory_;
    sequence_size size_;    
    
};

template <typename T>
class IntegerSequence 
{
public:
    using sequence_size = typename Sequence<T>::sequence_size;

    explicit IntegerSequence(sequence_size size) : s_(size)
    {}

    IntegerSequence(const IntegerSequence& rhs) : s_(rhs.s_)
    {}

    IntegerSequence& operator=(const IntegerSequence& rhs) = default;

    bool operator==( const IntegerSequence& rhs ) const
    {
        if(&rhs == this)
            return true;    

        return (s_ == rhs.s_);
    }

    T operator[](std::size_t index) const
    {
        return s_[index];
    }

    T at(std::size_t index) const
    {
        return s_.at(index);
    }
private:
    Sequence<T> s_;
};

int main()
{}
