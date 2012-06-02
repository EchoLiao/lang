#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Filename: using_dict.py


# 字典:
# 注意, 键必须是唯一的.
# 注意, 你只能使用不可变的对象(比如字符串)来作为字典的键.

# 键值对在字典中以这样的方式标记: d = {key1 : value1, key2 : value2 }.

# 记住字典中的键/值对是没有顺序的. 如果你想要一个特定的顺序, 那么你应该在使用前自己对它们排序.



# 'ab' is short for 'a'ddress'b'ook

ab={	'Swaroop'	: 'swaroopch@byteofpython.info',
	'Larry'		: 'larry@wall.org',
	'Matsumoto'	: 'matz@ruby-lang.org',
	'Spammer'	: 'spammer@hotmail.com'
   }

print "Swaroop's address is %s" %ab['Swaroop']

# Adding a key/value pair
ab['Guido']='guido@python.org'

# Deleting a key/value pair
del ab['Spammer']

print '\nThere are %d contacts in the address-book\n' %len(ab)
for name,address in ab.items():
	print 'Contact %s at %s' %(name,address)

if 'Guido' in ab: # OR ab.has_key('Guido')
	print "\nGuido's address is %s" %ab['Guido']

