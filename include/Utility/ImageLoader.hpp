#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "FileSystem/Blob.hpp"

class StbImageBlob : public IBlob {
public:
    StbImageBlob(unsigned char* _data) : m_data(_data) {
    }

    virtual ~StbImageBlob() {
        if (m_data) {
            stbi_image_free(m_data);
            m_data = nullptr;
        }
    }

    const void* data() const override {
        return m_data;
    }

    std::size_t size() const override {
        // nobody cares
        return 0;
    }

private:
    unsigned char* m_data = nullptr;

};

#endif
