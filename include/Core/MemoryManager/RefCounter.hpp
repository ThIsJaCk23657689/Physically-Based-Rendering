#ifndef REFCOUNTER_HPP
#define REFCOUNTER_HPP

#include <atomic>

template < class T >
class RefCounter : public T
{
public:
    unsigned long AddRef() override { return ++m_RefCount; }

    unsigned long Release() override
    {
        unsigned long result = --m_RefCount;
        if ( result == 0 ) delete this;
        return result;
    }

private:
    std::atomic< unsigned long > m_RefCount = 1;
};

#endif
