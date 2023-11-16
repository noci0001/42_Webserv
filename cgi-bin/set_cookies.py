#!/usr/bin/python3

import os
from http import cookies
import cgi, cgitb

form = cgi.FieldStorage()

key = form.getvalue('key')
value = form.getvalue('value')
cookie = cookies.SimpleCookie()
cookie[key] = value
print("HTTP/1.1 204 OK")
print(cookie.output())
print("\r\n")