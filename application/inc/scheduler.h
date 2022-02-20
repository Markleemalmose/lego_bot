/*
 *    File:   scheduler.h
 *
 *    A class that handles the scheduler.
 *
 *    Created: 24-09-2019
 *    Author: Mark Lee Malmose
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <string>
#include <chrono>
#include <vector>

namespace legobot {

    namespace application {

        /*
         * @brief   The SchedulerTask structure.
         */
        struct SchedulerTask {

            void (*handle)(void);
            uint32_t interval;
            uint8_t index;
            uint32_t consumption;
            std::string text;
        };

        /*
         * @brief   Scheduler class.
         */
        class Scheduler {

          private:

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
             * @brief   Declaration of the tasks vector.
             */
            static std::vector<SchedulerTask> tasks;

            /*
             * @brief   Declaration of the totalConsumption variable.
             */
            static uint32_t totalConsumption;

          public:

            /*
             * @brief    Add a task to the scheduler.
             *
             * @param    const SchedulerTask& task   The task to add.
             */
            static void add(const SchedulerTask& task);

            /*
             * @brief    Run the scheduler.
             */
            static void run(void);

            /*
             * @brief    Get the scheduler.
             *
             * @return   Return the scheduler.
             */
            static std::vector<SchedulerTask> get(void);

            /*
             * @brief    Set text on a specific task in the scheduler.
             *
             * @param    uint8_t index              Index indicating the specific task.
             * @param    const std::string& text    The text to set.
             */
            static void setText(uint8_t index, const std::string& text);

            /*
             * @brief    Get the total scheduler consumption.
             *
             * @return   Return the total scheduler consumption.
             */
            static uint32_t getTotalConsumption(void);
        };
    } // namespace application.
} // namespace legobot.

#endif /* SCHEDULER_H */