#include "request.h"
#include <iostream>
#include <string.h>
using namespace std;

void Request::parseRequest(){
    try{
    // cout<<"all_content: \n"<<all_content;    
    // get method name
    size_t method_found=all_content.find_first_of(" ");
    method_name=all_content.substr(0,method_found);
    
    if (method_name != "GET" && method_name != "POST" && method_name != "CONNECT") {
        cout<<"method name not exist.";
        method_name=" ";
        return;
    }

    // get request line
    size_t line_found=all_content.find_first_of("\r\n");
    request_line=all_content.substr(0,line_found);

    // get host name
    size_t host_found1= all_content.find("Host: ");
    string host_temp=all_content.substr(host_found1+6);
    cout<<"host_temp:"<<host_temp;
    size_t host_found2=host_temp.find("\r\n");
    host_name=host_temp.substr(0,host_found2);
    cout<<"host_name"<<host_name;

    // get request URI, not include hostname
    size_t uri_found1=all_content.find_first_of(host_name);
    string uri_temp=all_content.substr(uri_found1+host_name.size());
    size_t uri_found2=uri_temp.find_first_of(" ");
    string request_uri=uri_temp.substr(0,uri_found2);

    // get port number
    if(host_name.find(":")==string::npos){
        // not give port
        port="80";
    }
    else{   
        size_t port_found=host_name.find(":");
        port=host_name.substr(port_found+1);
        
        host_name=host_name.substr(0,port_found);
    }
    }
    catch (exception & e) {
        cout<<"failed to parse request.\n";
        return;
  }
}

bool Request::validDetermine(){
    if(method_name==" "){
        return false;
    }
    else{
        return true;
    }
  
}