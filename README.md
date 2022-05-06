## USELESS EMOTIONAL MACHINE

Minimal Arduino Useless Machine with only one emotion - anger!
Based on `Useless_Attitude.ino` sketch of Brandon Clark.

Original [code](https://github.com/braclark/Arduino/blob/master/Useless_Attitude.ino), [video](https://www.youtube.com/watch?v=nbF8ANHkJv4), [parts](https://www.thingiverse.com/thing:4561617/files)

## About

Very funny gadget on Digispark Tiny 85.
If you try hard to change the switch's state, it gets angry and stoically puts everything back where it was!

**Features:**
- If more than `N_SWITCHINGS_TO_MAD` switchings were made within `RAPID_SWITCHINGS_INTERVAL` milliseconds between each, it gets angry :angry:
- Cyclically (randomly or in sequence) perform emotional actions;
- Built-in LED lights up, when it becomes angry.


## Dependencies

Tested with:
 - Arduino IDE 1.8.10
 - "Digistump AVR Boards" board by Digistump version 1.6.7. More [here](http://digistump.com/wiki/digispark/tutorials/connecting).
