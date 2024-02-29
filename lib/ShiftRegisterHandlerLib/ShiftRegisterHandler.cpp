#include "ShiftRegisterHandler.h"

void ShiftRegisterHandler::updateHandlersFromFirstDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setFirstHandlersPairFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::updateHandlersFromSecondDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setSecondHandlersPairFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::processEvent(const MidiEventLib::Event *event)
{
    _state = _handlerContainer.processEvent(event, _state);
    _writeValuesToShiftRegister(_state);
}
