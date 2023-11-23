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
	LambdaTask( TLambda&& lambda, TArgs&&... args )
	: _lambda{ std::forward< TLambda >( lambda )  }
	, _args  { std::forward< TArgs >( args )...   }
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
