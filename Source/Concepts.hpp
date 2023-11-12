#pragma once

/// <summary>
/// Callable �������̽�
/// </summary>
template<typename T>
concept Callable = requires( T obj )
{
	{ obj() } -> std::same_as< void >;
};