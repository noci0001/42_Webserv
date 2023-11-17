#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

fileitem = form['filename']

if fileitem.filename:
    open(os.getcwd() + '/cgi-bin/upload/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
    message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded successfully'
else:
    message = 'No file was uploaded'

print("Content-Type: text/html; charset=utf-8")
print("Content-Type: text/html\r\n")
print("<h1> " + message + " </h1>")
print("<img src=\"./blue_ship.png\"> alt=\"uploaded image\"") 