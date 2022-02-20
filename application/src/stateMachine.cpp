/*
 *    File:   stateMachine.cpp
 *
 *    Created: 10-09-2019
 *    Author: Mark Lee Malmose
 */

#include <iostream>
#include <unistd.h>

#include "../inc/stateMachine.h"
#include "../../services/inc/softwareSettings.h"

void fsmInit(Fsm* fsm) {
    fsm->initial = true;
    fsm->currentState = fsm->nextState = STATE_INIT;

    // Reset the fsm flags.
    //fsm->flags.trainingEnabled = false;
    
}

void fsmHandleEvent(Fsm* fsm, FsmEvent evt) {

    // Set flag the state has changed.
    bool firstEntry = fsm->initial || (fsm->nextState != fsm->currentState);
    
    // Update current state.
    fsm->currentState = fsm->nextState;

    // Switch through states and events.
    // States are only allowed to react on events of type
    // `FsmEvent`, and all output/actions should be performed
    // through the `fsm->actions` interface.
    
    // Main state machine.
    switch(fsm->currentState) {
        
        case STATE_INIT:
            
            if(firstEntry) {
                std::cout << std::endl;
                std::cout << "STATE_INIT" << std::endl;
                
                // Setting the state machine state text on the scheduler.
                //fsm->actions->initStateSetScheduler(fsm);
                
                // Initialize motors
                //fsm->actions->initMotor(fsm);
                
                // Setting the fsm start flags.
                fsm->actions->setFsmStartFlags(fsm);
                
                // Reading the monitor file.
                //fsm->actions->readMonitorFile(fsm);
               
            }
            
            // Has the connection been established?
//            if(evt == EVENT_CONNECTION_TO_CAMERA_IS_ESTABLISHED) {
//                
//                // Is the training mode enabled?
//                if(fsm->flags.trainingEnabled == true) {
//                    std::cout << "Connection to camera is established..." << std::endl;
//                    std::cout << "Configuration is set to training..." << std::endl;
//                    
//                    // Go to the training state.
//                    fsm->nextState = STATE_TRAINING;
//                    break;
//                
//                // Is the video tracker enabled?
//                } else {
//                    std::cout << "Connection to camera is established..." << std::endl;
//                    std::cout << "Configuration is set to video tracking..." << std::endl;
//                    
//                    // Set the current radar time.
//                    fsm->actions->setRadarTime(fsm);
//                    
//                    // Go to the next state.
//                    fsm->nextState = STATE_CAMERA_SET_AUTO_FOCUS;
//                    break;
//                }
//            }
            
            // Go to the next state.
            fsm->nextState = STATE_HANDLE_XBOX_360_CONTROLLER;
            
            break;
        
        case STATE_HANDLE_XBOX_360_CONTROLLER:
            if(firstEntry) {
                std::cout << std::endl;
                //std::cout << "STATE_HANDLE_XBOX_360_CONTROLLER" << std::endl;
            }
            
            // Get controller data
            fsm->actions->getXbox360ControllerData(fsm);
            
            // Go to the next state.
            fsm->nextState = STATE_DRIVE;
            break;
        
        case STATE_DRIVE:
            if(firstEntry) {
                std::cout << std::endl;
                //std::cout << "STATE_DRIVE" << std::endl;
            }
            
            // Drive motor
            //fsm->actions->driveMotor(fsm);
            
            // Go to the next state.
            fsm->nextState = STATE_HANDLE_XBOX_360_CONTROLLER;
            
            break;
            

        default:
            // Unknown state.
            break;
    }

    fsm->initial = false;
}
