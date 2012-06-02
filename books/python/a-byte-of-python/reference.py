#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: reference.py


# 参考:
# 当你创建一个对象并给它赋一个变量的时候, 这个变量仅仅 参考 那个对象, 而不是表
# 示这个对象本身!也就是说, 变量名指向你计算机中存储那个对象的内存. 这被称作名
# 称到对象的绑定. 

# 记住, 列表的赋值语句不创建拷贝. 你得使用切片操作符来建立序列的拷贝.  


print 'Simple Assignment'
shoplist=['apple','mango','carrot','banana']
mylist=shoplist # mylist is just another name pointing to the same object!

del shoplist[0]

print 'shoplist is',shoplist
print 'mylist is',mylist
# notice that both shoplist and mylist both print the same list without
# the 'apple' confirming that they point to the same object

print 'Copy by making a full slice'
mylist=shoplist[:] # make a copy by doing a full slice
del mylist[0] # remove first item

print 'shoplist is', shoplist
print 'mylist is', mylist
# notice that now the two lists are different

