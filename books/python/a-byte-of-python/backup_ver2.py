#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: backup_ver2.py


# 版本二:
# 采用更好的文件名机制--使用 时间 作为文件名, 而当前的 日期 作为目录名, 存放在
# 主备份目录中.


import os
import time

# 1. The files and directories to be backed up are specified in a list.
source=['/home/scr/audio','/home/scr/bin']
# If you are using Windows, use source=[r'C:\Documents',r'D:\Work'] or something like that

# 2. The backup must be stored in a main backup directory
target_dir='/home/scr/e/backup/' #Remember to change this to what you will be using

# 3. The files are backed up into a zip file
# 4. The current day is the name of the subdirectory in the main directory
today=target_dir+time.strftime('%Y-%m-%d')
# The current time is the name of the zip archive
now=time.strftime('%H:%M:%S')

# Create the subdirectory if it isn't already there
if not os.path.exists(today):
	os.mkdir(today) # make directory
	print 'Successfully created directory',today

# The name of the zip file
# 注意os.sep变量的用法--这会根据你的操作系统给出目录分隔符, 即在Linux,Unix下它
# 是'/', 在Windows下它是'\\', 而在Mac OS下它是':'. 使用os.sep而非直接使用字符,
# 会使我们的程序具有移植性, 可以在上述这些系统下工作. 
target=today+os.sep+now+'.zip'

# 5. We use the zip command (in Unix/Linux) to put the files in a zip archive
zip_command="zip -qr '%s' %s" %(target,' '.join(source))

# Run the backup
if os.system(zip_command)==0:
	print 'Successful backup to',target
else:
	print 'Backup FAILED'

