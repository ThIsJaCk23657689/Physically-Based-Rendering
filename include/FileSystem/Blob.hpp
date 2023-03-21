#ifndef IBLOB_HPP
#define IBLOB_HPP

#include <cstddef>

class IBlob {
public:
    virtual ~IBlob() {}
    virtual const void* data() const = 0;
    virtual std::size_t size() const = 0;
};

class Blob : public IBlob {
public:
    Blob(void* data, std::size_t size);
    virtual ~Blob() override;
    virtual const void* data() const override;
    virtual std::size_t size() const override;

private:
    void* m_data;
    std::size_t m_size;

};

#endif
