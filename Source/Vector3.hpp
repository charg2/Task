#pragma once

#include <iostream>

struct Vector3
{
public:
    Vector3()
    {
        std::cout << "생성자" << std::endl;
    }

    Vector3( const Vector3& rhs )
    {
        std::cout << "복사 생성자" << std::endl;
    }

    Vector3( Vector3&& rhs ) noexcept
    {
        std::cout << "이동 생성자" << std::endl;
    }

    ~Vector3()
    {
        std::cout << "소멸자" << std::endl;
    }

    float _x{};
    float _y{};
    float _z{};
};

using Vector3Ptr = std::shared_ptr< Vector3 >;
using Vector3Ref = const Vector3Ptr&;
