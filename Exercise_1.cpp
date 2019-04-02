#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

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

    T& get() { return value_; }
    const T & get() const {return value_; }

private:
    T value_;    
};

class Shape 
{
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual void print() const
    {
        std::cout << "The area is: " << area() << std::endl;       
    }
};

class Rectangle : public Shape 
{
public:
    using width  = NumericType<double, struct width>;
    using height = NumericType<double, struct height>;

    //! Width, height are double type therefore pass by value
    explicit Rectangle(width w, height h) : w_(w), h_(h)
    {}

    ~Rectangle() = default;
    double area() const override
    {
        return w_.get() * h_.get();
    }

    void print() const override
    {
        std::cout << "Width: " << w_.get() << " Height: " << h_.get() << " ";
        Shape::print();
    }

private:
    width  w_;
    height h_;
};

class Circle : public Shape
{
public:
    using radius = NumericType<double, struct radius>;
    explicit Circle(radius r) : r_(r)
    {}

    ~Circle() = default;

    double area() const override
    {
        const auto pi = 3.1456l;
        return (pi * r_.get() * r_.get());
    }

    void print() const override
    {
        std::cout << "Radius: " << r_.get() << " ";
        Shape::print();
    }

private:
    radius r_;
};


int main()
{
    std::vector<std::unique_ptr<Shape>> container;

    // Store 2 rectangles
    container.push_back(std::make_unique<Rectangle>(Rectangle::width(2), Rectangle::height(3)));
    container.push_back(std::make_unique<Rectangle>(Rectangle::width(3), Rectangle::height(4)));
    
    // Store 2 Circles
    container.push_back(std::make_unique<Circle>(Circle::radius(2)));
    container.push_back(std::make_unique<Circle>(Circle::radius(3)));

    // Additional Rectangle
    container.push_back(std::make_unique<Rectangle>(Rectangle::width(4), Rectangle::height(5)));
    // Additional circle
    container.push_back(std::make_unique<Circle>(Circle::radius(4)));


    // Iterate & print
    for(const auto& s:container)
        s->print();

    // Negative test cases:
    // Try negative width
    try
    {
        container.push_back(std::make_unique<Rectangle>(Rectangle::width(-4), Rectangle::height(5)));    
    }
    catch(const std::range_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    // Negative test cases:
    // Try negative radius
    try
    {
        container.push_back(std::make_unique<Circle>(Circle::radius(-4)));    }
    catch(const std::range_error& e)
    {
        std::cerr << e.what() << '\n';
    }    
}
