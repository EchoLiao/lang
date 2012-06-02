#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: str_methods.py


# 字符串:
# 字符串都是str类的对象.


name='Swaroop' # This is a string object

if name.startswith('Swa'):
	print 'Yes, the string starts with "Swa"'

if 'a' in name:
	print 'Yes, it contains the string "a"'

if name.find('war')!=-1:
	print 'Yes, it contains the string "war"'

# str类也有以一个作为分隔符的字符串join序列的项目的整洁的方法, 它返回一个生成
# 的大字符串.
delimiter=' * '     # 定义一个字符串
mylist=['Brazil','Russia','India','China']
print delimiter.join(mylist)        # 使用str类的join()方法.

