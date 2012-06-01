#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: break.py

# break语句是用来 终止 循环语句的, 即哪怕循环条件没有称为False或序列还没有被完
# 全递归, 也停止执行循环语句.

# NOTES: 一个重要的注释是, 如果你从for或while循环中 终止 , 任何对应的循环else
# 块将不执行!!!!!

while True:
	s=raw_input('Enter something : ')
	if s=='quit':
		break
	print 'Length of the string is',len(s)
print 'Done'

