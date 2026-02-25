#include "Core/Debug/RAIISentinel.hpp"

#include <print>
#include <iostream>

USING_RUKEN_NAMESPACE

RAIISentinel::RAIISentinel()
{
	std::println(std::cout, "{{{0}}}.RAIISentinel()", m_instance_index);
}

RAIISentinel::RAIISentinel(const RAIISentinel& in_other)
{
	std::println(std::cout, "{{{0}}}.RAIISentinel(const RAIISentinel& {1})", m_instance_index, in_other.m_instance_index);
}

RAIISentinel::RAIISentinel(RAIISentinel&& in_other)
{
	std::println(std::cout, "{{{0}}}.RAIISentinel(RAIISentinel&& {1})", m_instance_index, in_other.m_instance_index);
}

RAIISentinel& RAIISentinel::operator=(const RAIISentinel& in_other)
{
	std::println(std::cout, "{{{0}}}.operator =(const RAIISentinel& {1})", m_instance_index, in_other.m_instance_index);

	return *this;
}

RAIISentinel& RAIISentinel::operator=(RAIISentinel&& in_other)
{
	std::println(std::cout, "{{{0}}}.operator =(RAIISentinel&& {1})", m_instance_index, in_other.m_instance_index);

	return *this;
}

RAIISentinel::~RAIISentinel()
{
	std::println(std::cout, "{{{0}}}.~RAIISentinel()", m_instance_index);
}
