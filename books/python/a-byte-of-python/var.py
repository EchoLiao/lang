# -*- coding: utf-8 -*-

# Filename : var.py


# 记住, Python把在程序中用到的任何东西都称为 对象 . 这是从广义上说的. 因此我们
# 不会说"某某 东西 ", 我们说"某个 对象 ".

# 使用变量时只需要给它们赋一个值. 不需要声明或定义数据类型.


i=5
print i
i=i+1
print i

s='''This is a multi-line string.
This is the second line.'''
print s


#################################################################


# 在多个物理行中写一个逻辑行的例子. 它被称为明确的行连接.

s = 'This is a string. \
This continues the string.'
print s

print \
i

# 有时候, 有一种暗示的假设, 可以使你不需要使用反斜杠. 这种情况出现在逻辑行中使
# 用了圆括号,方括号或波形括号的时候. 这被称为暗示的行连接.

