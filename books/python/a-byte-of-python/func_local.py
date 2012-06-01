#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_local.py


# 当你在函数定义内声明变量的时候, 它们与函数外具有相同名称的其他变量没有任何关
# 系, 即变量名称对于函数来说是 局部 的. 这称为变量的 作用域 . 所有变量的作用域
# 是它们被定义的块, 从它们的名称被定义的那点开始.

def func(x):
	print 'x is',x
	x=2
	print 'Changed local x to',x

x=50
func(x)
print 'x is still',x

