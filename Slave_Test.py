import pigpio


# James before you run this open a terminal and run "sudo pigpiod"
# Git Test
pi = None

slave_addr = 0x08

def i2cInterrupt():
   global pi
   global slave_addr
   status, bytes_read, data = pi.bsc_i2c(slave_addr) 

   if bytes_read:
      print("Hello World. Request Recieved")

pi = pigpio.pi()
int_handler = pi.event_callback(pigpio.EVENT_BSC, i2cInterrupt)
pi.bsc_i2c(slave_addr)
