/* 
 * File:   xbox360Controller.cpp
 * Author: markleemalmose
 *
 * Created on 20 September 2019, 16:08
 * 
 *  https://archives.seul.org/linuxgames/Aug-1999/msg00107.html
 *  https://gist.github.com/jasonwhite/c5b2048c15993d285130
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <linux/joystick.h>

#include <cstdlib>

#include "../inc/xbox360Controller.h"

#define BUTTON_TO_FLAG(b) (1 << (b))


namespace legobot {

    namespace drivers {
        
        ControllerState Xbox360Controller::state;
        
        ControllerData Xbox360Controller::data;

        std::mutex Xbox360Controller::mtxUpdateController;

        int Xbox360Controller::init()
        {
            const char *device;
            // char *button = NULL; 
            char name_of_joystick[80];
            int fd;
            int num_of_axis = 0;
            int num_of_buttons = 0;
            //int x;

            device = "/dev/input/js0";

            // Open controller
            if( ( fd = open( device , O_RDONLY)) == -1 )
            {
                printf( "Could not open XBOX 360 controller\n" );
                return -1;
            }

            // Get controller info
            ioctl( fd, JSIOCGAXES, &num_of_axis );
            ioctl( fd, JSIOCGBUTTONS, &num_of_buttons );
            ioctl( fd, JSIOCGNAME(80), &name_of_joystick );

            printf("Controller: %s, %d axis, %d buttons\n\n"
                , name_of_joystick
                , num_of_axis
                , num_of_buttons 
            );

            fcntl( fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */

            return fd;
        }


        /**
         * Reads a joystick event from the joystick device.
         *
         * Returns 0 on success. Otherwise -1 is returned.
         */
        int Xbox360Controller::readEvent(int fd, struct js_event *event)
        {
            ssize_t bytes;

            bytes = read(fd, event, sizeof(*event));

            if (bytes == sizeof(*event))
            {
                return 0;
            }
                
            /* Error, could not read full event. */
            return -1;
        }


        /**
         * Returns the number of axes on the controller or 0 if an error occurs.
         */
        size_t Xbox360Controller::getAxisCount(int fd)
        {
            __u8 axes;

            if (ioctl(fd, JSIOCGAXES, &axes) == -1)
            {   
                return 0;
            }   

            return axes;
        }


        /**
         * Returns the number of buttons on the controller or 0 if an error occurs.
         */
        size_t Xbox360Controller::getButtonCount(int fd)
        {
            __u8 buttons;
            
            if (ioctl( fd, JSIOCGBUTTONS, &buttons ) == -1)
            {
                return 0;
            }

            return buttons;
        }

        /**
         * Current state of an axis.
         */
        struct axis_state {
            short x;
            short y;
        };

        /**
         * Keeps track of the current axis state.
         *
         * NOTE: This function assumes that axes are numbered starting from 0, and that
         * the X axis is an even number, and the Y axis is an odd number. However, this
         * is usually a safe assumption.
         *
         * Returns the axis that the event indicated.
         */
        size_t Xbox360Controller::getAxisState(struct js_event *event, struct axis_state axes[3])
        {
            size_t axis = event->number / 2;

            if (axis < 3)
            {
                if (event->number % 2 == 0)
                {
                    axes[axis].x = event->value;
                }
                else
                {
                    axes[axis].y = event->value;
                }
            }

            return axis;
        }

        void Xbox360Controller::update(int fd)
        {
            struct js_event event;
            size_t axis;
            struct axis_state axes[3] = {0};

            read(fd, &event, sizeof(struct js_event));

            int button;

            // Lock before updating data - critical section.
            std::lock_guard<std::mutex> guard(Xbox360Controller::mtxUpdateController);


            switch (event.type)
            {
                case JS_EVENT_BUTTON:
                    // printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");

                    /* determine which button the event is for */
                    switch (event.number) {

                        case 0: 
                            button = BUTTON_A; 
                            Xbox360Controller::data.buttons.a = event.value;
                            break;
                        case 1: 
                            button = BUTTON_B; 
                            Xbox360Controller::data.buttons.b = event.value;
                            break;
                        case 2: 
                            button = BUTTON_X; 
                            Xbox360Controller::data.buttons.x = event.value;
                            break;
                        case 3:
                            button = BUTTON_Y; 
                            Xbox360Controller::data.buttons.y = event.value;
                            break;
                        case 4: button = BUTTON_LEFT_SHOULDER; break;
                        case 5: button = BUTTON_RIGHT_SHOULDER; break;
                        case 6: button = BUTTON_BACK; break;
                        case 7: button = BUTTON_START; break;
                        case 8: button = 0; break; /* XBOX button  */
                        case 9: button = BUTTON_LEFT_THUMB; break;
                        case 10: button = BUTTON_RIGHT_THUMB; break;
                        default: button = 0; break;
                    }

                    /* set or unset the button */
                    if (event.value) {

                        Xbox360Controller::state.bCurrent |= BUTTON_TO_FLAG(button);
                            //STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(button);
                    } else {
                            //STATE[gamepad].bCurrent ^= BUTTON_TO_FLAG(button);
                        Xbox360Controller::state.bCurrent ^= BUTTON_TO_FLAG(button);
                    }
                    // std::cout << "Xbox360Controller::state.bCurrent: " << Xbox360Controller::state.bCurrent << std::endl;
                    break;
                
                case JS_EVENT_AXIS:
                    
                    axis = Xbox360Controller::getAxisState(&event, axes);
                    
                    if (axis < 3) 
                    {
                        // printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
                        
                        // Left joystick
                        if (axis == 0)
                        {
                            // Up
                            if (axes[axis].y < 0)
                            {
                                Xbox360Controller::data.left_stick.up = scale(axes[axis].y);
                                Xbox360Controller::data.left_stick.down = 0;
                            } 
                            // Down
                            else if (axes[axis].y > 0)
                            {
                                Xbox360Controller::data.left_stick.up = 0;
                                Xbox360Controller::data.left_stick.down = scale(axes[axis].y);
                            }

                            // Left
                            if (axes[axis].x < 0)
                            {
                                Xbox360Controller::data.left_stick.left = scale(axes[axis].x);
                                Xbox360Controller::data.left_stick.right = 0;
                            } 
                            // Right
                            else if (axes[axis].x > 0)
                            {
                                Xbox360Controller::data.left_stick.left = 0;
                                Xbox360Controller::data.left_stick.right = scale(axes[axis].x);
                            }
                        }
                        
                        // Right joystick
                        // Left/Right
                        if (axis == 1)
                        {
                            // Left
                            if (axes[axis].y < 0)
                            {
                                Xbox360Controller::data.right_stick.left = scale(axes[axis].y);
                                Xbox360Controller::data.right_stick.right = 0;
                            } 
                            // Right
                            else if (axes[axis].y > 0)
                            {
                                Xbox360Controller::data.right_stick.left = 0;
                                Xbox360Controller::data.right_stick.right = scale(axes[axis].y);
                            }
                        }
                        // Up/Down
                        if (axis == 2)
                        {
                            // Up
                            if (axes[axis].x < 0)
                            {
                                Xbox360Controller::data.right_stick.up = scale(axes[axis].x);
                                Xbox360Controller::data.right_stick.down = 0;
                            } 
                            // Down
                            else if (axes[axis].x > 0)
                            {
                                Xbox360Controller::data.right_stick.up = 0;
                                Xbox360Controller::data.right_stick.down = scale(axes[axis].x);
                            }
                        }
                    }
 
                    break;
                    
                default:
                    /* Ignore init events. */
                    break;
            }
        }
        
        ControllerData Xbox360Controller::getData(void)
        {
            // Temp controller data
            ControllerData tempData;

            // Lock before getting data - critical section.
            std::lock_guard<std::mutex> guard(Xbox360Controller::mtxUpdateController);

            tempData = Xbox360Controller::data;

            // Return the ControllerData.
            return tempData;
        }
        
        int Xbox360Controller::scale(int value)
        {
            int tempVal = abs(value);
            
            // Calculate the scaled zoom.
            float scaledVal = ((tempVal * 100) / 32767);
            
            return static_cast<int>(scaledVal);
        }
        

    } // namespace drivers.
} // namespace legobot.