#include <Arduino.h>
#include <Button.h>
#include <ModuleOperatorInterface.h>
  
ModuleOperatorInterface::ModuleOperatorInterface(ModuleOperatorInterfaceClient **modeHandlers, unsigned int revertInterval) {
  this->modeHandlers = modeHandlers;
  this->revertInterval = (unsigned long) (revertInterval * 1000);
  this->currentMode = 0;
  this->currentAddress = -1;
  this->buttonPressedAt = 0UL;
}

int ModuleOperatorInterface::getCurrentMode() {
  return(this->currentMode);
}

unsigned long ModuleOperatorInterface::getButtonPressedAt() {
  return(this->buttonPressedAt);
}

void ModuleOperatorInterface::revertModeMaybe() {
  if (this->revertInterval != 0UL) {
    if (millis() > (this->buttonPressedAt + this->revertInterval)) {
      this->currentMode = 0;
      this->currentAddress = -1;
    }
  }
}
    
ModuleOperatorInterface::EventOutcome ModuleOperatorInterface::handleButtonEvent(bool buttonState, unsigned char value) {
  EventOutcome retval = MODE_CHANGE;
  unsigned long now = millis();

  if (buttonState == Button::PRESSED) {
    this->buttonPressedAt = now;
  } else {
    if ((this->buttonPressedAt) && (now < (this->buttonPressedAt + 1000))) {
      if (this->currentAddress != -1) {
        retval = (this->modeHandlers[this->currentMode]->processValue((unsigned int) this->currentAddress, value))?VALUE_ACCEPTED:VALUE_REJECTED;
        this->currentAddress = -1;
      } else {
        this->currentMode++;
        if (this->modeHandlers[this->currentMode] == 0) this->currentMode = 0;
        retval = MODE_CHANGE;
      }
    } else {
      if (this->modeHandlers[this->currentMode]->validateAddress((unsigned int) value)) {
        this->currentAddress = (int) value;
        retval = ADDRESS_ACCEPTED;
      } else {
        this->currentAddress = -1;
        retval = ADDRESS_REJECTED;
      }
    }
  }
  return(retval);
}
