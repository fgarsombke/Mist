#include "WeatherDataSource.h"

#include <boost/asio.hpp>

using std::string;
using std::ostream;
using std::vector;
using std::stringstream;



int main() 
{
   using namespace std;
   stringstream s;
   Mist::WeatherDataSource w;

   
   //int ret = w.GetHtml(string("192.168.7.11"), string("8080"), string("api/weather?latitude=30&longitude=-100&time=45"), cout, vector<string>(), 5000);
   int ret = w.GetHtml(string("192.168.7.11"), string("8080"), string("/api/weather?latitude=30&longitude=-100&time=45"), cout, vector<string>(), 5000);


   cout << "Result: " << ret << endl;
   
   return 0;
}

namespace Mist {

int WeatherDataSource::GetHtml(string &host_,string &port_, string url_path, ostream &out_, vector<string> &headers, unsigned int timeout) const
{
    try{
        using namespace boost::asio::ip;
        tcp::iostream request_stream;
        if (timeout>0){
            request_stream.expires_from_now(boost::posix_time::milliseconds(timeout));
        }
        request_stream.connect(host_,port_);
        if(!request_stream){
            return -1;
        }
        request_stream << "GET " << url_path << " HTTP/1.0\r\n";
        request_stream << "Host: " << host_ << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Cache-Control: no-cache\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream.flush();
        string line1;
        getline(request_stream,line1);
        if (!request_stream)
        {
            return -2;
        }
        stringstream response_stream(line1);
        string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        string status_message;
        getline(response_stream,status_message);
        if (!response_stream||http_version.substr(0,5)!="HTTP/")
        {
            return -1;
        }
        if (status_code!=200)
        {
            return (int)status_code;
        }
        string header;
        while (getline(request_stream, header) && header != "\r")
            headers.push_back(header);
        out_ << request_stream.rdbuf();
        return status_code;
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
        return -3;
    }

}


}