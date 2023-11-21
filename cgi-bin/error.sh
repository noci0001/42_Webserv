#!/bin/bash

# Server URL and port
SERVER_URL="http://127.0.0.1:8025"

# Define a function to make a request and display the response
make_request() {
    local url=$1
    echo "Requesting: $url"
    curl -i "$url"
    echo -e "\n\n"
}

# Test with various error scenarios
echo "Starting error handling tests for webserv"

# 1. Request to a non-existent page
make_request "${SERVER_URL}/nonexistentpage"

# 2. Malformed URL
make_request "http://127.0.0.1:8025/???badrequest"

# 3. Request with unsupported method
curl -X UNSUPPORTED_METHOD -i "${SERVER_URL}"

# Add more tests as needed for specific error cases you want to check

echo "Error handling tests completed"