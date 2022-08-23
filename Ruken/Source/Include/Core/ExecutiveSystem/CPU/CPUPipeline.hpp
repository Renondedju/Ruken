#pragma once

#include <vector>
#include <string>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class CentralProcessingQueue;

class CPUPipeline
{
	#pragma region Members

	// Cached data

	#pragma endregion

	public:

		struct QueueInfo
		{
			/**
			 * \brief Queue instance
			 */
			const CentralProcessingQueue& queue;

			/**
			 * \brief Optimal concurrency hint for the queue
			 *
			 * TODO: Meaning of 0
			 * This is only considered a hint, workers will adapt their behavior regarding concurrency
			 * depending of the current policies and workload of the system.
			 */
			const RkSize concurrency_hint;
		};

		#pragma region Members

		const std::string            name;
		const std::vector<QueueInfo> queue_infos;

		#pragma endregion

		#pragma region Constructors

		/**
		 * \brief Default constructor
		 * \param in_name Name of the pipeline
		 * \param in_queue_infos Queue infos
		 */
		CPUPipeline(std::string_view in_name, std::vector<QueueInfo>&& in_queue_infos) noexcept;

		CPUPipeline(CPUPipeline const&) = default;
		CPUPipeline(CPUPipeline&&)      = default;
		~CPUPipeline()                  = default;

		#pragma endregion

		#pragma region Operators

		CPUPipeline& operator=(CPUPipeline const&) = delete;
		CPUPipeline& operator=(CPUPipeline&&)      = delete;

		#pragma endregion
};

END_RUKEN_NAMESPACE