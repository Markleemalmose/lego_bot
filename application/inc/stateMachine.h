/*
 *    File:   stateMachine.h
 *
 *    Created: 10-09-2019
 *    Author: Mark Lee Malmose
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief	Function pointer for the action.
 *
 * @param	FsmActionFn fsm*
 *
 * @return	SUCCESS/FAILURE
 */
typedef bool (*FsmActionFn)(struct Fsm*);

/**
 * @brief States.
 */
enum FsmState {
    
    // Initialization states.
    STATE_INIT,
    
    // Controller states
    STATE_HANDLE_XBOX_360_CONTROLLER,
    
    // Process states
    STATE_DRIVE,
   
};

/**
 * @brief	Events / Inputs
 */
enum FsmEvent {

    // Initialization events.
    EVENT_CONNECTION_TO_CAMERA_IS_ESTABLISHED,
            
    NO_EVENT,
};

/**
 * @brief	Actions / Outputs
 * actions that can be called from within the state machine.
 * No other "outside" functions should be called.
 */
struct FsmActions {
    
    // Fsm flags actions.
    FsmActionFn setFsmStartFlags;
    
    // Scheduler actions.
    //FsmActionFn initStateSetScheduler;
    
    // XBOX Controller actions.
    FsmActionFn getXbox360ControllerData;
    
    // Motor actions
    FsmActionFn initMotor;
    FsmActionFn driveMotor;
    
};

/**
 * @brief	Control flags.
 * Used for example to disable/enable certain submodules at runtime.
 * As creating more state is bad design, the use of state variables like
 * these should be limited.
 */
struct FsmControlFlags {
    
    // bool trainingEnabled : 1;               // bit field.
};

/**
 * @brief Single encapsulation of the state machine.
 */
struct Fsm {
    bool initial;
    FsmState currentState;
    FsmState nextState;
    FsmActions* actions;
    FsmControlFlags flags;
};

/**
 * @brief Function for initializing the state machine.
 * 
 * @param Fsm* fsm
 */
void fsmInit(Fsm* fsm);

/**
 * @brief	Function for getting the next event for the state machine.
 *
 * @param	Fsm* fsm
 *
 * @return	fsmEvent		The next event in the queue.
 */
FsmEvent fsmGetNextEvent(Fsm* fsm);

/**
 * @brief	Function for executing the next state.
 *
 * @param	Fsm* fsm		The state machine.
 * @param	fsmEvent		The next event in the queue.
 */
void fsmHandleEvent(Fsm* fsm, FsmEvent evt);

#endif /* STATEMACHINE_H */
