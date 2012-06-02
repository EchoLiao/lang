#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_name.py


# 每个Python模块都有它的__name__, 如果它是'__main__', 这说明这个模块被用户单独
# 运行.

if __name__=='__main__':
    # 在程序本身被使用的时候运行主块,
	print 'This program is being run by itself'
else:
    # 而在它被别的模块输入的时候不运行主块.
	print 'I am being imported from another module'


## $ python using_name.py
## This program is being run by itself
## 
## $ python
## >>> import using_name
## I am being imported from another module
## >>>

