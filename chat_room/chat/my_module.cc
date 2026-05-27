#include "my_module.h"

#include <tide/tide.h>
#include "resource_servlet.h"
#include "chat_servlet.h"

namespace chat
{

    static tide::Logger::ptr g_logger = TIDE_LOG_ROOT();

    MyModule::MyModule()
        : tide::Module("chat_room", "1.0", "")
    {
    }

    bool MyModule::onLoad()
    {
        TIDE_LOG_INFO(g_logger) << "onLoad";
        return true;
    }

    bool MyModule::onUnload()
    {
        TIDE_LOG_INFO(g_logger) << "onUnload";
        return true;
    }

    static int32_t handle1(tide::http::HttpRequest::ptr req, tide::http::HttpResponse::ptr rsp, tide::http::HttpSession::ptr session)
    {
        TIDE_LOG_INFO(g_logger) << "handle";
        rsp->setBody("hello status");
        return 0;
    }

    bool MyModule::onServerReady()
    {
        TIDE_LOG_INFO(g_logger) << "onServerReady";
        std::vector<tide::TcpServer::ptr> svrs;
        if (!tide::Application::GetInstance()->getServer("http", svrs))
        {
            TIDE_LOG_INFO(g_logger) << "get http server count=" << svrs.size();
            return false;
        }

        for (auto &svr : svrs)
        {
            auto http_svr = std::dynamic_pointer_cast<tide::http::HttpServer>(svr);
            if (!http_svr)
            {
                TIDE_LOG_INFO(g_logger) << "failed to cast to HttpServer";
                return false;
            }
            auto slt = http_svr->getServletDispatch();

            chat::ResourceServlet::ptr resource_slt(new chat::ResourceServlet(tide::EnvMgr::GetInstance()->getCwd()));

            slt->addGlobServlet("/html/*", resource_slt);
        }

        svrs.clear();
        if(!tide::Application::GetInstance()->getServer("ws", svrs))
        {
            TIDE_LOG_INFO(g_logger) << "get ws server count=" << svrs.size();
            return false;
        }

        for(auto &svr : svrs)
        {
            auto ws_svr = std::dynamic_pointer_cast<tide::http::WSServer>(svr);
            if(!ws_svr)
            {
                TIDE_LOG_INFO(g_logger) << "failed to cast to WSServer";
                return false;
            }
            tide::http::ServletDispatch::ptr slt_dispatch = ws_svr->getWSServletDispatch();

            ChatWSServlet::ptr chat_slt(new ChatWSServlet);
            slt_dispatch->addServlet("/tide/chat", chat_slt);
        }

        return true;
    }

    bool MyModule::onServerUp()
    {
        TIDE_LOG_INFO(g_logger) << "onServerUp";

        return true;
    }

}

extern "C"
{

    tide::Module *CreateModule()
    {
        tide::Module *module = new chat::MyModule;
        TIDE_LOG_INFO(chat::g_logger) << "CreateModule " << module;
        return module;
    }

    void DestoryModule(tide::Module *module)
    {
        TIDE_LOG_INFO(chat::g_logger) << "DestoryModule " << module;
        delete module;
    }
}