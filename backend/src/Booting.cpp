#include "../include/Webserv.hpp"

void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list ap;

    errno_save = errno;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (errno_save != 0)
    {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        strerror(errno_save);
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    exit(1);
}

char* addHTTPResponse(const char* file_path) {
    try {
        std::ifstream input(file_path);
        if (!input) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return nullptr;
        }

        // Read the HTML content
        std::string html_content;
        std::string line;
        while (std::getline(input, line)) {
            html_content += line + "\n";
        }

        // Modify the content
        html_content = "HTTP/1.0 200 OK\r\n" + html_content + "\r\n";

        // Allocate memory for the C-style string
        char* result = new char[html_content.size() + 1];
        std::strcpy(result, html_content.c_str());

        std::cout << "HTTP response added successfully." << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return nullptr;
    }
}

Booting::Booting(ServerConfig& serverConfig) {
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE + 1];
    uint8_t recline[MAXLINE + 1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socker error.");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8023);

    if ((bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
        err_n_die("bind error.");

    if ((listen(listenfd, 10)) < 0)
        err_n_die("listen error.");

    for (;;) {
        struct sockaddr_in addr;
        socklen_t addr_len;

        printf("waiting for connection on port %d\n", 8023);
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        memset(recline, 0, MAXLINE);
        while ( ( n = read(connfd, recline, MAXLINE-1)) > 0) {
            printf("%s", recline);
            if (recline[n-1] == '\n') {
                break;
            }
            memset(recline, 0, MAXLINE);
        }
        if (n < 0)
            err_n_die("read error");

        char *format = addHTTPResponse("/Users/snocita/Desktop/42_Projects/Core/42_Webserv/root/index.html");

        printf("format: %s\n", format);
        snprintf((char *)buff, sizeof(buff), format);

        write(connfd, buff, strlen((char *)buff));
        close(connfd);
    }
}


Booting::~Booting() {

}