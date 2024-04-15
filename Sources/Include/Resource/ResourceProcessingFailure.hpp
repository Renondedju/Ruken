
#pragma once

#include <string>
#include <exception>

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Resource/Enums/EResourceLoadingFailureCode.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Resource loading failure exception.
 * 
 * This exception and all the derived exceptions are handled by the resource manager.
 */
struct ResourceProcessingFailure final : std::exception
{
    #pragma region Variables

    /**
     * \brief Long description of the exception.
     * This will be logged by the resource manager
     */
    std::string description;

    /**
     * \brief Resource validity after the exception.
     * If set to true, the error message will still be displayed but the resource will be considered as valid.
     * This can be useful if a resource by default has been loaded instead.
     */
    RkBool resource_validity;

    /**
     * \brief Error code.
     * This is a short description of the error and can be indicative of if a retry can be considered or not.
     */
    EResourceProcessingFailureCode code;

    #pragma endregion

    #pragma region Constructors

    explicit ResourceProcessingFailure (EResourceProcessingFailureCode in_code, RkBool in_validity = false, RkChar const* in_description = "") noexcept;

    ResourceProcessingFailure ()                                         noexcept = delete;
    ResourceProcessingFailure (ResourceProcessingFailure const& in_copy) noexcept = default;
    ResourceProcessingFailure (ResourceProcessingFailure&& in_move)         noexcept = default;
    virtual ~ResourceProcessingFailure()                                          = default;

    #pragma endregion

    #pragma region Methods
    
    char const* what() const override;

    #pragma endregion 

    #pragma region Operators

    /**
     * \brief std::string representation
     */
    [[nodiscard]]
    explicit operator std::string() const noexcept;

    ResourceProcessingFailure& operator=(ResourceProcessingFailure const& in_copy) noexcept = default;
    ResourceProcessingFailure& operator=(ResourceProcessingFailure&&      in_move) noexcept = default;

    #pragma endregion
};

END_RUKEN_NAMESPACE
