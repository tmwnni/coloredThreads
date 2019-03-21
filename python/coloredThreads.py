#!/usr/bin/env python

import time
from threading import Thread
from termcolor import colored
from random import randint

def RecursiveThing(number, color):
	if number >= 0:
		print colored('{} thread, number is {}.'.format(color, number), color)
		time.sleep(randint(0,9))
		RecursiveThing(number-1, color)
	else:
		print colored('{} thread complete!'.format(color), color)
	
colors = ['green','blue','yellow','red','cyan']
threads = []

[threads.append(Thread(target=RecursiveThing, args=(10, color))) for color in colors]
[thread.start() for thread in threads]
[thread.join() for thread in threads]

print('All threads complete!')
