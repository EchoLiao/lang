#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: try_except.py


# try..except

# 我们把所有可能引发错误的语句放在try块中, 然后在except从句/块中处理所有的错误
# 和异常. except从句可以专门处理单一的错误或异常, 或者一组包括在圆括号内的错误
# /异常. 如果没有给出错误或异常的名称, 它会处理 所有的 错误和异常. 对于每个try
# 从句, 至少都有一个相关联的except从句. 

# 如果某个错误或异常没有被处理, 默认的Python处理器就会被调用. 它会终止程序的运
# 行, 并且打印一个消息, 我们已经看到了这样的处理. 

# 你还可以让try..catch块关联上一个else从句. 当没有异常发生的时候, else从句将被
# 执行. 


import sys

try:
	s=raw_input('Enter something --> ')
except EOFError:
	print '\nWhy did you do an EOF on me?'
	sys.exit() # exit the program
except:
	print '\nSome error/exception occurred.'
	# here, we are not exiting the program

print 'Done'

