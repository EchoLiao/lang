#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: mymodule_demo.py


# 使用自己创建的模块. 

# 'mymodule'这个模块应该被放置在我们输入它的程序的同一个目录中, 或者在sys.path
# 所列目录之一. 

import mymodule

mymodule.sayhi()
print 'Version', mymodule.version

