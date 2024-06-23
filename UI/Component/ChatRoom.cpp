#include <vector>
#include <deque>
#include <queue>
#include <sstream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "ChatRoom.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"

Engine::ChatRoom::ChatRoom(
    float x, float y, float w, float h, float anchorX, float anchorY,
    float marginTop, float marginBottom, float marginLeft, float marginRight,
    int scrollScale,
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
    IncomingMessages(std::queue<Message>()),
    View(std::deque<std::vector<Message>::iterator>()),
    YOffset(0),
    ScrollScale(scrollScale),
    MarginTop(marginTop),
    MarginBottom(marginBottom),
    MarginLeft(marginLeft),
    MarginRight(marginRight),
    Visible(true),
    Enable(true)
{
    std::shared_ptr<ALLEGRO_FONT> username_font = (Engine::Resources::GetInstance().GetFont(usernameFont, usernameFontSize));
    std::shared_ptr<ALLEGRO_FONT> content_font = (Engine::Resources::GetInstance().GetFont(contentFont, contentFontSize));
    MaxViewCount = 1 + h / std::max(al_get_font_line_height(username_font.get()), al_get_font_line_height(content_font.get()));
}

void Engine::ChatRoom::Update(float deltaTime) {
    while (!IncomingMessages.empty()) {
        AllMessages.push_back(IncomingMessages.front());
        IncomingMessages.pop();
    }
    if (AllMessages.empty())
        return;
    int offset_acc = YOffset;
    for (int i = AllMessages.size() - 1; i >= 0; i--) {
        AllMessages[i].Position.y = Position.y + Size.y + offset_acc - AllMessages[i].CalculateSize().y - MarginBottom;
        offset_acc -= AllMessages[i].CalculateSize().y;
    }
}

void Engine::ChatRoom::Draw() const {
    if (!Visible)
        return;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, BackgroundColor);
    if (AllMessages.empty())
        return;
    for (const Engine::Message& mes : AllMessages)
        mes.Draw();
    // for Margin
    al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + MarginTop, BackgroundColor);
    al_draw_filled_rectangle(Position.x, Position.y, Position.x + MarginLeft, Position.y + Size.y, BackgroundColor);
    al_draw_filled_rectangle(Position.x, Position.y + Size.y - MarginBottom, Position.x + Size.x, Position.y + Size.y, BackgroundColor);
    al_draw_filled_rectangle(Position.x + Size.x - MarginRight, Position.y, Position.x + Size.x, Position.y + Size.y, BackgroundColor);
    // for Outside
    al_draw_filled_rectangle(0, 0, w, Position.y, CoverageColor);
    al_draw_filled_rectangle(0, 0, Position.x, h, CoverageColor);
    al_draw_filled_rectangle(0, Position.y + Size.y, w, h, CoverageColor);
    al_draw_filled_rectangle(Position.x + Size.x, 0, Position.x + Size.x, h, CoverageColor);
}

void Engine::ChatRoom::Append(std::string name, std::string message, MessageType type, Engine::Point imageSize, float imageDisplayRatio) {
    Engine::Message msg = Engine::Message(
        Position.x + MarginLeft, 0, 0, 0,
        Size.x - MarginLeft - MarginRight, Position.y + Size.y - MarginBottom, Position.y + MarginTop,
        UsernameFont, UsernameFontSize, ContentFont, ContentFontSize,
        name, message, type,
        imageSize.x, imageSize.y, imageDisplayRatio
    );
    IncomingMessages.push(msg);
    // AllMessages.push_back(msg);
}

void Engine::ChatRoom::UpdateView(int deltaY) {
    if (AllMessages.empty())
        return;
    int head_line = AllMessages.begin()->Position.y;
    int lower_bound = Position.y + MarginTop;
    int last_line = AllMessages.rbegin()->CalculateSize().y + AllMessages.rbegin()->Position.y;
    int upper_bound = Position.y + Size.y - MarginBottom;
    if (deltaY < 0 && last_line < upper_bound
     || deltaY > 0 && head_line > lower_bound
    )   return;
    YOffset += deltaY;
}


void Engine::ChatRoom::ToggleEnable(bool status) {
    Enable = status;
}

void Engine::ChatRoom::ToggleVisible(bool status) {
    Visible = status;
}

void Engine::ChatRoom::OnMouseDown(int button, int mx, int my) {}

void Engine::ChatRoom::OnMouseUp(int button, int mx, int my) {}

void Engine::ChatRoom::OnMouseScroll(int mx, int my, int delta) {
    if (!Enable)
        return;
    UpdateView(delta * ScrollScale);
}

Engine::Message::Message(
    float x, float y, float anchorX, float anchorY, float maxDisplayWidth, float maxYAxis, float minYAxis,
    std::string usernameFont, int usernameFontSize, std::string contentFont, int contentFontSize, 
    std::string name, std::string content, MessageType type, int imageWidth, int imageHeight, float imageDisplayRatio,
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
    ContentFont(Resources::GetInstance().GetFont(contentFont, contentFontSize)),
    ImageDisplay(nullptr),
    ImageBitmapSize(Engine::Point(imageWidth, imageHeight)),
    ImageDisplaySize(Engine::Point(imageWidth * imageDisplayRatio, imageHeight * imageDisplayRatio)),
    ImageDisplayRatio(imageDisplayRatio)
{
    UpdateDisplayContent();
}

void Engine::Message::UpdateDisplayContent() {
    if (Type == MessageType::Image) {
        auto map_char_to_colorcode = [](unsigned char c) -> int {
            switch (c) {
                case '0':
                    return 0xFFFFFF;
                case '1':
                    return 0x000000;
                case '2':
                    return 0x0000FF;
                case '3':
                    return 0x00FF00;
                case '4':
                    return 0xFF0000;
                default:
                    return 0x000000;
            }
        };

        if (ImageDisplay == nullptr)
            al_destroy_bitmap(ImageDisplay);
        ImageDisplay = al_create_bitmap(ImageBitmapSize.x, ImageBitmapSize.y);
        ALLEGRO_LOCKED_REGION* locked = al_lock_bitmap(ImageDisplay, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READWRITE);
        if (!locked) {
            LOG(LogType::ERROR) << "Failed to lock bitmap for bucket fill!";
            return;
        }
        unsigned char* data = static_cast<unsigned char*>(locked->data);
        const int pitch = locked->pitch;
        const int pixel_size = al_get_pixel_size(locked->format);
        for (int y = 0; y < ImageBitmapSize.y; y++) {
            unsigned char* row = data + y * pitch;
            for (int x = 0; x < ImageBitmapSize.x; x++) {
                unsigned char* pixel = row + x * pixel_size;
                char encoded_color = Content[ImageBitmapSize.x * y + x];
                // Mapping colors to characters as per the rules
                int decoded_colorcode = map_char_to_colorcode(encoded_color);
                pixel[0] = (decoded_colorcode >> 16) & (0xFF);
                pixel[1] = (decoded_colorcode >>  8) & (0xFF);
                pixel[2] = (decoded_colorcode >>  0) & (0xFF);
                pixel[3] = 0xFF;
            }
        }
        al_unlock_bitmap(ImageDisplay);
        return;
    }
    // Type == MessageType::PlainText
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
    if (Type == MessageType::Image) {
        return Engine::Point(
            std::max(al_get_text_width(UsernameFont.get(), Name.c_str()), static_cast<int>(ImageDisplaySize.x)),
            al_get_font_line_height(UsernameFont.get()) + ImageDisplaySize.y + /* magic number */ 10
        );
    }
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
    
    // draw Username
    if (!(
        Position.y + username_height < MinYAxis
     || Position.y > MaxYAxis
    )) {
        al_draw_text(UsernameFont.get(), UsernameColor, Position.x, Position.y, ALLEGRO_ALIGN_LEFT, Name.c_str());
    }
    
    // draw Content
    if (Type == MessageType::PlainText) {
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
                ContentColor, Position.x, Position.y + username_height + content_height * (shift - 1), ALLEGRO_ALIGN_LEFT, content.c_str()
            );
        }
    } else {
        int upper = Position.y + username_height;
        int lower = Position.y + username_height + ImageDisplaySize.y;
        if (upper > MaxYAxis)
            return;
        if (lower < MinYAxis)
            return;
        // al_draw_filled_circle(Position.x, Position.y + username_height, 5, {1, 1, 0, 1});
        // al_draw_filled_circle(Position.x + ImageDisplaySize.x, Position.y + username_height + ImageDisplaySize.y, 5, {1, 1, 0, 1});
        al_draw_scaled_bitmap(
            ImageDisplay,
            0, 0, al_get_bitmap_width(ImageDisplay), al_get_bitmap_height(ImageDisplay),
            Position.x, Position.y + username_height, ImageDisplaySize.x, ImageDisplaySize.y,
            ALLEGRO_ALIGN_LEFT
        );
    }
}
