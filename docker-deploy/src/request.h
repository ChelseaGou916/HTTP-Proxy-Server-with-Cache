#include <stdio.h>
#include <string>

using namespace std;

class Request {
    public:
        string method_name; // post/ get/ connect
        string host_name;
        string request_uri; // path
        string port;
        string request_line; // start/first line
        string all_content;

        Request(string content): all_content(content){
            parseRequest();
        }

        void parseRequest(); // get fields of Request
        bool validDetermine(); // method name is valid or not
};   