#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#include <string>
#include <map>
#include <memory>

namespace chat
{
    class ChatMessage
    {
    public:
        using ptr = std::shared_ptr<ChatMessage>;

        static ChatMessage::ptr Create(const std::string &msg);
        ChatMessage();
        
        std::string get(const std::string &name);
        void set(const std::string &name, const std::string &value);

        std::string toString() const;
    
    private:
        std::map<std::string, std::string> m_datas;
    };
}

#endif // PROTOCOL_H__