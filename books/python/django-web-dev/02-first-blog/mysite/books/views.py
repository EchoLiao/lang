# -*- coding: utf-8 -*-

from django.db.models import Q
from django.shortcuts import render_to_response
from models import Book
from forms import ContactForm


def search(request):
    # request.GET 返回表单传递的字典数据;
    # request.GET.get(ker, defVal) 返回字典中键为key的值, 若找不到则返回defVal.
    # 注意: 如果使用 request.GET['q'] 访问变量, 在Get数据时 q 不可得,可能引发 KeyError .
    query = request.GET.get('q', '')
    if query:
        qset = (
                # 技术上 Q 对象包含QuerySet.
                # icontains 使用SQL的 LIKE 操作符, 是大小写不敏感的. 
                Q(title__icontains=query) |
                Q(authors__first_name__icontains=query) |
                Q(authors__last_name__icontains=query)
                )
        results = Book.objects.filter(qset).distinct()
    else:
        results = []
    return render_to_response("books/search.html", {
        "results": results,
        "query": query
        })



def contact(request):
    form = ContactForm()
    return render_to_response('books/contact.html', {'form': form})

