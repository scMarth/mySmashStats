#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// lib curl
#include <curl/curl.h>
#include <curl/easy.h>

#include "errorStuff.h"
#include "regexStuff.h"

using namespace std;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
   int written = fwrite(ptr, size, nmemb, (FILE *)stream);
   return written;
}

void fetchUpdatedHistory(string username, string user_id){
   CURL *curl_handle;
   static const char *headerfilename = "../html/updatedHistories/updatedHistory.html";
   FILE *headerfile;
   /*
   static const char *bodyfilename = "body.txt";
   FILE *bodyfile;
   */
   curl_global_init(CURL_GLOBAL_ALL);

   /* init the curl session */
   curl_handle = curl_easy_init();

   /* set URL to get */
   string url = "https://www.smashladder.com/player/" + user_id + "/" + username + "/match-history";
   //curl_easy_setopt(curl_handle, CURLOPT_URL, "https://www.smashladder.com/player/178230/illlilliillillli/match-history");
   curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

   /* no progress meter please */
   curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

   /* send all data to this function  */
   curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

   /* open the files */
   headerfile = fopen(headerfilename,"w");
   if (headerfile == NULL) {
      curl_easy_cleanup(curl_handle);
      die("Could not open header file.");
   }

   /*
   bodyfile = fopen(bodyfilename,"w");
   if (bodyfile == NULL) {
      curl_easy_cleanup(curl_handle);
      return -1;
   }
   */

   /* we want the headers to this file handle */
   curl_easy_setopt(curl_handle,   CURLOPT_WRITEDATA, headerfile);

   /*
   * Notice here that if you want the actual data sent anywhere else but
   * stdout, you should consider using the CURLOPT_WRITEDATA option.  */

   /* get it! */
   curl_easy_perform(curl_handle);

   /* close the header file */
   fclose(headerfile);

   /* cleanup curl stuff */
   curl_easy_cleanup(curl_handle);
}


int main(){
   string config_filename = "../json/config/config.json";
   ifstream infile(config_filename);

   string username = "";
   string user_id = "";
   string line;

   username = scrollUntilFind("\"username\":(.)*", infile);
   user_id = scrollUntilFind("\"user_id\":(.)*", infile);

   if (username == "") die("username not found in config.json");
   if (user_id == "") die ("user_id not found in config.json");

   username = getSubstringThatMatches(":(.)*\"(.)*\"", username);
   user_id = getSubstringThatMatches(":(.)*\"(.)*\"", user_id);

   username = getSubstringThatMatches("\"(.)*\"", username);
   user_id = getSubstringThatMatches("\"(.)*\"", user_id);

   username = getSubstringThatMatches("[^\"]+", username);
   user_id = getSubstringThatMatches("[^\"]+", user_id);

   fetchUpdatedHistory(username, user_id);
   // cout << username << endl;
   // cout << user_id << endl;
   return 0;
}