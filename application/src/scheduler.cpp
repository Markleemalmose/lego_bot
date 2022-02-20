/*
 *    File:   scheduler.cpp
 *
 *    Created: 10-09-2019
 *    Author: Mark Lee Malmose
 */
#include "../inc/scheduler.h"
#include "../../schedulerTick_thread/inc/schedulerTickUpdate.h"
#include "../../services/inc/softwareSettings.h"

namespace legobot {

    namespace application {

        std::chrono::milliseconds Scheduler::msBegin;

        std::chrono::milliseconds Scheduler::msEnd;

        std::chrono::milliseconds Scheduler::msElapsed;

        std::vector<SchedulerTask> Scheduler::tasks;

        uint32_t Scheduler::totalConsumption;

        void Scheduler::add(const SchedulerTask& task) {

            // Add the task to the scheduler.
            Scheduler::tasks.push_back(task);
        }

        void Scheduler::run(void) {

            // Is the scheduler ready to be run?
            if(legobot::schedulertick::Update::isReady() == false) {

                // Return immediately.
                return;
            }

            // Loop through all the tasks.
            for(uint32_t i = 0; i < Scheduler::tasks.size(); i++) {

                // Is the task ready to be run?
                if((legobot::schedulertick::Update::getTicks() % (SCHEDULER_INTERVAL * Scheduler::tasks.at(i).interval)) == 0) {

                    // Take a begin task timestamp.
                    Scheduler::msBegin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

                    // Run the task.
                    Scheduler::tasks.at(i).handle();

                    // Take an end task timestamp.
                    Scheduler::msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

                    // Store the time consumption for the task.
                    Scheduler::msElapsed = Scheduler::msEnd - Scheduler::msBegin;
                    Scheduler::tasks.at(i).consumption = Scheduler::msElapsed.count();
                }
            }

            // Calculate the total scheduler consumption.
            Scheduler::totalConsumption = 0;

            for(uint32_t i = 0; i < Scheduler::tasks.size(); i++) {
                Scheduler::totalConsumption += tasks[i].consumption;
            }
        }

        std::vector<SchedulerTask> Scheduler::get(void) {

            // Return the scheduler.
            return Scheduler::tasks;
        }

        void Scheduler::setText(uint8_t index, const std::string& text) {

            // Set the text.
            Scheduler::tasks.at(index).text = text;
        }

        uint32_t Scheduler::getTotalConsumption(void) {

            // Return the total scheduler consumption.
            return Scheduler::totalConsumption;
        }
    } // namespace application.
} // namespace legobot.