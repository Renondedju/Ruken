#pragma once

#include "Filesystem/File.hpp"
#include "Filesystem/Filesystem.hpp"
#include "Filesystem/IOJobQueue.hpp"

#include "Core/Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 *
 */
struct ResourceImporter
{
	#pragma region Lifetime

	ResourceImporter()					                 = default;
	ResourceImporter(ResourceImporter const&)	         = default;
	ResourceImporter(ResourceImporter&&)		         = default;
	ResourceImporter& operator=(ResourceImporter const&) = default;
	ResourceImporter& operator=(ResourceImporter&&)		 = default;
    virtual ~ResourceImporter()					         = default;

	#pragma endregion

	#pragma region Methods

	/// @returns a list of the supported file extensions.
	virtual std::vector<std::string> SupportedExtensions() const noexcept = 0;

	/**
	 * Imports a file.
	 * @param in_file File to import.
	 * @return Import task.
	 */
	virtual IOTask<RkVoid> Import(FileHandle const& in_file) noexcept = 0;

	#pragma endregion
};

END_RUKEN_NAMESPACE