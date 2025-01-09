# Car Black Box

The Car Black Box is a compact and efficient data logging system designed to record critical vehicle parameters such as time, gear, and collision data. This system stores the collected information in EEPROM, ensuring data reliability and integrity even in the event of power loss. Once the memory reaches its capacity, the system starts overwriting the oldest logs to ensure continuous data collection.

## Features
- **Data Logging**: Captures vehicle parameters including time, gear position, and collision events.
- **Data Storage**: Stores data securely in EEPROM for permanent retention.
- **Data Overwriting**: After 10 logs, the system automatically starts overwriting the oldest data.
- **Potentiometer-Based Acceleration**: Utilizes a potentiometer for measuring vehicle acceleration, compatible with the PicGenious board.
- **Compact Design**: Lightweight and suitable for integration into vehicle electronics without compromising performance.

## Working Principle
The system uses a microcontroller to continuously log data from various sensors into the EEPROM. The data logged includes:
- **Time**: Captures timestamp information for every logged event.
- **Gear**: Monitors the gear position of the vehicle.
- **Collision Detection**: Detects impact events and logs them along with corresponding time.

Once the memory reaches 10 logs, the system begins to overwrite the oldest entry, ensuring continuous data recording without loss.

Logged Data Example:  
===================
  ```
   Time      Gear   Speed
   12:35:22  G3      67
  ```
## Car Black Box System
Below is the block diagram of the Car Black Box:

![Car Black Box Diagram](Images/Screenshot2024-08-29_154857.png)
