#pragma once

#ifndef LINELIST_HPP
#define LINELIST_HPP

#include <cstddef>
#include <vector>

template<typename T>
class LineListBase
{
public:
    virtual ~LineListBase() {};
    
    virtual T & operator[](size_t index) = 0;
    virtual void insert(size_t beforeIndex, T what) = 0;
    virtual void erase(size_t index) = 0;
    virtual size_t size() const = 0;

    inline void append(T what)
    {
        insert(size(), what);
    }
};

template<typename T>
class VectorLineList : public LineListBase<T>
{
public:
    inline virtual ~VectorLineList() {}

    inline virtual T & operator[](size_t index) override final
    {
        return lines_[index];
    }

    inline virtual void insert(size_t beforeIndex, T what) override final
    {
        lines_.insert(lines_.begin() + beforeIndex, std::move(what));
    }

    inline virtual void erase(size_t index) override final
    {
        lines_.erase(lines_.begin() + index);
    }

    inline virtual size_t size() const override final
    {
        return lines_.size();
    }

private:
    std::vector<T> lines_;
};

template<typename T>
using LineList = VectorLineList<T>;

#endif // LINELIST_HPP

