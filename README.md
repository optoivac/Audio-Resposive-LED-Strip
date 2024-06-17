# Audio Visualizer LED Strip

This project is an audio visualizer that streams data from a computer to an Arduino, which controls an LED strip. The LED strip changes colors based on the audio output from your computer. It is still a work in progress. Love Late night Ideas

Will most likely change to a esp bluethoth paired device over usb serial communication.

Currently useing audio ampltuide to calcate the colour and brightness, need to update it to use the pc volume  to tweek the ampltuide 
as at the moment you lose range depending on the volume. 

## Hardware Requirements

- Arduino (e.g., Arduino Uno)
- Programable LED strip
- Jumper wires or just wires or nothing realy up to you
- 5V power supply for the LED strip (Atm its from the Uno)
- Computer with audio output (If you dont have this I would have a few qestions)

## Software Requirements

- Any IDE
- Python Libraries:
  - `sounddevice`
  - `numpy`
  - `pyserial`

## To find the correct audio output run this:

import sounddevice as sd

def list_audio_output_devices():
    print("Available audio output devices:\n")
    devices = sd.query_devices()
    for idx, device in enumerate(devices):
        if device['max_output_channels'] > 0:
            print(f"{idx}: {device['name']}")

if __name__ == "__main__":
    list_audio_output_devices()
