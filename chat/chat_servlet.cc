#include "chat_servlet.h"

#include <tide/log.h>
#include <tide/mutex.h>
#include <tide/utils.h>

#include "protocol.h"

namespace chat
{
    static tide::Logger::ptr g_logger = TIDE_LOG_ROOT();
    tide::RWMutex m_mutex;

    std::map<std::string, tide::http::WSSession::ptr> m_sessions;

    int32_t SendMessage(tide::http::WSSession::ptr session,  ChatMessage::ptr msg)
    {
        TIDE_LOG_INFO(g_logger) << "SendMessage data=" << msg->toString();
        return session->sendMessage(msg->toString()) > 0 ? 0 : -1;
    }

    bool sessionExist(const std::string &id)
    {
        tide::RWMutex::ReadLock lock(m_mutex);
        return m_sessions.find(id) != m_sessions.end();
    }

    void session_add(const std::string &id, tide::http::WSSession::ptr session)
    {
        tide::RWMutex::WriteLock lock(m_mutex);
        m_sessions[id] = session;
    }
    
    void session_del(const std::string &id)
    {
        tide::RWMutex::WriteLock lock(m_mutex);
        m_sessions.erase(id);
    }

    void session_notify(ChatMessage::ptr msg, tide::http::WSSession::ptr session = nullptr)
    {
        tide::RWMutex::ReadLock lock(m_mutex);
        auto sessions = m_sessions;
        lock.unlock();

        for (auto &i : sessions)
        {
            if (session == i.second)
            {
                continue;
            }
            SendMessage(i.second, msg);
        }
    }

    ChatWSServlet::ChatWSServlet()
        : tide::http::WSServlet("ChatWSServlet")
    {
    }

    int32_t ChatWSServlet::onConnect(tide::http::HttpRequest::ptr req, tide::http::WSSession::ptr session)
    {
        TIDE_LOG_INFO(g_logger) << "ChatWSServlet onConnect";
        return 0;
    }

    int32_t ChatWSServlet::onClose(tide::http::HttpRequest::ptr req, tide::http::WSSession::ptr session)
    {
        auto id = req->getHeader("$id");
        TIDE_LOG_INFO(g_logger) << "ChatWSServlet onClose" << " id=" << id;
        if (!id.empty())
        {
            session_del(id);

            ChatMessage::ptr nty(new ChatMessage);
            nty->set("type", "user_leave");
            nty->set("time", tide::Time2Str());
            nty->set("name", id);
            session_notify(nty);

        }
        return 0;
    }

    int32_t ChatWSServlet::handle(tide::http::HttpRequest::ptr header, tide::http::WSFrameMessage::ptr msgx, tide::http::WSSession::ptr session)
    {
        TIDE_LOG_INFO(g_logger) << "handle" << session << " opcode=" << msgx->getOpcode() << " data=" << msgx->getData();

        auto msg = ChatMessage::Create(msgx->getData());
        auto id = header->getHeader("$id");
        if (!msg)
        {
            if (!id.empty())
            {
                tide::RWMutex::WriteLock lock(m_mutex);
                m_sessions.erase(id);
            }
            return -1;
        }

        ChatMessage::ptr rsp_msg(new ChatMessage);
        auto type = msg->get("type");
        if (type == "login_request")
        {
            rsp_msg->set("type", "login_response");
            auto name = msg->get("name");
            if (name.empty())
            {
                rsp_msg->set("result", "400");
                rsp_msg->set("msg", "name is null");
                return SendMessage(session, rsp_msg);
            }
            if (!id.empty())
            {
                rsp_msg->set("result", "401");
                rsp_msg->set("msg", "logined");
                return SendMessage(session, rsp_msg);
            }
            if (sessionExist(name))
            {
                rsp_msg->set("result", "402");
                rsp_msg->set("msg", "name exist");
                return SendMessage(session, rsp_msg);
            }

            id = name;
            header->setHeader("$id", id);
            rsp_msg->set("result", "200");
            rsp_msg->set("msg", "ok");
            session_add(id, session);

            // notify all
            ChatMessage::ptr nty(new ChatMessage);
            nty->set("type", "user_enter");
            nty->set("time", tide::Time2Str());
            nty->set("name", name);
            session_notify(nty, session);

            return SendMessage(session, rsp_msg);
        }
        else if (type == "send_request")
        {
            rsp_msg->set("type", "send_response");
            auto m = msg->get("msg");
            if (m.empty())
            {
                rsp_msg->set("result", "500");
                rsp_msg->set("msg", "msg is null");
                return SendMessage(session, rsp_msg);
            }

            if(id.empty())
            {
                rsp_msg->set("result", "501");
                rsp_msg->set("msg", "not logined");
                return SendMessage(session, rsp_msg);
            }

            rsp_msg->set("result", "200");
            rsp_msg->set("msg", "ok");
            
            ChatMessage::ptr nty(new ChatMessage);
            nty->set("type", "msg");
            nty->set("time", tide::Time2Str());
            nty->set("name", id);
            nty->set("msg", m);
            session_notify(nty, nullptr);
            
            return SendMessage(session, rsp_msg);

            // notify!!!
        }
        return 0;
    }

} // namespace chat