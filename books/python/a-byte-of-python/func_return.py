#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: func_return.py


# return语句用来从一个函数 返回 即跳出函数. 我们也可选从函数 返回一个值 .

def maximum(x,y):
	if x>y:
		return x
	else:
		return y

print maximum(2,3)



# 注意, 没有返回值的return语句等价于return None. None是Python中表示没有任何东
# 西的特殊类型. 例如, 如果一个变量的值为None, 可以表示它没有值. 

# 除非你提供你自己的return语句, 每个函数都在结尾暗含有return None语句

# pass语句在Python中表示一个空的语句块. 

def someFunction():
    pass

print someFunction()
