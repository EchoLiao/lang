#!/usr/bin/env python
#coding=utf-8
from django.db import models

from django.contrib.auth.admin import User
from datetime import datetime
from django.db.models import permalink
from django.contrib.sitemaps import ping_google

from mptt.models import MPTTModel

class Category(models.Model):
    name = models.CharField(max_length=50, unique=True)
    slug = models.SlugField()
    order = models.IntegerField(blank=True, null=True)
    
    class Meta:
        verbose_name = "分类"
        verbose_name_plural = "分类"
        ordering = ['order',]
        
    def __unicode__(self):
        return self.name
    
    @permalink
    def get_absolute_url(self):
        return ('blog-category', None, {'slug': self.slug})
    
    def save(self):
        if self.order is None:
            cates = Category.objects.all()
            if cates:
                max_order = cates.order_by('-order')[0]
                self.order = max_order.order + 1
            else:
                self.order = 1
            
        super(Category, self).save()
        
class Tag(models.Model):
    name = models.CharField(max_length=50, unique=True)
    slug = models.SlugField()
    
    articles = models.ManyToManyField("Article", through="ArticleTag")
    
    class Meta:
        verbose_name = "标签"
        verbose_name_plural = "标签"
        ordering = ['?']
        
    def __unicode__(self):
        return self.name
    
    @permalink
    def get_absolute_url(self):
        return ('blog-tag', None, {'slug': self.slug})
    
class Article(models.Model):
    STATUS_CHOICE = (
        (1, '编辑'),
        (2, '完成'),
        (3, '失效'),
    )
    
    title = models.CharField(max_length=100, verbose_name='标题')
    slug = models.SlugField(max_length=100)
    content = models.TextField(verbose_name='内容')
    status = models.IntegerField(choices=STATUS_CHOICE, default=1, verbose_name='状态')
    created = models.DateTimeField(auto_now_add=True, verbose_name='创建时间')
    modified = models.DateTimeField(default=datetime.now, verbose_name='修改时间')
    is_always_above = models.BooleanField(default=False, verbose_name='置顶')
    
    clicks = models.IntegerField(default=0, editable=False, verbose_name='点击次数')
    
    category = models.ForeignKey(Category, verbose_name='分类')
    author = models.ForeignKey(User, verbose_name='作者')
    
    tags = models.ManyToManyField(Tag, through="ArticleTag", verbose_name='标签')
    
    class Meta:
        verbose_name = "文章"
        verbose_name_plural = "文章"
        ordering = ['-is_always_above', '-created']
        
    def save(self):
        self.modified = datetime.now()
        super(Article, self).save()
        
        #try:
        ping_google()
        #except Exception:
        #    pass
        
    def click_once(self):
        self.clicks += 1
        super(Article, self).save()
        
    def __unicode__(self):
        return self.title
    
    @permalink
    def get_absolute_url(self):
        return ('blog-article', None, {'slug': self.slug})
    
class ArticleTag(models.Model):
    article = models.ForeignKey(Article)
    tag = models.ForeignKey(Tag)
    
    class Meta:
        verbose_name = "文章标签"
        verbose_name_plural = "文章标签"
        
    def __unicode__(self):
        return unicode(self.tag)