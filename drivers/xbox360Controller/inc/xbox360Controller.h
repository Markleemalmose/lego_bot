/* 
 * File:   xbox360Controller.h
 * Author: markleemalmose
 *
 * Created on 20 September 2019, 22:27
 */

#ifndef XBOX360CONTROLLER_H
#define XBOX360CONTROLLER_H

#include <mutex>

namespace legobot {

    namespace drivers {

        /*
         * @brief   The ControllerState structure.
         */        
        struct ControllerState {
            //GAMEPAD_AXIS stick[STICK_COUNT];
            //GAMEPAD_TRIGINFO trigger[TRIGGER_COUNT];
            int bLast, bCurrent, flags;
            char* device;
            int fd;
            int effect;
        };
        
        /*
         * @brief   The ControllerData structure.
         */ 
        struct ControllerData {
            
            struct LeftStick {
                int left;
                int right;
                int up;
                int down;
            }; 
            LeftStick left_stick;
            
            struct RightStick {
                int left;
                int right;
                int up;
                int down;
            }; 
            RightStick right_stick;

            struct Buttons {
                int a;
                int b;
                int x;
                int y;
            };
            Buttons buttons;
            
        };

        
        /*
         * @brief   The GAMEPAD_BUTTON enumeration.
         */
        enum GAMEPAD_BUTTON {
            BUTTON_DPAD_UP		    = 0,	/**< UP on the direction pad */
            BUTTON_DPAD_DOWN		= 1,	/**< DOWN on the direction pad */
            BUTTON_DPAD_LEFT		= 2,	/**< LEFT on the direction pad */
            BUTTON_DPAD_RIGHT		= 3,	/**< RIGHT on the direction pad */
            BUTTON_START			= 4,	/**< START button */
            BUTTON_BACK			    = 5,	/**< BACK button */
            BUTTON_LEFT_THUMB		= 6,	/**< Left analog stick button */
            BUTTON_RIGHT_THUMB		= 7,	/**< Right analog stick button */
            BUTTON_LEFT_SHOULDER    = 8,	/**< Left bumper button */
            BUTTON_RIGHT_SHOULDER	= 9,	/**< Right bumper button */
            BUTTON_A			    = 12,	/**< A button */
            BUTTON_B			    = 13,	/**< B button */
            BUTTON_X			    = 14,	/**< X button */
            BUTTON_Y			    = 15,	/**< Y button */

            BUTTON_COUNT				/**< Maximum number of supported buttons */
        };


        /*
         * @brief   Xbox360Controller class.
         */
        class Xbox360Controller {
            
            private:
                
                /*
                 * @brief   Declaration of the ControllerState structure.
                 */
                static ControllerState state;
                
                /*
                 * @brief   Declaration of the ControllerData structure.
                 */
                static ControllerData data;
            
                /*
                 * @brief    Declaration of the mutex for the controller data.
                 */
                static std::mutex mtxUpdateController;

                /*
                 * @brief   Declaration of the readEvent variable.
                 */
                static int readEvent(int fd, struct js_event *event);

                /*
                 * @brief   Declaration of the getAxisCount variable.
                 */
                static size_t getAxisCount(int fd);

                /*
                 * @brief   Declaration of the getButtonCount variable.
                 */
                static size_t getButtonCount(int fd);

                /*
                 * @brief   Declaration of the getAxisState variable.
                 */
                static size_t getAxisState(struct js_event *event, struct axis_state axes[3]);
                
                /*
                 * @brief   scale.
                 */
                static int scale(int value);
            
            public:
                
                /*
                 * @brief   Init xbox controller.
                 */
                static int init(void);
                
                /*
                 * @brief   Update xbox controller.
                 */
                static void update(int fd);
                
                /*
                 * @brief   Get ControllerData.
                 */
                static ControllerData getData(void);
        };

    } // namespace drivers.
} // namespace legobot.

#endif /* XBOX360CONTROLLER_H */

