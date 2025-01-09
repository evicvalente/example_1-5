#include "mbed.h"
#include "arm_book_lib.h"

int main()
{
    DigitalIn enterButton(BUTTON1);       
    DigitalIn gasDetector(D2);
    DigitalIn overTempDetector(D3);
    DigitalIn aButton(D4);
    DigitalIn bButton(D5);
    DigitalIn cButton(D6);
    DigitalIn dButton(D7);

    DigitalOut alarmLed(LED1);
    DigitalOut incorrectCodeLed(LED3);
    DigitalOut systemBlockedLed(LED2);

    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);
    aButton.mode(PullDown);
    bButton.mode(PullDown);
    cButton.mode(PullDown);
    dButton.mode(PullDown);

    alarmLed = OFF;
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;

    bool alarmState = OFF;
    int numberOfIncorrectCodes = 0;

    while (true) {

        if ( gasDetector || overTempDetector ) {
            alarmState = ON;
        }

        alarmLed = alarmState;

        // Allow code entry attempts only if the user has made fewer than 3 errors
        // (the 4th error locks the system)
        if ( numberOfIncorrectCodes < 3 ) {

            // "Retry" condition: user presses A and D simultaneously (and *not* B, C, or enter)
            if ( aButton && dButton && !bButton && !cButton && !enterButton ) {
                incorrectCodeLed = OFF;
            }

            // If the alarm is on, the user can press the enter button (B1) to try the code
            if ( enterButton && !incorrectCodeLed && alarmState ) {

                // Correct code: Press B and C (while not pressing A or D)
                if ( bButton && cButton && !aButton && !dButton ) {
                    // Turn off the alarm and reset the error counter
                    alarmState = OFF;
                    numberOfIncorrectCodes = 0;
                } else {
                    // Wrong code -> set the error LED and increment the error counter
                    incorrectCodeLed = ON;
                    numberOfIncorrectCodes++;
                }
            }

        } else {
            // Lock the system on the 4th incorrect attempt
            systemBlockedLed = ON;
        }
    }
}
