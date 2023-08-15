#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <fstream>

using namespace std;
static std::ofstream log_file("var/log/erss/proxy.log");
static mutex log_mtx; 


static void logFile(string content) {
    log_mtx.lock();
    log_file << content <<endl;
    log_mtx.unlock();
}

// set up server (till listen)
static int setUpServer(const char * myPort){
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *hostname = NULL;
  const char *port     = myPort;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: server: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
       host_info_list->ai_socktype, 
       host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname<< "," << port << ")" << endl;
    return -1;
  } //if

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  status = listen(socket_fd, 100);
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl; 
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  cout << "Waiting for connection on port " << port << endl;
  freeaddrinfo(host_info_list);

  return socket_fd;
}

// set up client, till connect
static int setUpClient(const char *hostname, const char *myPort){
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *port     = myPort;

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: client: cannot get address info for host" << endl;
    cerr << "  (" << port <<  ","<< hostname << ")" << endl;
    return -1;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
       host_info_list->ai_socktype, 
       host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if
  
  cout << "Connecting to " << hostname << " on port " << port << "..." << endl;
  
  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  freeaddrinfo(host_info_list);

  return socket_fd;
}

static int acceptClients(int server_fd, string & ip){
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connection_fd;
  client_connection_fd = accept(server_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connection_fd == -1) {
    cerr << "Error: cannot accept connection on socket" << endl;
    return -1;
  } //if

  struct sockaddr_in * addr = (struct sockaddr_in *)&socket_addr;
  ip = inet_ntoa(addr->sin_addr);
  // ip = inet_ntoa(((struct sockaddr_in *)&socket_addr)->sin_addr); // transfer into string ip

  return client_connection_fd;
}

static unordered_map<string,int> monthMap = {
    {"Jan", 1},
    {"Feb", 2},
    {"Mar", 3},
    {"Apr", 4},
    {"May", 5},
    {"Jun", 6},
    {"Jul", 7},
    {"Aug", 8},
    {"Sep", 9},
    {"Oct", 10},
    {"Nov", 11},
    {"Dec", 12},
};

// convert string to tm struct
static void getTimeStruct (struct tm & time, string timeStr){
    time.tm_year = atoi((timeStr.substr(12,4)).c_str())-1900;
    time.tm_mon = monthMap[timeStr.substr(8,3)]-1;
    time.tm_mday = atoi((timeStr.substr(5,2)).c_str());
    time.tm_hour = atoi((timeStr.substr(17,2)).c_str());
    time.tm_min = atoi((timeStr.substr(20,2)).c_str());
    time.tm_sec = atoi((timeStr.substr(23,2)).c_str());
}

static string timeString(time_t time) {
  string timeStr = string(asctime(gmtime(&time)));
  return timeStr.substr(0, timeStr.find("\n"));
}

// return current Time string
static string getCurrentTimeStr() {
  time_t currentTime = time(0);
  return timeString(currentTime);
}

static tm * getUTCtime(string rawTimeStr){
  struct tm ptm= {0};
  getTimeStruct(ptm, rawTimeStr); // convert string to tm struct
  time_t raw_time = mktime(&ptm); // tm to time_t
  return gmtime(&raw_time); // time_t to UTC tm
}
static string getExpiredTimeStr (string rawTimeStr, int extra){
  struct tm rawTime= {0};
  getTimeStruct(rawTime, rawTimeStr);
  time_t expiredTime = mktime(&rawTime) + extra;
  return timeString(expiredTime);
}
#endif