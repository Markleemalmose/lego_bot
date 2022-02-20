/*
 *    File:   schedulerTickUpdate.h
 *
 *    Created: 10-09-2019
 *    Author: Mark Lee Malmose
 */

#ifndef SCHEDULERTICKUPDATE_H
#define SCHEDULERTICKUPDATE_H

#include <stdint.h>
#include <stdbool.h>
#include <mutex>
#include <chrono>

namespace legobot {

    namespace schedulertick {

        /*
         * @brief   Update class.
         */
        class Update {

          private:

            /*
             * @brief    Declaration of the mutex for the ready flag.
             */
            static std::mutex mtxReady;

            /*
             * @brief    Declaration of the mutex for the tick variable.
             */
            static std::mutex mtxTick;

            /*
             * @brief    Declaration of the mutex for the terminate flag.
             */
            static std::mutex mtxTer;

            /*
             * @brief   Declaration of the scheduler begin timestamp in ms.
             */
            static std::chrono::milliseconds msBegin;

            /*
             * @brief   Declaration of the scheduler end timestamp in ms.
             */
            static std::chrono::milliseconds msEnd;

            /*
             * @brief   Declaration of the scheduler elapsed timestamp in ms.
             */
            static std::chrono::milliseconds msElapsed;

            /*
             * @brief    Declaration of the ticks variable.
             */
            static uint32_t ticks;

            /*
             * @brief    Declaration of the ready flag.
             */
            static bool ready;

            /*
             * @brief    Declaration of the terminated flag.
             */
            static bool tickTerminated;

          public:

            /*
             * @brief    Add the tick variable and keep track of the ticks.
             */
            static void addTick(void);

            /*
             * @brief    Get the ticks.
             *
             * @return   Return the ticks.
             */
            static uint32_t getTicks(void);

            /*
             * @brief    Checking if the scheduler tick has been updated.
             *
             * @return   Return true/false.
             */
            static bool isReady(void);

            /*
             * @brief    Setting the shedulerTerminated flag.
             */
            static void setTerminate(void);

            /*
             * @brief    Checking if the scheduler shall be terminated.
             *
             * @return   Return true/false.
             */
            static bool hasTerminateOccurred(void);
        };
    } // schedulertick.
} // legobot.

#endif /* SCHEDULERTICKUPDATE_H */