#!/usr/bin/env python
#coding=utf-8
from datetime import datetime

from django.db import models
from django.contrib.auth.admin import User
from django.db.models import permalink
from django.contrib.sitemaps import ping_google
from django.contrib.contenttypes import generic
from django.contrib.contenttypes.models import ContentType

from mptt.models import MPTTModel
from filebrowser.fields import FileBrowseField

class Category(models.Model):
    name = models.CharField(max_length=50, unique=True, verbose_name='分类名')
    slug = models.SlugField()
    order = models.IntegerField(blank=True, null=True, verbose_name='顺序')
    
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
    name = models.CharField(max_length=50, unique=True, verbose_name='标签名')
    slug = models.SlugField()
    
    articles = models.ManyToManyField("Article", through="ArticleTag", verbose_name='文章')
    
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
    commens = generic.GenericRelation('Comment')
    
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
    
class Comment(MPTTModel):
    username = models.CharField(max_length=50, verbose_name='用户名')
    email_address = models.EmailField(verbose_name='邮箱地址')
    site = models.URLField(verify_exists=False, blank=True, verbose_name='站点')
    avatar = models.URLField(blank=True, null=True, verbose_name='头像')
    content = models.TextField(verbose_name='内容')
    post_date = models.DateTimeField(editable=False, default=datetime.now, verbose_name='评论时间')
    visible = models.BooleanField(default=True, verbose_name='是否可见')
    
    # mptt
    reply_to_comment = models.ForeignKey("self", blank=True, null=True, related_name="children")
    
    # contenttypes
    content_type = models.ForeignKey(ContentType)
    object_id = models.PositiveIntegerField()
    comment_obj = generic.GenericForeignKey('content_type', 'object_id')
    
    class Meta:
        ordering = ['-post_date']
        verbose_name = '评论'
        verbose_name_plural = '评论'
        
    class MPTTMeta:
        parent_attr = 'reply_to_comment'
        
    def __unicode__(self):
        return self.content
    
class BlogUser(User):
    small_avatar = FileBrowseField(max_length=40, verbose_name='头像（40×40）')
    info = models.TextField(verbose_name='用户信息')
    
    class Meta:
        verbose_name = '用户'
        verbose_name_plural = '用户'
        
    def __unicode__(self):
        return self.info