#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_list.py


# 列表中的项目应该包括在方括号中, 这样Python就知道你是在指明一个列表. 一旦你创
# 建了一个列表, 你可以添加,删除或是搜索列表中的项目. 由于你可以增加或删除项目,
# 我们说列表是 可变的 数据类型, 即这种类型是可以被改变的. 


# This is my shopping list
shoplist=['apple','mango','carrot','banana']

print 'I have',len(shoplist),'items to purchase.'

print 'These items are:', # Notice the comma at end of the line
for item in shoplist:
	print item,

print '\nI also have to buy rice.'
shoplist.append('rice')
print 'My shopping list is now',shoplist

print 'I will sort my list now'
shoplist.sort()
print 'Sorted shopping list is',shoplist

print 'The first item I will buy is',shoplist[0]
olditem=shoplist[0]
del shoplist[0]
print 'I bought the',olditem
print 'My shopping list is now',shoplist

