#include "anepch.h"
#include "Logger.h"

namespace Engine
{
    std::queue<std::string_view>  Logger::Texts;

    void Logger::SubmitText(const std::string_view message)
    {
        Texts.push(message);
    }

    bool Logger::GetText(std::string_view& textToPrint)
    {
        if (!Texts.empty())
        {
            textToPrint = Texts.front();
            Texts.pop();
            return true;
        }
        return false;
    }
}
