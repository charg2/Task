#include <iostream>
#include <concurrent_queue.h>
#include "Vector3.hpp"
#include "Task.hpp"

class Reactor
{
    /// <summary>
    /// �½�ũ ť Ÿ�� ����
    /// </summary>
    using TaskQueue = concurrency::concurrent_queue< ITask* >;

public:
    void Do()
    {
        std::cout << "Do" << std::endl;
    }

    void DoDo( Vector3 Vector3 )
    {
        std::cout << "DoDo" << std::endl;
    }

    void DoDo2( Vector3& vector3 )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void DoDo3( const Vector3& Vector3 )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void DoDo34( const Vector3& fromPos, const Vector3& toPos )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void MoveTo( const Vector3& fromPos, const Vector3& toPos )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void MoveTo( Vector3& Vector3 )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void MoveTo( Vector3 Vector3 )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

    void MoveTo( std::shared_ptr< Vector3 >&& vector3 )
    {
        std::cout << __FUNCDNAME__ << std::endl;
    }

public:
    /// <summary>
    /// �۾��� �����Ѵ�.
    /// </summary>
    template< typename... Args, std::invocable< Args... > TLambda >
    auto RunTask( TLambda&& lambda, Args&&... args )
    {
        _taskQueue.push( new LambdaTask< TLambda, Args... >(
            std::forward< TLambda >( lambda ), 
            std::forward< Args >( args )... ) );
    }

    /// <summary>
    /// �۾��� ����
    /// </summary>
    auto Flush()
    {
        ITask* task;
        while ( _taskQueue.try_pop( task ) )
        {
            task->Run();
            delete task;
        }
    }

private:
    /// <summary>
    /// �½�ũ ť
    /// </summary>
    TaskQueue _taskQueue;
};

using ReactorPtr = std::shared_ptr< Reactor >;
using ReactorRef = const ReactorPtr&;