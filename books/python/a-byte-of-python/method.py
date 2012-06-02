#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: method.py


# 对象的方法:
# 我们已经讨论了类/对象可以拥有像函数一样的方法, 这些方法与函数的区别只是一个
# 额外的self变量. 现在我们来学习一个例子. 


class Person:
	def sayHi(self):
		print 'Hello, how are you?'

p=Person()
p.sayHi()

# This short example can also be written as Person().sayHi()

