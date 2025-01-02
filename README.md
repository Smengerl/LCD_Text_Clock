# LCD Text Clock

A text-based clock using an LCD display with I2C interface on an ESP microcontroller with automatic NTP time sync.
- Displays the current time as text like "five minutes to four"
- 16x2 or 20x4 character LCD display support
- Customizable animations
- Can be extended with rotary encoders or buttons to flip between multiple pages with different information
- Automatic backlight switching and dimming 
- Automatic time synchronization using NTP
- Energy saving measures

The whole project rather serves as an example how text on a LCD display can be beautifully presented to users and can easily be extended/adapter to match your usecase.

<img src="./print/photos/example_usage.jpg" alt="example_usage"/>


## Hardware Requirements

- ESP32 Dev module
- 16x2 or 20x4 character LCD display with I2C interface
- I2C module for LCD (e.g., PCF8574)
- Optional: Push buttons


## Software Requirements

- PlatformIO
- LiquidCrystal_I2C (via PIO)
- ESPDateTime (via PIO)
- WiFiManager (via PIO)

## Mechanics

Renderings / Prototype:
| Front                                                | Back                                                 | Isometric |
| ---------------------------------------------------- | ---------------------------------------------------- | --------- |
| <img src="./print/rendering/front.png" alt="front"/> | <img src="./print/rendering/back.png" alt="back"/>   | <img src="./print/rendering/iso.png" alt="iso"/> |
| <img src="./print/photos/front.jpg" alt="front"/> | <img src="./print/photos/back.jpg" alt="back"/>   | <img src="./print/photos/iso.jpg" alt="iso"/> |


The only mechanical part required is the housing which consists of a front and a back piece.
Using the acrylic plate on top of the LED matrix display is optional but gives a cleaner look (not used in the example photos)

### 3D-Printed Parts

| Filename                     | Thumbnail                                                                | Required | Notes |
| ---------------------------- | -------------------------------------------------------------------------| -------- | ------|
| `./print/frontside.stl`      | <img src="./print/rendering/frontside.png" alt="frontside" width="300"/> | 1        | |
| `./print/backside.stl`       | <img src="./print/rendering/backside.png" alt="backside" width="300"/>   | 1        | |

Printer settings:
- All printed parts designed for PETG. 
- Best experience on my printer was to print the front upside down (the actual front of the case facing the print bed) as this does not require any supports. For a cleaner look you can consider to print it reversed with ironing enabled but note that this requires a lot of support material. 
- Using fuzzy skin for all outside walls creates a nice touch
- No rafts/brim etc. reguired for any model.

### Required parts

| Name              | Spec                          | Required | Notes |
| ----------------- | ----------------------------- | -------- | ------|
| countersunk screw | M3 5mm, e.g. DIN EN ISO 4762  | 4        | To attach ESP to back of housing |
| countersunk screw | M3 10mm, e.g. DIN EN ISO 4762 | 4        | To attach display to back of housing |
| countersunk screw | M3 10mm, e.g. DIN EN ISO 4762 | 4        | To fix back and front of housing |
| semi-transparent acrylic board | max 2mm, 140-150mm * 33-35mm | 1        | Optional, for cleaner look |

### Assembly

- The ESP is screwed to the back of the housing.
- The acrylic plate is glued to the front.
- The LCD is then fix via screws
- Finally the front of the housing is snapped into the back and tightened via the screws from the back.

![assembly](./print/rendering/assembly.gif)



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
- Write your own animations for text transitions
- Adjust the NTP server and time zone settings as needed.