
const char configuration[] PROGMEM = 
{   "3.4.enableTrim:"
    "3.7.enableThrottle:"
    "11.14.5.TrimWheel:"
    "11.15.5.Throttle:"
    "4.0.0.0.4.50.TrimSet:"
    "4.1.1.1.7.5.ThrottleSet:"
    "1.10.SyncLostTrim:"
    "1.11.SyncLostThrottle:"
//    "9.2.3.2.3.0.StepperTrim:"
//    "9.5.6.5.6.0.StepperThrottle:"
};

/*
    Stepper:    2 / 3   Trim Stepper
    Output:     4       Trim enable
    Stepper:    5 / 6   Throttle Stepper
    Output:     7       Throttle enable
    Button:     10      Trim Button sync lost
    Button:     11      Throttle Button sync lost
    Analog:     14      Trim Analog out
    Analog:     15      Throttle Analog out

    "4.0.0.0.4.50.TrimSet:"
               ^
               | Startposition in 0...100%
              ^
              | Pin number for enable stepper
            ^
            | stepper number
         ^
         | button number for sync lost
       ^
       | analog in number for actual value
     ^
     | Type 7Segment, is used for getting setpoint
*/

struct Key {
    uint8_t pin;
    char *name;
};
struct Out {
  uint8_t pin;
  char *name;
};

const Key key[] PROGMEM = {
  10, "SyncLostTrim",
  11, "SyncLostWheel"
};

const Out out[] PROGMEM = {
  4, "enableTrim",
  7, "enableThrottle"
};
