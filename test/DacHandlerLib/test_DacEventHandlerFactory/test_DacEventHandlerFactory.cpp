#include <unity.h>
#include "DacEventHandlerFactory.h"

void setUp() {}

void tearDown() {}

void test_DacEventHandlerFactory_createNoteEventHandler() {
    DacPresetConfig dacConfig;
    DacEventHandler *noteEventHandler = DacEventHandlerFactory::createEventHandler(&dacConfig, true);
    TEST_ASSERT_NOT_NULL(noteEventHandler);

    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 1, 1);
    DacValues dacValues;
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
}

void test_DacEventHandlerFactory_createCCEventHandler() {
    DacPresetConfig dacConfig;
    dacConfig.CCNumber1 = 1;
    DacEventHandler *ccEventHandler = DacEventHandlerFactory::createEventHandler(&dacConfig, false);
    TEST_ASSERT_NOT_NULL(ccEventHandler);

    MIDICCEvent ccEvent = MIDICCEvent(0, 1, 1);
    DacValues dacValues;
    TEST_ASSERT_TRUE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacEventHandlerFactory_createNoteEventHandler);
    RUN_TEST(test_DacEventHandlerFactory_createCCEventHandler);
    UNITY_END();
}
