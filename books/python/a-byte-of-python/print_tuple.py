#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: print_tuple.py


# 元组和列表十分类似, 只不过元组和字符串一样是 不可变的 即你不能修改元组. 元组
# 通过圆括号中用逗号分割的项目定义. 元组通常用在使语句或用户定义的函数能够安全
# 地采用一组值的时候, 即被使用的元组的值不会改变. 

# 含有0个或1个项目的元组:
# 一个空的元组由一对空的圆括号组成, 如myempty = (). 然而, 含有单个元素的元组必
# 须写成 singleton = (2, ); 写成 singleton = (2) 是错误的!!!!!



age=22
name='Swaroop'

# 在第二个print语句中, 我们使用了一个定制, 后面跟着%符号后的单个项目--没有圆括
# 号. 这只在字符串中只有一个定制的时候有效. 
print '%s is %d years old' %(name,age)
print 'Why is %s playing with that python?' %name



