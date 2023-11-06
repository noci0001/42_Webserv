#include "../../include/Webserv.hpp"

int ft_stoi(std::string str)
{
    std::stringstream ss(str);
    if (str.length() > 10)
        throw std::exception();
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!isdigit(str[i]))
            throw std::exception();
    }
    int res;
    ss >> res;
    return res;
}

std::string statusCodeString(short statusCode)
{
    switch  (statusCode)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocol";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 305:
            return "Use Proxy";
        case 306:
            return "Switch Proxy";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authenticatoion Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Lenght Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable for Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefinded";
    }
}

std::string getErrorPage(short statusCode)
{
	return ("<html>\r\n<head><title>" + toString(statusCode) + " " +
			statusCodeString(statusCode) + "</title></head>\r\n<body>\r\n" +
			"<center><h1>" + toString(statusCode) + " " + statusCodeString(statusCode) +
			"</h1></center>\r\n");
}

int	buildHtmlIndex(std::string &dir_name, std::vector<uint8_t> &body, size_t &body_length)
{
	struct dirent	*dir;
	DIR				*directory;
	std::string		html;

	directory = opendir(dir_name.c_str());
	if (directory == NULL)
	{
		std::cerr << "buildHtmlIndex: Error: opendir failed" << std::endl;
		return 1;
	}
	html.append("<html>\n");
	html.append("<head>\n");
	html.append("<title> Index of");
	html.append(dir_name);
	html.append("</title>\n");
	html.append("</head>\n");
	html.append("<body >\n");
	html.append("<h1> Index of " + dir_name + "</h1>\n");
	html.append("<table style=\"width:80%; font-size: 15px\">\n");
	html.append("<hr>\n");
	html.append("<th style=\"text-align:left\"> File Name </th>\n");
	html.append("<th style=\"text-align:left\"> Last Modification  </th>\n");
	html.append("<th style=\"text-align:left\"> File Size </th>\n");

	struct stat	stat_file;
	std::string	file_path;

	while ((dir = readdir(directory)) != NULL)
	{
		if (strcmp(dir->d_name, ".") == 0)
			continue ;
		file_path = dir_name + dir->d_name;
		stat(file_path.c_str(), &stat_file);
		html.append("<tr>\n");
		html.append("<td>\n");
		html.append("<a href=\"");
		html.append(dir->d_name);
		if (S_ISDIR(stat_file.st_mode))
			html.append("/");
		html.append("\">");
		html.append(dir->d_name);
		if (S_ISDIR(stat_file.st_mode))
			html.append("/");
		html.append("</a>\n");
		html.append("</td>\n");
		html.append("<td>\n");
		html.append(ctime(&stat_file.st_mtime));
		html.append("</td>\n");
		html.append("<td>\n");
		if (!S_ISDIR(stat_file.st_mode))
			html.append(toString(stat_file.st_size));
		html.append("</td>\n");
		html.append("</tr>\n");
	}
	html.append("</table>\n");
	html.append("<hr>\n");

	html.append("</body>\n");
	html.append("</html>\n");

	body.insert(body.begin(), html.begin(), html.end());
	body_length = body.size();
	return 0;
}

unsigned int fromHexToDecimal(const std::string &decimal)
{
	unsigned int result;
	std::stringstream ss;
	ss << decimal;
	ss >> std::hex >> result;
	return result;
}

std::vector<std::string> ftSplit( std::string text, char separator )
{
    std::string              str;
    std::stringstream        ss( text );
    std::vector<std::string> result;

    while ( getline( ss, str, separator ) ) {
        result.push_back( str );
    }

    return result;
}
