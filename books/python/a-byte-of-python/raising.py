#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: raising.py


# 引发异常:
# 你可以使用raise语句 引发 异常. 你还得指明错误/异常的名称和伴随异常 触发的 异
# 常对象. 你可以引发的错误或异常应该分别是一个Error或Exception类的直接或间接导
# 出类. 


class ShortInputException(Exception):
	'''A user-defined exception class.'''
	def __init__(self,length,atleast):
		Exception.__init__(self)
		self.length=length
		self.atleast=atleast

try:
	s=raw_input('Enter something --> ')
	if len(s)<3:
		raise ShortInputException(len(s),3)
	# Other work can continue as usual here
except EOFError:
	print '\nWhy did you do an EOF on me?'
except ShortInputException,x:
	print 'ShortInputException: The input was of length %d, \
was expecting at least %d' %(x.length,x.atleast)
else:
	print 'No exception was raised.'

