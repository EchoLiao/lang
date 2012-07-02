# -*- coding: utf-8 -*-

from django.db import models


# 我们来假定下面的这些概念,字段和关系: 
#
#   作者有尊称(例如, 先生或者女士), 姓, 名, 还有Email地址, 头像. 
#
#   出版商有名称, 地址, 所在城市,省, 国家, 网站. 
#
#   书籍有书名和出版日期. 它有一个或多个作者(和作者是多对多的关联关系
#   [many-to-many]),  只有一个出版商(和出 版商是一对多的关联关系[one-to-many], 
#   也被称作外键[foreign key])


class Publisher(models.Model):
    name = models.CharField(max_length=30)
    address = models.CharField(max_length=50)
    city = models.CharField(max_length=60)
    state_province = models.CharField(max_length=30)
    country = models.CharField(max_length=50)
    website = models.URLField()


class Author(models.Model):
    salutation = models.CharField(max_length=10)
    first_name = models.CharField(max_length=30)
    last_name = models.CharField(max_length=40)
    email = models.EmailField()
    headshot = models.ImageField(upload_to='/tmp')


class Book(models.Model):
    title = models.CharField(max_length=100)
    authors = models.ManyToManyField(Author)
    publisher = models.ForeignKey(Publisher)
    publication_date = models.DateField()

