#!/usr/bin/python3
'''
Copyright (c) 2019-2020 Ricardo Tonet
Use of this source code is governed by the MIT license, see LICENSE
'''

'''
# \file t_protocol.py
# \brief Python script to test communication protocol via UART
'''

import bluetooth
import sys

#
# General functions
#########################################################################

def test_cmd(bth_sock, test_name, cmd, rsp_cmd, rsp_cmd_size):
  max_tries = 5
  bth_sock.send(cmd)
  rsp = b""
  while len(rsp) < rsp_cmd_size:
    rsp += bth_sock.recv(rsp_cmd_size)
    
  # t = 0
  # while (rsp == b"" and t < max_tries):
  #   bth_sock.send(cmd)
  #   rsp = bth_sock.recv(rsp_cmd_size)
  #   t += 1

  assert (rsp == rsp_cmd), f"Test '{test_name}': FAILED, rsp: {rsp}"
  print(f"Test '{test_name}': PASSED")

def test_general_cmds(bth_sock):
  print("Testing general commands...")

  # Test 'ping'
  test_cmd(bth_sock, "ping", b"\xBD\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'init'
  test_cmd(bth_sock, "init", b"\xBD\x02\x01\x01\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'init with data'
  test_cmd(bth_sock, "init with data", b"\xBD\x06\x01\x01\x00\x00\x00\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'shutdown'
  test_cmd(bth_sock, "shutdown", b"\xBD\x02\x01\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'description'
  test_cmd(bth_sock, "description", b"\xBD\x02\x01\x03\xCC", b"\xA0\x0c\x10\x10SD-PHEAD-1\xCC", 15)

  # Test 'state_led'
  test_cmd(bth_sock, "state_led", b"\xBD\x02\x01\x04\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'general bad command'
  test_cmd(bth_sock, "general bad command", b"\xBD\x02\x01\x05\xCC", b"\xA1\x01\x01\xCC", 4)

def test_calibration_cmds(bth_sock):
  print("\nTesting calibration commands...")

  # Test 'calibration'
  test_cmd(bth_sock, "calibration", b"\xBD\x03\x02\x01\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'calibration invalid volume'
  test_cmd(bth_sock, "calibration invalid volume", b"\xBD\x03\x02\x01\x64\xCC", b"\xA1\x01\x07\xCC", 4)

  # Test 'calibration invalid cmd missing volume'
  test_cmd(bth_sock, "calibration invalid cmd missing volume", b"\xBD\x02\x02\x01\xCC", b"\xA1\x01\x01\xCC", 4)

  # Test 'calibration bad command'
  test_cmd(bth_sock, "calibration bad command", b"\xBD\x03\x02\x05\x63\xCC", b"\xA1\x01\x01\xCC", 4)

def test_battery_cmds(bth_sock):
  print("\nTesting battery commands...")

  # Test 'read battery' - Needs to be in battery mode
  # test_cmd(bth_sock, "read_battery", b"\xBD\x02\x03\x01\xCC", b"\xA0\x02\x00\x00\xCC", 5)

  # Test 'read battery no bat' - Plug mode
  test_cmd(bth_sock, "read_battery error no bat", b"\xBD\x02\x03\x01\xCC", b"\xA1\x01\x04\xCC", 4)

  # Test 'battery bad command' - Needs to be in battery mode
  test_cmd(bth_sock, "battery bad command", b"\xBD\x02\x03\x02\xCC", b"\xA1\x01\x01\xCC", 4)

def test_print_cmds(bth_sock):
  print("\nTesting print commands...")

  # Test 'start_print'
  test_cmd(bth_sock, "start_print", b"\xBD\x04\x04\x01\xFF\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'stop_print'
  test_cmd(bth_sock, "stop_print", b"\xBD\x02\x04\x02\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'set_speed'
  test_cmd(bth_sock, "set_speed", b"\xBD\x04\x04\x03\xFF\x00\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'set_speed wrong len'
  test_cmd(bth_sock, "set_speed wrong len", b"\xBD\x02\x04\x03\xFF\x00\xCC", b"\xA1\x01\x01\xCC", 4)

  # Test 'refill'
  test_cmd(bth_sock, "refill", b"\xBD\x02\x04\x04\xCC", b"\xA0\x01\x00\xCC", 4)

  # Test 'print bad command'
  test_cmd(bth_sock, "print bad command", b"\xBD\x02\x04\x05\xCC", b"\xA1\x01\x01\xCC", 4)

def test_temperature_cmds(bth_sock):
  print("\nTesting temperature commands...")

  # Test 'temperature disabled' - Temperature needs to be disabled
  test_cmd(bth_sock, "temperature disabled", b"\xBD\x02\x05\x01\xCC", b"\xA1\x01\x05\xCC", 4)

  # Test 'temperature bad command' - Temperature needs to be available
  # test_cmd(bth_sock, "temperature bad command", b"\xBD\x02\x05\x03\xCC", b"\xA1\x01\x01\xCC", 4)

def test_uv_light_cmds(bth_sock):
  print("\nTesting uv lights commands...")

  # Test 'uv lights disabled' - UV lights needs to be disabled
  test_cmd(bth_sock, "uv lights disabled", b"\xBD\x02\x06\x01\xCC", b"\xA1\x01\x06\xCC", 4)

  # Test 'uv lights bad command' - UV lights needs to be available
  # test_cmd(bth_sock, "uv lights bad command", b"\xBD\x02\x06\x05\xCC", b"\xA1\x01\x01\xCC", 4)

#
# Main
#########################################################################

def main():
  host = "98:D3:31:FC:23:18"
  port = 1
  sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
  sock.connect((host, port))
  sock.settimeout(5)

  try:
    test_general_cmds(sock)
    test_calibration_cmds(sock)
    test_battery_cmds(sock)
    test_print_cmds(sock)
    test_temperature_cmds(sock)
    test_uv_light_cmds(sock)
  except AssertionError as e:
    print(e)
  finally:
    sock.close()


if __name__ == "__main__":
    main()
    sys.exit()