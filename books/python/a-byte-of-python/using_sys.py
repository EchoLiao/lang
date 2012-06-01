#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_sys.py



# 为了在其他程序中重用模块, 模块的文件名必须以.py为扩展名.

# sys.path包含输入模块的目录名列表. 我们可以观察到sys.path的第一个字符串是空的
# --这个空的字符串表示当前目录也是sys.path的一部分, 这与PYTHONPATH环境变量是相
# 同的. 这意味着你可以直接输入位于当前目录的模块. 否则, 你得把你的模块放在
# sys.path所列的目录之一. 



import sys

print 'The command line arguments are:'
for i in sys.argv:
	print i

print '\n\nThe PYTHONPATH is',sys.path,'\n'

