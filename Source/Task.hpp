#pragma once

#include <tuple>
#include "Concepts.hpp"

/// <summary>
/// �½�ũ �������̽�
/// </summary>
class ITask
{
public:
	/// <summary>
	/// �Ҹ���
	/// </summary>
	virtual ~ITask() = default;

	/// <summary>
	/// �����Ѵ�
	/// </summary>
	virtual void Run() 
	{
	}

	/// <summary>
	/// operator() ������
	/// </summary>
	void operator()()
	{
		Run();
	}

public:
	ITask* _next{ nullptr };
};

/// <summary>
/// ���ٸ� ������ �½�ũ
/// </summary>
template< typename TLambda, typename... TArgs >
struct LambdaTask : public ITask
{
public:
	/// <summary>
	/// ���� Ÿ��
	/// </summary>
	using Args = std::tuple< TArgs... >;

public:
	/// <summary>
	/// ������
	/// </summary>
	LambdaTask( TLambda&& lambda, TArgs&&... args )
	: _lambda{ std::forward< TLambda >( lambda )  }
	, _args  { std::forward< TArgs >( args )...   }
	{
	}

	/// <summary>
	/// �Ҹ���
	/// </summary>
	virtual ~LambdaTask() override final 
	{};

	/// <summary>
	/// �����Ѵ�
	/// </summary>
	virtual void Run() override final
	{
		std::apply( _lambda, _args );
	}

private:
	/// <summary>
	/// ���� �ν��Ͻ�
	/// </summary>
	TLambda _lambda;

	/// <summary>
	/// �޼����� ����
	/// </summary>
	Args _args;
};
