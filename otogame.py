# coding: utf-8

import RPi.GPIO as GPIO
import time
import signal, os
import sys

def handler(signum,frame):
    print('\n cleanup \n')
    myShiftOut(SER, SRCLK, 0x0000, 16)
    GPIO.cleanup()
    sys.exit(0)

signal.signal(signal.SIGINT, handler)

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

SRCLK = 16
RCLK = 20
SER = 21
Button_judge = 19
speaker = 26

n_s = 0


GPIO.setup(SRCLK,GPIO.OUT)
GPIO.setup(RCLK,GPIO.OUT)
GPIO.setup(SER,GPIO.OUT)
GPIO.setup(Button_judge,GPIO.IN)
GPIO.setup(speaker, GPIO.OUT, initial = GPIO.LOW)

PATTERNS = [
    0x8889,
    0x4445,
    0x2223,
    0x1111,
]


def callback(channel):
    judge()
    
GPIO.add_event_detect(Button_judge,GPIO.FALLING, callback=callback, bouncetime=200)

#Button_judgeによって判定
def judge():
    global n_s
    data_len = len(PATTERNS)
    num_judge = n_s % data_len
    if num_judge == data_len-1:
        print('success')
    else:
        print('miss')

def myShiftOut(dataPin,clockPin,data,dataLength):
    for i in range(dataLength):
        GPIO.output(dataPin, (data & 1 << i))
        GPIO.output(clockPin,GPIO.HIGH)
        GPIO.output(clockPin,GPIO.LOW)

def data_out():
    global n_s
    data_len = len(PATTERNS)
    sft = n_s % data_len
    myShiftOut(SER, SRCLK, PATTERNS[sft], 16)
    n_s += 1

def act_speaker(spk):
    global n_s
    data_len = len(PATTERNS)
    num_speak = n_s % data_len
    Hz = 100
    p = GPIO.PWM(spk, 1)
    
    p.start(50)
    if num_speak == data_len-1:
        Hz = 400

    p.ChangeFrequency(Hz)
    time.sleep(0.2)

    
    p.stop()

def main():
    while True:
        act_speaker(speaker)

        GPIO.output(RCLK,GPIO.LOW)

        data_out()

        GPIO.output(RCLK,GPIO.HIGH)


        time.sleep(0.5)

main()
