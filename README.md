# Smart Aquarium Monitoring System

An embedded system project built using the Dragon Plus (MC9S12DG256) microcontroller to automate aquarium monitoring and control.

The system monitors:

- Water Temperature (DS18B20 – Digital)
- Water Level (Analog)
- Turbidity (Analog)
- pH Level (Analog)

And automatically controls:

- Cooling Fan (Temperature regulation)
- Water Pump (Water level control)

Sensor data is transmitted via serial communication to a Python backend for logging and visualization through a web dashboard.

---

## System Architecture

Dragon Plus Board (MC9S12DG256)
        ↓
Analog/Digital Sensors
        ↓
Embedded C Firmware
        ↓ (SCI Serial 9600 baud)
Python Flask Server
        ↓
Web Dashboard (HTML + Chart.js)

---

## Technologies Used

- Embedded C (CodeWarrior)
- MC9S12DG256 Microcontroller
- ADC (Analog to Digital Conversion)
- Serial Communication (SCI)
- Python (Flask, PySerial, Pandas)
- HTML + Chart.js

---

## How It Works

1. Sensors send data to the microcontroller.
2. Firmware reads values and checks thresholds.
3. Fan turns on if temperature > 29°C.
4. Pump turns on if water level < threshold.
5. Data is sent via serial.
6. Python logs data and serves it to a web dashboard.

---

## Running the Project

### 1. Flash the Firmware
Compile `main.c` using CodeWarrior and upload to the Dragon Plus board.

### 2. Run the Python Server
```bash
pip install -r requirements.txt
python app.py
