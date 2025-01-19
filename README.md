# LCD Text Clock

A text-based clock using an LCD display with I2C interface on an ESP microcontroller with automatic NTP time sync.
- Displays the current time and date as text like "five minutes to four"
- 16x2, 40*2 or 20x4 character LCD display support
- Customizable animations
- Supports buttons to flip between modes showing different information and can be easily extended by further or them 
- Automatic backlight switching and dimming (optional feature) 
- Automatic time synchronization using NTP

The whole project rather serves as an example how text on a LCD display can be beautifully presented to users and can easily be extended/adapter to match your usecase.

<img src="./print/photos/example_usage.jpg" alt="example_usage"/>


## Hardware Requirements

- ESP32 Dev module
- 20x4 character LCD display with I2C interface (this one fits the housing)
- I2C module for LCD (e.g., PCF8574) - ideally one that has a jumper to enable/disable backlight as we can "misuse" this to dim the backlight via GPIO
- Optional: Push buttons 


## Software Requirements

- PlatformIO
- LiquidCrystal_I2C (via PIO)
- ESPDateTime (via PIO)
- WiFiManager (via PIO)

## Mechanics

Two different types of housings for 20*4 LCD displays are provided.
Using the acrylic plate on top of the LCD is optional but gives a cleaner look (not used in the example photos)

1. Without pushbuttons:

Very simple housing that consist of a front and a back piece.

TBD


2. With 4 pushbuttons:

Simple housing that has a row of four pushbuttons on top of the case.

TBD


### 3D-Printed Parts

1. Without pushbuttons:

| Filename                           | Thumbnail                                                                            | Required | Notes |
| ---------------------------------- | -------------------------------------------------------------------------------------| -------- | ------|
| `./print/Lower_part_nobuttons.stl` | <img src="./print/rendering/Lower_part_nobuttons.png" alt="Lower part" width="300"/> | 1        | |
| `./print/Upper_part_nobuttons.stl` | <img src="./print/rendering/Upper_part_nobuttons.png" alt="Upper part" width="300"/> | 1        | |


2. With 4 pushbuttons:


| Filename                       | Thumbnail                                                                              | Required | Notes |
| ------------------------------ | ---------------------------------------------------------------------------------------| -------- | ------|
| `./print/Lower_part.stl`       | <img src="./print/rendering/Lower_part.png" alt="Lower part" width="300"/>             | 1        | |
| `./print/Upper_part.stl`       | <img src="./print/rendering/Upper_part.png" alt="Upper part" width="300"/>             | 1        | |
| `./print/Pushbutton_base.stl`  | <img src="./print/rendering/Pushbutton_base.png" alt="Pushbutton_base" width="300"/>   | 1        | |
| `./print/Pushbutton_cover.stl` | <img src="./print/rendering/Pushbutton_cover.png" alt="Pushbutton_cover" width="300"/> | 1        | |


Printer settings:
- All printed parts designed for PETG. 
- Best experience on my printer was to print the front upside down (the actual front of the case facing the print bed) as this does not require any supports. For a cleaner look you can consider to print it reversed with ironing enabled but note that this requires a lot of support material. 
- Using fuzzy skin for all outside walls creates a nice look
- Use ironing to get a nice clean look on top of the pushbutton baseplate and cover
- No rafts/brim etc. reguired for any model.

### Required parts

TBD




### Assembly

- The ESP is screwed to the back of the housing.
- The acrylic plate is glued to the front (optional)
- The LCD is then fix via screws to the front
- When using the pushbuttons, click the pushbutton PCB into the baseplate and snap-in the pushbutton top cover. Then slide it into the lower part of the housing  
- Finally the lower and the upper part of the housing are assembled and tightened via the screws.


Assembly Video TBD


## Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/smengerl/LCD_Text_Clock.git
2. Open the project in PlatformIO.
3. Wait for PIO to configure and download the required libraries
4. Compile and upload the project to your ESP32 microcontroller.


## Usage
- Power on the ESP32.
- On first usage only: Configure the ESP to your local WiFi. For this, connect to the ESP's access point and use the default configuration page to enter your WLAN SSID and password 
- Wait for NTP sync
- The current time will be displayed on the LCD display.
- Use the (optional) buttons to flip through the pages and watch the animation


## Customization ideas

- Modify the text and backlight settings in the code to customize the display.
- Adjust the NTP server and time zone settings as needed.
- Add more pages, e.g. showing sensor data
- Get weather forecast via internet and display it on the screen with custom characters
- Write your own animations for text transitions
