# 42_Webserv

BUILDING A HTTP WEWBSERVER C++98 COMPATIBLE FROM SCRATCH


## MEMBERS OF THE TEAM AND TASKS DELEGATION ##

### 1. Tom (Server Core) ###
   
a) Establish the fundamental architecture of the server.
- Decide on a single-threaded vs. multi-threaded architecture given the non-blocking requirement.
- Design the data structures to hold client connections and their states.

b) Implement the server's main loop using poll().
- Setup the pollfd structure to monitor sockets.
- Set the initial timeout values for poll().
- Differentiate between and handle the return values of poll() (timeout, error, activity).

c) Handle client connections within the core loop.
- Detect new incoming connections and integrate them into the poll() monitoring.
- Accept new connections and add their socket descriptors to the pollfd list.
- Process active client connections based on the events reported by poll().

d) Implement core error-handling mechanisms.
- Detect and handle errors like "connection reset by peer" or "broken pipe".
- Implement timeouts for inactive or stalled clients to ensure the server doesn't hang.
- Provide a mechanism to report errors to the Response Builder for appropriate client notifications.

e) Server Multi-port Listening.
- Design a mechanism to allow the server core to listen on multiple ports simultaneously.
- Ensure each port's activity is integrated into the poll() loop without conflicts.
- Implement a strategy to distribute or handle incoming traffic across the different ports.

f) Configuration File Parsing at the Core Level.
- Read the configuration file during server startup.
- Extract essential core-related configurations, like port numbers and default paths.
- Initialize the server based on the extracted configurations.

### 2. Sam (Request Parser + Front-end) ###

   #### Request Parser: #### 
a) Develop a parser to read incoming HTTP requests.
- Buffered reading of data from the socket.
- Identification of request's end.
- Splitting the request: start line, headers, body.

b) Extract required information from the request.
- Parse the start line: method, URI, HTTP version.
- Parse headers into a structured format.
- Handle chunked requests appropriately.

c) Handle parsing of chunked requests.
- Identify the Transfer-Encoding: chunked header.
- Read data until the chunk size is identified.
- Read the chunk data based on the specified size.
- Repeat until a zero-size chunk is encountered.

d) Forward the request to the appropriate module.
- Compare the request URI with route configurations.
- Determine the handling module (static file, CGI, error handler, etc.).
- Forward request data to the determined module for processing.


  #### Front-end: #### 
a) Design the user interface.
- Sketch wireframes/mockups for the static website pages.
- Decide on a color palette, typography, and overall design theme.

b) Develop the HTML templates.
- Write the HTML structure for main pages.
- Ensure mobile responsiveness using appropriate meta tags and CSS.
- Add accessibility features like ARIA labels and proper semantic HTML.

c) Style the website using CSS.
- Create a main stylesheet with global styles.
- Implement component-specific styles (e.g., buttons, forms, navigation).
- Optimize for different screen sizes using media queries.

d) Implement client-side interactivity using JavaScript.
- Decide if frameworks/libraries like React or Vue.js are needed.
- Write scripts for client-side events: form submissions, animations, modals, etc.
- Handle AJAX calls for dynamic content loading or interactions with the server.

e) Ensure cross-browser compatibility.
- Test the website on multiple browsers (Chrome, Firefox, Safari, Edge, etc.).
- Address any browser-specific quirks or issues.
- Use tools or platforms like BrowserStack for testing on different devices and browsers.

f) Optimize for performance.
- Minify and compress CSS and JS files.
- Optimize image sizes and formats.
- Use browser caching strategies for static assets.

g) Integration with the back-end.
- Ensure forms and input fields send data correctly to the server.
- Handle and display server responses (e.g., success messages, error notifications).
- Implement real-time features if required, possibly using WebSockets or Server-Sent Events.

### 3. Yana (Response Builder and CGI) ###
   
a) Build appropriate HTTP responses.
- Construct the status line based on the processing outcome.
- Add general headers (Date, Server, etc.).
- Add specific headers based on content (e.g., Content-Type).
- Attach the response body if necessary.
- Check for 'Accept-Encoding' header in the request.
- Use gzip or deflate compression if supported by the client.

b) Include default error pages.
- Maintain a map of error codes to HTML content.
- If an error occurs, look up the corresponding error page.
- Serve the error page as the response body.

c) Execute CGI scripts.
- Identify the requested script's path.
- Setup necessary environment variables (REQUEST_METHOD, QUERY_STRING, etc.).
- Use fork() and exec() to run the CGI script.
- Capture the CGI script's output for the response.
- Sanitize input values to prevent command injection.
- Limit the resources available to the CGI script to prevent misuse.

d) Format CGI output for the client.
- Parse headers from the CGI output.
- Identify Content-Length or use chunked encoding.
- Ensure correct ordering and format of headers and body in the final response.

e) Handle file uploads.
- Identify multipart/form-data in the request's Content-Type header.
- Parse the boundaries to extract individual files.
- Save each file to the server's designated directory.
- Respond with a success message or error details.

### 4. All ###

a) Configuration File Setup: Each team member will introduce their own configuration file, inspired by NGINX. Their configuration file should handle:
- Port and host configuration.
- Server names setup.
- Default error pages.
- Client body size limits.
- Route setups with specific rules/configurations (HTTP methods, redirections, directory roots, directory listing, default files, CGI execution, file uploads, etc.).

b) Validate configuration file content.
- Ensure the provided values (e.g., port numbers, paths) are valid and usable.
- Provide meaningful error messages if invalid configurations are detected.

c)  Integration & Testing:
- Integration Testing
- Unit Testing:
  Each module or functionality (like parsing, response building, server core operations) should be unit-tested to ensure individual components work as expected.
- End-to-End Testing:
  Simulate actual user behaviors, like accessing a webpage, uploading a file, or deleting content, to ensure the entire system works from the user's perspective.
