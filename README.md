# ESP32 CAN-Bus Node Simulation Project

> This project simulates a vehicle's CAN-bus network using two ESP32 nodes (ECU0 and ECU1). It includes a custom-designed DBC file for message management and a specialized C++ library for encoding and decoding CAN messages with support for different endianness and scaling factors.

## Project Overview

The system consists of two main Electronic Control Units (ECUs) communicating over a CAN-bus network at 500 kbps:

- ECU0 (Sensor & Gear Simulation): This node simulates various vehicle sensors and the transmission control unit.

  - Sensors: Uses potentiometers to simulate real-time data for Engine RPM, Vehicle Speed, Coolant Temperature, Environment Temperature, and Fuel Level.

  - Gearbox: Handles gear state transitions (P, R, N, D) via physical buttons.

- ECU1 (Light Control Cluster): This node interfaces with a physical steering switch cluster to monitor lighting signals.

  - Inputs: Monitors signals for Fog Lights, Hazard Lights, Low Beam, High Beam, and Turn Signals (Left/Right).
-  GATEWAY: The Raspberry Pi serves as a centralized Gateway, bridging the CAN-bus network with a high-resolution HMI (Human Machine Interface). It captures all frames from the bus and visualizes real-time vehicle telemetry through a Qt-Quick application.

## Key Components

### 1. CAN_Message Library

-  A custom C++ library (CAN_Message.h/cpp) designed to handle DBC-based communication.

-  Packing/Unpacking: Automatically converts physical values (e.g., speed in kph) to raw CAN data and vice-versa.

-  Endianness Support: Supports both Intel (Little Endian) and Motorola (Big Endian) formats.

-  Signal Scaling: Manages signal factors, offsets, and min/max constraints as defined in the DBC.

### 2. Database (DBC) Files

The project uses custom DBC files to define the network's communication matrix:

-  MyDBC.dbc: Defines messages such as INDICATOR_BLOCK (0x444), SYS_INFOR (0x455), GEARBOX_INFOR (0x454), and HAVC (0x456).

-  Base_DBC.dbc: Includes standard vehicle signals like steering sensors and wheel speeds.

### 3. Hardware Architecture

- Microcontrollers: ESP32.

- CAN Transceivers: TJA1050 or MCP2515 modules.

- Peripherals: Potentiometers for ADC simulation, push buttons for gear selection, and a vehicle steering switch cluster.

- Visualization: Integrated with an Infotainment display to visualize simulated data (RPM, Speed, Temperature).

### 4. Raspberry Pi Gateway & HMI Setup

**Hardware Configuration**

The Raspberry Pi communicates with the MCP2515 module via the SPI interface. Ensure the following connections (using SPI0):
| PIN |  |
| :--- | :---: |
|VCC  |5V / 3.3V |
|GND  |  Ground  |
|CS   |GPIO 8 (SPI_CE0_N)|
|SO (MISO)|GPIO 9| 
|SI (MOSI)|GPIO 10|
|SCK|GPIO 11|
|INT|GPIO 25 (Interrupt pin used in driver configuration)|

**Driver Installation & OS Setup**

To enable the MCP2515 driver on Raspberry Pi OS, modify the boot configuration file:

Open the config file: ```sudo nano /boot/config.txt```

Append the following lines to enable SPI and the CAN overlay:

```bash
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```
*Note: If your MCP2515 module specifically uses an 8MHz crystal, adjust oscillator=8000000 accordingly*

**Automatic CAN Boot Configuration**

To ensure the ```can0``` interface starts automatically upon system boot without manual intervention, follow these steps

Create the configuration file:

```bash
sudo nano /etc/systemd/network/80-can.network
```

Insert the following content:
```bash
[Match]
Name=can0

[CAN]
BitRate=500K
RestartSec=100ms
```
Enable the service:

```bash
sudo systemctl enable systemd-networkd
sudo systemctl start systemd-networkd
sudo reboot
```

**Verify the connection**

To monitor incoming traffic from ECU0 and ECU1 in the terminal, you can use:
```bash
candump can0
```
**Qt Infotainment Application**

-  The HMI is developed using the Qt Framework (Qt 6 recommended), leveraging the QtSerialBus module to interact with SocketCAN.

-  Backend: Uses the socketcan plugin to read structured data via the CAN_Message logic.

-  Visualization: Displays real-time gauges for Engine RPM (0-10,000), Vehicle Speed (0-240 km/h), and critical sensor data (Coolant Temp, Fuel Level).

-  Map Integration: Includes a navigation interface as part of the Infotainment dashboard.

[Qt Cross-Compilation for Raspberry Pi 4 with Docker](https://github.com/haidinh-me/Qt6CrossRaspi)

[Qt-Infotainment App](https://github.com/haidinh-me/Qt-Infotainment)

## Installation & Setup

### Library Configuration
To use the CAN_Lib across different ECU sketches, create a symbolic link in your Arduino libraries folder. Run the following command in Command Prompt (Administrator):
```bash
mklink /D "C:\Users\YourUser\Documents\Arduino\libraries\CAN_Lib_Link" "C:\Path\To\Project\CAN_Lib"
```
*Note: Adjust the paths to match your local directory structure*

Wiring
- ECU0 Pins: ADCs connected to pins 32, 35, 25, 33, 26. Gear buttons on pins 18, 19, 16, 21.

- ECU1 Pins: Light switches connected to pins 15, 14, 17, 16, 12, 13.

- CAN Bus: Ensure proper termination (120-ohm resistor) on the CAN high/low lines.

## Project Structure
/CAN_Lib: Core logic for DBC message processing.

/DBC: Database files and conversion logs.

/ECU0: Source code for sensor and gear simulation.

/ECU1: Source code for light cluster signal processing.

/assets: Hardware setup images and wiring views.

## Results

![result](assets/font_view.jpg)

![result](assets/top_view.jpg)

[Video](https://drive.google.com/file/d/1GxtVI-kOqTYB-53vUPZy9jE4fNOOrv_C/view?usp=sharing)

## Developed by @haidinh-me
