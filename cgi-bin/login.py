from http import cookies
import os
import cgi
import time
import hashlib
import pickle
import sys

class Session:
    def __init__(self, name):
        self.name = name
        self.sessionid = hashlib.sha1(str(time.time()).encode("utf-8")).hexdigest()
        with open('cgi-bin/sessions/session_' + self.sessionid, 'wb') as f:
            pickle.dump(self, f)
    def getSessionId(self):
        return self.sessionid

class DatabaseUsers:
    def __init__(self):
        self.user_passw = {}
        self.user_firstname = {}
    def addUser(self, username, password, firstname):
        self.user_passw[username] = password
        self.user_firstname[username] = firstname
        with open('cgi-bin/database_users', 'wb') as f:
            pickle.dump(self, f)

def printLoginPage(session):
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>Login Page</title>")
    print("</head>")
    print("<body>")
    print("<h1>Login Page: ", session.name, "</h1>")
    print("<p>Your session ID: ", session.getSessionId(), "</p>")
    print("</body>")
    print("<a href=\"/index.html\"> Click here to go back to the main page </a>")
    print("</html>")

def printMessageUser(message):
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>Message to User</title>")
    print("</head>")
    print("<body>")
    print("<h1>", message, "</h1>")
    print("</body>")
    print("<a href=\"/index.html\"> Click here to go back to the main page </a>")
    print("</html>")

def printLogin():
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("meta charset=\"UTF-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">")
    print("<link rel=\"stylesheet\" href=\"/style.css\">")
    print("<title>Login</title>")
    print("</head>")
    print("<body>")
    print("<h1>Login 42 WEBFLIX</h1> </center>")
    print("<form action=\"/cgi-bin/login.py\" method=\"get\">")
    print("<div class=\"cookies\">")
    print("<label <b>Username: </b></label>")
    print("<input type=\"text\" placeholder=\"Enter Username\" name=\"username\" required>")
    print("<label <b>Password: </b></label>")
    print("<input type=\"password\" placeholder=\"Enter Password\" name=\"password\" required>")
    print("<button type=\"submit\">Login</button>")
    print("Not a member yet? <a href=\"\">Register</a>")
    print("</div>")
    print("</form>")
    print("</body>")
    print("</html>")

def checkUser(name, password):
    if os.path.exists("cgi-bin/database_users"):
        with open('cgi-bin/database_users', 'rb') as f:
            database_users = pickle.load(f)
            if name in database_users.user_passw and database_users.user_passw[name] == password:
                session = Session(database_users.user_firstname[name])
                return session
            else:
                return None
    else:
        return None

def executeLogin():
    username = form.getvalue("username")
    password = form.getvalue("password")
    firstname = form.getvalue("firstname")
    if username == None:
        printLogin()
    elif firstname == None:
        session = checkUser(form.getvalue("username"), form.getvalue("password"))
        if session == None:
            printMessageUser("Wrong username or password")
        else:
            print("User authenticated. Welcome back!", file=sys.stderr)
            cookies.clear()
            cookies["SID"] = session.getSessionId()
            cookies["SID"]["expires"] = 120 # 2 minutes expiration
            print("HTTP/1.1 301 OK")
            print(cookies.output())
            print("Location: login.py")
            print("\r\n")
    else:
        if os.path.exists("cgi-bin/database_users"):
            with open("cgi-bin/database_users", "rb") as f:
                database_users = pickle.load(f)
                if username in database_users.user_passw:
                    printMessageUser("User Account already exists")
                else:
                    database_users.addUser(username, password, firstname)
                    printMessageUser("User Account created successfully")
        else:
            database_users = DatabaseUsers()
            if username in database_users.user_passw:
                printMessageUser("User Account already exists")
            else:
                database_users.addUser(username, password, firstname)
                printMessageUser("User Account created successfully")

form = cgi.FieldStorage()
if "HTTP_COOKIE" in os.environ:
    cookies = cookies.SimpleCookie()
    cookies.load(os.environ["HTTP_COOKIE"])
    if "SID" in cookies:
        print("Your session ID is: ", cookies["SID"].value, file=sys.stderr)
        with open('cgi-bin/sessions/session_' + cookies["SID"].value, 'rb') as f:
            session = pickle.load(f)
        printLoginPage(session)
    else:
        executeLogin()
else:
    executeLogin()