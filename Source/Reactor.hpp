#include <iostream>
#include <concurrent_queue.h>
#include "Vector3.hpp"
#include "Task.hpp"

class Reactor
{
    /// <summary>
    /// 태스크 큐 타입 정의
    /// </summary>
    using TaskQueue = concurrency::concurrent_queue< ITask >;

public:
    void Do()
    {
        std::cout << "Do" << std::endl;
    }

    void DoDo( Vector3&& Vector3 )
    {
        std::cout << "DoDo" << std::endl;
    }

    void DoDo2( Vector3& Vector3 )
    {
        std::cout << "DoDo2" << std::endl;
    }

    void DoDo3( const Vector3& Vector3 )
    {
        std::cout << "DoDo3" << std::endl;
    }

    void DoDo4( Vector3&& Vector3 )
    {
        std::cout << "DoDo4" << std::endl;
    }

    void DoDo5( std::shared_ptr< Vector3 >&& vector3 )
    {
        std::cout << "DoDo5" << std::endl;
    }

public:
    template< typename T, typename... Args >
    auto RunTask( void ( T::* memfunc )( Args... ), Args&&... args )
    {
        std::cout << "---------------------------------------" << __FUNCDNAME__ << std::endl;

        auto m = MethodTask< T, Args...>( static_cast< T* >( this ), memfunc, std::forward< Args >( args )... );
        m();
        std::cout << "메서드: " << sizeof( m ) << std::endl;

        // _taskQueue.push( m );


        return;
    }

    template< typename... Args, std::invocable< Args... > TLambda >
    auto RunTask( TLambda&& lambda, Args&&... args )
    {
        std::cout << "---------------------------------------" << __FUNCDNAME__ << std::endl;

        auto l = LambdaTask< TLambda, Args... >( std::forward< TLambda >( lambda ), std::forward< Args >( args )... );
        l();
        
        std::cout << "람다: " << sizeof( l ) << std::endl;

        // _taskQueue.push( l );

        return;
    }

private:
    /// <summary>
    /// 태스크 큐
    /// </summary>
    concurrency::concurrent_queue< ITask > _taskQueue;
};

using ReactorPtr = std::shared_ptr< Reactor >;
using ReactorRef = const ReactorPtr&;