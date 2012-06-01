#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_default.py


# 可以在函数定义的形参名后加上赋值运算符(=)和默认值, 从而给形参指定默认参数值.

# 只有在形参表末尾的那些参数可以有默认参数值, 即你不能在声明函数形参的时候, 先
# 声明有默认值的形参而后声明没有默认值的形参. 
# 例如, def func(a, b=5)是有效的, 但是def func(a=5, b)是 无效 的. 

def say(message,times=1):
	print message*times

say('Hello')
say('World',5)

