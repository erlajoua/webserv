## Webserv

### Description ✨
Webserv is a 42 project, written mainly in C++ (and some Html, Css, Js, Php), which consisted in coding a HTTP server.
A config file is provided at the execution, example with `config/default.conf`

#### _Example of config file_

```
server {
    port                    8020;
    host                    127.0.0.1;
    server_name             team_tester;
    root                    ./www;
    errors                  custom_errors/403.html;
    client_body_size        200000;

    location / {
        methods   get post delete;
        index     index.html;
    }

    location /odd_pages {
        methods   get delete;
        index     index_odd.html;
    }
}

server {
    port                8055;
    host                127.0.0.1;
    server_name         autoindex_tester;
    root                ./www;
    errors              custom_errors/403.html;
    client_body_size    500;

    location / {
        methods     get;
        autoindex   on;
    }
}
```

- `server` block is a the creation of a serveur with:
  - `port` is the port listening the requests
  - `host` is the address of the server
  - `server_name` is the name of the server
  - `root` is the default path to the directory containing the files or the web server
  - `errors` is the path to custom error files, if a custom error file is not present, server has default error pages
  - `client_body_size` is the maximum number of bytes accepted on the body of the request
- `location` block creates a route where you can set some settings as
  - `methods` are the methods allowed on this route (**get**, **post**, **delete**)
  - `autoindex` if there is no index on this route and autoindex is on, the page shows the list of the files and directories, else `403` error is returned
  - `upload_dir` is the path where uploads are stored
  - `cgi_extension` and `cgi_bin` are respectively the name of the cgi and the path of the cgi's executable
  - `redirection` is the status code of the redirection

### Main purpose :page_facing_up:
The goal of the project was to understand how an HTTP Web server works
- Parsing the configuration file to get the desired server configuration.
- Setup the server that listens to the right ports on the right address.
- Receive the requests (via a browser or command lines), process the request and return the appropriate response (status code, body, params)
- The server must be non-blocking and use only one poll() (or equivalent) to process all requests arriving on the server.
- Create default error pages if they are not provided in the configuration file.
- Execution of CGIs (PHP example)
- Possibility to upload files on the server
- Managed methods are GET, POST, UPDATE and DELETE
- The server must resist to a Stress Test
- The server must be able to listen to several ports

### How to use :rocket:
First, run ``make``
Then run:
```
./webserv config/default.conf
```

Try the webserv on your web browser at ``http://localhost:8020``

Enjoy the test!
