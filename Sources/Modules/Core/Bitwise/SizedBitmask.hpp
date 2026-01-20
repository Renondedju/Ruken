#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Core/Types/Concepts/IsIntegral.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A bitmask can be used to store multiple flags into a single integer.
 * \tparam TChunk Chunk data type. Must be an integral type.
 * \tparam TSize Number of flags to hold into the bitmask, where flags are packed by chunks.
 *               The number of flags in the bitmask is given by TSize * 8 * sizeof(TChunk)
 *               Using a power of 2 is highly recommended to preserve performances !
 *
 * \note Any operations will cost O(TSize), thus bigger chunk sizes allows better performances
 *       but might be a waste of memory if you don't use every provided flag.
 *       If you use less than 64 flags, then TSize should stay at one and you should decrease
 *       the chunk size accordingly to match your needs.
 */
template <RkSize TSize, IsIntegral TChunk = RkSize>
struct SizedBitmask
{
	static constexpr RkSize sizeof_chunk = sizeof(TChunk) * 8;
	static constexpr RkSize flags_count  = TSize * sizeof_chunk;

	#pragma region Constructors

	template <IsIntegral... TData>
	constexpr SizedBitmask(TData... in_data) noexcept;
	SizedBitmask& operator= (SizedBitmask const&) = default;
	SizedBitmask& operator= (SizedBitmask&&     ) = default;
	SizedBitmask            (SizedBitmask const&) = default;
	SizedBitmask            (SizedBitmask&&     ) = default;
	~SizedBitmask()                               = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * @brief Checks if the bitmask has all the flags passed as enabled.
	 * @tparam TData Flags to check must be an integral type
	 * @param in_data Flags to check
	 * @return True if the bitmask has all the specified flags enabled
	 *
	 * @warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
	 *          this function will result in an undefined behavior, you must make sure that your values are correct !
	 */
	template <IsIntegral... TData>
	[[nodiscard]] constexpr RkBool HasAll(TData... in_data)               const noexcept;
	[[nodiscard]] constexpr RkBool HasAll(SizedBitmask const& in_bitmask) const noexcept;

	/**
	 * @brief Checks if the bitmask has at least one the flags passed as enabled.
	 * @tparam TData Flags to check must be an integral type
	 * @param in_data Flags to check
	 * @return True if the bitmask has at least one of the specified flags enabled
	 *
	 * @warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
	 *          this function will result in an undefined behavior, you must make sure that your values are correct !
	 */
	template <IsIntegral... TData>
	[[nodiscard]] constexpr RkBool HasOne(TData... in_data)               const noexcept;
	[[nodiscard]] constexpr RkBool HasOne(SizedBitmask const& in_bitmask) const noexcept;

	/**
	 * @brief Returns the number of enabled flags in the bitmask.
	 *        The implementation is based on the Brian Kernighan's Algorithm.
	 *
	 * @return Number of enabled flags
	 * @note Time Complexity: O(log n).
	 */
	[[nodiscard]] constexpr RkUint16 Popcnt() const noexcept;

	/**
	 * @brief Enables the specified flags.
	 * @tparam TData Flags to enable must be an integral type
	 * @param in_data Flags to enable
	 *
	 * @warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
	 *          this function will result in an undefined behavior, you must make sure that your values are correct !
	 */
	template <IsIntegral... TData>
	constexpr RkVoid Set(TData... in_data)               noexcept;
	constexpr RkVoid Set(SizedBitmask const& in_bitmask) noexcept;

	/**
	 * @brief Disables the specified flags.
	 * @tparam TData Flags to disable must be an integral type
	 * @param in_data Flags to disable
	 *
	 * @warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
	 *          this function will result in an undefined behavior, you must make sure that your values are correct !
	 */
	template <IsIntegral... TData>
	constexpr RkVoid Clear(TData... in_data)               noexcept;
	constexpr RkVoid Clear(SizedBitmask const& in_bitmask) noexcept;

	/// @brief Clears the bitmask.
	constexpr RkVoid Clear() noexcept;

	/**
	 * @brief Creates a hash code for the given bitmask
	 * @return Generated hash code
	 */
	constexpr RkSize HashCode() const noexcept;

	/**
	 * @brief Executes a function pointer on each enabled flag in the bitmask.
	 * @tparam TLambdaType Type of the lambda, the signature of the function used must be RkVoid (*in_lambda)(TEnumType in_flag)
	 * @tparam TPreCast Type to cast the value into before sending it into the predicate
	 * @param in_lambda Function pointer or lambda (in case of a lambda, this will automatically be inlined by the compiler)
	 */
	template <typename TLambdaType, typename TPreCast = TChunk>
	constexpr RkVoid Foreach(TLambdaType in_lambda) const noexcept;

	#pragma endregion

	#pragma region Operators

	constexpr SizedBitmask  operator+ (SizedBitmask const& in_bitmask) const noexcept;
	constexpr SizedBitmask  operator- (SizedBitmask const& in_bitmask) const noexcept;
	constexpr RkBool        operator==(SizedBitmask const& in_other)   const noexcept;
	constexpr SizedBitmask& operator+=(SizedBitmask const& in_bitmask)       noexcept;
	constexpr SizedBitmask& operator-=(SizedBitmask const& in_bitmask)       noexcept;

	#pragma endregion

	private:

		TChunk m_data[TSize];
};

#include "SizedBitmask.inl"

END_RUKEN_NAMESPACE
