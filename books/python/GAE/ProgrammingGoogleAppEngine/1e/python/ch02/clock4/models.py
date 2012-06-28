# -*- coding: utf-8 -*-

from google.appengine.api import users
from google.appengine.ext import db

# 定义一个用于将用户偏好存放到数据存储区的类, 数据建模接口
# 由 google.appengine.ext.db 模块提供.
class UserPrefs(db.Model):
    tz_offset = db.IntegerProperty(default=0)
    user = db.UserProperty(auto_current_user_add=True)
 

# 用于获取指定用户的 UserPrefs 对象.
def get_userprefs(user_id=None):
    if not user_id:
        user = users.get_current_user()
        if not user:
            return None
        user_id = user.user_id()
        
    # 通过类别'UserPrefs'和键名(用户ID)构造出实体的"数据存储区键"
    key = db.Key.from_path('UserPrefs', user_id)
    userprefs = db.get(key) # key 为 "userprefs"+user_id
    if not userprefs:       # 实体不在数据存储区中?
        userprefs = UserPrefs(key_name=user_id)
    return userprefs

