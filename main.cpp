#include "Any.h"

#include <iostream>

int main(void)
{
	// Test property getters
	{
		Any any;
		std::cout << "any.mType[" << any.mType << "]" << std::endl;
		std::cout << "any.mTypeName[" << any.mTypeName << "]" << std::endl;
		std::cout << "any.mWholeNumber[" << any.mWholeNumber << "]" << std::endl;
		std::cout << "any.mDecimalNumber[" << any.mDecimalNumber << "]" << std::endl;
		std::cout << "any.mTextString[" << any.mTextString << "]" << std::endl;
		std::cout << "any.mArrayGroup[" << any.mArrayGroup << "]" << std::endl;
		std::cout << std::endl;
	}

	// Test property setters
	{
		Any any;
		std::cout << "any.mType[" << any.mType << "]" << std::endl;
		std::cout << "any.mTypeName[" << any.mTypeName << "]" << std::endl;
		any.mWholeNumber = 8;
		std::cout << "any.mWholeNumber[" << any.mWholeNumber << "]" << std::endl;
		any.mWholeNumber = -1073;
		std::cout << "any.mWholeNumber[" << any.mWholeNumber << "]" << std::endl;
		any.mDecimalNumber = 8;
		std::cout << "any.mDecimalNumber[" << any.mDecimalNumber << "]" << std::endl;
		any.mDecimalNumber = -1073;
		std::cout << "any.mDecimalNumber[" << any.mDecimalNumber << "]" << std::endl;
		any.mDecimalNumber = -0.3;
		std::cout << "any.mDecimalNumber[" << any.mDecimalNumber << "]" << std::endl;
		std::cout << std::endl;
	}

	// Test array usage
	{
		Any parent;
		Any child1;
		Any child2;
		Any child3;
		std::cout << "parent.mType[" << parent.mType << "]" << std::endl;
		std::cout << "parent.mTypeName[" << parent.mTypeName << "]" << std::endl;
		std::cout << "child1.mType[" << child1.mType << "]" << std::endl;
		std::cout << "child1.mTypeName[" << child1.mTypeName << "]" << std::endl;
		std::cout << "child2.mType[" << child2.mType << "]" << std::endl;
		std::cout << "child2.mTypeName[" << child2.mTypeName << "]" << std::endl;
		std::cout << "child3.mType[" << child3.mType << "]" << std::endl;
		std::cout << "child3.mTypeName[" << child3.mTypeName << "]" << std::endl;
		parent.emplace_back(child1);
		child2.mWholeNumber = 2;
		parent.emplace_back(child2);
		child3.mTextString = "Child 3 is a string";
		parent.emplace_back(std::move(child3));
		parent.emplace_back(std::move(Any("Child 4")));
		parent.emplace_back(std::move("Child 5"));
		parent.emplace_back(std::move((DECIMAL_NUMBER_TYPE)6.66));
		std::cout << "parent.mType[" << parent.mType << "]" << std::endl;
		std::cout << "parent.mTypeName[" << parent.mTypeName << "]" << std::endl;
		std::cout << "child1.mType[" << child1.mType << "]" << std::endl;
		std::cout << "child1.mTypeName[" << child1.mTypeName << "]" << std::endl;
		std::cout << "child2.mType[" << child2.mType << "]" << std::endl;
		std::cout << "child2.mTypeName[" << child2.mTypeName << "]" << std::endl;
		std::cout << "child3.mType[" << child3.mType << "]" << std::endl;
		std::cout << "child3.mTypeName[" << child3.mTypeName << "]" << std::endl;
		std::cout << "parent[" << parent << "]" << std::endl;
		std::cout << "child1[" << child1 << "]" << std::endl;
		std::cout << "child2[" << child2 << "]" << std::endl;
		std::cout << "child3[" << child3 << "]" << std::endl;
		std::cout << std::endl;
	}

	char waitForChar;
	std::cin >> waitForChar;
	return 0;
}