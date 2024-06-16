import sounddevice as sd
import numpy as np
import serial
import time

# Set the correct COM port
com_port = 'COM7'  # Replace 'COM7' with the actual COM port from Device Manager
scaling_factor = 1024  # Scale amplitude values to range 0-1024

# Function to open the serial port with retries
def open_serial_port(port, baud_rate, retries=5):
    for attempt in range(retries):
        try:
            ser = serial.Serial(port, baud_rate, timeout=1)
            print(f"Connected to {port} on attempt {attempt + 1}")
            return ser
        except serial.SerialException as e:
            print(f"Attempt {attempt + 1} failed: {e}")
            time.sleep(2)
    raise Exception(f"Could not open {port} after {retries} attempts")

def write_to_serial(ser, data):
    try:
        ser.write(data.encode('utf-8'))
    except serial.SerialException as e:
        print(f"Serial write failed: {e}")

def check_serial_connection(ser):
    if not ser.is_open:
        try:
            ser.open()
            print("Serial port reopened")
        except serial.SerialException as e:
            print(f"Failed to reopen serial port: {e}")

try:
    ser = open_serial_port(com_port, 9600)
    time.sleep(2)  # Give some time for the connection to establish
except Exception as e:
    print(f"Could not open {com_port}: {e}")
    exit(1)

# Use Stereo Mix device
device_id = 14  # Replace with the device ID for Stereo Mix

# Check the capabilities of the selected device
device_info = sd.query_devices(device_id)
print(device_info)

# Use the maximum number of input channels supported by the device
channels = device_info['max_input_channels']
if channels < 1:
    raise ValueError(f"Selected device does not support input channels: {device_info}")

def audio_callback(indata, frames, time, status):
    if status:
        print(status)
    amplitude = np.max(np.abs(indata))
    amplitude = int(amplitude * scaling_factor)  # Scale to 0-1024
    amplitude = max(0, min(1024, amplitude))  # Ensure amplitude is within range
    print(f"Amplitude: {amplitude}")  # Debug print
    write_to_serial(ser, f"{amplitude}\n")
    check_serial_connection(ser)

try:
    # Start the audio stream
    stream = sd.InputStream(callback=audio_callback, channels=channels, samplerate=44100, device=device_id)
    with stream:
        print(f"Streaming audio output from device {device_id} with {channels} channels...")
        while True:
            sd.sleep(1000)  # Keep the script running
except KeyboardInterrupt:
    print("Script interrupted by user")
except Exception as e:
    print(f"Failed to start audio stream: {e}")
finally:
    ser.close()
    print("Serial connection closed")
