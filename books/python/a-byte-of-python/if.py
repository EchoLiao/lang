#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: if.py

number=23
guess=int(raw_input('Enter an integer : '))

# 注意if语句在结尾处包含一个冒号--我们通过它告诉Python下面跟着一个语句块. 

if guess==number:
	print 'Congratulations, you guessed it.' # New block starts here
	print "(but you do not win any prizes!)" # New block ends here
elif guess<number:
	print 'No, it is a little higher than that' # Another block
	# You can do whatever you want in a block ...
else:
	print 'No, it is a little lower than that'
	# you must have guess > number to reach here

print 'Done'
# This last statement is always executed, after the if statement is executed

