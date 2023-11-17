#!/usr/bin/env python

import os
import cgi
import cgitb
import errno 

cgitb.enable()

upload_dir = './cgi-bin/uploads/'

# Check if the directory already exists
if not os.path.exists(upload_dir):
    try:
        # Create the directory
        os.makedirs(upload_dir)
    except OSError as e:
        if e.errno != errno.EEXIST:
            # If the error is not due to the directory already existing, raise it
            raise

form = cgi.FieldStorage()
print(form.keys())  # Print all keys in the form
file_item = form.getvalue('file')
print(file_item)  # Print the value of the 'file' key

if file_item:
    file_name = os.path.basename(file_item.filename)
    file_path = os.path.join(upload_dir, file_name)

    with open(file_path, 'wb') as file:
        file.write(file_item.file.read())

    file_item = cgi.FieldStorage()
    print(form.keys())  # Print all keys in the form
    file_item = form.get('file')
    print(file_item)  # Print the value of the 'file' key

    print("Content-Type: text/html")
    print()
    print("<html>")
    print("<head><title>File Upload</title></head>")
    print("<body>")
    print("<h1>File Upload Successful</h1>")
    print("<p>Uploaded file: {0}</p>".format(file_name))
    print("</body>")
    print("</html>")
else:
    print("Content-Type: text/html")
    print()
    print("<html>")
    print("<head><title>File Upload</title></head>")
    print("<body>")
    print(form.keys()) 
    print(file_item)
    print("<h1>File Upload Failed</h1>")
    print("<p>No file was uploaded.</p>")
    print("</body>")
    print("</html>")
