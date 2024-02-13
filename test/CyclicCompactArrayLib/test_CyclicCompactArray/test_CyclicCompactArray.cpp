#include <unity.h>
#include <CyclicCompactArray.h>

void setUp() {}

void tearDown() {}

void test_CyclicCompactArray_empty() {
    CyclicCompactArray<1> cyclicCompactArray;
    TEST_ASSERT_EQUAL(0, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_append_full() {
    CyclicCompactArray<1> cyclicCompactArray;
    cyclicCompactArray.append(1);
    TEST_ASSERT_EQUAL(1, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_append_notFull() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    TEST_ASSERT_EQUAL(1, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_append_wrapping() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.append(2);
    cyclicCompactArray.append(3);
    TEST_ASSERT_EQUAL(3, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_remove_full() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.append(2);
    TEST_ASSERT_FALSE(cyclicCompactArray.remove(0));
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(1));
    TEST_ASSERT_EQUAL(2, cyclicCompactArray.getTail());

    cyclicCompactArray.append(3);
    TEST_ASSERT_EQUAL(3, cyclicCompactArray.getTail());
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(3));
    TEST_ASSERT_EQUAL(2, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_remove_notFull() {
    CyclicCompactArray<3> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.append(2);
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(0));
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(1));
    TEST_ASSERT_EQUAL(2, cyclicCompactArray.getTail());

    cyclicCompactArray.append(3);
    TEST_ASSERT_EQUAL(3, cyclicCompactArray.getTail());
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(3));
    TEST_ASSERT_EQUAL(2, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_remove_wrapping() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.append(2);
    cyclicCompactArray.append(3);
    TEST_ASSERT_FALSE(cyclicCompactArray.remove(1));
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(2));
    TEST_ASSERT_EQUAL(3, cyclicCompactArray.getTail());

    cyclicCompactArray.append(4);
    TEST_ASSERT_EQUAL(4, cyclicCompactArray.getTail());
    TEST_ASSERT_TRUE(cyclicCompactArray.remove(4));
    TEST_ASSERT_EQUAL(3, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_resetToZero_single() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.remove(1);
    TEST_ASSERT_EQUAL(0, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_resetToZero_legato() {
    CyclicCompactArray<2> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.append(2);
    cyclicCompactArray.remove(2);
    cyclicCompactArray.remove(1);
    TEST_ASSERT_EQUAL(0, cyclicCompactArray.getTail());
}

void test_CyclicCompactArray_resetToZero_successive()
{
    CyclicCompactArray<4> cyclicCompactArray;
    cyclicCompactArray.append(1);
    cyclicCompactArray.remove(1);
    cyclicCompactArray.append(2);
    cyclicCompactArray.remove(2);
    TEST_ASSERT_EQUAL(0, cyclicCompactArray.getTail());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_CyclicCompactArray_empty);
    RUN_TEST(test_CyclicCompactArray_append_full);
    RUN_TEST(test_CyclicCompactArray_append_notFull);
    RUN_TEST(test_CyclicCompactArray_append_wrapping);
    RUN_TEST(test_CyclicCompactArray_remove_full);
    RUN_TEST(test_CyclicCompactArray_remove_notFull);
    RUN_TEST(test_CyclicCompactArray_remove_wrapping);
    RUN_TEST(test_CyclicCompactArray_resetToZero_single);
    RUN_TEST(test_CyclicCompactArray_resetToZero_legato);
    RUN_TEST(test_CyclicCompactArray_resetToZero_successive);
    UNITY_END();
}
