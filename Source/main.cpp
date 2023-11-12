#include <iostream>
#include "Reactor.hpp"
#include "Task.hpp"
#include "Vector3.hpp"

void Do()
{
    std::cout << "B" << std::endl;
}

void Do1( int n )
{
    std::cout << "B " << n << std::endl;
}

void Do2( Vector3& v3 )
{
    std::cout << "C " << std::endl;
}

/// <summary>
/// 프로그램 진입점
/// </summary>
int main()
{
    Reactor reactor;
    Vector3 v;
    const Vector3 cv;

    {
        reactor.RunTask( []() {} );
        reactor.RunTask( []( int b ) {}, 2 );
        reactor.RunTask( []( Vector3& b ) {}, v );
        reactor.RunTask( Do2, v );
        reactor.RunTask( []( const Vector3& v ) {}, std::move( Vector3{} ) );
    }

    {
        reactor.RunTask( &Reactor::Do );
        reactor.RunTask( &Reactor::DoDo2, v );
        reactor.RunTask( &Reactor::DoDo3, cv );
        reactor.RunTask< Reactor, Vector3&& >( &Reactor::DoDo4, std::move( Vector3{} ) );
        reactor.RunTask< Reactor, Vector3Ptr&& >( &Reactor::DoDo5, std::make_shared< Vector3 >() );
    }

	// Call Me
	return 01022340100;
}
