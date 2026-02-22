from flask import Flask, jsonify, send_file
import serial
import threading
import time
import pandas as pd
from datetime import datetime

app = Flask(__name__)

# Configure serial port (CHANGE if needed)
ser = serial.Serial('COM3', 9600, timeout=1)

latest_reading = None
history = []

def read_serial():
    global latest_reading, history
    while True:
        try:
            line = ser.readline().decode().strip()
            if line:
                value = float(line)
                timestamp = datetime.now()

                latest_reading = value

                history.append({
                    "timestamp": timestamp,
                    "temperature": value,
                    "waterLevel": value,
                    "turbidity": value,
                    "pH": value
                })

        except:
            pass

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

@app.route("/data")
def get_data():
    return jsonify({
        "sensor_reading": latest_reading,
        "alerts": []
    })

@app.route("/history")
def get_history():
    return jsonify(history)

@app.route("/download")
def download_csv():
    df = pd.DataFrame(history)
    filename = "aquarium_data.csv"
    df.to_csv(filename, index=False)
    return send_file(filename, as_attachment=True)

if __name__ == "__main__":
    app.run(debug=True)
