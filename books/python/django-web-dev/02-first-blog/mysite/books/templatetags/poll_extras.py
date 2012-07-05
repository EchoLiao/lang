# -*- coding: utf-8 -*-

from django import template


register = template.Library()




##################### 过滤器 ########################

@register.filter(name='cut')
def cut(value, arg):
    return value.replace(arg, '*')






##################### 标签 ##########################

# 例如, 这个自定义模板标签:  {% upper %} , 它把自己和 {% endupper %} 之间的所
# 有内容都变成大写:
#   {% upper %}
#       This will appear in uppercase, {{ your_name }}.
#   {% endupper %}
@register.tag(name='upper')
def do_upper(parser, token):
    nodelist = parser.parse(('endupper',))
    parser.delete_first_token()
    return UpperNode(nodelist)

class UpperNode(template.Node):
    def __init__(self, nodelist):
        self.nodelist = nodelist
    def render(self, context):
        # 对节点列表中的每个 Node简单的调用 render() .
        output = self.nodelist.render(context)
        return output.upper()
