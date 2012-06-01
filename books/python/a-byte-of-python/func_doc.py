#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_doc.py


# 在 函数 的第一个逻辑行的字符串是这个函数的 文档字符串.
# 注意, DocStrings也适用于模块和类, 我们会在后面相应的章节学习它们.

# 文档字符串的惯例是一个多行字符串, 它的首行以大写字母开始, 句号结尾. 第二行是
# 空行, 从第三行开始是详细的描述.

# 使用__doc__调用文档字符串属性.

def printMax(x,y):
	'''Prints the maximum of two numbers.

	The two values must be integers.'''
	x=int(x) # convert to integers, if possible
	y=int(y)

	if x>y:
		print x,'is maximum'
	else:
		print y,'is maximum'

printMax(3,5)
print printMax.__doc__

