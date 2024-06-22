#include <vector>
#include <deque>
#include <sstream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "ChatRoom.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"

Engine::ChatRoom::ChatRoom(
    float x, float y, float w, float h, float anchorX, float anchorY,
    float marginTop, float marginBottom, float marginLeft, float marginRight,
    std::string usernameFont, int usernameFontSize, std::string contentFont, int contentFontSize,
    ALLEGRO_COLOR backgroundColor,
    ALLEGRO_COLOR coverageColor,
    ALLEGRO_COLOR usernameFontColor,
    ALLEGRO_COLOR contentFontColor
):
    IObject(x, y, w, h, anchorX, anchorY),
    UsernameFont(usernameFont),
    UsernameFontSize(usernameFontSize),
    ContentFont(contentFont),
    ContentFontSize(contentFontSize),
    BackgroundColor(backgroundColor),
    CoverageColor(coverageColor),
    UsernameFontColor(usernameFontColor),
    ContentFontColor(contentFontColor),
    AllMessages(std::vector<Message>()),
    View(std::deque<std::vector<Message>::iterator>()),
    YOffset(0),
    MarginTop(marginTop),
    MarginBottom(marginBottom),
    MarginLeft(marginLeft),
    MarginRight(marginRight)
{
    std::shared_ptr<ALLEGRO_FONT> username_font = (Engine::Resources::GetInstance().GetFont(usernameFont, usernameFontSize));
    std::shared_ptr<ALLEGRO_FONT> content_font = (Engine::Resources::GetInstance().GetFont(contentFont, contentFontSize));
    MaxViewCount = 1 + h / std::max(al_get_font_line_height(username_font.get()), al_get_font_line_height(content_font.get()));
}

void Engine::ChatRoom::Update(float deltaTime) {
    if (AllMessages.empty())
        return;
    int offset_acc = YOffset;
    for (int i = AllMessages.size() - 1; i >= 0; i--) {
        AllMessages[i].Position.y = Position.y + Size.y + offset_acc - AllMessages[i].CalculateSize().y;
        offset_acc -= AllMessages[i].CalculateSize().y;
    }
}

void Engine::ChatRoom::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, BackgroundColor);
    if (AllMessages.empty())
        return;
    for (const Engine::Message& mes : AllMessages)
        mes.Draw();
    al_draw_filled_rectangle(0, 0, w, Position.y, CoverageColor);
    al_draw_filled_rectangle(0, 0, Position.x, h, CoverageColor);
    al_draw_filled_rectangle(0, Position.y + Size.y, w, h, CoverageColor);
    al_draw_filled_rectangle(Position.x + Size.x, 0, Position.x + Size.x, h, CoverageColor);
}

void Engine::ChatRoom::Append(std::string name, std::string message) {
    Engine::Message msg = Engine::Message(Position.x + MarginLeft, 0, 0, 0, Size.x - MarginLeft - MarginRight, Position.y + Size.y, Position.y, UsernameFont, UsernameFontSize, ContentFont, ContentFontSize, name, message);
    AllMessages.push_back(msg);
    // auto scroll
    // if (View.)
}

void Engine::ChatRoom::UpdateView(int deltaY) {
    if (AllMessages.empty())
        return;
    int head_line = AllMessages.begin()->Position.y;
    int lower_bound = Position.y;
    int last_line = AllMessages.rbegin()->CalculateSize().y + AllMessages.rbegin()->Position.y;
    int upper_bound = Position.y + Size.y;
    if (deltaY < 0 && last_line < upper_bound
     || deltaY > 0 && head_line > lower_bound
    )   return;
    YOffset += deltaY;
}

void Engine::ChatRoom::OnMouseDown(int button, int mx, int my) {}

void Engine::ChatRoom::OnMouseUp(int button, int mx, int my) {}

void Engine::ChatRoom::OnMouseScroll(int mx, int my, int delta) {
    UpdateView(delta * 5);
}

Engine::Message::Message(
    float x, float y, float anchorX, float anchorY, float maxDisplayWidth, float maxYAxis, float minYAxis,
    std::string usernameFont, int usernameFontSize, std::string contentFont, int contentFontSize, 
    std::string name, std::string content, MessageType type,
    ALLEGRO_COLOR usernameColor,
    ALLEGRO_COLOR contentColor
):
    IObject(x, y, 0, 0, anchorX, anchorY),
    MaxDisplayWidth(maxDisplayWidth),
    MaxYAxis(maxYAxis),
    MinYAxis(minYAxis),
    Name(name),
    Content(content),
    DisplayContent(std::list<std::string>()),
    Type(type),
    UsernameColor(usernameColor),
    ContentColor(contentColor),
    UsernameFont(Resources::GetInstance().GetFont(usernameFont, usernameFontSize)),
    ContentFont(Resources::GetInstance().GetFont(contentFont, contentFontSize))
{
    UpdateDisplayContent();
}

void Engine::Message::UpdateDisplayContent() {
    DisplayContent.clear();
    std::stringstream ss(Content);
    std::string element;
    std::string buffer;
    while (ss >> element) {
        std::string next_possible = buffer + element;
        if (al_get_text_width(ContentFont.get(), next_possible.c_str()) < MaxDisplayWidth) {
            buffer = next_possible + ' ';
        } else if (!buffer.empty() && al_get_text_width(ContentFont.get(), element.c_str()) < MaxDisplayWidth) {
            buffer.pop_back();
            DisplayContent.emplace_back(buffer);
            buffer = element + ' ';
        } else if (!buffer.empty()) {
            goto UPDATE_DISPLAY_CONTENT_SPLIT;
        } else {
            UPDATE_DISPLAY_CONTENT_SPLIT:
            for (int i = 0; i < element.length(); i++) {
                char c = element[i];
                buffer += c;
                // buffer      - [xxxxxx c]
                // restriction - [       ]
                if (al_get_text_width(ContentFont.get(), buffer.c_str()) >= MaxDisplayWidth && i == 0) {
                    buffer.pop_back();
                    buffer.pop_back();
                    DisplayContent.emplace_back(buffer);
                    buffer = c;
                }
                // buffer      - [xxxx aac]
                // restriction - [       ]
                else if (al_get_text_width(ContentFont.get(), buffer.c_str()) >= MaxDisplayWidth) {
                    // try [xxxx aa-]
                    buffer.back() = '-';
                    if (al_get_text_width(ContentFont.get(), buffer.c_str()) < MaxDisplayWidth) {
                        DisplayContent.emplace_back(buffer);
                        buffer = c;
                    }
                    // go [xxxx a-]
                    //    [ac]
                    else {
                        buffer.pop_back();
                        char last = buffer.back();
                        buffer.back() = '-';
                        DisplayContent.emplace_back(buffer);
                        buffer.clear();
                        buffer += last;
                        buffer += c;
                    }
                }
                // buffer      - [xxxx aac]
                // restriction - [              ]
                // keep going
            }
            buffer += ' ';
        }
    }
    buffer.pop_back();
    DisplayContent.emplace_back(buffer);
}

Engine::Point Engine::Message::CalculateSize() const {
    int max_curr_width = 0;
    for (const std::string& content : DisplayContent)
        max_curr_width = std::max(max_curr_width, al_get_text_width(ContentFont.get(), content.c_str()));
    return Engine::Point(
        std::max(al_get_text_width(UsernameFont.get(), Name.c_str()), max_curr_width),
        al_get_font_line_height(UsernameFont.get()) + al_get_font_line_height(ContentFont.get()) * DisplayContent.size()
    );
}

void Engine::Message::Draw() const {
    int username_height = al_get_font_line_height(UsernameFont.get());
    int content_height = al_get_font_line_height(ContentFont.get());
    
    if (!(
        Position.y + username_height < MinYAxis
     || Position.y > MaxYAxis
    )) {
        al_draw_text(UsernameFont.get(), UsernameColor, Position.x, Position.y, ALLEGRO_ALIGN_LEFT, Name.c_str());
    }
    
    int shift = 0;
    for (const std::string& content : DisplayContent) {
        int upper = Position.y + username_height + content_height * shift;
        int lower = Position.y + username_height + content_height * (shift + 1);
        shift++;
        if (upper > MaxYAxis)
            continue;
        if (lower < MinYAxis)
            continue;
        al_draw_text(
            ContentFont.get(),
            ContentColor, Position.x, Position.y + username_height + content_height * (shift - 1) , ALLEGRO_ALIGN_LEFT, content.c_str()
        );
    }
}
