/* 
 * File:   main.cpp
 * Author: markleemalmose
 *
 * Created on 9 September 2019, 21:17
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "../inc/stateMachine.h"
#include "../inc/stateMachineImpl.h"
#include "../inc/scheduler.h"
#include "../../schedulerTick_thread/inc/schedulerTickUpdate.h"
#include "../../services/inc/softwareSettings.h"
#include "../../drivers/xbox360Controller/inc/xbox360Controller.h"

using namespace std;

// Fsm declaration.
static Fsm fsm;

/*
 * @brief   This thread handles the timing for the scheduler.
 *
 * @note    This function will be called from a thread in main().
 */
static void schedulerHandler(void) {

    // Run through the schedulerHandler main loop.
    while(1) {

        // Update the scheduler ticks.
        legobot::schedulertick::Update::addTick();

        // Shall the schedulerHandler be terminated?
        if(legobot::schedulertick::Update::hasTerminateOccurred() == true) {

            // Return the thread immediately.
            return;
        }
    }
}

/*
 * @brief   This thread handles the controller communication
 *
 * @note    This function will be called from a thread in main().
 */
 static void controllerHandler(void) {

    //  Initialize controller
    int fd = legobot::drivers::Xbox360Controller::init();

     // Run through the controllerHandler main loop
     while(1) {

        // Update controller
        legobot::drivers::Xbox360Controller::update(fd);
            
        // Wait 8 ms - 125 Hz
        usleep(8000);
     }

 }

/**
 * @brief   Tracker task to run through the scheduler.
 */
static void trackerTask(void) {

    // Handle the main state machine.
    FsmEvent evt = fsmGetNextEvent(&fsm);
    fsmHandleEvent(&fsm, evt);
}


/**
 * @function main
 */
int main(void) { 
    
    
    // Create a scheduler timer handler thread.
    std::thread t1(schedulerHandler);

    // Create a controllerHandler thread.
    std::thread t2(controllerHandler);
    
    // Setup the main state machine. 
    fsmInit(&fsm);
    fsm.actions = (FsmActions*) &fsmTargetActions;
    
    // Add the main tracker task to the scheduler.
    legobot::application::SchedulerTask task;
    task.handle = &trackerTask;
    task.interval = TASK_TRACKER;
    task.index = 0;
    task.consumption = 0;
    legobot::application::Scheduler::add(task);

    
    // The infinite loop for the parent.
    while(1) {
        
        // Run the scheduler.
        legobot::application::Scheduler::run();
        
        //check to see if a button has been pressed.
        //this delay is necessary for proper operation of this program
        //if removed, frames will not have enough time to refresh and a blank 
        //image will appear.
//        switch (cv::waitKey(1)) {
//
//            case 27: // 'esc' key has been pressed, exit program.
//                return 0;
//            
//        }
    }
    return 0;
}

