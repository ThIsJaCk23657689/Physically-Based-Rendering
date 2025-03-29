#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <cstdint>

typedef std::uint32_t GLObjectType;

namespace GLObjectTypes
{
constexpr GLObjectType GL_VertexArray = 0x00000001;
constexpr GLObjectType GL_Buffer = 0x00000002;
constexpr GLObjectType GL_Texture = 0x00000003;
}  // namespace GLObjectTypes

struct GLObject
{
    union
    {
        std::uint64_t integer;
        void* pointer;
    };

    GLObject( std::uint64_t i ) : integer( i ) {}
    GLObject( void* p ) : pointer( p ) {}

    template < typename T >
    operator T*() const
    {
        return static_cast< T* >( pointer );
    }
};

#endif
