#include "../include/Webserv.hpp"
#include "../include/ParsingUtils.hpp"

ParsingUtils::ParsingUtils()
{
	this->_server_number = 0;
}

ParsingUtils::~ParsingUtils() {}

int ParsingUtils::printConfigFile()
{
	std::cout << "******PRINTING CONFIG FILE******" << std::endl;
	for (size_t i = 0; i < _server_config.size(); i++)
	{
		std::cout << "Server Number: " << i + 1 << std::endl;
		std::cout << "Server Name: " << _server_config[i].getServerName() << std::endl;
		std::cout << "Server Port: " << _server_config[i].getPort() << std::endl;
		std::cout << "Server Root: " << _server_config[i].getRoot() << std::endl;
		std::cout << "Server Index: " << _server_config[i].getIndex() << std::endl;
		std::cout << "Host: " << _server_config[i].getHost() << std::endl;
		std::cout << "Max Body Size: " << _server_config[i].getMaxBodySizeClient() << std::endl;
		std::cout << "Error Pages: " << _server_config[i].getErrorPages().size() << std::endl;
		std::map<short, std::string>::const_iterator cito = _server_config[i].getErrorPages().begin();
		while (cito != _server_config[i].getErrorPages().end())
		{
			std::cout << (*cito).first << "-" << cito->second << std::endl;
			++cito;
		}
		std::cout << "Locations: " << _server_config[i].getLocations().size() << std::endl;
		std::vector<Location>::const_iterator cit = _server_config[i].getLocations().begin();
		while (cit != _server_config[i].getLocations().end())
		{
			std::cout << "Location Path: " << cit->getPath() << std::endl;
			std::cout << "Location Methods: " << cit->getPrintMethods() << std::endl;
			std::cout << "Location Index: " << cit->getIndex() << std::endl;
			if (cit->getCgiPath().empty())
			{
				std::cout << "Location Root: " << cit->getRoot() << std::endl;
				if (!cit->getReturn().empty())
					std::cout << "Location Return: " << cit->getReturn() << std::endl;
				if (!cit->getAlias().empty())
					std::cout << "Location Alias: " << cit->getAlias() << std::endl;
			}
			else
			{
				std::cout << "CGI Root: " << cit->getRoot() << std::endl;
				std::cout << "CGI Path: " << cit->getCgiPath().size() << std::endl;
				std::cout << "CGI Extension: " << cit->getCgiExtension().size() << std::endl;
			}
			++cit;
		}
		cit = _server_config[i].getLocations().begin();
		std::cout << "****END PRINTING CONFIG FILE****" << std::endl;
	}
	return 0;
}

int ParsingUtils::createCluster(const std::string &config_file)
{
	std::string	content;
	Configfile	file(config_file);

	if (file.getTypePath(file.getOriginalConfigFile()) != 1)
		throw ErrorException("createCluster: Error: Wrong file type");
	if (file.checkFile(file.getOriginalConfigFile(), 4) == -1)
		throw ErrorException("createCluster: Error: File - wrong permissions");
	content = file.readFile(config_file);
	if (content.empty())
		throw ErrorException("createCluster: Error: File is empty");
	removeComments(content);
	removeWhiteSpaces(content);
	splitConfigFile(content);
	if (this->_config_file.size() != this->_server_number)
		throw ErrorException("createCluster: Error: Server number mismatch");
	for (size_t i = 0; i < this->_server_number; i++)
	{
		ServerConfig server_config;
		createServer(this->_config_file[i], server_config);
		this->_server_config.push_back(server_config);
	}
	if (this->_server_number > 1)
		checkServers();
	return 0;
}

void ParsingUtils::removeComments(std::string &content)
{
	size_t position = 0;
	position = content.find('#');
	while (position != std::string::npos)
	{
		size_t position_end;
		position_end = content.find('\n', position);
		content.erase(position, position_end - position);
		position = content.find('#');
	}
}

void ParsingUtils::removeWhiteSpaces(std::string &content)
{
	size_t w = 0;
	while (content[w] && isspace(content[w]))
		w++;
	content = content.substr(w);
	w = content.length() - 1;
	while (w > 0 && isspace(content[w]))
		w--;
	content = content.substr(0, w + 1);
}

void ParsingUtils::splitConfigFile(std::string &content)
{
	size_t position = 0;
	size_t position_end = 1;
	if (content.find("server", 0) == std::string::npos)
		throw ParsingUtils::ErrorException("ParsingUtil: Error: No server found");
	while (position != position_end && position < content.length())
	{
		position = findServerStart(position, content);
		position_end = findServerEnd(position, content);
		if (position == position_end)
			throw ErrorException("ParsingUtils: Error: Problem whith Scope");
		this->_config_file.push_back(content.substr(position, position_end - position + 1));
		this->_server_number++;
		position = position_end + 1;
	}
}

size_t ParsingUtils::findServerStart(size_t start, std::string &content)
{
	size_t fss;
	for (fss = start; content[fss]; fss++)
	{
		if (content[fss] == 's')
			break ;
		if (!isspace(content[fss]))
			throw ErrorException("ParsingUtils: Error: Wrong Character - isspace");
	}
	if (!content[fss])
		return start;
	if (content.compare(fss, 6, "server") != 0)
		throw ErrorException("ParsingUtils: Error: Wrong Character - compare");
	fss += 6;
	while (content[fss] && isspace(content[fss]))
		fss++;
	if (content[fss] == '{')
		return fss;
	else
		throw ErrorException("ParsingUtils: Error: Wrong Character - {");
}

size_t ParsingUtils::findServerEnd(size_t start, std::string &content)
{
	size_t fse;
	size_t scope = 0;

	for (fse = start + 1; content[fse]; fse++)
	{
		if (content[fse] == '{')
			scope++;
		if (content[fse] == '}')
		{
			if (!scope)
				return fse;
			scope--;
		}
	}
	return start;
}

std::vector<std::string> splitParams(std::string line, std::string seperator)
{
	std::vector<std::string> params;
	std::string::size_type   start, end;

	start = end = 0;
	while (true)
	{
		end = line.find_first_of(seperator, start);
		if (end == std::string::npos)
			break ;
		std::string temp = line.substr(start, end - start);
		params.push_back(temp);
		start = line.find_first_not_of(seperator, end);
		if (start == std::string::npos)
			break ;
	}
	return params;
}

void ParsingUtils::createServer(std::string &config_file, ServerConfig &server_config)
{
	std::vector<std::string> params;
	std::vector<std::string> error_codes;
	int location_flag = 1;
	bool autoindex_flag = false;
	bool max_size_flag = false;

	params = splitParams(config_file += ' ', std::string(" \n\t"));
	if (params.size() < 3)
		throw ErrorException("ParsingUtils: Error: createServer: Server validation failed");
	for (size_t i = 0; i < params.size(); i++)
	{
		if (params[i] == "listen" && (i + 1) < params.size() && location_flag)
		{
			if (server_config.getPort())
				throw ErrorException("ParsingUtils: Error: createServer: Port already set");
			server_config.setPorts(params[++i]);
		}
		else if (params[i] == "location" && (i + 1) < params.size())
		{
			std::string path;
			i++;
			if (params[i] == "{" || params[i] == "}")
				throw ErrorException("ParsingUtils: Error: createServer: Location validation failed");
			path = params[i];
			std::vector<std::string> location_params;
			if (params[++i] != "{")
				throw ErrorException("ParsingUtils: Error: createServer: Location validation failed");
			i++;
			while (i < params.size() && params[i] != "}")
				location_params.push_back(params[i++]);
			server_config.setLocations(path, location_params);
			if (i < params.size() && params[i] != "}")
				throw ErrorException("ParsingUtils: Error: createServer: Location validation failed");
			location_flag = 0;
		}
		else if (params[i] == "host" && (i + 1) < params.size() && location_flag)
		{
			if (server_config.getHost())
				throw ErrorException("ParsingUtils: Error: createServer: Host already set");
			server_config.setHost(params[++i]);
		}
		else if (params[i] == "root" && (i + 1) < params.size() && location_flag)
		{
			if (!server_config.getRoot().empty())
				throw ErrorException("ParsingUtils: Error: createServer: Root already set");
			server_config.setRoot(params[++i]);
		}
		else if (params[i] == "error_page" && (i + 1) < params.size() && location_flag)
		{
			while (++i < params.size())
			{
				error_codes.push_back(params[i]);
				if (params[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= params.size())
					throw ErrorException("ParsingUtils: Error: createServer: Error page validation failed");
			}
		}
		else if (params[i] == "client_max_body_size" && (i + 1) < params.size() && location_flag)
		{
			if (max_size_flag)
				throw ErrorException("ParsingUtils: Error: createServer: Max body size already set");
			server_config.setMaxBodySizeClient(params[++i]);
			max_size_flag = true;
		}
		else if (params[i] == "server_name" && (i + 1) < params.size() && location_flag)
		{
			if (!server_config.getServerName().empty())
				throw ErrorException("ParsingUtils: Error: createServer: Server name already set");
			server_config.setServerName(params[++i]);
		}
		else if (params[i] == "index" && (i + 1) < params.size() && location_flag)
		{
			if (!server_config.getIndex().empty())
				throw ErrorException("ParsingUtils: Error: createServer: Index already set");
			server_config.setIndex(params[++i]);
		}
		else if (params[i] == "autoindex" && (i + 1) < params.size() && location_flag)
		{
			if (autoindex_flag)
				throw ErrorException("ParsingUtils: Error: createServer: Autoindex already set");
			server_config.setAutoIndex(params[++i]);
			autoindex_flag = true;
		}
		else if (params[i] != "}" && params[i] != "{")
		{
			if (!location_flag)
				throw ErrorException("ParsingUtils: Error: createServer: Parameters after location");
			else
				throw ErrorException("ParsingUtils: Error: createServer: Unknown parameter");
		}
	}
	if (server_config.getRoot().empty())
		server_config.setRoot("/;");
	if (server_config.getHost() == 0)
		server_config.setHost("localhost;");
	if (server_config.getIndex().empty())
		server_config.setIndex("index.html;");
	if (Configfile::isFileExistAndReadable(server_config.getRoot(), server_config.getIndex()))
		throw ErrorException("ParsingUtils: Error: createServer: Index file not found in Configfile");
	if (server_config.checkLocation())
		throw ErrorException("ParsingUtils: Error: createServer: Location already set");
	if (!server_config.getPort())
		throw ErrorException("ParsingUtils: Error: createServer: Port not set");
	server_config.setErrorPages(error_codes);
	if (!server_config.validErrorPages())
		throw ErrorException("ParsingUtils: Error: createServer: Error page not found or Error Code wrong");
}

void ParsingUtils::checkServers()
{
	std::vector<ServerConfig>::iterator ito1;
	std::vector<ServerConfig>::iterator ito2;

	for (ito1 = this->_server_config.begin(); ito1 != this->_server_config.end() - 1; ito1++)
	{
		for (ito2 = ito1 + 1; ito2 != this->_server_config.end(); ito2++)
		{
			if (ito1->getPort() == ito2->getPort() && ito1->getHost() == ito2->getHost() && ito1->getServerName() == ito2->getServerName())
				throw ErrorException("ParsingUtils: Error: checkServers: Server Validation failed");
		}
	}
}

std::vector<ServerConfig> ParsingUtils::getServers()
{
	return (this->_server_config);
}