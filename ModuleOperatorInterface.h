/**
 * @file ModuleOperatorInterface.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief 
 * @version 0.1
 * @date 2023-05-10
 * @copyright Copyright (c) 2023
 */

#ifndef MODULE_OPERATOR_INTERFACE_H
#define MODULE_OPERATOR_INTERFACE_H

#include <ModuleOperatorInterfaceClient.h>

/**
 * @brief ADT implementing an operator interface for NOP100-based modules.
 *
 * ModuleOperatorInterface implements a multi-modal user interface
 * driven by an eight position DIL switch and a momentary push-buton.
 * User interaction processing is triggered by the handleButtonEvent()
 * method which takes button state and DIL switch values as its
 * arguments.
 * 
 * The interface supports an arbitrary number of operating modes each
 * delivered by a client class that implements the
 * ModuleOperatorInterfaceClient interface.
 * A single short button press allows the user to cycle through the
 * available operating modes and so change the semantics of subsequent
 * user interaction events.
 * 
 * A user interaction event comprises a two-step protocol which when
 * completed successfully returns two data values: *address* and
 * *value*.
 * 
 * This abstract class's performAction() method provides the functional
 * link between a user interaction transaction and its implementation.
 * A user transaction consists of two data values, *address* and
 * *value*.
 * 
 * Button short press - change mode
 * Button long press - start transaction and set address parameter to the value of the DIL switch
 * Button short press (after long press) - end transaction, set value parameter from DIL switch and call performAction().
 */
class ModuleOperatorInterface {

  public:
    /**
     * @brief Result codes for the handleButtonEvent() method.
     */
    enum EventOutcome { MODE_CHANGE, ADDRESS_ACCEPTED, ADDRESS_REJECTED, VALUE_ACCEPTED, VALUE_REJECTED };

    /**
     * @brief Create a new ModuleInterface object.
     * 
     * One or more mode handlers which implement application specific
     * processing of user entered data must be passed to a new
     * ModuleOperatorInstance via the \a modeHandlers array.
     * The first mode handler in the array is set as the default handler
     * and the final handler in the array must be followed by a pointer
     * with the value 0.
     * 
     * In a multi-mode application automatic reversion to the default
     * mode can be configured by specifying a \a revertInterval in
     * seconds.
     * This argument is optional and defaults to 0 seconds which
     * disables automatic reversion.
     * 
     * @param modeHandlers - array of pointers to mode handlers which implement the ModuleOperatorInterfaceClient interface.
     * @param revertInterval - number of seconds that must elapse without user activity before the default mode is restored.
     */
    ModuleOperatorInterface(ModuleOperatorInterfaceClient **modeHandlers, unsigned int revertInterval = 0);

    /**
     * @brief Get the index in the modeHandlers array of the current operating mode.
     * 
     * @return int - index of the current operating mode.
     */
    int getCurrentMode();

    /**
     * @brief Get the time in milliseconds of the last invocation of the handleButtonEvent().
     * 
     * @return unsigned long - timetsamp in milliseconds.
     */
    unsigned long getButtonPressedAt();

    /**
     * @brief Handle a user interaction event (i.e. the press or release of the interface button).
     * 
     * This method should be called with \a buttonState set to either
     * Button::PRESSED or Button::RELEASED and, optionally, with 
     * \a value set to the current value of the DIL switch.
     * 
     * On a button press event a timestamp is taken which allows
     * subsequent release event to be associated with either a short
     * or long button press.
     * 
     * On a button release event there are several processing options:
     * 
     * A **long button press** will result in \a value being
     * passed to the current mode handler's validateAddress() method
     * and if characterised valid being saved as an *address* for
     * subsequent processing.
     * The method will return one of ADDRESS_ACCEPTED or
     * ADDRESS_REJECTED.
     *
     * A **short button press** consequent on a previously accepted
     * *address* will result in a call to the current mode handler's
     * processValue() method and, dependent upon the success or
     * failure or this call, the return of one of VALUE_ACCEPTED or
     * VALUE_REJECTED.
     * 
     * A **short button press** with no previously accepted *address*
     * will result in cycling of the currently active mode handler and
     * the return of MODE_CHANGE. 
     * 
     * @param buttonState - one of Button::PRESSED or Button::RELEASED.
     * @param value  - either an address or a value.
     * @return int - a value from the EventOutcome enum.
     */
    EventOutcome handleButtonEvent(bool buttonState, unsigned char value = 0);

    /**
     * @brief Revert to default operating mode if user interaction has dried up.
     * 
     * This method should be called from loop() to enable automatic
     * mode reversion after the revert interval set at instantiation.
     */
    void revertModeMaybe();
    
  private:
    ModuleOperatorInterfaceClient **modeHandlers;
    unsigned int currentMode;
    int currentAddress;
    unsigned long buttonPressedAt;
    unsigned long revertInterval;

};

#endif
