# -*- coding: utf-8 -*-

from django import forms as forms


TOPIC_CHOICES = (
        ('general', 'General enquiry'),
        ('bug', 'Bug report'),
        ('suggestion', 'Suggestion'),
        )


class ContactForm(forms.Form):
    # ChoiceField等会自动校验用户输入.
    topic = forms.ChoiceField(choices=TOPIC_CHOICES)
    # 使用多行文本框: <textarea>
    message = forms.CharField(widget=forms.Textarea())
    sender = forms.EmailField(required=False)

