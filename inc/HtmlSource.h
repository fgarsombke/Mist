#pragma once
#include "MistStd.h"

namespace Mist {

class HTMLSource;
typedef std::unique_ptr<HTMLSource> uPtrHTMLSource;

class HTMLSource {
public:
   HTMLSource(const std::string &host,
              const std::string &port)
      : host_(host), port_(port)
   {
   }

   int PostHtml(const std::string & urlPath, 
               const std::string &contentType,
               const std::string &data, 
               std::vector<std::string> &responseHeaders,
               unsigned int timeout) const;

   int GetHtml(const std::string & urlPath, 
               std::ostream &out, 
               std::vector<std::string> &headers,
               unsigned int timeout) const;
private:
   // Reference class only
   HTMLSource &operator=(const HTMLSource &other);
   HTMLSource(const HTMLSource &other);

   const std::string host_;
   const std::string port_;
};


}