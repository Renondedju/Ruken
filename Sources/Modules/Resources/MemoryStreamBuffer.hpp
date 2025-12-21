#pragma once

#include "Core/Types/FundamentalTypes.hpp"

#include <streambuf>
#include <iosfwd>

BEGIN_RUKEN_NAMESPACE

/// @brief Utility class used to convert an in memory buffer to a stream buffer.
///		   This avoids having to copy the contents of the buffer using a std::stringbuf.
struct MemoryStreamBuffer final : std::streambuf
{
	#pragma region Lifetime

	/**
	 * Constructor
	 * @param in_begin Begin pointer
	 * @param in_end End pointer
	 */
	explicit MemoryStreamBuffer(RkByte* in_begin, RkByte* in_end) noexcept;
	MemoryStreamBuffer			 (MemoryStreamBuffer const&) = default;
	MemoryStreamBuffer			 (MemoryStreamBuffer&&)	     = default;
	MemoryStreamBuffer& operator=(MemoryStreamBuffer const&) = default;
	MemoryStreamBuffer& operator=(MemoryStreamBuffer&&)	     = default;
	~MemoryStreamBuffer() override							 = default;

	#pragma endregion

	#pragma region Methods

	pos_type seekoff(off_type in_off, std::ios_base::seekdir in_dir, std::ios_base::openmode in_which) override;
	pos_type seekpos(pos_type in_sp								   , std::ios_base::openmode in_which) override;

	#pragma endregion
};

END_RUKEN_NAMESPACE