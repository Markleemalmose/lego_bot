/* 
 * File:   mc33926_motor_driver.h
 * Author: markleemalmose
 *
 * Created on 15 January 2020, 20:13
 */

#ifndef MC33926_MOTOR_DRIVER_H
#define MC33926_MOTOR_DRIVER_H

namespace legobot {

    namespace drivers {
        
        class Motor {
            
            private:
            
                /*
                 * @brief   M1 direction
                 */
                static unsigned char _M1_DIRECTION;
                
                /*
                 * @brief   M1 PWM
                 */
                static unsigned char _M1_PWM;
                
                /*
                 * @brief   
                 */
                static unsigned char _M1_ENABLE;
                
                /*
                 * @brief   
                 */
                static unsigned char _M1_SF;
                
                /*
                 * @brief   M2 direction
                 */
                static unsigned char _M2_DIRECTION;
                
                /*
                 * @brief   M2 PWM
                 */
                static unsigned char _M2_PWM;
                
                /*
                 * @brief   
                 */
                static unsigned char _M2_ENABLE;
                
                /*
                 * @brief   
                 */
                static unsigned char _M2_SF;
                
                
            public:
                
                /*
                 * @brief   Initialize motors.
                 */
                static void init(void);
                
                /*
                 * @brief   Enable motors.
                 */
                static void enable(void);
                
                /*
                 * @brief   Disable motors.
                 */
                static void disable(void);
                
                /*
                 * @brief   Set speed for Motor 1.
                 */
                static void setM1Speed(int speed);
                
                /*
                 * @brief   Set speed for Motor 2.
                 */
                static void setM2Speed(int speed);
                
                /*
                 * @brief   Set Speed for both motors.
                 */
                static void setSpeeds(int m1Speed, int m2Speed);
            
        };
        
    } // namespace drivers.
} // namespace legobot.

    
    
#endif /* MC33926_MOTOR_DRIVER_H */

