#include <MessageManager.h>

MessageManager::MessageManager(float width, float height)
{
    resizeDimensions(width, height);
}

void MessageManager::resizeDimensions(float width, float height)
{
    this->width = width;
    this->height = height;
}

void MessageManager::addMessage(string msg)
{
    Timer *newTimer = new Timer();
    newTimer->start();

    if (messages.size() >= 5)
    {
        messages.erase(messages.begin());
        messageTimers.erase(messageTimers.begin());
    }

    messages.push_back(msg);
    messageTimers.push_back(newTimer);
}

void MessageManager::displayMessages(SDL_Renderer *renderer)
{
    for (int i = 0; i < messages.size(); i++)
    {
        Utilities::displayMessage(renderer, width, height, messages[i], i + 1);
    }

    if (!messages.empty() && !messageTimers.empty())
    {
        if (messageTimers[0]->getTicks() / 1000 >= 3)
        {
            messages.erase(messages.begin());
            messageTimers.erase(messageTimers.begin());
        }
    }
}
