#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: class_init.py

# __init__方法在类的一个对象被建立时, 马上运行. 这个方法可以用来对你的对象做一
# 些你希望的 初始化 . 注意, 这个名称的开始和结尾都是双下划线. 


class Person:
	def __init__(self,name):
		self.name=name      # 声明并初始化name!
	def sayHi(self):
		print 'Hello, my name is',self.name

p=Person('Swaroop')
p.sayHi()

# This short example can also be written as Person('Swaroop').sayHi()

