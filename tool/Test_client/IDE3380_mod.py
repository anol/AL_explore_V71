import argparse
import serial
import time
import tomllib
import sys
import crcmod.predefined
import struct
import numpy as np
import readchar
import msvcrt
import csv
import threading
import datetime

# For Plotly Dash
import dash
from dash import dcc, html, Input, Output
import plotly.graph_objs as go

crc32_fn = crcmod.predefined.mkPredefinedCrcFun('crc-32')
serial_update = 0

# Global synchronization objects for test mode
data_lock = threading.Lock()  # Protects access to accum_data
stop_event = threading.Event()  # Signals when test mode should end


class bcolors:
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


def pack_register_data_from_toml(bitmap, config_data):
    packed_data = {}
    bit_mask = {}
    register_index = 0
    for reg_name, register_entries in bitmap["REG"].items():
        for register in register_entries:
            bits = register["bits"]
            reg_values = dict(config_data["REG"][reg_name].items())
            packed_value = 0
            current_bit_position = 0
            for bit_def in bits:
                var_name = bit_def["name"]
                bit_length = bit_def["length"]
                value = reg_values.get(var_name, 0)
                if value >= (1 << bit_length):
                    raise ValueError(f"Value {value} for {var_name} exceeds {bit_length} bits.")
                packed_value = (packed_value << bit_length) | value
                current_bit_position += bit_length
            packed_data[register_index] = packed_value
            bit_mask[register_index] = current_bit_position
            register_index += 1
    return packed_data, bit_mask


def send_command(ser, sent_data, expected_response):
    for i in range(3):  # try 3 times
        ser.write(sent_data)
        time.sleep(0.2)
        try:
            response = ser.readline().strip()
        except Exception as e:
            print(bcolors.FAIL + f"{sent_data} command failed: {e}" + bcolors.ENDC)
            return 0

        # Normalization of valyes ensuring any and all prefix of '0' is stripped.
        normalized_expected = expected_response.decode().lstrip('0').replace(',0', ',').encode()
        normalized_response = response.decode().lstrip('0').replace(',0', ',').encode()

        if response == expected_response:
            print(bcolors.OKGREEN + f"{sent_data} -> {response} command successful" + bcolors.ENDC)
            return 1

    print(bcolors.FAIL + f"Expected {expected_response} but got {response}" + bcolors.ENDC)
    return 0


def send_command_nak(ser, sent_data, expected_response):
    ser.write(sent_data)
    time.sleep(0.2)
    response = ser.readline().strip()
    if response == expected_response:
        print(bcolors.OKGREEN + f"{sent_data} -> {response} command successful" + bcolors.ENDC)
    else:
        print(bcolors.FAIL + f"{sent_data} command failed")


def IDE3380_serial_setup_f(ser):
    toml_file_path = 'config_test.toml'
    bitmap_toml_file_path = 'bitmap.toml'
    with open(toml_file_path, 'rb') as file:
        data = tomllib.load(file)
    with open(bitmap_toml_file_path, 'rb') as file:
        bitmap = tomllib.load(file)

    # Process REG category
    category_name = 'REG'
    if category_name in data:
        packed_data, bit_mask = pack_register_data_from_toml(bitmap, data)
        for reg, value in packed_data.items():
            command = f"AT+ASIC_{category_name}={reg},{value}\n"
            command_bytes = command.encode()
            return_command = f"OK={reg},{value}"
            return_command_bytes = return_command.encode()
            if send_command(ser, command_bytes, return_command_bytes) == 0:
                return
    # Process GENERAL category
    category_name = 'GENERAL'
    if category_name in data:
        for section_name, section_data in data[category_name].items():
            command = f"AT+{section_name}={section_data}\n"
            command_bytes = command.encode()
            return_command = f"OK={section_data}"
            return_command_bytes = return_command.encode()
            if send_command(ser, command_bytes, return_command_bytes) == 0:
                return

    # Test calibration
    c = 200  # number of pulses
    for i in range(0, 1, 1):
        v = 200  # Dac value
        command = f"AT+CAL={v},{c}\n"
        command_bytes = command.encode()
        return_command = f"OK={v},{c}"
        return_command_bytes = return_command.encode()
        if send_command(ser, command_bytes, return_command_bytes) == 0:
            return

    ser.readline().strip()
    # output_type = data['GENERAL']['OUTPUT']
    # return output_type


def IDE3380_serial_loop(ser):
    global serial_update
    while True:
        if serial_update == 1:
            print("STARTING SETUP")
            IDE3380_serial_setup_f(ser)
            serial_update = 0
        response = ser.readline().strip()
        try:
            data_str = response.decode('utf-8').strip('\n')
            data_list_str = data_str.split(',')
            data_list_int = [int(num.strip()) for num in data_list_str if num.strip()]
            if not data_list_int:
                print('.', end='', flush=True)
                continue
            if len(data_list_int) != 4096 + 1 + 2:  # 4096 data + CRC + Vbias + Temperature
                print(f"Expected 4096 numbers, but received {len(data_list_int)}.")
                continue
        except ValueError:
            print("IN DEEPSLEEP")
            continue
        # Return only the first (4096+2) numbers (ignoring CRC checking here)
        return data_list_int[0:(4096 + 2)], 0


def IDE3380_serial_setup():
    global serial_update
    serial_update = 1


def print_help():
    print(bcolors.ENDC + bcolors.BOLD + "Commands:" + bcolors.ENDC)
    print(" 0: Get status")
    print(" 1: Update values")
    print(" 2: Live view (Gamma MCA)")
    print(" 3: Isotope identification")
    print(" 4: Deep sleep (saved to SD card)")
    print(" 5: Test mode (saved to SD card and transmitted to plotly via web browser)")
    print(" 6: Set V offset = 0")
    print(" 7: Set V offset = 255")
    print(" 8: Get max")
    print(" 9: Fetch file")
    print(" a: ASIC dump: Readout all the ASIC registers")
    print(" c: Clean: Use the default setup and erase the persistent storage")
    print(" d: Dump: Get a list of the current setup values")
    print(" h: Help: Show this list of commands")
    print(" l: Load: Use the setup from the persistent storage")
    print(" n: Next channel 0..18")
    print(" s: Save: Store the current setup to the persistent storage")
    print(" u: Output format 3")
    print(" v: Output format 4")
    print(" w: Output format 1")
    print(" x: Run a short test sequence")
    print(" y: Run a long test sequence")
    print(" q: Quit\n")

def set_output_format(format_number):
    print("Output format: ", format_number)
    command = 'AT+OUTPUT=' + str(format_number) + '\n'
    expected = 'OK=' + str(format_number)
    send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
    now = datetime.datetime.now()
    yymmdd = now.strftime("%y%m%d")
    hhmm = now.strftime("%H%M")
    command = f"AT+DEEPSLEEP={yymmdd},{hhmm}\n".encode('utf-8')
    command_ok = f"OK={yymmdd},{hhmm}".encode('utf-8')
    send_command(ser, command, command_ok)
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line:
            break
        print("  ", line)

def next_channel():
    next_channel.number = next_channel.number +1
    if next_channel.number > 18:
        next_channel.number = 0
    print("Channel: ", next_channel.number)
    command = 'AT+PRINTF_CHANNEL=' + str(next_channel.number) + '\n'
    expected = 'OK=' + str(next_channel.number)
    send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))

next_channel.number = 0

def toggle_printing():
    if toggle_printing.on_off > 0:
        toggle_printing.on_off = 0
        print("Stop printing\n")
        command = 'AT+OUTPUT=' + str(toggle_printing.on_off) + '\n'
        expected = 'OK=' + str(toggle_printing.on_off)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
    else:
        toggle_printing.on_off = 1
        print("Start printing\n")
        command = 'AT+OUTPUT=' + str(toggle_printing.on_off) + '\n'
        expected = 'OK=' + str(toggle_printing.on_off)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
        now = datetime.datetime.now()
        yymmdd = now.strftime("%y%m%d")
        hhmm = now.strftime("%H%M")
        command = f"AT+DEEPSLEEP={yymmdd},{hhmm}\n".encode('utf-8')
        command_ok = f"OK={yymmdd},{hhmm}".encode('utf-8')
        send_command(ser, command, command_ok)

toggle_printing.on_off = 0

def test_sequence_X():
    print("Run a short test sequence\n")
    for gpc in range(20):
        command = 'AT+A=' + str(gpc) + '\n'
        expected = 'OK=' + str(gpc)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
        command = 'AT+B=' + str(gpc) + '\n'
        expected = 'OK=' + str(gpc)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
        if send_command(ser, b'AT+SAVE\n', b'OK') == 0:
            break
        send_command(ser, b'AT+STATUS\n', b'OK')
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                break
            print("  ", line)

    print("Test sequence completed\n")


def test_sequence_Y():
    print("Run a long test sequence\n")
    old_time = time.time_ns()
    for gpc in range(1000):
        print(gpc)
        command = 'AT+A=' + str(gpc) + '\n'
        expected = 'OK=' + str(gpc)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
        command = 'AT+B=' + str(gpc) + '\n'
        expected = 'OK=' + str(gpc)
        send_command(ser, command.encode("utf-8"), expected.encode("utf-8"))
        failed = send_command(ser, b'AT+SAVE\n', b'OK') == 0
        new_time = time.time_ns()
        delta_time = (new_time - old_time) / 1000000000
        print(delta_time)
        old_time = new_time
        if failed:
            break


print("Test sequence completed\n")


def command_handler(key):
    if key == '0':  # Request miscellaneous status information
        print("Get status\n")
        send_command(ser, b'AT+STATUS\n', b'OK')
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                break
            print("  ", line)
        return True

    if key == '1':  # Update values
        print("Updating values\n")
        IDE3380_serial_setup_f(ser)
        print(bcolors.OKGREEN + bcolors.BOLD + "Update Done!\n" + bcolors.ENDC)
        return True

    if key == '2':  # Live Gamma View
        send_command(ser, b'AT+MODE_DEMO\n', b'OK')
        print("Output demo for Gamma MCA ('https://spectrum.nuclearphoenix.xyz/')")
        now = datetime.datetime.now()
        yymmdd = now.strftime("%y%m%d")
        hhmm = now.strftime("%H%M")
        command = f"AT+TIME={yymmdd},{hhmm}\n".encode('utf-8')
        command_ok = f"OK={yymmdd},{hhmm}".encode('utf-8')
        send_command(ser, command, command_ok)
        ser.close()
        exit(1)

    if key == '3':  # Isotope Identification
        send_command(ser, b'AT+OUTPUT=2\n', b'OK=2')
        print("Output made for Gamma source identification\n")
        now = datetime.datetime.now()
        yymmdd = now.strftime("%y%m%d")
        hhmm = now.strftime("%H%M")
        command = f"AT+DEEPSLEEP={yymmdd},{hhmm}\n".encode('utf-8')
        command_ok = f"OK={yymmdd},{hhmm}".encode('utf-8')
        send_command(ser, command, command_ok)
        print("Please wait: creating file on sd card")
        try:
            while True:
                line = ser.readline()
                decoded_line = line.decode('utf-8')
                if decoded_line.strip():  # Check if there's any non-whitespace content
                    print(decoded_line, end='')
        except KeyboardInterrupt:
            print(bcolors.WARNING + "\nUser stopped isotope identification.\n" + bcolors.ENDC)
            ser.close()
            exit(0)
        return True

    if key == '4':  # Deep sleep
        print("\nEntering Deep Sleep Mode...\n")
        # Step 1: Stop any active output stream
        if not send_command(ser, b'AT+OUTPUT=0\n', b'OK=0'):
            print(bcolors.WARNING + "\nWarning: Could not confirm OUTPUT=0, continuing anyway.\n" + bcolors.ENDC)
            # This is a consideration due to the possible firmware limitation with serial communcation.
            # Device may not actually be able to recieve commands whilst data is still streaming.
        # Step 2: Prepare timestamped deep-sleep command
        now = datetime.datetime.now()
        yymmdd = now.strftime("%y%m%d")
        hhmm = now.strftime("%H%M")
        command = f"AT+DEEPSLEEP={yymmdd},{hhmm}\n".encode('utf-8')
        command_ok = f"OK={yymmdd},{hhmm}".encode('utf-8')
        # Step 3: Attempt to send the command gracefully
        try:
            success = send_command(ser, command, command_ok)
            if success:
                print(bcolors.OKGREEN + "\nDevice acknowledged deep sleep entry.\n" + bcolors.ENDC)
            else:
                print(
                    bcolors.WARNING + "\nNo explicit OK received, device may have already entered sleep.\n" + bcolors.ENDC)

            # Step 4: Allow the MCU to finish shutting down
            print("\nAllowing device to finalize deep sleep...\n")
            time.sleep(1.0)

        except serial.SerialTimeoutException:
            print(
                bcolors.WARNING + "\nSerial write timed out, device likely entered deep sleep before response.\n" + bcolors.ENDC)
        except Exception as e:
            print(bcolors.FAIL + f"\nUnexpected serial error during deep sleep.\n: {e}" + bcolors.ENDC)
        finally:
            # Step 5: Clean up regardless of success/failure
            try:
                ser.flushInput()
                ser.flushOutput()
                ser.close()
            except Exception:
                pass

            print(bcolors.OKBLUE + "\nSerial port closed. Device is now in deep sleep." + bcolors.ENDC)
            print("To wake it, press the external reset button.\n")
            exit(0)

    if key == '5':  # Test mode using Plotly Dash
        # Send test commands
        send_command(ser, b'AT+TEST=2\n', b'OK')
        while True:
            line = ser.readline()
            decoded_line = line.decode('utf-8')
            if decoded_line.strip():  # Check if there's any non-whitespace content
                print(decoded_line, end='')
        return True

    if key == '6':  # Get Device status
        send_command(ser, b'AT+TEST=4\n', b'OK')
        return True

    if key == '7':  # Get Device status
        send_command(ser, b'AT+TEST=5\n', b'OK')
        return True

    if key == '8':  # Test mode using Plotly Dash
        # Send test commands
        send_command(ser, b'AT+TEST=6\n', b'OK')
        try:
            while True:
                line = ser.readline()
                decoded_line = line.decode('utf-8')
                if decoded_line.strip():  # Check if there's any non-whitespace content
                    print(decoded_line, end='')
        except KeyboardInterrupt:
            print(bcolors.WARNING + "\nUser stopped Max-Readout mode.\n" + bcolors.ENDC)
            ser.close()
            exit(0)

    if key == '9':  # GETFILE test
        print("\n--- GETFILE Test ---")

        # Ask user which file to request
        filename = input("Enter filename on device (default: last.n42): ").strip()
        if filename == "":
            filename = "last.n42"

        local_filename = "received_" + filename

        # Send the AT command
        cmd = f"AT+GETFILE={filename}\n".encode('utf-8')
        ser.write(cmd)
        time.sleep(0.2)

        print(f"Request sent: AT+GETFILE={filename}")
        print("Waiting for STARTFILE marker...")

        # Wait until device responds "STARTFILE="
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue
            print("Device:", line)

            if line.startswith("STARTFILE"):
                break
            if line.startswith("ERR"):
                print("\nDevice reported an error. Aborting.\n")
                break

        # Now begin receiving file
        print(f"\nReceiving file and saving to {local_filename}...\n")

        with open(local_filename, "wb") as outfile:
            while True:
                chunk = ser.readline()

                # Check for ENDFILE
                try:
                    decoded = chunk.decode().strip()
                    if decoded == "ENDFILE":
                        print("\n--- File transfer completed ---\n")
                        break
                except:
                    pass  # binary data can't decode -> ignore

                # Write raw binary chunk
                outfile.write(chunk)

        print(f"File saved successfully as {local_filename}\n")
        return True

    if key == 'a' or key == 'A':  # ASIC register dump
        print("Dump ASIC registers\n")
        send_command(ser, b'AT+ASIC_DUMP\n', b'OK')
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                break
            print("  ", line)
        return True

    if key == 'c' or key == 'C':  # Clean: default setup and erase storage
        print("Clean device\n")
        send_command(ser, b'AT+CONFIG_CLEAN\n', b'OK')
        return True

    if key == 'd' or key == 'D':  # Dump: get the current setup
        print("Dump settings\n")
        send_command(ser, b'AT+CONFIG_DUMP\n', b'OK')
        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                break
            print("  ", line)
        return True

    if key == 'l' or key == 'L':  # load: load setup from storage
        print("Load settings from flash\n")
        send_command(ser, b'AT+CONFIG_LOAD\n', b'OK')
        return True

    if key == 'n' or key == 'N':  # Next channel
        next_channel()
        return True

    if key == 'h' or key == 'H' or key == '?':  # Help: display list of commands
        print_help()
        return True

    if key == 'q' or key == 'Q':  # Quit
        print(bcolors.BOLD + "\nInterface Terminated\n" + bcolors.ENDC)
        ser.close()
        exit(1)

    if key == 's' or key == 'S':  # Save: save current setup to storage
        print("Save settings to flash\n")
        send_command(ser, b'AT+CONFIG_SAVE\n', b'OK')
        return True

    if key == 't':  # Debug test mode
        pass

    if key == 'u' or key == 'U':
        set_output_format(3)
        return True

    if key == 'v' or key == 'V':
        set_output_format(4)
        return True

    if key == 'w' or key == 'W':
        set_output_format(1)
        return True

    if key == 'x' or key == 'X':
        test_sequence_X()
        return True

    if key == 'y' or key == 'Y':
        test_sequence_Y()
        return True

    if key == ' ':
        toggle_printing()
        return True

    return False



# Thread function for reading serial data, updating the accumulator and logging CSV,
# and printing temperature and Vbias information.
def serial_read_thread(ser, accum_data, data_lock, csv_writer, stop_event):
    while not stop_event.is_set():
        data, exit_flag = IDE3380_serial_loop(ser)
        if exit_flag:
            break
        if data:
            new_data = np.array(data[0:4096])
            with data_lock:
                accum_data += new_data
                current_total = accum_data.sum()
            csv_writer.writerow(data)

            # Compute and print temperature and Vbias info.
            # data[4096] is Vbias, data[4097] is temperature.
            temperature = float(data[4097]) / 100
            Vbias = float(data[4096]) / 1000
            Vbias_setpoint = -40.7 - (0.034 * (temperature - 25))
            Vbias_offset = (Vbias_setpoint - Vbias)
            print("Total accumulated:", current_total)
            print("Temperature: %0.2f and Vbias: %0.3f | Setpoint: %0.3f | Vbias_offset: %0.3f" %
                  (temperature, Vbias, Vbias_setpoint, Vbias_offset))


if __name__ == '__main__':
    print("\nInterface Started")
    parser = argparse.ArgumentParser()
    parser.add_argument("com_port", help="COM port of the device")
    args = parser.parse_args()

    ser = serial.Serial(args.com_port, 115200, timeout=2)
    print("Serial port is open\n")
    print_help()

    try:
        while True:
            key = readchar.readkey()
            if not command_handler(key):
                print_help()

    except serial.SerialException:
        print(bcolors.FAIL + "\nDevice disconnected or serial port lost." + bcolors.ENDC)
        try:
            ser.close()
        except Exception:
            pass
        exit(2)
    except KeyboardInterrupt:
        print(bcolors.WARNING + "\nUser interrupted program.\n" + bcolors.ENDC)
        try:
            ser.close()
        except Exception:
            pass
        exit(0)
