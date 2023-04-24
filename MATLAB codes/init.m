% Initialize serial connection to Arduino. 
port = "/dev/cu.usbserial-1140";     % Will vary from system to system
arduino = serialport(port,115200);  % Initialize serial connection