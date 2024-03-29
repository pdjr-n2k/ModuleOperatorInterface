# class `ModuleOperatorInterface` 

ADT implementing an operator interface for NOP100-based modules.

[ModuleOperatorInterface](#classModuleOperatorInterface) implements a multi-modal user interface driven by an eight position DIL switch and a momentary push-buton. User interaction processing is triggered by the [handleButtonEvent()](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287) method which takes button state and DIL switch values as its arguments.

The interface supports an arbitrary number of operating modes each delivered by a client class that implements the [ModuleOperatorInterfaceClient](#classModuleOperatorInterfaceClient) interface. A single short button press allows the user to cycle through the available operating modes and so change the semantics of subsequent user interaction events.

A user interaction event comprises a two-step protocol which when completed successfully returns two data values: *address* and *value*.

This abstract class's performAction() method provides the functional link between a user interaction transaction and its implementation. A user transaction consists of two data values, *address* and *value*.

Button short press - change mode Button long press - start transaction and set address parameter to the value of the DIL switch Button short press (after long press) - end transaction, set value parameter from DIL switch and call performAction().

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`ModuleOperatorInterface`](#classModuleOperatorInterface_1a00d07785822e52939616b63331cf59a1)`(`[`ModuleOperatorInterfaceClient`](#classModuleOperatorInterfaceClient)` ** modeHandlers,unsigned int revertInterval)` | Create a new ModuleInterface object.
`public int `[`getCurrentMode`](#classModuleOperatorInterface_1a974eefcd01d49ebd2b8b75d23fc7c2e0)`()` | Get the index in the modeHandlers array of the current operating mode.
`public unsigned long `[`getButtonPressedAt`](#classModuleOperatorInterface_1a7aecf5e1d413e91f2d7615dbe54f11b6)`()` | Get the time in milliseconds of the last invocation of the [handleButtonEvent()](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287).
`public `[`EventOutcome`](#classModuleOperatorInterface_1a5625e661d0c567f2274d73428eca7c7f)` `[`handleButtonEvent`](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287)`(bool buttonState,unsigned char value)` | Handle a user interaction event (i.e. the press or release of the interface button).
`public void `[`revertModeMaybe`](#classModuleOperatorInterface_1ac545afdf3d37c19990bd3f064ae54f4c)`()` | Revert to default operating mode if user interaction has dried up.
`enum `[`EventOutcome`](#classModuleOperatorInterface_1a5625e661d0c567f2274d73428eca7c7f) | Result codes for the [handleButtonEvent()](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287) method.

## Members

#### `public  `[`ModuleOperatorInterface`](#classModuleOperatorInterface_1a00d07785822e52939616b63331cf59a1)`(`[`ModuleOperatorInterfaceClient`](#classModuleOperatorInterfaceClient)` ** modeHandlers,unsigned int revertInterval)` 

Create a new ModuleInterface object.

One or more mode handlers which implement application specific processing of user entered data must be passed to a new ModuleOperatorInstance via the *modeHandlers* array. The first mode handler in the array is set as the default handler and the final handler in the array must be followed by a pointer with the value 0.

In a multi-mode application automatic reversion to the default mode can be configured by specifying a *revertInterval* in seconds. This argument is optional and defaults to 0 seconds which disables automatic reversion.

#### Parameters
* `modeHandlers` - array of pointers to mode handlers which implement the [ModuleOperatorInterfaceClient](#classModuleOperatorInterfaceClient) interface. 

* `revertInterval` - number of seconds that must elapse without user activity before the default mode is restored.

#### `public int `[`getCurrentMode`](#classModuleOperatorInterface_1a974eefcd01d49ebd2b8b75d23fc7c2e0)`()` 

Get the index in the modeHandlers array of the current operating mode.

#### Returns
int - index of the current operating mode.

#### `public unsigned long `[`getButtonPressedAt`](#classModuleOperatorInterface_1a7aecf5e1d413e91f2d7615dbe54f11b6)`()` 

Get the time in milliseconds of the last invocation of the [handleButtonEvent()](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287).

#### Returns
unsigned long - timetsamp in milliseconds.

#### `public `[`EventOutcome`](#classModuleOperatorInterface_1a5625e661d0c567f2274d73428eca7c7f)` `[`handleButtonEvent`](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287)`(bool buttonState,unsigned char value)` 

Handle a user interaction event (i.e. the press or release of the interface button).

This method should be called with *buttonState* set to either Button::PRESSED or Button::RELEASED and, optionally, with *value* set to the current value of the DIL switch.

On a button press event a timestamp is taken which allows subsequent release event to be associated with either a short or long button press.

On a button release event there are several processing options:

A **long button press** will result in *value* being passed to the current mode handler's validateAddress() method and if characterised valid being saved as an *address* for subsequent processing. The method will return one of ADDRESS_ACCEPTED or ADDRESS_REJECTED.

A **short button press** consequent on a previously accepted *address* will result in a call to the current mode handler's processValue() method and, dependent upon the success or failure or this call, the return of one of VALUE_ACCEPTED or VALUE_REJECTED.

A **short button press** with no previously accepted *address* will result in cycling of the currently active mode handler and the return of MODE_CHANGE.

#### Parameters
* `buttonState` - one of Button::PRESSED or Button::RELEASED. 

* `value` - either an address or a value. 

#### Returns
int - a value from the EventOutcome enum.

#### `public void `[`revertModeMaybe`](#classModuleOperatorInterface_1ac545afdf3d37c19990bd3f064ae54f4c)`()` 

Revert to default operating mode if user interaction has dried up.

This method should be called from loop() to enable automatic mode reversion after the revert interval set at instantiation.

#### `enum `[`EventOutcome`](#classModuleOperatorInterface_1a5625e661d0c567f2274d73428eca7c7f) 

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
MODE_CHANGE            | 
ADDRESS_ACCEPTED            | 
ADDRESS_REJECTED            | 
VALUE_ACCEPTED            | 
VALUE_REJECTED            | 

Result codes for the [handleButtonEvent()](#classModuleOperatorInterface_1a62b30f88d2cfbdf12632fda30baec287) method.

# class `ModuleOperatorInterfaceClient` 

Abstract class defining interface used by ModuleInterface client classes.

Each mode supported by ModuleInterface is handled by a class which must implement this interface, probably by sub-classing it.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool `[`validateAddress`](#classModuleOperatorInterfaceClient_1ac5b010e665f11767732afb367f37289c)`(unsigned int address)` | Validate the *address* value deriving from a ModuleInterface dialogue.
`public bool `[`processValue`](#classModuleOperatorInterfaceClient_1a0097cfda6a7a833323024b1d2fdb2fa6)`(unsigned int address,unsigned char value)` | Process an *address* and an associated *value*.

## Members

#### `public bool `[`validateAddress`](#classModuleOperatorInterfaceClient_1ac5b010e665f11767732afb367f37289c)`(unsigned int address)` 

Validate the *address* value deriving from a ModuleInterface dialogue.

ModuleInterface calls this function immediately an address has been supplied by the user and before entry of an associated value. If validation of the address fails, then ModuleInterface can signal an issue and cancel a problematic dialogue.

#### Parameters
* `address` - *address* value to be validated. 

#### Returns
true - the address is valid. 

#### Returns
false - the address is not valid.

#### `public bool `[`processValue`](#classModuleOperatorInterfaceClient_1a0097cfda6a7a833323024b1d2fdb2fa6)`(unsigned int address,unsigned char value)` 

Process an *address* and an associated *value*.

ModuleInterface calls this function after an address has been validated with the value supplied by the user consequent on entry af the address.

#### Parameters
* `address` - the previously validated *address* value. 

* `value` - the *value* value. 

#### Returns
true - processing of the data pair was successfully. 

#### Returns
false - processing of the data pair failed. Maybe value is invalid, maybe the address/value combination is invalid, maybe the invoked process failed.

Generated by [Moxygen](https://sourcey.com/moxygen)