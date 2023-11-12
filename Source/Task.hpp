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
	LambdaTask( TLambda func, TArgs&&... args )
	: _lambda{ func                             }
	, _args  { std::forward< TArgs >( args )... }
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

/// <summary>
/// ������Ʈ�� thiscall �޼��带 ������ �½�ũ
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
	/// ���� Ÿ��
	/// </summary>
	using Args = std::tuple< TArgs... >;

	/// <summary>
	/// T��ü�� �޼��� Ÿ�� ����
	/// </summary>
	using TMethod = void( TObject::* )( TArgs... );

public:
	/// <summary>
	/// ������
	/// </summary>
	MethodTask( TObject* obj, TMethod method, TArgs&&... args )
	: _owner { obj                              }
	, _method{ method                           }
	, _args  { std::forward< TArgs >( args )... }
	{
	}

	/// <summary>
	/// �Ҹ���
	/// </summary>
	virtual ~MethodTask() override final
	{};

	/// <summary>
	/// �����Ѵ�
	/// </summary>
	virtual void Run() override final
	{
		DoRun( _owner, _method, _args );
	}

private:
	/// <summary>
	/// T Ŭ���� ��ü�� ������
	/// </summary>
	TObject* _owner;
	
	/// <summary>
	/// thiscall �޼��� �Լ� ������
	/// </summary>
	TMethod _method;

	/// <summary>
	/// �޼����� ����
	/// </summary>
	Args _args;
};
