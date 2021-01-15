#include "Any.h"

////////////////////////////////////////////////////////////////////////////////
// Public implementation

// Lookup table for type names
// Index into with the type enumeration
const char* const Any::TypeNames[(unsigned)Any::Type::COUNT] = {
	"Integer",
	"Double",
	"String",
	"Group",
	"Invalid"
};

// Initialize to an invalid type so that very little work needs to be done
// All properties and references must be initialized in every constructor
// The other constructors call this one to set properties and references
Any::Any()
	: mType(mInternalType)
	, mTypeName(mInternalTypeName)
	, mWholeNumber(*this, mData.mWholeNumber, &Any::_getWholeNumber, &Any::_setWholeNumber)
	, mDecimalNumber(*this, mData.mDecimalNumber, &Any::_getDecimalNumber, &Any::_setDecimalNumber)
	, mTextString(*this, mData.mTextString, &Any::_getTextString, &Any::_setTextString)
	, mArrayGroup(*this, mData.mArrayGroup, &Any::_getObjectGroup, &Any::_setObjectGroup)
	, mInternalType(Type::INVALID_UNSET)
	, mInternalTypeName(TypeNames[(unsigned)Type::INVALID_UNSET])
	, mInternalWholeNumber(mData.mWholeNumber)
	, mInternalDecimalNumber(mData.mDecimalNumber)
	, mInternalTextString(mData.mTextString)
	, mInternalArrayGroup(mData.mArrayGroup)
{
}

// Set the type with a plain memory copy, but deep copy the value
Any::Any(const Any& other)
	: Any(other.mType)
{
	_copyValue(other);
}

// TODO: Investigate performance optimization
Any& Any::operator=(const Any& other)
{
	_setType(other.mType);
	_copyValue(other);
	return *this;
}

// Do minimal work to get a valid Any object, then swap
Any::Any(Any&& other)
	: Any()
{
	_swapContents(std::move(other));
}

// The other object will handle destruction of this one's content, just swap
Any& Any::operator=(Any&& other)
{
	_swapContents(std::move(other));
	return *this;
}

// Do minimal work to get a valid Any object, then initialize with a valid type
// TODO: Investigate performance optimization
Any::Any(Type type)
	: Any()
{
	_setType(type);
}

// Do minimal work to get a valid Any object, then initialize with a valid value
// TODO: Investigate performance optimization
Any::Any(WHOLE_NUMBER_TYPE value)
	: Any(Type::WHOLE_NUMBER)
{
	mInternalWholeNumber = value;
}

// Do minimal work to get a valid Any object, then initialize with a valid value
// TODO: Investigate performance optimization
Any::Any(DECIMAL_NUMBER_TYPE value)
	: Any(Type::DECIMAL_NUMBER)
{
	mInternalDecimalNumber = value;
}

// Do minimal work to get a valid Any object, then initialize with a valid value
// TODO: Investigate performance optimization
Any::Any(TEXT_STRING_TYPE value)
	: Any(Type::TEXT_STRING)
{
	mInternalTextString = value;
}

Any::Any(const char* const value)
	: Any(value, static_cast<unsigned>(std::strlen(value)))
{
}

Any::Any(const char* const value, unsigned length)
	: Any(TEXT_STRING_TYPE(value, length))
{
}

// Do minimal work to get a valid Any object, then initialize with a valid value
// TODO: Investigate performance optimization
Any::Any(Any::Array value)
	: Any(Type::ARRAY_GROUP)
{
	mInternalArrayGroup = value;
}

// Destroy the value and clear the type
// TODO: Investigate performance optimization
Any::~Any()
{
	_deinit();
	mInternalType = Type::INVALID_UNSET;
}

Any::Array::Iterator Any::emplace_back(const Any& any)
{
	_setType(Type::ARRAY_GROUP);
	return mInternalArrayGroup.emplace_back(any);
}

Any::Array::Iterator Any::emplace_back(Any&& any)
{
	_setType(Type::ARRAY_GROUP);
	return mInternalArrayGroup.emplace_back(std::move(any));
}

Any Any::pop_back()
{
	_setType(Type::ARRAY_GROUP);
	return mInternalArrayGroup.pop_back();
}

Any::Array::Iterator Any::begin()
{
	_setType(Type::ARRAY_GROUP);
	return mInternalArrayGroup.begin();
}

Any::Array::Iterator Any::end()
{
	_setType(Type::ARRAY_GROUP);
	return mInternalArrayGroup.end();
}

////////////////////////////////////////////////////////////////////////////////
// Private implementation

// Forward on to the assignment operator
Any::Data::Data(const Data& other)
{
	*this = other;
}

// Normally a bad idea, but only called in swap, so a plain memory copy is safe
Any::Data& Any::Data::operator=(const Data& other)
{
	memcpy(this, &other, sizeof(Data));
	return *this;
}

// If not the desired type, reset contents as the desired type
void Any::_setType(Type type)
{
	// Do not modify if already the right type
	if (mInternalType != type)
	{
		// Destroy the old value
		_deinit();
		// Update the type and type name
		mInternalType = type;
		mInternalTypeName = TypeNames[(unsigned)mInternalType];
		// Construct the new value
		_init();
	}
}

// Clean up the value if a destructor needs to be called
void Any::_deinit()
{
	switch (mInternalType)
	{
	case Type::TEXT_STRING:
		// Release the string of text
		mInternalTextString.~basic_string();
		break;
	case Type::ARRAY_GROUP:
		// Release all contained objects
		mInternalArrayGroup.~Array();
		break;
	default:
		// No cleanup necessary for built in or invalid types
		break;
	}
}

// Set the value to the default value of the appropriate type
void Any::_init()
{
	switch (mInternalType)
	{
	case Any::Type::WHOLE_NUMBER:
		mInternalWholeNumber = 0;
		break;
	case Any::Type::DECIMAL_NUMBER:
		mInternalDecimalNumber = 0;
		break;
	case Type::TEXT_STRING:
		// Obtain the string of text with placement new
		new (&mInternalTextString) std::string();
		break;
	case Type::ARRAY_GROUP:
		// Obtain all contained objects with placement new
		new (&mInternalArrayGroup) Array();
		break;
	default:
		// No setup necessary for invalid types
		break;
	}
}

// Ignore type and just copy the value
// Only use internally when you already know the type matches
void Any::_copyValue(const Any& other)
{
	switch (mInternalType)
	{
	case Any::Type::WHOLE_NUMBER:
		mInternalWholeNumber = other.mInternalWholeNumber;
		break;
	case Any::Type::DECIMAL_NUMBER:
		mInternalDecimalNumber = other.mInternalDecimalNumber;
		break;
	case Any::Type::TEXT_STRING:
		mInternalTextString = other.mInternalTextString;
		break;
	case Any::Type::ARRAY_GROUP:
		mInternalArrayGroup = other.mInternalArrayGroup;
		break;
	default:
		break;
	}
}

// Only use internally when changing the other's content is fine
void Any::_swapContents(Any&& other)
{
	std::swap(mInternalType, other.mInternalType);
	std::swap(mInternalTypeName, other.mInternalTypeName);
	std::swap(mData, other.mData);
}

Any::Array::Iterator::Iterator()
	: mAny(nullptr)
{
}

Any::Array::Iterator::Iterator(const Iterator& other)
	: mAny(other.mAny)
{
}

Any::Array::Iterator& Any::Array::Iterator::operator=(const Iterator& other)
{
	mAny = other.mAny;
	return *this;
}

Any::Array::Iterator::Iterator(Iterator&& other)
	: mAny(other.mAny)
{
}

Any::Array::Iterator& Any::Array::Iterator::operator=(Iterator&& other)
{
	mAny = other.mAny;
	return *this;
}

Any::Array::Iterator::Iterator(Any* any)
	: mAny(any)
{
}

bool Any::Array::Iterator::operator==(const Iterator& other)
{
	return mAny == other.mAny;
}

bool Any::Array::Iterator::operator!=(const Iterator& other)
{
	return !operator==(other);
}

Any::Array::Iterator& Any::Array::Iterator::operator++()
{
	++mAny;
	return *this;
}

Any& Any::Array::Iterator::operator*()
{
	return *mAny;
}

Any* Any::Array::Iterator::operator->()
{
	return mAny;
}

Any::Array::Array()
{
}

Any::Array::~Array()
{
}

Any::Array::Iterator Any::Array::emplace_back(const Any& any)
{
	mGroup.emplace_back(any);
	return Iterator(&mGroup.back());
}

Any::Array::Iterator Any::Array::emplace_back(Any&& any)
{
	mGroup.emplace_back(std::move(any));
	return Iterator(&mGroup.back());
}

Any Any::Array::pop_back()
{
	Any back = mGroup.back();
	mGroup.pop_back();
	return back;
}

Any::Array::Iterator Any::Array::begin()
{
	return mGroup.empty() ? Iterator() : Iterator(&mGroup.front());
}

Any::Array::Iterator Any::Array::end()
{
	return mGroup.empty() ? Iterator() : ++Iterator(&mGroup.back());
}
