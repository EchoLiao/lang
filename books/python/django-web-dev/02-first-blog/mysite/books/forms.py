# -*- coding: utf-8 -*-

from django import forms as forms
from models import Publisher
from django.forms.models import modelform_factory as form_for_model



TOPIC_CHOICES = (
        ('general', 'General enquiry'),
        ('bug', 'Bug report'),
        ('suggestion', 'Suggestion'),
        )


class ContactForm(forms.Form):
    # ChoiceField等会自动校验用户输入.
    topic = forms.ChoiceField(choices=TOPIC_CHOICES)
    # 使用多行文本框: <textarea>
    message = forms.CharField(widget=forms.Textarea(), 
            initial="Replace with your feedback")
    sender = forms.EmailField(required=False)

    # 自定义校验规则:
    # 方法名书写规则: clean_属性名
    # 这个新的方法将在默认的字段校验器之后被调用(在本例中, 就是CharField的校验器)
    def clean_message(self):
        message = self.data.get('message', '')
        num_words = len(message.split()) # 以空白字符分隔
        if num_words < 4:
            raise forms.ValidationError("Not enough words!")
        return message



# 一个新的form，用于提交一个新出版商的信息到我们的book应用
PublisherForm = form_for_model(Publisher)

