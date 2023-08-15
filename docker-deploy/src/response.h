#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

class Response {
    public:
        string all_content;////vector?
        string response_line;// first/start line
        bool no_cache;
        int max_age; // = max-age-Age
        bool must_revalidate;
        bool no_store;
        bool private_directive;
        bool public_directive;
        int max_stale;
        string expires;
        string status_code;
        string etag;
        string last_modified;
        string date;

        Response():
            all_content(""),
            response_line(""),
            no_cache(false),
            max_age(-1),
            must_revalidate(false),
            no_store(false),
            private_directive(false),
            public_directive(false),
            max_stale(-1),
            expires(""),
            status_code(""), 
            etag(""),
            last_modified(""),date("") {}
        
        Response(string content): all_content(content){
            
            parseResponse();
        }
        void parseResponse();  // get all headers or directive
        int calculateAge();
        int calculateMaxStale();
        string findHeader(string target);
        string getStatusCode();
        void printAll();
    };