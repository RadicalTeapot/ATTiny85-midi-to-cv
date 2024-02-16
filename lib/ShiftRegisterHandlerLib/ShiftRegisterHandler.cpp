#include "ShiftRegisterHandler.h"

ShiftRegisterHandler::WriteValuesToShiftRegister ShiftRegisterHandler::writeValuesToShiftRegister = ShiftRegisterHandler::defaultWriteValuesToShiftRegister;

void ShiftRegisterHandler::updateHandlersFromFirstDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setFirstTwoHandlersFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::updateHandlersFromSecondDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _handlerContainer.setSecondTwoHandlersFromDacConfig(dacConfig, isNoteHandler);
}

void ShiftRegisterHandler::processEvent(const MidiEvent *event)
{
    _handlerContainer.processEvent(event, &_state);
    writeValuesToShiftRegister(_state);
}
