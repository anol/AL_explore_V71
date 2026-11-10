import argparse
import time

import serial
from tqdm import tqdm

from IDE3380_mod import IDE3380_serial_setup_f

SERIAL_PORT_BAUD = 115200
NUM_CALIBRATION_READINGS = 10000
CALIBRATION_READING_DELAY_MS = 2

if __name__ == "__main__":
    print("Executing Calibration Routine")
    parser = argparse.ArgumentParser(
        prog="ASTRA-LEO EM Calibration Readout",
        description="Runs the internal calibration source multiple times. Does not read the data, use another tool for that.",
    )
    parser.add_argument("com_port", help="COM port of the EM MCU interface")
    args = parser.parse_args()

    ser = serial.Serial(args.com_port, SERIAL_PORT_BAUD, timeout=2)

    # Configure the ASIC
    IDE3380_serial_setup_f(ser, "configs/calibration_config.toml")

    # Do the baseline reading
    for x in tqdm(range(NUM_CALIBRATION_READINGS)):
        ser.write(b"ASIC REG 0x1C 1\n")
        time.sleep(CALIBRATION_READING_DELAY_MS / 1000)
        ser.write(b"ASIC REG 0x1C 0\n")
        time.sleep(CALIBRATION_READING_DELAY_MS / 1000)
