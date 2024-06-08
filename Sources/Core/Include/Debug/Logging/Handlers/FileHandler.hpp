
#pragma once

#include <fstream>

#include "Debug/Logging/Handlers/LogHandler.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class sends logging output to a disk file.
 */
class FileHandler final : public LogHandler
{
    private:

        #pragma region Members

        std::ofstream m_stream;

        #pragma endregion

    public:

        #pragma region Contructors

        explicit FileHandler(LogFormatter const& in_formatter,
                             std::string  const& in_path,
                             std::ios::openmode  in_mode) noexcept;

        FileHandler(FileHandler const& in_copy) = delete;
        FileHandler(FileHandler&&      in_move) = delete;

        ~FileHandler() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Empties the entire queue to the file.
         * \note  This method must not be called on multiple threads at a time.
         */
        RkVoid Flush() override;

        #pragma endregion

        #pragma region Operators

        FileHandler& operator=(FileHandler const& in_other) = delete;
        FileHandler& operator=(FileHandler&&      in_other) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE