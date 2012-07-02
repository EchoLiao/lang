# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
import datetime



def current_datetime(request):
    #now = datetime.datetime.now()
    #return render_to_response('current_datetime.html', {'current_date': now})
    current_date = datetime.datetime.now()
    return render_to_response('current_datetime.html', locals())


def hours_ahead(request, offset):
    off_set = int(offset)
    dt_date = datetime.datetime.now() + datetime.timedelta(hours=off_set)
    return render_to_response('hours_ahead.html', locals())

