# LED Board
#### Client Name: Roy Luo

## Project Requirements
### High Level PCB Requirements
- The PCB must be about 1.5cm by 1.5cm, with LED bulb extruding outwards in direction of the plane.
- Battery holder must be attached to the bottom of PCB board, and within that 1.5cm by 1.5cm dimension constraint.
- Battery holder will hold 2 CR1216 or CR1220 batteries, providing a battery life of at least 5 hours.
- Components must be chosen and sourced based on availability - we want components to be immediately available or available within at most 2 weeks for manufacturers to solder onto the PCB. Preferred is JLCPCB.
- The LED light will be flashing different colours, at different rates, to produce unique patterns when in motion, all of which is to be programmed and controlled via the microcontroller.
- Safety of the device is of high importance

### Key Electrical Components
- Addressable LED
    - Flashes 3 different colours at the same time
    - 16 possible colour options - can be reduced to save implementation costs
- Microcontroller
    - holds in memory the programmed colour settings (This point does not mean the memory should be retained when device goes off, it only means all the color combinations reside in flash so that they can be activated when device is ON)
    - controls the logic for user to change colour settings
- Button
    - Turns LED on/off (On press and hold of the button)
    - Sets light to colour selection mode
    - Cycle through colour selection
- Battery
    - 2 x CR1216 or CR1220 equivalent

### High Level Firmware Requirements
There are 3 states for the LED light:
1. OFF State
2. ON State
3. Cycle (Color Selection) State

- Pressing button once from OFF state turns LED to ON state
    - LED turns on upon release of button, not when button is pressed down
    - When in ON state, it will flash the first flash pattern in the cycle of patterns
- Pressing button once from ON state will make the LED flash the next pattern in the cycle
    - Pressing the button on the last pattern will take the user to the first pattern
- Pressing button and holding for 2 seconds from ON state turns LED to OFF state
- Pressing button and holding for 2 seconds from OFF state turns LED to Colour Selection state
    - LED will flash green for 2 seconds before entering colour selection mode
    - After the green flash the LED will stay on the first colour of the colour palette
    - Subsequent presses of the button will cycle the user through the colour palette
    - To choose a colour, press and hold on the desired colour, and the LED will flash that colour on and off for 1 second before returning to the first colour selection again
    - Repeat this 3 times to select the 3 colour choices, and after the final colour is selected, LED will flash the first flash pattern with the new colours
 
### Color Palette
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/1069aed5-bd84-4d40-92bf-90b2500b1282)


### Strobe Pattern
Below are 5 strobe patterns we would like LED to be able to flash. Such patterns are made visible to the eye by waving the LED light around in circular motion.

#### 1. Standard Strobe
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/75f1c882-2b0e-406d-8c58-4fc0e2419b81)

#### 2. Long Strobe
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/70fcf54d-384f-447e-b22b-ab4fc3512201)

#### 3. Dots
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/0d2b24b3-d527-4cc0-95db-720b7c82994f)

#### 4. Flicker
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/3221ddf2-7893-4f55-9b50-e15c654356da)

#### 5. Streamline/ribbon
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/9fe1b16b-f108-4faf-a77c-39fe556084a2)

### Reference Product Images
![image](https://github.com/eSpark-Consultants/royluo-ledboard/assets/76909080/ed92cbf9-958e-43b8-9786-4598957a4836)


