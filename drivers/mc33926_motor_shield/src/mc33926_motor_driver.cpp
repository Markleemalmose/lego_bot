/* 
 * File:   mc33926_motor_driver.cpp
 * Author: markleemalmose
 *
 * Setup for Raspberry Pi 3 Model B
 * 
 * Created on 15 January 2020, 20:13
 */
#include <iostream>

#include <wiringPi.h>

#include "../inc/mc33926_motor_driver.h"

#define MAX_SPEED   480  // 19.2 MHz / 2 / 480 = 20 kHz

namespace legobot {

    namespace drivers {

                unsigned char Motor::_M1_DIRECTION;
                unsigned char Motor::_M1_PWM;
                unsigned char Motor::_M1_ENABLE;
                unsigned char Motor::_M1_SF;
                unsigned char Motor::_M2_DIRECTION;
                unsigned char Motor::_M2_PWM;
                unsigned char Motor::_M2_ENABLE;
                unsigned char Motor::_M2_SF;
        
          
        void Motor::init(void) {
            
            // Setup the wiring pi GPIO library
            wiringPiSetup();	
            
            //Pin map
            // M1
            Motor::_M1_DIRECTION = 24;
            Motor::_M1_PWM = 12;
            Motor::_M1_ENABLE = 22;
            Motor::_M1_SF = 5;
            
            // M1
            Motor::_M2_DIRECTION = 25;
            Motor::_M2_PWM = 13;
            Motor::_M2_ENABLE = 23;
            Motor::_M1_SF = 6;
            
            // Initialize Hardware PWM output
            // Motor speed input:
            // A PWM (pulse-width modulation) signal on this pin corresponds 
            // to a PWM output on the corresponding driver’s motor outputs. 
            // When this pin is low, the motor brakes low. When it is high, 
            // the motor is on. The maximum allowed PWM frequency is 20 kHz.
            pinMode(Motor::_M1_PWM, PWM_OUTPUT);    // GPIO 12 (PWM0) M1
            pinMode(Motor::_M2_PWM, PWM_OUTPUT);    // GPIO 13 (PWM1) M2

            // set the PWM to 20kHZ.x
            pwmSetMode(PWM_MODE_MS);
            pwmSetRange(MAX_SPEED);
            pwmSetClock(2);

            // Enable input: 
            // This pin is internally pulled low, 
            // putting the motor driver IC into a low-current sleep mode and 
            // disabling the motor outputs (setting them to high impedance). 
            // EN must be driven high to enable the motor driver.
            pinMode(Motor::_M1_ENABLE, OUTPUT);    // GPIO 22  M1 Enable
            pinMode(Motor::_M2_ENABLE, OUTPUT);    // GPIO 23  M2 Enable
            
            // Motor direction input: 
            // When DIR is low, motor current flows from output A to output B; 
            // when DIR is high, current flows from B to A.
            pinMode(Motor::_M1_DIRECTION, OUTPUT);    // GPIO 24  M1 Direction
            pinMode(Motor::_M2_DIRECTION, OUTPUT);    // GPIO 25  M2 Direction
            
            // Disable and status flag
            //pinMode(_nD2,OUTPUT);
            //digitalWrite(_nD2,HIGH); // default to on
            //pinMode(_nSF,INPUT);
            
        }
        
        void Motor::setM1Speed(int speed) {
            
            // Set speed for motor 1, speed is a number between -400 and 400
            
            unsigned char reverse = 0;

            if (speed < 0)
            {
                // Make speed a positive quantity
                speed = -speed;  
                
                // Preserve the direction
                reverse = 1;  
            }
            
            if (speed > 400)  
            {
                // Max PWM duty cycle
                speed = 400;
            }
              
            // map 400 to 255
            analogWrite(Motor::_M1_PWM, speed * 51 / 80); 

            if (reverse)
            {
                digitalWrite(Motor::_M1_DIRECTION, HIGH);
            }
            else
            {
                digitalWrite(Motor::_M1_DIRECTION, LOW);
            }
        }
        
        void Motor::setM2Speed(int speed) {
            
            // Set speed for motor 2, speed is a number between -400 and 400
            
            unsigned char reverse = 0;

            if (speed < 0)
            {
                // Make speed a positive quantity
                speed = -speed;  
                
                // Preserve the direction
                reverse = 1;  
            }
            
            if (speed > 400)
            {
                // Max PWM duty cycle
                speed = 400;
            }
            
            // map 400 to 255
            analogWrite(Motor::_M2_PWM, speed * 51 / 80); 
            
            if (reverse)
            {
                digitalWrite(Motor::_M2_DIRECTION, HIGH);
            }
            else
            {
                digitalWrite(Motor::_M2_DIRECTION, LOW);
            }
        }
        
        void Motor::setSpeeds(int m1Speed, int m2Speed) {
            
            // Set speed for both M1 and M2.
            Motor::setM1Speed(m1Speed);
            Motor::setM2Speed(m2Speed);
        }
        
        void Motor::enable(void) {
            std::cout << "Enable " << std::endl;
            // Enable M1 and M2
            digitalWrite(Motor::_M1_ENABLE, HIGH);
            digitalWrite(Motor::_M2_ENABLE, HIGH);
        }
        
        void Motor::disable(void) {
            
            // Enable M1 and M2
            digitalWrite(Motor::_M1_ENABLE, LOW);
            digitalWrite(Motor::_M2_ENABLE, LOW);
        }
        
    } // namespace drivers.
} // namespace legobot.