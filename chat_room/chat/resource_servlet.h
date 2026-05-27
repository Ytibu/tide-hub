#ifndef _RESOURCE_SERVLET_H_
#define _RESOURCE_SERVLET_H_

#include <tide/tide.h>

namespace chat
{

class ResourceServlet : public tide::http::Servlet
{
public:
    typedef std::shared_ptr<ResourceServlet> ptr;

    ResourceServlet(const std::string &path);

    virtual int32_t handle(tide::http::HttpRequest::ptr req, tide::http::HttpResponse::ptr rsp, tide::http::HttpSession::ptr session) override;

private:
    std::string m_path;
};

} // namespace chat

#endif // _RESOURCE_SERVLET_H_