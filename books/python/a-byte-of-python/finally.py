#!/usr/bin/env python
# Filename: finally.py


# try..finally
# 假如你在读一个文件的时候, 希望在无论异常发生与否的情况下都关闭文件, 该怎么做
# 呢?这可以使用finally块来完成. 注意, 在一个try块下, 你可以同时使用except从句
# 和finally块. 如果你要同时使用它们的话, 需要把一个嵌入另外一个. 


import time

try:
	f=file('poem.txt')
	while True: # our usual file-reading idiom
		line=f.readline()
		if len(line)==0:
			break
		time.sleep(2)
		print line,
finally:
	f.close()
	print 'Cleaning up...closed the file'

