#pragma once

#include <source_location>
#include <memory>

namespace std
{
	template <>
	struct hash<source_location>
	{
		size_t operator()(source_location const& in_source_location) const noexcept
		{
			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return (hash<decltype(in_source_location.file_name())>()(in_source_location.file_name())
			      ^ hash<decltype(in_source_location.line     ())>()(in_source_location.line()) << 1) >> 1
			      ^ hash<decltype(in_source_location.column   ())>()(in_source_location.column()) << 1;
		}
	};

	inline bool operator==(source_location const& i_lhs, source_location const& i_rhs) noexcept
	{
		return i_lhs.file_name() == i_rhs.file_name() &&
		       i_lhs.line     () == i_rhs.line     () &&
			   i_lhs.column   () == i_rhs.column   ();
	}
}