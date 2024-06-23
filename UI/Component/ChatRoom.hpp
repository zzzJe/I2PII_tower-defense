#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <memory>
#include <list>
#include <allegro5/allegro_font.h>
#include <allegro5/color.h>
#include "Engine/IObject.hpp"
#include "Engine/IControl.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class Message; // for scope recognition
    class ChatRoom; // for scope recognition

    enum MessageType {
        PlainText,
        Image,
    };

    class Message : public IObject {
    public:
        friend class ChatRoom;
    private:
        float MaxDisplayWidth;
        float MaxYAxis;
        float MinYAxis;
        std::string Name;
        std::string Content;
        std::list<std::string> DisplayContent;
        ALLEGRO_BITMAP* ImageDisplay;
        Engine::Point ImageDisplaySize;
        Engine::Point ImageBitmapSize;
        float ImageDisplayRatio;
        MessageType Type;
        ALLEGRO_COLOR UsernameColor;
        ALLEGRO_COLOR ContentColor;
        std::shared_ptr<ALLEGRO_FONT> UsernameFont;
        std::shared_ptr<ALLEGRO_FONT> ContentFont;
    public:
        Message() = default;
        Message(
            float x, float y, float anchorX, float anchorY, float maxDisplayWidth, float maxYAxis, float minYAxis,
            std::string usernameFont, int usernameFontSize, std::string contentFont, int contentFontSize, 
            std::string name, std::string content, MessageType type = PlainText, int imageWidth = 600, int imageHeight = 400, float imageDisplayRatio = .5,
            ALLEGRO_COLOR usernameColor = {0.6, 0.6, 0.8, 1},
            ALLEGRO_COLOR contentColor = {1, 1, 1, 1}
        );
        void UpdateDisplayContent();
        Engine::Point CalculateSize() const;
        void Draw() const override;
    };
    
    class ChatRoom :
        public Engine::IObject,
        public Engine::IControl
    {
    private:
        ALLEGRO_COLOR BackgroundColor;
        ALLEGRO_COLOR CoverageColor;
        ALLEGRO_COLOR UsernameFontColor;
        ALLEGRO_COLOR ContentFontColor;
        std::string UsernameFont;
        std::string ContentFont;
        int UsernameFontSize;
        int ContentFontSize;
        std::vector<Message> AllMessages;
        std::queue<Message> IncomingMessages;
        std::deque<std::vector<Message>::iterator> View;
        int MaxViewCount;
        int YOffset;
        float MarginTop;
        float MarginLeft;
        float MarginRight;
        float MarginBottom;
        int ScrollScale;
        bool Visible;
        bool Enable;
    public:
        ChatRoom(
            float x, float y , float w, float h, float anchorX, float anchorY,
            float marginTop, float marginBottom, float marginLeft, float marginRight,
            int scrollScale,
            std::string usenameFont, int usernameFontSize, std::string contentFont, int contentFontSize, 
            ALLEGRO_COLOR backgroundColor = {0.153, 0.153, 0.153, 1},
            ALLEGRO_COLOR coverageColor = {0, 0, 0, 1},
            ALLEGRO_COLOR usernameFontColor = {0.6, 0.6, 0.8, 1},
            ALLEGRO_COLOR contentFontColor = {1, 1, 1, 1}
        );
        ~ChatRoom() = default;
        void Append(std::string name, std::string message, MessageType type, Engine::Point imageSize = Engine::Point(0, 0), float imageDisplayRatio = .5);
        void UpdateView(int deltaY);
        void Draw() const override;
        void ToggleEnable(bool status = true);
        void ToggleVisible(bool status = true);
        void Update(float deltaTime) override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseUp(int button, int mx, int my) override;
        void OnMouseScroll(int mx, int my, int delta) override;
    };
}

#endif // CHATROOM_HPP
