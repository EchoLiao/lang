#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_tuple.py

# 元组和列表十分类似, 只不过元组和字符串一样是 不可变的 即你不能修改元组. 元组
# 通过圆括号中用逗号分割的项目定义. 元组通常用在使语句或用户定义的函数能够安全
# 地采用一组值的时候, 即被使用的元组的值不会改变. 

# 含有0个或1个项目的元组:
# 一个空的元组由一对空的圆括号组成, 如myempty = (). 然而, 含有单个元素的元组必
# 须写成 singleton = (2, ); 写成 singleton = (2) 是错误的!!!!!

zoo=('wolf','elephant','penguin')
print 'Number of animals in the zoo is',len(zoo)

new_zoo=('monkey','dolphin',zoo)
print 'Number of animals in the new zoo is',len(new_zoo)
print 'All animals in new zoo are',new_zoo
print 'Animals brought from old zoo are',new_zoo[2]
print 'Last animal brought from old zoo is',new_zoo[2][2]

