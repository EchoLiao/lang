#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_global.py


# 如果你想要为一个定义在函数外的变量赋值, 那么你就得告诉Python这个变量名不是局
# 部的, 而是 全局 的. 我们使用global语句完成这一功能. 没有global语句, 是不可能
# 为定义在函数外的变量赋值的.

# 你可以使用定义在函数外的变量的值(假设在函数内没有同名的变量). 然而, 我并不鼓
# 励你这样做, 并且你应该尽量避免这样做, 因为这使得程序的读者会不清楚这个变量是
# 在哪里定义的. 使用global语句可以清楚地表明变量是在外面的块定义的.

# 你可以使用同一个global语句指定多个全局变量. 例如global x, y, z. 

def func():
	global x

	print 'x is',x
	x=2
	print 'Changed local x to',x

x=50
func()
print 'Value of x is',x

