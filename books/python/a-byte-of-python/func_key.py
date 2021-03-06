#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_key.py

# 如果你的某个函数有许多参数, 而你只想指定其中的一部分, 那么你可以通过命名来为
# 这些参数赋值--这被称作 关键参数 --我们使用名字(关键字)而不是位置(我们前面所
# 一直使用的方法)来给函数指定实参.

# 这样做有两个 优势 :
#     一, 由于我们不必担心参数的顺序, 使用函数变得更加简单了.
#     二,假设其他参数都有默认值, 我们可以只给我们想要的那些参数赋值.

def func(a,b=5,c=10):
	print 'a is',a,'and b is',b,'and c is',c

func(3,7)
func(25,c=24)
func(c=50,a=100)

