#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: backup_ver1.py

# 一个可以为我的所有重要文件创建备份的程序


import os
import time

# 1. The files and directories to be backed up are specified in a list.
source=['/home/scr/audio','/home/scr/bin']
# If you are using Windows, use source=[r'C:\Documents',r'D:\Work'] or something like that

# 2. The backup must be stored in a main backup directory
target_dir='/home/scr/e/backup/' #Remember to change this to what you will be using

# 3. The files are backed up into a zip file
# 4. The name of the zip archive is the current date and time
target=target_dir+time.strftime('%Y-%m-%dT%H:%M:%S')+'.zip'

# 5. We use the zip command (in Unix/Linux) to put the files in a zip archive
# str类的join()方法, ' '.join()
zip_command="zip -qr '%s' %s" %(target,' '.join(source))

# Run the backup
if os.system(zip_command)==0:
	print 'Successful backup to',target
else:
	print 'Backup FAILED'

