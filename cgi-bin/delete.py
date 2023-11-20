import os

folder_path = os.path.join(os.getcwd(), 'root', 'index', 'assets', 'uploads')
did_encounter_error = False

# Check if the folder exists
if os.path.exists(folder_path) and os.path.isdir(folder_path):
    # Get the list of files in the folder
    files = os.listdir(folder_path)

    # Check if there are files in the folder
    if files:
        print(f"Deleting {len(files)} files from the 'uploads' folder.")

        # Delete each file
        for file_name in files:
            file_path = os.path.join(folder_path, file_name)
            try:
                os.remove(file_path)
                print(f"Deleted: {file_path}")
            except Exception as e:
                did_encounter_error = True
                print(f"Error deleting {file_path}: {e}")

    if not did_encounter_error:
        # HTML output
        print("Content-Type: text/html; charset=utf-8")
        print("Content-Type: text/html\r\n")
        print("<html>")
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
        print("<body>")
        print("<h1>Movies deleted correctly</h1>")
        print('<button onclick="goBack()">GO BACK HOME</button>')
        print("<script>")
        print("function goBack() {")
        print('  window.location.href = "/";')
        print("}")
        print("</script>")
        print("</body>")
        print("</html>")
    else:
        print("Content-Type: text/html; charset=utf-8")
        print("Content-Type: text/html\r\n")
        print("<html>")
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
        print("No files found in the 'uploads' folder.")
        print("<body>")
        print("<h1>Movies deleted correctly</h1>")
        print('<button onclick="goBack()">GO BACK HOME</button>')
        print("<script>")
        print("function goBack() {")
        print('  window.location.href = "/";')
        print("}")
        print("</script>")
        print("</body>")
        print("</html>")
else:
    print("The 'uploads' folder does not exist.")