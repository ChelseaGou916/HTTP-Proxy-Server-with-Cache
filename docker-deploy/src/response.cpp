#include "response.h"
#include <iostream>
#include <string.h>
using namespace std;

void Response::parseResponse(){
    try{

        // get response_line
        size_t line_found=all_content.find_first_of("\r\n");
        response_line=all_content.substr(0,line_found);

        // get no_cache;
        size_t nc_found=all_content.find("no-cache");
        if(nc_found!=string::npos){no_cache=true;}else{no_cache=false;}

        // get max_age (= max-age-Age)
        max_age=calculateAge();
        if(max_age==-100){
            return; // error
        }
        if(max_age==0){no_cache=true;}// treat max-age=0 as no_cache

        // get must_revalidate
        size_t mr_found=all_content.find("must-revalidate");
        if(mr_found!=string::npos){must_revalidate=true;}else{must_revalidate=false;}

        // get no_store;
        size_t ns_found=all_content.find("no-store");
        if(ns_found!=string::npos){no_store=true;}else{no_store=false;}

        // get private_directive;
        size_t private_found=all_content.find("private");
        if(private_found!=string::npos){
            private_directive=true;
            no_store=true;
        }else{
            private_directive=false;
        }

        // get public_directive;
        size_t public_found=all_content.find("public");
        if(public_found!=string::npos){public_directive=true;}else{public_directive=false;}

        // get max_stale;
        max_stale=calculateMaxStale();

        // get expires;
        expires=findHeader("Expires: ");

        // get status_code;
        status_code=getStatusCode();

        // get etag;
        // ETag: "33a64df551" -> store "33a64df551" including ""
        etag=findHeader("ETag: ");
        
        // get last_modified;
        last_modified=findHeader("Last-Modified: ");

        // get date
        date=findHeader("Date: ");

        printAll();

    }catch (exception & e) {
        cout<<"failed to parse response.\n";
        return;
    }
}


int Response::calculateAge(){
    // 1.Cache-Control: public, max-age=604800, immutable ->“,”
    // 2.Cache-Control: max-age=604800 ->"\r\n"
    // Age: 100

    // get max-age:
    size_t ma_start_found=all_content.find("max-age=");
    if(ma_start_found==string::npos){
        cout<<"\nstep1\n";
        return -1; // don't have max-age
    }

    string temp=all_content.substr(ma_start_found+strlen("max-age="));

    size_t ma_end_found;
    size_t ma_end_found1=temp.find_first_of(",");
    size_t ma_end_found2=temp.find_first_of("\r\n");

    if(ma_end_found1!=string::npos && ma_end_found1<ma_end_found2){
        //"," exist
        ma_end_found=ma_end_found1;
    }
    else{
        if(ma_end_found2==string::npos){
            cout<<"find max-age error.\n";
            return -100;
        }
        ma_end_found=ma_end_found2;
    }

    string ma_str=temp.substr(0,ma_end_found);
    if(ma_str.find(".")!=string::npos){
        cout<<"max_age is not int.\n";
        return -100;
    }
    int ma=atoi(ma_str.c_str()); //  C-string

    // get Age:
    int age;
    size_t age_found=all_content.find_first_of("Age: ");
    if(age_found==string::npos){age=0;}
    else{
    string a_str=all_content.substr(age_found+strlen("Age: "));
    age=atoi(a_str.c_str());
    }
    
    // max_age = max_age - Age
    ma=ma-age;
    if(ma<0){
        cout<<"max_age<0 error.\n";
        return -100;
    }

    return ma;
}

int Response::calculateMaxStale(){
    size_t start_found=all_content.find("max-stale=");
    if(start_found==string::npos){
        return -1; // don't have max-stale
    }
    else{
        string temp=all_content.substr(start_found+strlen("max-stale="));
        int ms=atoi(temp.c_str());
        return ms;
    }
}


string Response::findHeader(string target){
    // find the header
    // get the value of the target between target string and "\r\n"
    size_t start_found=all_content.find(target);
    if(start_found==string::npos){
        return ""; // don't find target string
    }

    string temp=all_content.substr(start_found+strlen(target.c_str()));

    size_t end_found=temp.find_first_of("\r\n");
    string ans=temp.substr(0,end_found);

    return ans;
}


string Response::getStatusCode(){
    string status="";
    if(all_content.find("200 OK")!=string::npos){
        cout<<"status code is 200 OK.\n";
        status="200";
    }
    if(all_content.find("504 Gateway Timeout")!=string::npos){
        cout<<"status code is 504 Gateway Timeout.\n";
        status="504";
    }
    if(all_content.find("304 Not Modified")!=string::npos){
        cout<<"status code is 304 Not Modified.\n";
        status="304";
    }
    return status;
}

void Response::printAll(){
    cout<<"all_content "<<all_content<<endl;////vector?
    cout<<"response_line "<<response_line<<endl;// first/start line
    cout<<"no_cache "<<no_cache<<endl;
    cout<<"max_age "<<max_age<<endl; // = max-age-Age
    cout<<"must_revalidate "<<must_revalidate<<endl;
    cout<<"no_store "<<no_store<<endl;
    cout<<"private_directive "<<private_directive<<endl;
    cout<<"public_directive "<<public_directive<<endl;
    cout<<"max_stale "<<max_stale<<endl;
    cout<< "expires "<<expires<<endl;
    cout<<"status_code "<<status_code<<endl;
    cout<<"etag "<<etag<<endl;
    cout<<"last_modified "<<last_modified<<endl;
    cout<<"date "<<date<<endl;
}