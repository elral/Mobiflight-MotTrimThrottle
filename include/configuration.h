
const char configuration[] PROGMEM = 
{   "3.4.enableTrim:"
    "3.7.enableThrottle:"
    "11.14.5.TrimWheel:"
    "11.15.5.Throttle:"
    "4.0.0.0.15.1.TrimSet:"
    "4.1.1.1.15.1.ThrottleSet:"
    "1.10.SyncLostTrim:"
    "1.11.SyncLostThrottle:"
    "9.2.3.2.3.0.StepperTrim:"
    "9.5.6.5.6.0.StepperThrottle:"
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
*/