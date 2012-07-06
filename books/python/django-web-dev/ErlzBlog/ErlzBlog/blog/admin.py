#!/usr/bin/env python
#coding=utf-8
'''
Created on 2012-1-21

@author: Chine
'''

from django.contrib import admin
from django import forms

from models import *

class ArticleTagInline(admin.TabularInline):
    model = ArticleTag

class CategoryAdmin(admin.ModelAdmin):
    list_display = ('name', 'slug', 'order')

class TagAdmin(admin.ModelAdmin):
    inlines = (ArticleTagInline,)
    prepopulated_fields = {"slug": ("name",)}
    
class ArticleAdmin(admin.ModelAdmin):
    list_display = ('title', 'is_always_above', 'status', 'clicks', 'created', 'modified')
    list_filter = ('status', 'created', 'modified')
    prepopulated_fields = {"slug": ("title",)}
    search_fields = ('title', 'content')
    formfield_overrides = {
        models.TextField: {'widget': forms.Textarea(attrs={'class': 'mceEditor', 'rows': '50'})},
    }
    fieldsets = [
        ('文章编辑', {'fields': ('title', 'slug', 'content',)}),
        ('日期', {'fields': ('created', 'modified')}),
        ('信息', {'fields': ('category', 'author', 'status', 'is_always_above')}),
    ]
    readonly_fields = ("created",)
    #inlines = (ArticleTagInline, CommentInline,)
    list_per_page = 10
    ordering = ['-created']
    
    class Media:
        js = ('/static/grappelli/tinymce/jscripts/tiny_mce/tiny_mce.js',
               '/static/grappelli/tinymce_setup/tinymce_setup.js',)

admin.site.register(Article, ArticleAdmin)
admin.site.register(Category, CategoryAdmin)
admin.site.register(Tag, TagAdmin)