#pragma once

#include <tuple>
#include "Concepts.hpp"

/// <summary>
/// 태스크 인터페이스
/// </summary>
class ITask
{
public:
	/// <summary>
	/// 소멸자
	/// </summary>
	virtual ~ITask() = default;

	/// <summary>
	/// 실행한다
	/// </summary>
	virtual void Run() 
	{
	}

	/// <summary>
	/// operator() 재정의
	/// </summary>
	void operator()()
	{
		Run();
	}

public:
	ITask* _next{ nullptr };
};

/// <summary>
/// 람다를 래핑한 태스크
/// </summary>
template< typename TLambda, typename... TArgs >
struct LambdaTask : public ITask
{
public:
	/// <summary>
	/// 인자 타입
	/// </summary>
	using Args = std::tuple< TArgs... >;

public:
	/// <summary>
	/// 생성자
	/// </summary>
	LambdaTask( TLambda func, TArgs&&... args )
	: _lambda{ func                             }
	, _args  { std::forward< TArgs >( args )... }
	{
	}

	/// <summary>
	/// 소멸자
	/// </summary>
	virtual ~LambdaTask() override final 
	{};

	/// <summary>
	/// 실행한다
	/// </summary>
	virtual void Run() override final
	{
		std::apply( _lambda, _args );
	}

private:
	/// <summary>
	/// 람다 인스턴스
	/// </summary>
	TLambda _lambda;

	/// <summary>
	/// 메서드의 인자
	/// </summary>
	Args _args;
};

/// <summary>
/// 오브젝트의 thiscall 메서드를 래핑한 태스크
/// </summary>
template< class TObject, typename... TArgs >
class MethodTask : public ITask
{
	template< int N >
	struct MethodHelper
	{
		template< class TObject, class... MethodArgs, class... TupleArgs, class... Args >
		static void DoRun( TObject* obj, void ( TObject::* method )( MethodArgs... ), const std::tuple< TupleArgs... >& targ, Args&&... args )
		{
			if constexpr ( N == 0 )
			{
				( obj->*method )( std::forward< Args >( args )... );
			}
			else
			{
				using TElement = typename std::tuple_element< N - 1, std::tuple< TupleArgs... > >::type;
				MethodHelper< N - 1 >::DoRun( obj, method, targ, std::forward< TElement >( std::get< N - 1 >( targ ) ), std::forward< Args >( args )... );
			}
		}
	};

	template< typename TObject, typename... MethodArgs, typename... TupleArgs >
	void DoRun( TObject* obj, void ( TObject::* method )( MethodArgs... ), std::tuple< TupleArgs... > const& targ )
	{
		MethodHelper< sizeof...( TupleArgs ) >::DoRun( obj, method, targ );
	}

public:
	/// <summary>
	/// 인자 타입
	/// </summary>
	using Args = std::tuple< TArgs... >;

	/// <summary>
	/// T객체의 메서드 타입 정의
	/// </summary>
	using TMethod = void( TObject::* )( TArgs... );

public:
	/// <summary>
	/// 생성자
	/// </summary>
	MethodTask( TObject* obj, TMethod method, TArgs&&... args )
	: _owner { obj                              }
	, _method{ method                           }
	, _args  { std::forward< TArgs >( args )... }
	{
	}

	/// <summary>
	/// 소멸자
	/// </summary>
	virtual ~MethodTask() override final
	{};

	/// <summary>
	/// 실행한다
	/// </summary>
	virtual void Run() override final
	{
		DoRun( _owner, _method, _args );
	}

private:
	/// <summary>
	/// T 클래스 객체의 포인터
	/// </summary>
	TObject* _owner;
	
	/// <summary>
	/// thiscall 메서드 함수 포인터
	/// </summary>
	TMethod _method;

	/// <summary>
	/// 메서드의 인자
	/// </summary>
	Args _args;
};
