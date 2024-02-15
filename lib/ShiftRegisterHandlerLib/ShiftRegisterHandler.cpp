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

void ShiftRegisterHandler::processEvent(const MidiEvent *event) const
{
    uint8_t result = _handlerContainer.processEvent(event);
    writeValuesToShiftRegister(result);
}
