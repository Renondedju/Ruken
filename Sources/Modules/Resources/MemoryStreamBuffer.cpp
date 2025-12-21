#include "Resources/MemoryStreamBuffer.hpp"

USING_RUKEN_NAMESPACE

MemoryStreamBuffer::MemoryStreamBuffer(RkByte* in_begin, RkByte* in_end) noexcept
{
	this->setg(in_begin, in_begin, in_end);
}

std::streambuf::pos_type MemoryStreamBuffer::seekoff(
	off_type			   const in_off,
	std::ios_base::seekdir const in_dir,
	std::ios_base::openmode		 in_which)
{
	if (in_dir == std::ios_base::cur)
		gbump(in_off);
	else if (in_dir == std::ios_base::end)
		setg(eback(), egptr() + in_off, egptr());
	else if (in_dir == std::ios_base::beg)
		setg(eback(), eback() + in_off, egptr());
	return gptr() - eback();
}

std::streambuf::pos_type MemoryStreamBuffer::seekpos(pos_type const in_sp, std::ios_base::openmode const in_which)
{
	return seekoff(in_sp - pos_type(0), std::ios_base::beg, in_which);
}
