from http import cookies
import os, sys

def show_cookie(cookie):
    for a, morsel in cookie.items():
        print(a, ':', morsel.value, '<br>')

cookie = cookies.BaseCookie()
print("Content-Type: text/html\r\n")
if 'HTTP_COOKIE' in os.environ:
    cookie.load(os.environ["HTTP_COOKIE"])
    show_cookie(cookie)
else:
    print("No popcorn was found.")