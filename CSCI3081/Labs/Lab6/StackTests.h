#ifndef STACKTEST_H_
#define STACKTEST_H_

#include <cxxtest/TestSuite.h>
#include "Stack.h"

class StackTests: public CxxTest::TestSuite {
public:
	// Test for default stack constructor, create a stack and assign the size
	// (say 2), then compare the getSize() with the assigned size (say 2).
	void test_Stack_Size() {
		SimpleStack stack(2);
		TS_ASSERT_EQUALS(stack.getSize(), 2);

	}
	// Test for the isEmpty function, create a stack and assign the size (say 2),
	// then compare the "isEmpty()" with "true".
	void test_Stack_Empty() {
		SimpleStack stack(2);
		TS_ASSERT_EQUALS(stack.isEmpty(), 1);
	}
	// Test for Push and Pop operations, create a stack and assign the size (say 2),
	// then push two items into the stack, then pop out the two items, compare the popped two items with the original pushed two items, test whether they are the same.
	void test_Stack_Push_Pop() {
		SimpleStack stack(2);
		stack.push('a');
		stack.push('b');
		TS_ASSERT_EQUALS(stack.pop(), 'b');
		TS_ASSERT_EQUALS(stack.pop(), 'a');
	}
	// Test for Push more items than the stack size, create a stack and assign
	// the size (say 2), then push three items into the stack, then check the buffer
	// overflow, then pop out three items, compare the first popped out two items
	// with the first pushed two items, compare the third popped out items with -1,
	// test whether they are the same.
	void test_Stack_Push_More() {
		SimpleStack stack(2);
		stack.push('a');
		stack.push('b');
		stack.push('c');
		TS_ASSERT_EQUALS(stack.pop(), 'b');
		TS_ASSERT_EQUALS(stack.pop(), 'a');
		TS_ASSERT_EQUALS(stack.pop(),-1);
	}

	// Test for Pop out more items than the pushed items, create a stack and assign
	// the size (say 2), then push one items into the stack, then pop out the two
	// items, compare the first popped item with the first pushed item, compare the
	// second popped item with -1, test whether they are the same.
	void test_Stack_Pop_More() {
		SimpleStack stack(2);
		stack.push('a');
		TS_ASSERT_EQUALS(stack.pop(), 'a');
		TS_ASSERT_EQUALS(stack.pop(), -1);
	}
};

#endif /*STACKTEST_H_*/
