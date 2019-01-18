Build Guide
=============

1. Setting Up The IR Sensor
2. Updating Up The Control Code
3. Prototyping Single Fan Writing
4. Building The Control Board
5. Connecting & Testing
6. Fabricating The Fan Shrouds
7. Constructing Nylon Flappy Guys
8. Building The Base
9. Putting It All Together

### 1. Setting Up The IR Sensor

Let's go ahead and get the code side of this project out of the way, since it's the easiest. The pvodied `control-code.ino` file contains everything you need to start/stop your five flappy guys, but you'll probably need to change the IR input values, depending on your IR remote. 

So the first step is finding these values.

To do this, first begin by plugging your IR receiver into your Arduino. This shouldn't be a parmanent connection at this point, just temporarily connect the IR receiver with jumper wires as indicated below.

![IR Wiring](https://kellishaver.com/misc/flappy-guys/01.png)

Next, plug your Arduino into your computer and open `ir-read-utility.ino` in your Arduino IDE. In the tools menu, make sure you have the right COM port selected and flash the read utility code to your Arduino. 

Once you've flashed the board, open the serial monitor (Tools > Serial Monitor). 

Now start pressing buttons on your remote. In my build, I used buttons 1-5 to control each of the 5 fans, and used a big red center button in the middle of the remote to toggle everything off/on. 

It doesn't really matter what order you assign buttons in, so long as it's logical to you. Using numbers 1-5 makes the most sense, though, so I'll assume you're doing that! 

Whenever you press a button on the remote, you should see an integer displayed in the serial monitor. You'll want to make note of which integers belong to which buttons.

Please note, with these inexpensive IR sensors, it's very easy to get interference. As I was working on this, my heartrate sensor from my Fitbit kept interfering with the IR sensor when my wrist was next to it. 

So push buttons a few times and make sure you're getting consistent readings! Recode those numbers and ignore any inconsistencies, as they indicate interference with the IR signal. 

### 2. Updating The Control Code

Once you've made note of all of your buttons and their corresponding integers, you can open `control-code.stl` and update the values listed there to match the output from your remote. 

On lines 48 and 49, you'll find two variables:

`fanBtns` - This is an array of 5 buttons to toggle fans. Put your 1-5 IR codes in there, in order.
`allFansBtn` - This is the button that will turn all fans on/off at once. Put your toggle button IR code in there.

Once you've updated these variables and safed the file, you can flash `control-code.ino` to your Arduino, overwriting the previous IR read utility code. 

