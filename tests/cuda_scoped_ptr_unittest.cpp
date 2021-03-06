#include "../cuda_scoped_ptr.hpp"
#include "test_object.h"
#include <gtest/gtest.h>

class CudaScopedPtrTest : public testing::Test
{
protected:
    CudaScopedPtrTest()
    {
        nullTestObject = nullptr;
    }

    TestObject* createSampleTestObject()
    {
        TestObject* d_sampleTestObject;
        TestObject* h_sampleTestObject = new TestObject(69);
        cudaMalloc((void**)&d_sampleTestObject, sizeof(TestObject));
        cudaMemcpy(
            d_sampleTestObject,
            h_sampleTestObject,
            sizeof(TestObject),
            cudaMemcpyHostToDevice);
        delete h_sampleTestObject;
        return d_sampleTestObject;
    }

    TestObject* nullTestObject;
};

TEST_F(CudaScopedPtrTest, DefaultConstructorNullPointer)
{
  cuda_scoped_ptr<TestObject> ptr(nullptr);
  EXPECT_EQ(nullTestObject, ptr.get());
}

TEST_F(CudaScopedPtrTest, DefaultConstructorZero)
{
    size_t n = 0;
    cuda_scoped_ptr<TestObject> ptr(n);
    EXPECT_EQ(nullTestObject, ptr.get());
}

TEST_F(CudaScopedPtrTest, PointerConstructorEqualsPointer)
{
  TestObject* to = createSampleTestObject();
  cuda_scoped_ptr<TestObject> ptr(to);
  EXPECT_EQ(to, ptr.get());
}

TEST_F(CudaScopedPtrTest, ResetClearsPointer)
{
  cuda_scoped_ptr<TestObject> ptr(createSampleTestObject());
  EXPECT_NE(nullTestObject, ptr.get());
  ptr.reset(nullptr);
  EXPECT_EQ(nullTestObject, ptr.get());
}
