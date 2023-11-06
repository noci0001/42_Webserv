#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include "Webserv.hpp"

class MimeType
{
	public:
		MimeType();
		std::string getMimeType( std::string extension );

	private:
		std::map<std::string, std::string> _mime_types;

};

#endif