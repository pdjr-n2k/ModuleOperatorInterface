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
 * based upon operation of an eight position DIL switch and momentary
 * push-buton.
 * 
 * The interface supports an arbitrary number of operating modes each
 * delivered by a class that implements ModuleOperatorClientInterface.
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
 * 
 * Button short press - change mode
 * Button long press - start transaction and set address parameter to the value of the DIL switch
 * Button short press (after long press) - end transaction, set value parameter from DIL switch and call performAction().
 * 
 * 
 * 
 * At any one time the interaction is in a particular operating mode with
 *  
 * Substantive actions resulting from user interaction are processed by
 * classes implementing the .
 * One or more such classes must be passed to ModuleOperatorInterface
 * when it is instantiated, each establishing an operating modes as an and each such clien
 * The user interface of a NOP100-based device consists of a DIL switch
 * and momentary button.
 * The momentary button can be operated with either a short-press or a
 * long-press:
 * 
 * LONG-PRESS: causes the value set on the DIL switch to be treated as
 * a potential address
 * 
 * 
 * This ADT implements mode-vased interaction: at any point in time the
 * interface is in one of an arbitrary number of modes each of which
 * has a dedicated handler class that implements the
 * ModuleOperatorInterfaceClient interface.
 * 
 * model that is mode basedwith a basic two
 * component data entry model in which the user can enter an address
 * followed by a value.
 * t is enter a value on the DIL switch and
 * press the button for confirmation.
 * This is extended to a two-stage protocol which involves entry of an
 * address followed by entry of a value.
 *  around the notion of setting a value 
 */
class ModuleOperatorInterface {

  public:
    /**
     * @brief Result codes for the handleButtonEvent() method.
     */
    enum EventOutcome { MODE_CHANGE, ADDRESS_ACCEPTED, ADDRESS_REJECTED, VALUE_ACCEPTED, VALUE_REJECTED };

    /**
     * @brief Construct a new ModuleInterface object.
     * 
     * @param modeHandlers   - Array of pointers to objects which
     *                         implement the ModuleOperatorInterfaceClient
     *                         interface in order of the mode which
     *                         they support. The last element in the
     *                         array must be the value 0. 
     * @param revertInterval - Number of milliseconds that must elapse
     *                         without a call to handleButtonEvent()
     *                         before the current operating mode
     *                         reverts to mode zero.
     */
    ModuleOperatorInterface(ModuleOperatorInterfaceClient **modeHandlers, unsigned long revertInterval = 30000);

    /**
     * @brief Get the current operating mode.
     * 
     * @return int    - the current operating mode.
     */
    int getCurrentMode();

    /**
     * @brief Get the time in milliseconds of the last invocation of handleButtonEvent().
     * 
     * @return unsigned long - timetsamp in milliseconds.
     */
    unsigned long getButtonPressedAt();

    /**
     * @brief Calls the an appropriate handler method for the current
     *        operating mode.
     * 
     * This method should be called with buttonState set to either
     * Button::PRESSED or Button::RELEASED. On a release, a long press
     * will result in a call to the current mode handler's setAddress()
     * method, otherwise to the current mode handler's processValue()
     * method, but only if an address has previously been set.
     * 
     * @param buttonState 
     * @param value 
     * @return int 
     */
    EventOutcome handleButtonEvent(bool buttonState, unsigned char value = 0);

    /**
     * @brief
     * Revert operating mode to 0 if the revert iterval has been
     * exceeded since the last button press.
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
