#!/usr/bin/python
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

def test_general_cmds(serial_port):
  # Test 'ping'
  cmd = b"\xBD\x80\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b""), f"Test 'ping': FAILED, rsp: {rsp}"
  # assert (rsp == b"\xA0\x01\x80\xCC"), f"Test 'ping': FAILED, rsp: {rsp}"
  print("Test 'ping': PASSED")

  # Test 'init'
  cmd = b"\xBD\x02\x01\x01\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b"\xA0\x01\x00\xCC"), f"Test 'init': FAILED, rsp: {rsp}"
  print("Test 'init': PASSED")

  # Test 'init with data'
  cmd = b"\xBD\x02\x01\x01\x00\x00\x00\x00\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b"\xA0\x01\x00\xCC"), f"Test 'init with data': FAILED, rsp: {rsp}"
  print("Test 'init with data': PASSED")

  # Test 'shutdown'
  cmd = b"\xBD\x02\x01\x02\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b"\xA0\x01\x00\xCC"), f"Test 'shutdown': FAILED, rsp: {rsp}"
  print("Test 'shutdown': PASSED")

  # Test 'description'
  cmd = b"\xBD\x02\x01\x03\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(25) # Should be only four bytes
  assert (rsp == b"\xA0\x0c\x10\x10SD-PHEAD-1\xCC"), f"Test 'description': FAILED, rsp: {rsp}"
  print("Test 'description': PASSED")

  # Test 'state_led'
  cmd = b"\xBD\x02\x01\x04\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b"\xA0\x01\x00\xCC"), f"Test 'state_led': FAILED, rsp: {rsp}"
  print("Test 'state_led': PASSED")

  # Test 'bad command'
  cmd = b"\xBD\x02\x01\x05\xCC"
  serial_port.write(cmd)
  rsp = serial_port.read(4) # Should be only four bytes
  assert (rsp == b"\xA1\x01\x01\xCC"), f"Test 'bad command': FAILED, rsp: {rsp}"
  print("Test 'bad command': PASSED")

def test_calibration_cmds(serial_port):
  pass

def test_battery_cmds(serial_port):
  pass

def test_print_cmds(serial_port):
  pass

def test_temperature_cmds(serial_port):
  pass

def test_uv_light_cmds(serial_port):
  pass

#
# Main
#########################################################################

def main():
  baud = 115200 # Baud rate
  timeout = 5 # 5 second(s)
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