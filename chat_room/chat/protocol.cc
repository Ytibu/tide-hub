#include "protocol.h"

#include <sstream>
#include <jsoncpp/json/json.h>
#include <tide/util/json_util.h>
#include <memory>

namespace chat
{

    ChatMessage::ChatMessage()
    {
    }

    ChatMessage::ptr ChatMessage::Create(const std::string &msg)
    {
        Json::Value val;
        if(!tide::JsonUtil::FromString(val, msg))
        {
            return nullptr;
        }

        ChatMessage::ptr rt(new ChatMessage);
        for(auto &i : val.getMemberNames())
        {
            rt->m_datas[i] = val[i].asString();
        }
        return rt;
    }

    std::string ChatMessage::get(const std::string &name)
    {
        auto it = m_datas.find(name);
        return it == m_datas.end() ? "" : it->second;
    }

    void ChatMessage::set(const std::string &name, const std::string &value)
    {
        m_datas[name] = value;
    }

    std::string ChatMessage::toString() const
    {
        Json::Value val;
        for(auto &i : m_datas)
        {
            val[i.first] = i.second;
        }
        return tide::JsonUtil::ToString(val);
    }

} // namespace chat