import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(38, GPIO.IN)

#Switch on
#GPIO.output(38, GPIO.HIGH)

#To Read state
state = GPIO.input(38)

if state:
    print('on')

else:
    print('off')
