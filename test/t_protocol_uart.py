#!/usr/bin/python3
'''
Copyright (c) 2019-2020 Ricardo Tonet
Use of this source code is governed by the MIT license, see LICENSE
'''

'''
# \file t_protocol.py
# \brief Python script to test communication protocol via UART
'''

import serial
import sys

#
# General functions
#########################################################################

def test_cmd(serial_port, test_name, cmd, rsp_cmd, rsp_cmd_size):
  max_tries = 5
  serial_port.write(cmd)
  rsp = serial_port.read(rsp_cmd_size)
  t = 0
  while (rsp == b"" and t < max_tries):
    serial_port.write(cmd)
    rsp = serial_port.read(rsp_cmd_size)
    t += 1

  assert (rsp == rsp_cmd), f"Test '{test_name}': FAILED, rsp: {rsp}"
  print(f"Test '{test_name}': PASSED")

def test_general_cmds(serial_port):
  print("Testing general commands...")

  # Test 'ping'
  test_cmd(serial_port, "ping", b"\xBD\x00\xCC", b"\xA0\x01\x00\xCC", 10)

  # Test 'init'
  test_cmd(serial_port, "init", b"\xBD\x02\x01\x01\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'init with data'
  test_cmd(serial_port, "init with data", b"\xBD\x06\x01\x01\x00\x00\x00\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'shutdown'
  test_cmd(serial_port, "shutdown", b"\xBD\x02\x01\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'description'
  test_cmd(serial_port, "description", b"\xBD\x02\x01\x03\xCC", b"\xA0\x0c\x10\x10SD-PHEAD-1\xCC", 22)

  # Test 'state_led'
  test_cmd(serial_port, "state_led", b"\xBD\x02\x01\x04\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'general bad command'
  test_cmd(serial_port, "general bad command", b"\xBD\x02\x01\x05\xCC", b"\xA1\x01\x01\xCC", 4)

def test_calibration_cmds(serial_port):
  print("\nTesting calibration commands...")

  # Test 'calibration'
  test_cmd(serial_port, "calibration", b"\xBD\x03\x02\x01\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'calibration invalid volume'
  test_cmd(serial_port, "calibration invalid volume", b"\xBD\x03\x02\x01\x64\xCC", b"\xA1\x01\x07\xCC", 4)

  # Test 'calibration invalid cmd missing volume'
  test_cmd(serial_port, "calibration invalid cmd missing volume", b"\xBD\x02\x02\x01\xCC", b"\xA1\x01\x01\xCC", 4)

  # Test 'calibration bad command'
  test_cmd(serial_port, "calibration bad command", b"\xBD\x03\x02\x05\x63\xCC", b"\xA1\x01\x01\xCC", 4)

def test_battery_cmds(serial_port):
  print("\nTesting battery commands...")

  # Test 'read battery' - Needs to be in battery mode
  # test_cmd(serial_port, "read_battery", b"\xBD\x02\x03\x01\xCC", b"\xA0\x02\x00\x00\xCC", 5)

  # Test 'read battery no bat' - Plug mode
  test_cmd(serial_port, "read_battery error no bat", b"\xBD\x02\x03\x01\xCC", b"\xA1\x01\x04\xCC", 4)

  # Test 'battery bad command' - Needs to be in battery mode
  test_cmd(serial_port, "battery bad command", b"\xBD\x02\x03\x02\xCC", b"\xA1\x01\x01\xCC", 4)

def test_print_cmds(serial_port):
  print("\nTesting print commands...")

  # Test 'start_print'
  test_cmd(serial_port, "start_print", b"\xBD\x04\x04\x01\xFF\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'stop_print'
  test_cmd(serial_port, "stop_print", b"\xBD\x02\x04\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'set_speed'
  test_cmd(serial_port, "set_speed", b"\xBD\x04\x04\x03\xFF\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'set_speed wrong len'
  test_cmd(serial_port, "set_speed wrong len", b"\xBD\x02\x04\x03\xFF\x00\xCC", b"\xA1\x01\x01\xCC", 4)

  # Test 'refill'
  test_cmd(serial_port, "refill", b"\xBD\x02\x04\x04\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'print bad command'
  test_cmd(serial_port, "print bad command", b"\xBD\x02\x04\x05\xCC", b"\xA1\x01\x01\xCC", 4)

def test_temperature_cmds(serial_port):
  print("\nTesting temperature commands...")

  # Test 'temperature disabled' - Temperature needs to be disabled
  test_cmd(serial_port, "temperature disabled", b"\xBD\x02\x05\x01\xCC", b"\xA1\x01\x05\xCC", 4)

  # Test 'temperature bad command' - Temperature needs to be available
  # test_cmd(serial_port, "temperature bad command", b"\xBD\x02\x05\x03\xCC", b"\xA1\x01\x01\xCC", 4)

def test_uv_light_cmds(serial_port):
  print("\nTesting uv lights commands...")

  # Test 'uv lights disabled' - UV lights needs to be disabled
  test_cmd(serial_port, "uv lights disabled", b"\xBD\x02\x06\x01\xCC", b"\xA1\x01\x06\xCC", 4)

  # Test 'uv lights bad command' - UV lights needs to be available
  # test_cmd(serial_port, "uv lights bad command", b"\xBD\x02\x06\x05\xCC", b"\xA1\x01\x01\xCC", 4)

#
# Main
#########################################################################

def main():
  baud = 115200 # Baud rate
  timeout = 3 # 3 second(s)
  s = serial.Serial('/dev/ttyUSB0', baud, timeout=timeout)  # open serial port

  try:
    test_general_cmds(s)
    test_calibration_cmds(s)
    test_battery_cmds(s)
    test_print_cmds(s)
    test_temperature_cmds(s)
    test_uv_light_cmds(s)
  except AssertionError as e:
    print(e)
  finally:
    s.close()


if __name__ == "__main__":
    main()
    sys.exit()