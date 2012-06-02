#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_file.py


# 文件:
# 你可以通过创建一个file类的对象来打开一个文件, 分别使用file类的read,readline
# 或write方法来恰当地读写文件. 对文件的读写能力依赖于你在打开文件时指定的模式.
# 最后, 当你完成对文件的操作的时候, 你调用close方法来告诉Python我们完成了对文
# 件的使用. 


poem='''\
Programming is fun
When the work is done
if you wanna make your work also fun:
	use Python!
'''

f=file('poem.txt','w') # open for 'w'riting
f.write(poem) # write text to file
f.close() # close the file

f=file('poem.txt')
# if no mode is specified, 'r'ead mode is assumed by default
while True:
	line=f.readline()
	if len(line)==0: # Zero length indicates EOF
		break
	print line,
	# Notice comma to avoid automatic newline added by Python
f.close() # close the file

