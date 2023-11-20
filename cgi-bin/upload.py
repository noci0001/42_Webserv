#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

fileitem = form['filename']

if fileitem.filename:
    open(os.getcwd() + '/root/index/assets/uploads/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
    message = 'The file "' + os.path.basename(fileitem.filename) + "\""  + "<br>" + " was uploaded successfully!"
else:
    message = 'No file was uploaded'

print("Content-Type: text/html; charset=utf-8")
print("Content-Type: text/html\r\n")
print("<html>")
print("<head><title>File Upload</title>")
print("<meta charset=\"UTF-8\">")
print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">")
print("<link rel=\"stylesheet\" href=\"/style.css\">")
print("<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css2?family=Cormorant+Upright:wght@300;400;500;600;700&family=Open+Sans:wght@300;400;500;600;700&display=swap\">")
print("<title>42 WEBFLIX</title>")
print("<head>")
print("<body style=\"margin-left: 15%; \">")
print("<h1> " + message + " </h1>")
print('<h2><a href="/upload.html"><bold>GO BACK TO SEE YOUR CHANGES<bold></a></h2>')
print("</body>")
print("</html>")