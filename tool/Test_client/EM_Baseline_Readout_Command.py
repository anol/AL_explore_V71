import argparse
import time

import serial
from tqdm import tqdm

from IDE3380_mod import IDE3380_serial_setup_f

SERIAL_PORT_BAUD = 115200
NUM_BASELINE_READINGS = 10000
BASELINE_READING_DELAY_MS = 40

if __name__ == "__main__":
    print("Executing Baseline Readout")
    parser = argparse.ArgumentParser(
        prog="ASTRA-LEO EM Baseline Readout",
        description="Calls repeated forced readout commands to get a baseline. Does not read the data, use another tool for that.",
    )
    parser.add_argument("com_port", help="COM port of the EM MCU interface")
    args = parser.parse_args()

    ser = serial.Serial(args.com_port, SERIAL_PORT_BAUD, timeout=2)

    # Configure the ASIC
    IDE3380_serial_setup_f(ser)

    # Do the baseline reading
    for x in tqdm(range(NUM_BASELINE_READINGS)):
        ser.write(b"ASIC REG 0x1D 1\n")
        time.sleep(BASELINE_READING_DELAY_MS / 1000)
