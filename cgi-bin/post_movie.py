#!/usr/bin/python3

import requests
import cgi
import pdb; pdb.set_trace()

# Server details
url = "http://localhost/index/uploads/"  # Replace with your actual server and endpoint

# Receive the file from the client
form = cgi.FieldStorage()

if 'file' in form:
    file_item = form['file']

    # Get the filename
    filename = file_item.filename

    print(f"Uploaded filename: {filename}")
    echo

    # Send the file to the server
    files = {'file': (filename, file_item.file)}
    data = {'key': 'value'}  # Additional data if needed

    # Send the POST request
    response = requests.post("index/uploads/", files=files, data=data)

    # Print the server's response
    print(response.text)

    print("File successfully uploaded to the server.")
else:
    print("No file uploaded.")