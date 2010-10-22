/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "StackTests.h"

static StackTests suite_StackTests;

static CxxTest::List Tests_StackTests = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_StackTests( "StackTests.h", 7, "StackTests", suite_StackTests, Tests_StackTests );

static class TestDescription_StackTests_test_Stack_Size : public CxxTest::RealTestDescription {
public:
 TestDescription_StackTests_test_Stack_Size() : CxxTest::RealTestDescription( Tests_StackTests, suiteDescription_StackTests, 11, "test_Stack_Size" ) {}
 void runTest() { suite_StackTests.test_Stack_Size(); }
} testDescription_StackTests_test_Stack_Size;

static class TestDescription_StackTests_test_Stack_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_StackTests_test_Stack_Empty() : CxxTest::RealTestDescription( Tests_StackTests, suiteDescription_StackTests, 18, "test_Stack_Empty" ) {}
 void runTest() { suite_StackTests.test_Stack_Empty(); }
} testDescription_StackTests_test_Stack_Empty;

static class TestDescription_StackTests_test_Stack_Push_Pop : public CxxTest::RealTestDescription {
public:
 TestDescription_StackTests_test_Stack_Push_Pop() : CxxTest::RealTestDescription( Tests_StackTests, suiteDescription_StackTests, 24, "test_Stack_Push_Pop" ) {}
 void runTest() { suite_StackTests.test_Stack_Push_Pop(); }
} testDescription_StackTests_test_Stack_Push_Pop;

static class TestDescription_StackTests_test_Stack_Push_More : public CxxTest::RealTestDescription {
public:
 TestDescription_StackTests_test_Stack_Push_More() : CxxTest::RealTestDescription( Tests_StackTests, suiteDescription_StackTests, 36, "test_Stack_Push_More" ) {}
 void runTest() { suite_StackTests.test_Stack_Push_More(); }
} testDescription_StackTests_test_Stack_Push_More;

static class TestDescription_StackTests_test_Stack_Pop_More : public CxxTest::RealTestDescription {
public:
 TestDescription_StackTests_test_Stack_Pop_More() : CxxTest::RealTestDescription( Tests_StackTests, suiteDescription_StackTests, 50, "test_Stack_Pop_More" ) {}
 void runTest() { suite_StackTests.test_Stack_Pop_More(); }
} testDescription_StackTests_test_Stack_Pop_More;

#include <cxxtest/Root.cpp>
