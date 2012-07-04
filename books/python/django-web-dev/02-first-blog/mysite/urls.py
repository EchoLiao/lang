# -*- coding: utf-8 -*-

from django.conf.urls.defaults import *
from mysite.views import current_datetime, hours_ahead
from django.views.generic import list_detail
from mysite.books.models import Publisher

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()


publisher_info = {
        "queryset" : Publisher.objects.all(),
        }

urlpatterns = patterns('',
    (r'^time/$', current_datetime), # 使用"对象"指定视图函数
    # 圆括号"()"表示其将作为一个参数(offset)来传递.
    # def hours_ahead(request, offset):
    (r'^time/plus/(\d{1,2})/$', hours_ahead),

    (r'^search/$','mysite.books.views.search'), # 使用"字符串"指定视图函数
    (r'^contact/$', 'mysite.books.views.contact'),
    (r'^contact/thanks/$', 'mysite.books.views.thanks'),
    (r'^add_publisher/$', 'mysite.books.views.add_publisher'),
    (r'^publishers/$', list_detail.object_list, publisher_info),

    # Example:
    (r'^blog/', include('mysite.blog.urls')),

    # Uncomment the admin/doc line below and add 'django.contrib.admindocs' 
    # to INSTALLED_APPS to enable admin documentation:
    (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^admin/(.*)', admin.site.root),
)
