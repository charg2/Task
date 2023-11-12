#pragma once

/// <summary>
/// Callable 인터페이스
/// </summary>
template<typename T>
concept Callable = requires( T obj )
{
	{ obj() } -> std::same_as< void >;
};