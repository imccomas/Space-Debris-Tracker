import pigpio


# James before you run this open a terminal and run "sudo pigpiod"
# Git Test
pi = None

slave_addr = 0x40  #changed from 0X08 JR

def i2cInterrupt():
    global pi
    global slave_addr
    status, bytes_read, data = pi.bsc_i2c(slave_addr) 

    while bytes_read is True: #was just 'if bytes_read:'
        print("Hello World.")

pi = pigpio.pi()
int_handler = pi.event_callback(pigpio.EVENT_BSC, i2cInterrupt)
pi.bsc_i2c(slave_addr)
i2cInterrupt()
