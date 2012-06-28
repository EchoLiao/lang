# -*- coding: utf-8 -*-

from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
import models

class PrefsPage(webapp.RequestHandler):
    def post(self):
        userprefs = models.get_userprefs()
        try:
            tz_offset = int(self.request.get('tz_offset'))  # 从表间中获取用户设置
            userprefs.tz_offset = tz_offset
            userprefs.put()     # 将其保存到数据存储区
        except ValueError:
            # User entered a value that wasn't an integer. Ignore for now.
            pass
        
        self.redirect('/')


application = webapp.WSGIApplication([('/prefs', PrefsPage)],
                                     debug=True)

def main():
    run_wsgi_app(application)
    
if __name__ == '__main__':
    main()
