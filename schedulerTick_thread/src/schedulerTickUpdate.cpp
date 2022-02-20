/*
 *    File:   schedulerTickUpdate.cpp
 *
 *    A class that handles the ticks used for the scheduler.
 *
 *    Created: 24-09-2019
 *    Author: Mark Lee Malmose
 */

#include "../inc/schedulerTickUpdate.h"
#include "../../services/inc/softwareSettings.h"

namespace legobot {

    namespace schedulertick {

        std::mutex Update::mtxReady;

        std::mutex Update::mtxTick;

        std::mutex Update::mtxTer;

        std::chrono::milliseconds Update::msBegin;

        std::chrono::milliseconds Update::msEnd;

        std::chrono::milliseconds Update::msElapsed;

        uint32_t Update::ticks;

        bool Update::ready;

        bool Update::tickTerminated;

        void Update::addTick(void) {

            // Take the elapsed time.
            Update::msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            Update::msElapsed = Update::msEnd - Update::msBegin;

            // Has the elapsed time passed?
            if((uint32_t)Update::msElapsed.count() >= SCHEDULER_INTERVAL) {

                // Update the ticks.
                Update::ticks += SCHEDULER_INTERVAL;

                // Handle a possible overflow.
                if(Update::ticks >= (0xFFFFFFFF - 1)) {
                    Update::ticks = 0;
                }

                // Take a new timestamp.
                Update::msBegin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

                // Set the ready flag.
                Update::ready = true;
            }
        }

        uint32_t Update::getTicks(void) {

            // ticks variable temporary initialization.
            uint32_t ticksTemp = 0;

            // Encapsulate the tick in ms variable handling - critical section.
            Update::mtxTick.lock();

            // Set the ticks temporary variable.
            ticksTemp = Update::ticks;

            Update::mtxTick.unlock();

            // Return the tickMs flag.
            return ticksTemp;
        }

        bool Update::isReady(void) {

            // Ready flag temporary initialization.
            bool readyTemp = false;

            // Encapsulate the scheduler ready flag handling - critical section.
            Update::mtxReady.lock();

            // Has ready occurred?
            if(Update::ready == true) {

                // Set the temporary ready flag.
                readyTemp = true;

                // Reset the ready flag.
                Update::ready = false;
            }

            Update::mtxReady.unlock();

            // Return the ready flag.
            return readyTemp;
        }

        void Update::setTerminate(void) {

            // Encapsulate the terminate flag handling - critical section.
            Update::mtxTer.lock();

            // Set the tickTerminated flag.
            Update::tickTerminated = true;

            Update::mtxTer.unlock();
        }

        bool Update::hasTerminateOccurred(void) {

            // tickTerminated flag temporary initialization.
            bool tickTerminatedTemp = false;

            // Encapsulate the tickTerminated flag handling - critical section.
            Update::mtxTer.lock();

            // Has tickTerminated occurred?
            if(Update::tickTerminated == true) {

                // Set the tickTerminated temporary flag.
                tickTerminatedTemp = true;

                // Reset the tickTerminated flag.
                Update::tickTerminated = false;
            }

            Update::mtxTer.unlock();

            // Return the tickTerminated flag.
            return tickTerminatedTemp;
        }
    } // schedulertick.
} // legobot.
