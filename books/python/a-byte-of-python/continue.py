#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: continue.py


# continue语句被用来告诉Python跳过当前循环块中的剩余语句, 然后 继续 进行
# 下一轮循环. 

while True:
	s=raw_input('Enter something : ')
	if s=='quit':
		break
	if len(s)<3:
		continue
	print 'Input is of sufficient length'
	# Do other kinds of processing here...

