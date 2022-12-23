#pragma once

#include <vector>
#include <string>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/ECPUQueueMode.hpp"

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
			const CentralProcessingQueue& queue;
			const ECPUQueueMode           mode;
			const RkUint32                priority {0ULL};
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