#ifndef CHAT_SERVLET_H__
#define CHAT_SERVLET_H__

#include <tide/http/ws_servlet.h>

namespace chat
{

    class ChatWSServlet : public tide::http::WSServlet
    {
    public:
        using ptr = std::shared_ptr<ChatWSServlet>;
        ChatWSServlet();

        virtual int32_t handle(tide::http::HttpRequest::ptr req, tide::http::WSFrameMessage::ptr session, 
            tide::http::WSSession::ptr ws_session) override;

        virtual int32_t onConnect(tide::http::HttpRequest::ptr req, tide::http::WSSession::ptr session) override;
        virtual int32_t onClose(tide::http::HttpRequest::ptr req, tide::http::WSSession::ptr session) override;
    };
}

#endif // CHAT_SERVLET_H__