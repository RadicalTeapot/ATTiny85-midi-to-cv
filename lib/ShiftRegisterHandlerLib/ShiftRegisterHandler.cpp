#include "ShiftRegisterHandler.h"

ShiftRegisterHandler::WriteValuesToShiftRegister ShiftRegisterHandler::writeValuesToShiftRegister = ShiftRegisterHandler::defaultWriteValuesToShiftRegister;

void ShiftRegisterHandler::updateHandlersFromFirstDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setFirstHandlersPairFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::updateHandlersFromSecondDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setSecondHandlersPairFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::processEvent(const MidiEvent *event)
{
    _handlerContainer.processEvent(event, &_state);
    writeValuesToShiftRegister(_state);
}
