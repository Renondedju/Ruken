
#pragma once

#include <ostream>

#include "Debug/Logging/Handlers/LogHandler.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class sends logging output to a stream.
 */
class StreamHandler : public LogHandler
{
    protected:

        #pragma region Members

        std::ostream m_stream;

        #pragma endregion

    public:

        #pragma region Contructors

        explicit StreamHandler(LogFormatter const& in_formatter, std::ostream const& in_stream) noexcept;

        StreamHandler(StreamHandler const& in_copy) = delete;
        StreamHandler(StreamHandler&&      in_move) = delete;

        ~StreamHandler() override;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Empties the entire queue to the stream.
         * \note  This method must not be called on multiple threads at a time.
         */
        RkVoid Flush() final;

        #pragma endregion

        #pragma region Operators

        StreamHandler& operator=(StreamHandler const& in_other) = delete;
        StreamHandler& operator=(StreamHandler&&      in_other) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE