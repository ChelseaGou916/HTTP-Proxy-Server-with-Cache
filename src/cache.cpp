#include "cache.h"

Response Cache::get(string & request_line) {
    mtx.lock();
    unordered_map<std::string, Response>::iterator it = cache_map.begin();
    it = cache_map.find(request_line);
    if (cache_map.count(request_line) == 0) {
        mtx.unlock();
        return Response(); // create new response and return
    }
    logFile("(no-id): NOTE get " + request_line  +" from cache");
    mtx.unlock();
    return it->second;
};

void Cache::put(string & request_line, Response response){
  cout << "put into cache: " << response.response_line << endl;
  cout << "cache size: " << cache_map.size() << endl;
      mtx.lock();
      if (cache_map.size() == size && cache_map.count(request_line) == 0){
        cache_map.erase (cache_map.begin());
        logFile("(no-id): NOTE remove " + request_line +" from cache");
      }
      cache_map.insert(pair<string, Response>(request_line, response));
      logFile("(no-id): NOTE put " + request_line +" into cache");
      mtx.unlock();
    };

void Cache::remove(string & request_line){
      mtx.lock();
      cache_map.erase(request_line);
      logFile("(no-id): NOTE remove " + request_line +" from cache");
      mtx.unlock();
    };