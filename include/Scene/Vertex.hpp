#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <type_traits>

struct Vertex
{
    struct Position
    {
        float x, y, z;
        Position& operator=( const glm::vec3& vec )
        {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            return *this;
        }
        float* ptr() { return reinterpret_cast< float* >( this ); }
    };

    struct Normal
    {
        float x, y, z;
        Normal& operator=( const glm::vec3& vec )
        {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            return *this;
        }
        float* ptr() { return reinterpret_cast< float* >( this ); }
    };

    struct TexCoord
    {
        float u, v;
        TexCoord& operator=( const glm::vec2& vec )
        {
            u = vec.x;
            v = vec.y;
            return *this;
        }
        float* ptr() { return reinterpret_cast< float* >( this ); }
    };

    Position position;
    Normal normal;
    TexCoord texCoord;
};

static_assert( std::is_standard_layout_v< Vertex > );
static_assert( std::is_standard_layout_v< Vertex::Position > );
static_assert( std::is_standard_layout_v< Vertex::Normal > );
static_assert( std::is_standard_layout_v< Vertex::TexCoord > );
#endif
