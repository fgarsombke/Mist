#include "MistStd.h"

#include "HTMLSource.h"

namespace Mist {

int HTMLSource::GetHtml(const std::string &urlPath, 
                        std::ostream &out, 
                        std::vector<std::string> &headers, 
                        unsigned int timeout) const
{
   try{
      using namespace boost::asio::ip;
      using namespace std;

      tcp::iostream request_stream;

      if (timeout>0) {
         request_stream.expires_from_now(boost::posix_time::milliseconds(timeout));
      }

      request_stream.connect(host_,port_);
      if(!request_stream){
         return -1;
      }

      request_stream << "GET " << urlPath << " HTTP/1.1\r\n"
      << "User-Agent: curl/7.29.0\r\n"
      << "Host: " << host_ << ":" << port_ << "\r\n"
      << "Cache-Control: no-cache\r\n"
      << "Accept: */*\r\n"
      << "Connection: keep-alive\r\n"
		<< "\r\n";
      request_stream.flush();

      string line1;
      getline(request_stream,line1);

      if (!request_stream) {
         return -2;
      }

      stringstream response_stream(line1);
      string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      string status_message;
      getline(response_stream,status_message);

      if (!response_stream||http_version.substr(0,5)!="HTTP/") {
         return -1;
      }

      if (status_code!=200) {
         return (int)status_code;
      }

      string header;
      while (getline(request_stream, header) && header != "\r") {
         headers.push_back(header);
      }

      // TODO HACK: Ask Michael A WHY????
      string hackLine;
      getline(request_stream, hackLine);

      if (hackLine.length() > 10) {
         out << hackLine;
      }

      while (std::getline(request_stream, hackLine) && hackLine != "0\r") {
         out << hackLine;
      }
      return status_code;
   }catch(std::exception &e){
      std::cout << e.what() << std::endl;
      return -3;
   }

}

int HTMLSource::PostHtml(const std::string & urlPath, 
               const std::string &contentType,
               const std::string &data, 
               std::vector<std::string> &responseHeaders,
               unsigned int timeout) const
{
      try{
      using namespace boost::asio::ip;
      using namespace std;

      tcp::iostream request_stream;

      if (timeout>0) {
         request_stream.expires_from_now(boost::posix_time::milliseconds(timeout));
      }

      request_stream.connect(host_,port_);
      if(!request_stream){
         return -1;
      }

      request_stream << "POST " << urlPath << " HTTP/1.1\r\n"
      << "From: michael@mgraczyk.com\r\n"
      << "User-Agent: curl/7.29.0"
      << "Content-Type: " << contentType << "\r\n"
      << "Content-Length: " << data.length()
		<< "\r\n\r\n"
      << data;
      request_stream.flush();

      string line1;
      getline(request_stream,line1);

      if (!request_stream) {
         return -2;
      }

      // TODO: Return response data

      stringstream response_stream(line1);
      string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      string status_message;
      getline(response_stream,status_message);

      if (!response_stream||http_version.substr(0,5)!="HTTP/") {
         return -1;
      }

      if (status_code!=200) {
         return (int)status_code;
      }


      return status_code;
   }catch(std::exception &e){
      std::cout << e.what() << std::endl;
      return -3;
   }
}


}