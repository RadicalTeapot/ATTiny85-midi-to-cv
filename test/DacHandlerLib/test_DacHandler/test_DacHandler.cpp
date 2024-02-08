#include <unity.h>
#include "DacHandler.h"
#include "DacEventHandlerFactory.h"

void setUp() {}

void tearDown() {}

void test_DacHandler_handleEvent_nullHandlerPointer_DontHandle() {
    DacHandler dacHandler;
    MIDINoteOnEvent event = MIDINoteOnEvent(0, 1, 1);
    TEST_ASSERT_FALSE(dacHandler.handleEvent(&event));
}

void test_DacHandler_handleEvent_nullMidiEventPointer_DontHandle() {
    DacHandler dacHandler;
    DacPresetConfig dacConfig;
    DacEventHandler *eventHandler = DacEventHandlerFactory::createEventHandler(&dacConfig, true);
    dacHandler.setHandler(eventHandler);
    TEST_ASSERT_FALSE(dacHandler.handleEvent(nullptr));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacHandler_handleEvent_nullHandlerPointer_DontHandle);
    RUN_TEST(test_DacHandler_handleEvent_nullMidiEventPointer_DontHandle);
    UNITY_END();
}
