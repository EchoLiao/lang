#!/usr/bin/env python
# Filename: list_comprehension.py


# 例如, 你有一个数的列表, 而你想要得到一个对应的列表, 使其中所有大于2的数都是
# 原来的2倍. 对于这种应用, 列表综合是最理想的方法. 


listone=[2,3,4]
listtwo=[2*i for i in listone if i>2]
print listtwo

