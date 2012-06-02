#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: seq.py


# 序列:
# 列表,元组和字符串都是序列, 但是序列是什么, 序列的两个主要特点是索引操作符和
# 切片操作符. 索引操作符让我们可以从序列中抓取一个特定项目. 切片操作符让我们能
# 够获取序列的一个切片, 即一部分序列. 

# 索引操作符:
# 索引同样可以是负数, 在那样的情况下, 位置是从序列尾开始计算的. 因此,
# shoplist[-1]表示序列的最后一个元素而shoplist[-2]抓取序列的倒数第二个项目. 

# 切片操作符:
# 切片操作符是序列名后跟一个方括号, 方括号中有一对可选的数字, 并用冒号分割. 记
# 住数是可选的, 而冒号是必须的. 

# 切片操作符中的第一个数(冒号之前)表示切片开始的位置, 第二个数(冒号之后)表示切
# 片到哪里结束. 如果不指定第一个数, Python就从序列首开始. 如果没有指定第二个数
# , 则Python会停止在序列尾. 注意, 返回的序列从开始位置 开始 , 刚好在 结束 位置
# 之前结束. 即开始位置是包含在序列切片中的, 而结束位置被排斥在切片外. 
# 这样, shoplist[1:3]返回从位置1开始, 包括位置2, 但是停止在位置3的一个序列切片
# , 因此返回一个含有两个项目的切片. 类似地, shoplist[:]返回整个序列的拷贝. 

# 可以用负数做切片. 负数用在从序列尾开始计算的位置. 例如, shoplist[:-1]会返回
# 除了最后一个项目外包含所有项目的序列切片. 


shoplist=['apple','mango','carrot','banana']

# Indexing or 'Subscription' operation
print 'Item 0 is', shoplist[0]
print 'Item 1 is', shoplist[1]
print 'Item 2 is', shoplist[2]
print 'Item 3 is', shoplist[3]
print 'Item -1 is', shoplist[-1]
print 'Item -2 is', shoplist[-2]

# Slicing on a list
print 'Item 1 to 3 is', shoplist[1:3]
print 'Item 2 to end is', shoplist[2:]
print 'Item 1 to -1 is', shoplist[1:-1]
print 'Item start to end is', shoplist[:]

# Slicing on a string
name='swaroop'
print 'characters 1 to 3 is', name[1:3]
print 'characters 2 to end is', name[2:]
print 'characters 1 to -1 is', name[1:-1]
print 'characters start to end is', name[:]

