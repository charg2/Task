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
    ReactorPtr reactor = std::make_shared< Reactor >();
    Vector3 v;
    const Vector3 cv;

    {
        reactor->RunTask( [ reactor ]() {} );
        reactor->RunTask( []( int b ) {}, 2 );
        reactor->RunTask( []( Vector3& b ) {}, v );
        reactor->RunTask( Do2, v );
        reactor->RunTask( []( const Vector3& v ) {}, Vector3{} );
    }

    {
        // reactor.RunTask( &Reactor::Do );
        // reactor.RunTask( &Reactor::DoDo, v );
        // reactor.RunTask( &Reactor::DoDo2, v );

        v._x = 99;

        // reactor.RunTask( &Reactor::DoDo3, cv );
        // reactor.RunTask< Reactor, Vector3&& >( &Reactor::MoveTo, std::move( Vector3{} ) );
        // reactor.RunTask( &Reactor::MoveTo, Vector3{} );
        //reactor.RunTask< Reactor, Vector3Ptr&& >( &Reactor::MoveTo, std::make_shared< Vector3 >() );
    }

    reactor->Flush();

	// Call Me
	return 01022340100;
}
