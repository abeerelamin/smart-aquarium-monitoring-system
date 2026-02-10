# Smart Aquarium Monitoring System

This project presents an embedded system for automating aquarium monitoring and control.  
The system monitors water quality parameters and automatically controls actuators to maintain a stable aquatic environment.

## Project Context
- Course: Embedded Systems Lab (CENG329)
- Type: Team project (3 members)
- Microcontroller: Dragon Plus Board

## My Contribution
I contributed to:
- Writing embedded C code for sensor reading and actuator control
- Python scripts for data logging and analysis
- HTML-based monitoring interface
- System testing, calibration, and integration

## System Features
- Real-time monitoring of:
  - Water temperature
  - pH level
  - Turbidity
  - Water level
- Automatic control of:
  - Cooling fan (temperature regulation)
  - Water pump (water level control)
  - Buzzer alerts for abnormal conditions
- LCD display for live sensor readings
- Data transfer and logging using Python
- Basic web interface for monitoring

## Hardware Components
- Dragon Plus Trainer Board
- pH sensor
- Turbidity sensor
- Temperature sensor
- Water level sensor
- Cooling fan
- Water pump
- Active buzzer
- LCD display

## Software Stack
- Embedded C (CodeWarrior)
- Python (data logging and analysis)
- HTML (monitoring interface)

## How the System Works
1. Sensors send analog/digital signals to the microcontroller.
2. Sensor data is processed using predefined thresholds.
3. Actuators are triggered automatically when abnormal conditions are detected.
4. Sensor readings are displayed locally and logged for further analysis.
5. Alerts are generated using a buzzer and visual indicators.

## Limitations
- Designed for small to medium-sized aquariums
- Limited scalability due to hardware constraints
- No remote IoT cloud integration

## Future Improvements
- IoT-based remote monitoring
- Mobile-friendly dashboard
- Predictive maintenance using machine learning
