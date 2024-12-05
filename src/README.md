# Countdown Timer Project

### Welcome to the Countdown Timer Project
This project demonstrates a robust embedded system application developed using **Mbed OS**. It integrates user interaction, visual feedback, and auditory alerts into a fully functional countdown timer. This implementation is a great example of applied embedded programming skills.

---

## What Does It Do?
The Countdown Timer:
- Allows users to set a time (in minutes and seconds) using potentiometers.
- Displays the countdown progress on an **LCD screen** with a dynamic progress bar.
- Provides visual feedback through **LEDs** (indicating running, paused, or completed states).
- Activates a buzzer sound when the timer finishes.
- Supports features like pausing, resetting, and a pre-start countdown.

---

## Features

- **LCD Display**: Displays time, progress, and user instructions.
- **Potentiometers**: Adjust the timer's duration for minutes (0-9) and seconds (0-59).
- **LEDs**: Visual indicators for the timer state.
- **Buzzer**: Provides an alert upon timer completion.
- **Joystick Controls**:
  - `Fire`: Start/Resume/Pause the timer.
  - `Up`: Reset the timer.
  - `Down`: Quit to the initial setup.
- **Progress Bar**: Visually tracks the countdown's progress.
- **Hourglass Countdown**: A brief 3-second countdown animation before the timer begins.

---

## Hardware Requirements
To implement this project, you will need:
- **Microcontroller**: An Mbed-compatible board. This project specifically uses the **STM32F401RE** with the **Mbed Application Shield**.
- **LCD Display**: C12832 or a similar compatible module. The library used for this display can be found [here](http://os.mbed.com/users/askksa12543/code/C12832/).
- **Potentiometers**: Two potentiometers for user input (minutes and seconds).
- **LEDs**: Three LEDs (Red, Green, and Blue).
- **Buzzer**: A piezoelectric buzzer.
- **Joystick Module**: Alternatively, push buttons can be used for control.

---

## How It Works
1. **Set the Timer**: Use the potentiometers to adjust the desired minutes and seconds.
2. **Start the Timer**: Press the `Fire` button to initiate the countdown.
3. **Visual Countdown**: Observe the hourglass animation followed by the dynamic progress bar as time counts down.
4. **Pause or Reset**: The timer can be paused or reset at any point using joystick controls.
5. **Completion Alert**: The buzzer activates when the timer finishes, accompanied by visual cues from the LEDs.

---

## Code Structure
- **Main Logic**: Manages the timer's states and user interactions.
- **Object-Oriented Design**: Encapsulates hardware functionalities into classes (`LED`, `Buzzer`, `Potentiometer`).
- **State Machine**: Implements a structured flow with states like `Initialisation`, `countdownState`, `timer_running`, and more.

---

## Running the Project
1. Ensure you are using **Mbed OS 2** for compatibility.
2. Connect the required hardware components.
4. Compile and flash the program onto your microcontroller.
5. Test the system functionality.

---

## Suggestions for Improvement
- Extend the timer's maximum duration or allow custom alarms.
- Enhance visual effects with more intricate LED patterns.
- Replace joystick input with a touchscreen interface for more modern control.

---

## Acknowledgments
This project showcases practical applications of embedded programming and hardware integration. Thanks to Mbed OS for simplifying the development process and making hardware interaction seamless.

---

Feel free to explore the code and contribute improvements or new features!

