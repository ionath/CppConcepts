#include <iostream>

// Concepts
#ifdef CONCEPTS
template <class T>
concept bool EqualityComparible = requires(T a, T b)
{
	{a == b}->bool;
	{a != b}->bool;
};

template <class T>
concept bool OutputStreamCompatible = requires(std::ostream& strm, T a)
{
	{strm << a}->std::ostream&;
};
#endif

class Test
{
public:
	template <class T>
	void assert_equal(T&& t1, T&& t2) 
#ifdef CONCEPTS
		requires EqualityComparible<T> && OutputStreamCompatible<T>
#endif
	{
		if (t1 == t2)
		{
			successCount++;
		}
		else
		{
			failCount++;
			std::cout << "Test failed! Expected: " << t1 << "   Actual: " << t2 << std::endl;
		}
	}

	void finish()
	{
		int totalTests = successCount + failCount;
		if (successCount == totalTests)
		{
			std::cout << "All tests passed (" << successCount << "/" << totalTests << ")" << std::endl;
		}
		else
		{
			std::cout << "Tests passed: " << successCount << "/" << totalTests << std::endl
				<< "Tests failed: " << failCount << "/" << totalTests << std::endl;
		}
	}

private:
	int successCount = 0;
	int failCount = 0;
};

class Unequatable
{
public:
	Unequatable(int _foo) : foo(_foo) {}
	int foo;
};

class NoStreamOutput
{
public:
	NoStreamOutput(int _foo) : foo(_foo) {}

	bool operator==(const NoStreamOutput& other)
	{
		return foo == other.foo;
	}
	bool operator!=(const NoStreamOutput& other)
	{
		return foo != other.foo;
	}

private:
	int foo;
};

class MyValidType
{
public:
	MyValidType(int _foo) : foo(_foo) {}
	bool operator==(const MyValidType& other)
	{
		return foo == other.foo;
	}
	bool operator!=(const MyValidType& other)
	{
		return foo != other.foo;
	}
	friend std::ostream& operator<<(std::ostream& strm, const MyValidType& myValidType);

private:
	int foo;
};

std::ostream& operator<<(std::ostream& strm, const MyValidType& myValidType)
{
	strm << "MyValidType(" << myValidType.foo << ")";
	return strm;
}

int main()
{
	Test test;
	test.assert_equal(1, 1);
	test.assert_equal(1, 2);
	test.assert_equal("bar", "foo");
	// if CONCEPTS is defined and supported the next 2 tests should give more meaningfull errors and easier to understand.
	// With concepts this should failed because EqualityComparible concept and OutputStreamCompatible concept are not sastified
	test.assert_equal(Unequatable(1), Unequatable(2));
	// With concepts this should failed because OutputStreamCompatible concept is not sastified
	test.assert_equal(NoStreamOutput(1), NoStreamOutput(2));
	// With concepts this type satisfies both concepts and will sucessfully compile
	test.assert_equal(MyValidType(1), MyValidType(2));
	test.finish();

	return 0;
}