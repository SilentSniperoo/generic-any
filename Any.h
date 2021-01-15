#pragma once

#include "Property.h" // Member variable get/set methods

// Feel free to replace types with custom ones
#include <ostream> // Output
#include <string> // TEXT_STRING_TYPE
#include <vector> // Any::Array
typedef long long int WHOLE_NUMBER_TYPE;
typedef long double DECIMAL_NUMBER_TYPE;
typedef std::string TEXT_STRING_TYPE;
typedef void* INVALID_UNSET_TYPE;

class Any
{
public:
	// The type of value(s) stored in the object
	enum class Type
	{
		WHOLE_NUMBER,
		DECIMAL_NUMBER,
		TEXT_STRING,
		ARRAY_GROUP,
		INVALID_UNSET,
		COUNT
	};
	// The name of the type of value(s) stored in the object
	static const char* const TypeNames[(unsigned)Type::COUNT];

	// The internal class for nesting Any objects inside each other
	class Array
	{
	public:
		// The managed pointer to the contents of the container
		class Iterator
		{
		public:
			// Constructors/Destructor
			Iterator();
			Iterator(const Iterator& other);
			Iterator& operator=(const Iterator& other);
			Iterator(Iterator&& other);
			Iterator& operator=(Iterator&& other);
			Iterator(Any* any);
			~Iterator() {}

			// Equivalency operators for iteration
			bool operator==(const Iterator& other);
			bool operator!=(const Iterator& other);

			// Preincrement operator for iteration
			Iterator& operator++();

			// Dereference and member access operators for iteration
			Any& operator*();
			Any* operator->();

		private:
			// The actual element we are pointing to in the container
			Any* mAny;
		};

		// Constructor/Destructor
		Array();
		~Array();

		// Add an element to the end of the array
		Iterator emplace_back(const Any& any);
		Iterator emplace_back(Any&& any);

		// Remove the last element in the array and return a copy
		Any pop_back();

		// Managed pointers to the first and one past the last elements in the array
		Iterator begin();
		Iterator end();

	private:
		// The actual container data
		std::vector<Any> mGroup;
	};

	// Default construction (results in INVALID_UNSET)
	Any();
	// Copy construction (results in type of other)
	Any(const Any& other);
	// Copy assignment (results in type of other)
	Any& operator=(const Any& other);
	// Move construction (takes ownership of other)
	Any(Any&& other);
	// Move assignment (takes ownership of other and gives ownership of self)
	Any& operator=(Any&& other);

	// Conversion construction from data types (results in type given)
	Any(Type type);
	Any(WHOLE_NUMBER_TYPE value);
	Any(DECIMAL_NUMBER_TYPE value);
	Any(TEXT_STRING_TYPE value);
	Any(const char* const value);
	Any(const char* const value, unsigned length);
	Any(Array value);

	// Destruction (releases resources)
	~Any();

	// Output friend functions
	friend std::ostream& operator<<(std::ostream& stream, Type type);
	friend std::ostream& operator<<(std::ostream& stream, const Any& any);
	friend std::ostream& operator<<(std::ostream& stream, const Property<Any, WHOLE_NUMBER_TYPE>& property);
	friend std::ostream& operator<<(std::ostream& stream, const Property<Any, DECIMAL_NUMBER_TYPE>& property);
	friend std::ostream& operator<<(std::ostream& stream, const Property<Any, TEXT_STRING_TYPE>& property);
	friend std::ostream& operator<<(std::ostream& stream, const Property<Any, Any::Array>& property);

	// The public readonly type
	const Type& mType;
	// The public readonly type name
	const char* const& mTypeName;

	// Public accessors to the values (will automatically convert on use)
	Property<Any, WHOLE_NUMBER_TYPE> mWholeNumber;
	Property<Any, DECIMAL_NUMBER_TYPE> mDecimalNumber;
	Property<Any, TEXT_STRING_TYPE> mTextString;
	Property<Any, Any::Array> mArrayGroup;

	Array::Iterator emplace_back(const Any& any);
	Array::Iterator emplace_back(Any&& any);

	Any pop_back();

	Array::Iterator begin();
	Array::Iterator end();

private:
	// The private read/write type
	Type mInternalType;
	// The private read/write type name
	const char* mInternalTypeName;

	// The private accessors to the values (will not automatically convert on use)
	WHOLE_NUMBER_TYPE& mInternalWholeNumber;
	DECIMAL_NUMBER_TYPE& mInternalDecimalNumber;
	TEXT_STRING_TYPE& mInternalTextString;
	Any::Array& mInternalArrayGroup;

	// The actual value
	union Data
	{
		// Default construction
		Data() {}
		// Plain memory copy
		// Never use a plain memory copy unless private and very careful
		// You need to GUARANTEE that you will never mishandle deep copies
		// That means no accidentally shared or lost pointers to dynamic memory
		// In this case, the ONLY time we are ever copying is safe
		// It is when swapping in Any::_swapContents
		// For simple swapping, doing a plain memory copy is sufficient
		// Both objects should be in a valid state before the swap
		// Therefore both should be in a valid state after the swap
		Data(const Data& other);
		Data& operator=(const Data& other);
		// Destruction
		~Data() {}

		// All these members of the union share the same memory
		// This makes the object take less memory and be more flexible
		WHOLE_NUMBER_TYPE mWholeNumber;
		DECIMAL_NUMBER_TYPE mDecimalNumber;
		TEXT_STRING_TYPE mTextString;
		Any::Array mArrayGroup;
		INVALID_UNSET_TYPE mInvalidUnset;
	} mData;

	// Make sure we are the correct type, if not, change our type
	void _setType(Type type);
	// Destroy the value based on its type
	void _deinit();
	// Construct the value based on its type
	void _init();
	// Only copy the value based on its type (do not copy anything else)
	void _copyValue(const Any& other);
	// Swap all contents, including value, type, and typename
	void _swapContents(Any&& other);

	// Property get/set methods for automatic type conversions
	const WHOLE_NUMBER_TYPE& _getWholeNumber()
	{
		_setType(Type::WHOLE_NUMBER);
		return mInternalWholeNumber;
	}
	void _setWholeNumber(const WHOLE_NUMBER_TYPE& other)
	{
		_setType(Type::WHOLE_NUMBER);
		mInternalWholeNumber = other;
	}
	const DECIMAL_NUMBER_TYPE& _getDecimalNumber()
	{
		_setType(Type::DECIMAL_NUMBER);
		return mInternalDecimalNumber;
	}
	void _setDecimalNumber(const DECIMAL_NUMBER_TYPE& other)
	{
		_setType(Type::DECIMAL_NUMBER);
		mInternalDecimalNumber = other;
	}
	const TEXT_STRING_TYPE& _getTextString()
	{
		_setType(Type::TEXT_STRING);
		return mInternalTextString;
	}
	void _setTextString(const TEXT_STRING_TYPE& other)
	{
		_setType(Type::TEXT_STRING);
		mInternalTextString = other;
	}
	const Any::Array& _getObjectGroup()
	{
		_setType(Type::ARRAY_GROUP);
		return mInternalArrayGroup;
	}
	void _setObjectGroup(const Any::Array& other)
	{
		_setType(Type::ARRAY_GROUP);
		mInternalArrayGroup = other;
	}
};

inline std::ostream& operator<<(std::ostream& stream, Any::Type type)
{
	bool valid = (unsigned)type < (unsigned)Any::Type::COUNT;
	std::string s = Any::TypeNames[(unsigned)(valid ? type : Any::Type::INVALID_UNSET)];
	return stream << s;
}
inline std::ostream& operator<<(std::ostream& stream, const Any& any)
{
	stream << any.mType << std::string("=");
	switch (any.mType)
	{
	case Any::Type::WHOLE_NUMBER:
		stream << any.mWholeNumber;
		break;
	case Any::Type::DECIMAL_NUMBER:
		stream << any.mDecimalNumber;
		break;
	case Any::Type::TEXT_STRING:
		stream << any.mTextString;
		break;
	case Any::Type::ARRAY_GROUP:
		stream << any.mArrayGroup;
		break;
	default:
		stream << std::string("N/A");
		break;
	}
	return stream;
}
inline std::ostream& operator<<(std::ostream& stream, const Property<Any, WHOLE_NUMBER_TYPE>& property)
{
	return stream << static_cast<WHOLE_NUMBER_TYPE>(property);
}
inline std::ostream& operator<<(std::ostream& stream, const Property<Any, DECIMAL_NUMBER_TYPE>& property)
{
	return stream << static_cast<DECIMAL_NUMBER_TYPE>(property);
}
inline std::ostream& operator<<(std::ostream& stream, const Property<Any, TEXT_STRING_TYPE>& property)
{
	return stream << static_cast<TEXT_STRING_TYPE>(property);
}
inline std::ostream& operator<<(std::ostream& stream, const Property<Any, Any::Array>& property)
{
	bool first = true;
	for (auto&& any : static_cast<Any::Array>(property))
	{
		if (first)
		{
			stream << std::string("[ ");
		}
		else
		{
			stream << std::string(", ");
		}
		first = false;
		stream << any;
	}
	if (first == false)
	{
		stream << std::string(" ]");
	}
	return stream;
}