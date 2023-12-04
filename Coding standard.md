# Ruken engine C++ coding standard

> This standard is mainly focused on code readability as well as reducing bug prone patterns while keeping the code fairly pretty and easy on the eyes.  
> For this reason, we encourage anyone using this standard to comment as much their code as they can, even for things that could be thought trivial at first.  
> The documentation standard used for function and class headers is the doxygen syntax.  
> A good knowledge of the [cpp core guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html) is also encouraged !

## 1. Naming standard

If you need any help regarding the naming standard, please refer to any present code  
Using plugins like ReSharper or Visual assist is encouraged to make sure the naming standard is followed as it should

- Classes and structs      : UpperCamelCase
- Enums 			       : EUpperCamelCase 
- Enum members             : UpperCamelCase
- Unions 			       : UpperCamelCase
- Template parameters      : TUpperCamelCase_UnderscoreTolerant
- Parameters 		       : in_snake_case or out_snake_case or inout_snake_case
- Local variables 	       : snake_case
- Functions and Methods    : UpperCamelCase
- Members or fields        : m_snake_case
- Public members or fields : snake_case
- Constants 			   : snake_case
- Namespaces 			   : snake_case
- Typedefs 				   : UpperCamelCase_UnderscoreTolerant
- Macros 				   : UPPER_SNAKE_CASE
- Concepts				   : UpperCamelCase

> Note: For parameters, the "in_" prefix is used for input parameters, "out_" for output params (usually non-const references or pointers) and "inout_" for parameters that do both input and output
> even if such parameters should not be used

## 2. Structs should only be used as containers without logic or for template hells

Here are some examples:


## 3. Attributes should be on top of the function definition, with exception of documentation-less getters

```cpp 
// Ok

/**
 * \brief If a value other than 'NotSet' has been set using "SetLevel()", it is returned.
 * Otherwise, the hierarchy is traversed towards the root until a value other than 'NotSet' is found, and that value is returned.
 * \return The effective level of this logger.
 */
[[nodiscard]]
ELogLevel GetEffectiveLevel() const noexcept;

[[nodiscard]] int GetA() const noexcept;
[[nodiscard]] int GetA()       noexcept;

[[nodiscard]] int GetB() const noexcept;
[[nodiscard]] int GetB()       noexcept;
``` 

```cpp 
// Wrong !

/**
 * \brief If a value other than 'NotSet' has been set using "SetLevel()", it is returned.
 * Otherwise, the hierarchy is traversed towards the root until a value other than 'NotSet' is found, and that value is returned.
 * \return The effective level of this logger.
 */
[[nodiscard]] ELogLevel GetEffectiveLevel() const noexcept;
// ^ This is tolerated if it increase the readability of your code

[[nodiscard]]
int GetA() const noexcept;

[[nodiscard]]
int GetA()       noexcept;

[[nodiscard]]
int GetB() const noexcept;

[[nodiscard]]
int GetB()       noexcept;
```

## 1.4. Initialize everything in the header if possible, specialize in the constructor(s)

```cpp
// Ok

// Usually member implementation should go in the associated .cpp file
// but for the sake of simplicity we wont here.
class Data
{
	public:

		#pragma region Members

		int  a {1};
		int  b {2};
		char d {'c'};
		
		#pragma endregion

		#pragma region Constructors

		Data() = default;
		Data(int in_a) noexcept:
			a {in_a}
		{ }

		#pragma endregion 
};
```


```cpp

// Wrong !
class Data
{
	public:

		#pragma region Members

		int  a;
		int  b;
		char d;
		
		#pragma endregion

		#pragma region Constructors

		Data() noexcept:
			a {1},
			b {2},
			d {'c'}
		{ }

		Data(int in_a) noexcept:
			a {in_a},
			b {2},
			d {'c'}
		{ }

		#pragma endregion 
};
```

## 5. Statements (`if`, `for`, `while` ...) should not use "{}" if they only contain 1 instruction

```cpp
// OK
if (propagate && m_parent)
	m_parent->ForceHandle(in_record);

// Acceptable but the for should not have braces
for (auto const& handler : m_handlers)
{
	if (handler)
		handler->Handle(in_record);
}
```

## 6. Constructors should use curly brackets initialization

Even for sub classes

```cpp
Test::Test(int dummy) noexcept: // <- the colon is glued to the noexcept
	m_data {dummy},
	m_test {0} // <- Curly brackets are used instead of parenthesis, also note that the value is glued to the curly brackets
{ }
```

## 7. Orders of regions

Here is the preferred order of `#pragma region` in a header

- Members
- Constructors
- Methods
- Operators

## 8. Don't delete default constructors

```cpp
// OK
#pragma region Constructors

template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
constexpr Bitmask(TData... in_data) noexcept;

constexpr Bitmask(Bitmask const& in_copy) = default;
constexpr Bitmask(Bitmask&&      in_move) = default;

#pragma endregion

// Wrong !

#pragma region Constructors

template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
constexpr Bitmask(TData... in_data) noexcept;

constexpr Bitmask()                       = delete;
constexpr Bitmask(Bitmask const& in_copy) = default;
constexpr Bitmask(Bitmask&&      in_move) = default;

#pragma endregion
``` 

## 9. Keep comments compacts

```cpp
// OK
/**
 * \brief Swaps the endianness of the value (32).
 * \param in_x Base value
 * \return Converted value
 */
 
// Wrong
/**
 * \brief Swaps the endianness of the value (32).
 *
 * \param in_x Base value
 *
 * \return Converted value
 */
``` 

## 10. Always specify default constructors and operators *(if there are any)*

A default class should look like that

```cpp
/**
 * Some well written docs to explain precisely what this class does
 */
class TestClass
{
	public:

		#pragma region Constructors

		TestClass() 						= default;
		TestClass(TestClass const& in_copy) = default;
		TestClass(TestClass&& 	   in_move) = default;
		~TestClass() 						= default;

		#pragma endregion

		#pragma region Operators

		TestClass& operator=(TestClass const& in_copy) noexcept = default;
		TestClass& operator=(TestClass&&      in_move) noexcept = default;

		#pragma endregion
};
```

## 11. Prefer uppercase literals for readability

```cpp
// Do
float  test_float {0.5F};
size_t test_size  {2ULL};

// Don't
float  test_float {0.5f};
size_t test_size  {2ull};
```