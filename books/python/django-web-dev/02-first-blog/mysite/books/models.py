# -*- coding: utf-8 -*-

from django.db import models
from django.contrib import admin


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

    def __str__(self):
        return self.name

    class Meta:
        ordering = ["name"]



class Author(models.Model):
    salutation = models.CharField(max_length=10)
    first_name = models.CharField(max_length=30)
    last_name = models.CharField(max_length=40)
    email = models.EmailField()
    headshot = models.ImageField(upload_to='/tmp')
    last_accessed = models.DateField()

    def __str__(self):
        return '%s %s' % (self.first_name, self.last_name)



class Book(models.Model):
    title = models.CharField(max_length=100)
    authors = models.ManyToManyField(Author)
    publisher = models.ForeignKey(Publisher)
    publication_date = models.DateField()
    num_pages = models.IntegerField(blank=True, null=True)

    def __str__(self):
        return self.title



class BookAdmin(admin.ModelAdmin):
    list_display = ('title', 'publisher', 'publication_date', 'num_pages',)
    # list_filter 选项在右边创建一个过滤条. 我们允许它按日期过滤(它可以让你只
    # 显示过去一周, 一个月等等出版的书籍)和按出版商过滤. 
    list_filter = ('publisher', 'publication_date')
    ordering = ('-publication_date',)
    search_fields = ('title',)




admin.site.register(Publisher)
admin.site.register(Author)
admin.site.register(Book, BookAdmin)

