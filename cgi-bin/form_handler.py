#!/usr/bin/python3  # run with Python 3

# A function to clean a string so that it only contains valid characters.
def clean_string(s):
	import string  # Import the string module, which contains a collection of string constants.
	valid_chars = string.ascii_letters + string.digits + " "  # Define valid characters as letters, digits, and space.
	# Return the cleaned string which includes only valid characters and remove leading/trailing whitespace.
	return "".join(c for c in s if c in valid_chars).strip()

# A function to handle the HTTP request in a CGI context.
def handle_req():
	import cgi  # Import the cgi module to work with CGI form data.
	import cgitb  # Import the cgitb module for CGI traceback management, which helps to debug.

	cgitb.enable()  # Enable the CGI traceback feature for easier debugging of exceptions.

	form = cgi.FieldStorage()  # Create an instance of FieldStorage to get form data.
	s = str(form.getvalue('name'))  # Get the value of the form field 'name' and convert it to a string.
	# If the name provided is too short, print an error message.
	if len(s) <= 4:
		print("<H1>Error</H1>")
		print("Please fill in a name with at least 5 characters.")
		return

	# If the name is of valid length, greet the user with the cleaned name.
	print(f"<h1>Hello there {clean_string(s)}!<h1>")

# A common Python idiom to make sure the code that follows will only be executed if the script is run directly.
if __name__ == "__main__":
	handle_req()  # Call the function to handle the request.
