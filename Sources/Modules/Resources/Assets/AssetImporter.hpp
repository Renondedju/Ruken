#pragma once

#include "Core/Service.hpp"
#include "Core/Meta/Meta.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/File.hpp"

#include "Resources/Resource.hpp"
#include "Resources/ImportContext.hpp"

#include <vector>

BEGIN_RUKEN_NAMESPACE

struct AssetImporter final : Service
{
	/// @brief Pretty name of the service.
	constexpr static std::string_view service_name = RUKEN_STRING(AssetImporter);

	#pragma region Lifetime

	/// @brief Constructor
	explicit AssetImporter(ServiceProvider& in_provider) noexcept;
	AssetImporter(AssetImporter const&)	           = delete;
	AssetImporter(AssetImporter&&)		           = delete;
	AssetImporter& operator=(AssetImporter const&) = delete;
	AssetImporter& operator=(AssetImporter&&)	   = delete;
	~AssetImporter() override					   = default;

	#pragma endregion

	struct Importer
	{
		#pragma region Lifetime

		Importer()							 = default;
		Importer(Importer const&)	         = default;
		Importer(Importer&&)		         = default;
		Importer& operator=(Importer const&) = default;
		Importer& operator=(Importer&&)	     = default;
		virtual ~Importer()					 = default;

		#pragma endregion

		#pragma region Methods

		/**
		 * Imports a file.
		 * @param in_context Import context.
		 * @return Import task.
		 */
		virtual IOTask<RkVoid> Import(ImportContext& in_context) noexcept = 0;

		/// @returns a list of the supported file extensions.
		virtual std::vector<std::string> SupportedExtensions() const noexcept = 0;

		#pragma endregion
	};

	#pragma region Methods

	/// @brief Provides a new importer type to the library.
	template <typename TImporter> requires std::is_base_of_v<Importer, TImporter>
	RkVoid ProvideImporter() { m_importers.emplace_back(std::make_unique<TImporter>()); }

	/// @brief Tries to import the passed file.
	IOTask<RkVoid> Import(FilePath const& in_file_path) const noexcept;

	#pragma endregion

	private:

		#pragma region Members

		std::vector<std::unique_ptr<Importer>> m_importers {};

		#pragma endregion

		#pragma region Methods

		/// @brief Looks though the importers and returns the first compatible one or nullptr if not found.
		Importer* GetCompatibleImporter(std::filesystem::path const& in_extension) const;

		#pragma endregion
};

END_RUKEN_NAMESPACE