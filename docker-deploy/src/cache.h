#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <mutex>

#include "request.h"
#include "response.h" 
#include "helper.h"
using namespace std;

class Cache {
  private:
    std::mutex mtx;
    unordered_map<string, Response> cache_map;
  public:
    int size = 10;
    Cache(int capacity): size(capacity) {
        unique_lock<std::mutex> lock(mtx);
    }
    Response get(string& request_line);
    void put(string & request_line, Response response);
    void remove(string & request_line);
};