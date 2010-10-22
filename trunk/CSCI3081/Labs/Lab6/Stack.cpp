// stack.cpp: stack functions
// A simple stack to be used for filo data structure
#include "Stack.h"

// Construct a simple stack with the given size.  Allocate an internal array
// to hold the stack entries.
SimpleStack::SimpleStack(int size) {
	size_ = size;
	top_ = 0;
	items_ = new char[size_];
}

SimpleStack::~SimpleStack() {
	delete[] items_;
}

// Push a char to the stack.  If stack is full, don't do anything
void SimpleStack::push(char c) {
	if (top_ >= size_) {
		return;
	}
	items_[top_] = c;
	top_++;
}

// Pop a char from the stack.  If stack is empty, return -1
char SimpleStack::pop() {
	if (isEmpty() || getSize() <= getTop() - 1 || getTop() <= 0) {
		return -1;
	}
	top_--;
	char c = items_[top_];
	return c;
}

int SimpleStack::getSize() {
	return size_;
}

bool SimpleStack::isEmpty() {
	return top_ == 0;
}

int SimpleStack::getTop() {
	return top_;
}
