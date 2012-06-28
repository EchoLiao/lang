# -*- coding: utf-8 -*-

from google.appengine.api import memcache
from google.appengine.api import users
from google.appengine.ext import db
import logging  # 日志模块

class UserPrefs(db.Model):
    tz_offset = db.IntegerProperty(default=0)
    user = db.UserProperty(auto_current_user_add=True)
    
    # 调用 memcache.set() 方法把实体保存到 memcache 中.
    def cache_set(self):
        logging.info('cache set')
        # 实体的键名用作键, 整个对象作为值, 实体的类别作为命名空间.
        memcache.set(self.key().name(), self, namespace=self.key().kind())
        
    # 重写 put() 方法
    def put(self):
        self.cache_set()
        db.Model.put(self)

def get_userprefs(user_id=None):
    if not user_id:
        user = users.get_current_user()
        if not user:
            return None
        user_id = user.user_id()
            
    userprefs = memcache.get(user_id, namespace='UserPrefs')
    if not userprefs:   # memcache 中没有实体?
        logging.info('cache miss')
        key = db.Key.from_path('UserPrefs', user_id)
        userprefs = db.get(key)
        if userprefs:   # 数据存储区中已有实体?
            userprefs.cache_set()
        else:           # 数据存储区中没有实体?
            userprefs = UserPrefs(key_name=user_id)
    else:
        logging.info('cache hit')

    return userprefs
