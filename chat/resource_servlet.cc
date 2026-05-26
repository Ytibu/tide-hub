#include "resource_servlet.h"

#include <fstream>
#include <sstream>

namespace chat
{
    tide::Logger::ptr g_logger = TIDE_LOG_ROOT();

    ResourceServlet::ResourceServlet(const std::string &path)
        : tide::http::Servlet("ResourceServlet")
        , m_path(path)
    {
    }

    int32_t ResourceServlet::handle(tide::http::HttpRequest::ptr req, tide::http::HttpResponse::ptr rsp, tide::http::HttpSession::ptr session)
    {
        auto path = m_path + req->getPath();
        TIDE_LOG_INFO(g_logger) << "handle path=" << path;
        if(path.find("..") != std::string::npos)
        {
            rsp->setBody("invalid path");
            rsp->setStatus(tide::http::HttpStatus::HTTP_STATUS_NOT_FOUND);
            return 0;
        }
        std::ifstream ifs(path);
        if(!ifs)
        {
            rsp->setBody("file not found");
            rsp->setStatus(tide::http::HttpStatus::HTTP_STATUS_NOT_FOUND);
            return 0;
        }

        std::stringstream ss;
        std::string line;

        while(std::getline(ifs, line))
        {
            ss << line << std::endl;
        }
        rsp->setBody(ss.str());
        rsp->setHeader("Content-Type", "text/html; charset=utf-8");

        return 0;
    }

} // namespace chat