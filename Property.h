#pragma once

// Encapsulates a member variable to allow custom get and set methods
template<typename OwnerType, typename MemberType>
class Property
{
public:
	// We need to know everything about the member variable
	Property(
		// The object of which it is a member
		OwnerType& owner,
		// The member variable itself
		MemberType& value,
		// A member pointer to the get method for reading attempts
		const MemberType& (OwnerType::* get)(),
		// A member pointer to the set method for writing attempts
		void (OwnerType::* set)(const MemberType&));

	// An explanation of the `.*` here:
	// What we are trying to do is call a method on the owner object by pointer
	// In order to do so, we would have to dereference the pointer to the member
	// Thus the `*` preceding the method pointer
	// The result of dereferencing a pointer is a reference
	// A reference to a member must be used in the context of the owning object
	// Thus we connect the context to the reference with the `.`
	// Thus, accessing a member (method or variable) looks like: `object.*member`
	// Finally, since we are accessing a method, which we want to call, use `()`
	// The first set of parenthesis merely groups the result of the previous steps
	// The second set of parenthesis is the actual call to the function
	// Thus the very last step is to put in the parameters you want to pass
	// No parameters for get, but for set, the value being assigned
	operator const MemberType& () const
	{
		return (mOwner.*mGet)();
	}
	void operator=(const MemberType& other)
	{
		(mOwner.*mSet)(other);
	}

private:
	OwnerType& mOwner;
	MemberType& mValue;
	const MemberType& (OwnerType::* mGet)();
	void (OwnerType::* mSet)(const MemberType&);
};

template<typename OwnerType, typename MemberType>
inline Property<OwnerType, MemberType>::Property(
	OwnerType& owner,
	MemberType& value,
	const MemberType& (OwnerType::* get)(),
	void (OwnerType::* set)(const MemberType&))
	: mOwner(owner)
	, mValue(value)
	, mGet(get)
	, mSet(set)
{
}
