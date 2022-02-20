/*
 *    File:   stateMachineImpl.cpp
 *
 *    Created: 12-09-2019
 *    Author: Mark Lee Malmose
 */

#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>

#include "../inc/stateMachineImpl.h"
#include "../../services/inc/softwareSettings.h"
#include "../inc/scheduler.h"
#include "../../drivers/xbox360Controller/inc/xbox360Controller.h"
#include "../../drivers/mc33926_motor_shield/inc/mc33926_motor_driver.h"



using namespace std;

/**
 * Action handlers
 */

// Fsm flags actions.
static bool setFsmStartFlags(Fsm* fsm);

// Scheduler actions.
//static bool initStateSetScheduler(Fsm* fsm);

// Xbox360Controller actions
static bool getXbox360ControllerData(Fsm* fsm);

// Motor actions
static bool initMotor(Fsm* fsm);
static bool driveMotor(Fsm* fsm);


/**
 * Action list
 */
FsmActions fsmTargetActions = {
    
    // Fsm flags actions.
    .setFsmStartFlags = setFsmStartFlags,
    
    // Scheduler actions.
    //.initStateSetScheduler = initStateSetScheduler,
    
    // Xbox360Controller actions
    .getXbox360ControllerData = getXbox360ControllerData,
    
    // Motor actions
    .initMotor = initMotor,
    .driveMotor = driveMotor,
    
};

/**
 * Get the next event for the state machine.
 * This is the target specific implementation of this function,
 * as it extracts the events directly from the hardware. A stub of
 * this function should be implemented for testing.
 *
 * @param       Fsm* fsm        The state machine.
 * @return      The next event in the queue.
 */
FsmEvent fsmGetNextEvent(Fsm* fsm) 
{
    
    if(fsm->currentState == STATE_INIT) 
    {
        if(fsm->nextState == STATE_INIT) 
        {
            if(!fsm->initial) 
            {
                
//                // Has the connection the camera been established?
//                if(cameraInterfaceHasConnectionBeenEstablished()) {
//                    return EVENT_CONNECTION_TO_CAMERA_IS_ESTABLISHED;
//                }
            }
        }
    }
    
    if(fsm->currentState == STATE_HANDLE_XBOX_360_CONTROLLER) 
    {
        if(fsm->nextState == STATE_HANDLE_XBOX_360_CONTROLLER) 
        {
            
        }
    }
    
    if(fsm->currentState == STATE_DRIVE) 
    {
        if(fsm->nextState == STATE_DRIVE) 
        {
            
        }
    }
    
    
        
    return NO_EVENT;
}

/**
 * @brief	Functions for setting fsm start flags.

 * @return	true/false.
 */
static bool setFsmStartFlags(Fsm* fsm) {

    // Finally return true.
    return true;
}

/**
 * @brief	Functions for controller handling.

 * @return	true/false.
 */
static bool getXbox360ControllerData(Fsm* fsm) {
    
    // Get the ControllerData.
    legobot::drivers::ControllerData controllerData;
    controllerData = legobot::drivers::Xbox360Controller::getData();
    
    std::cout << "Left stick: Left: " << controllerData.left_stick.left << "\tRight: " << controllerData.left_stick.right << "\tUp: " << controllerData.left_stick.up << "\tDown: " << controllerData.left_stick.down << std::endl;
    std::cout << "Right stick: Left: " << controllerData.right_stick.left << "\tRight: " << controllerData.right_stick.right << "\tUp: " << controllerData.right_stick.up << "\tDown: " << controllerData.right_stick.down << std::endl;
    std::cout << "Buttons: A: " << controllerData.buttons.a << "\tB: " << controllerData.buttons.b << "\tX: " << controllerData.buttons.x << "\tY: " << controllerData.buttons.y << std::endl;
    
    return true;
}

/**
 * @brief	Functions for drive handling.

 * @return	true/false.
 */
static bool initMotor(Fsm* fsm) {
    
    // Init motor
    //legobot::drivers::Motor::Motor();
    legobot::drivers::Motor::init();
    
    legobot::drivers::Motor::enable();
    
    legobot::drivers::Motor::setSpeeds(0, 0);
}

static bool driveMotor(Fsm* fsm) {
    
    
    legobot::drivers::Motor::setSpeeds(100, 100);
    
}