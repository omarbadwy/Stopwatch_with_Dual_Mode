Project Title :
Stopwatch with Dual Mode (Increment and Countdown) Using ATmega32 and Seven-Segment 
Display. 
 Key Features:
The stopwatch operates in two modes:
1. Count up Mode: The stopwatch starts in this mode by default ,Counts up from zero, displaying time in seconds, minutes, and hours.
2. Countdown Mode: Counts down from a preset time until it reaches zero, triggering an alert, with easy mode switching.

Mode Toggle: Users can easily switch between the two modes with the press of a button, offering versatility for different needs.
Resume Function: Resumes counting from the paused time.
Pause Function: Pauses the time counting, allowing the stopwatch to hold its current 
value. 
Reset Function: Resets the time to zero in both modes. 

Timer1 Configuration :
The entire stopwatch timing mechanism is driven by Timer1, configured in CTC mode. This ensures precise and accurate timekeeping by generating time-based interrupts.

External Interrupts for User Control:
The project utilizes external interrupts to handle various button inputs, allowing for immediate and responsive control.
INT0 (Reset): Triggered on a falling edge, this interrupt is tied to the reset button, instantly resetting the stopwatch to zero.
INT1 (Pause): Triggered on a rising edge, this interrupt is connected to the pause button. When activated, it stops Timer1 from updating, freezing the current time.
INT2 (Resume): Triggered on a falling edge, this interrupt resumes Timer1’s counting from the paused time, whether in increment or countdown mode.

Additional Features:
Manual Time Adjustment: Users can modify the hours, minutes, and seconds manually using the adjustment buttons, allowing flexibility in countdown mode.

🔗 Code will be upload on GitHub after deadline : 
https://lnkd.in/e3nubPnM
🔗 Watch the video on my YouTube channel :
https://lnkd.in/e-ffAN-z
