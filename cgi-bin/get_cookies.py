#!/usr/bin/python3

import os
from http import cookies
import cgi, cgitb

form = cgi.FieldStorage()

key = form.getvalue('key')
cookie = cookies.SimpleCookie()
if 'HTTP_COOKIE' in os.environ:
    cookie.load(os.environ["HTTP_COOKIE"])
if key in cookie:
    print("HTTP/1.1 200 OK")
    print("Content-Type: text/plain\r\n")
    print("Your Popcorn", key, "has the flavor of: ", cookie[key].value)
else:
    print("HTTP/1.1 200 OK")
    print("Conten-Type: text/plain\r\n")
    print("No popcorn was found.")