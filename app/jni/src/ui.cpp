//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/ui.h"
#include "../headers/rendering.h"
#include "../headers/holds.h"
#include <iterator>
#include <string>

UIElement::UIElement(const std::string& id, int x, int y, int w, int h) 
: id(id), rect({x, y, w, h}) {
    this->black = {0, 0, 0, 255};
    this->gray = {200, 200, 200, 255};
}

UIElement::~UIElement()  { }

bool UIElement::isClicked(int mouseX, int mouseY) {
    applyScroll();
    bool a = (mouseX >= this->rect.x && mouseX <= this->rect.x + this->rect.w && mouseY >= this->rect.y && mouseY <= this->rect.y + this->rect.h);
    revertScroll();
    return a;
}

std::string UIElement::getId() {
    return this->id;
}

void UIElement::render(SDL_Renderer* renderer) {}
void UIElement::hover(int mouseX, int mouseY) {}

void UIElement::applyScroll() {
    this->rect.y += this->scroll;
}

void UIElement::revertScroll() {
    this->rect.y -= this->scroll;
}

void UIElement::setScroll(int scroll) {
    this->scroll = scroll;
}

IconCard::IconCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, int borderRadius, SDL_Color color, SDL_Color hoverColor, const std::string text, SDL_Texture* icon, SDL_Color iconColor, bool drawInCircle) 
    : UIElement(id, x, y, w, h), color(color), hoverColor(hoverColor), icon(icon), iconColor(iconColor), borderRadius(borderRadius), drawInCircle(drawInCircle) {
    this->text = getTextureFromText(renderer, font, text, &black, &textW, &textH);
}

IconCard::~IconCard() {
    SDL_DestroyTexture(text);
}

void IconCard::render(SDL_Renderer* renderer) {
    bool corners[4] = {true, true, true, true};

    if (color.a != 0) {
        // scuffed drop shadow
        SDL_SetRenderDrawColor(renderer, 183, 174, 169, 255);
        SDL_Rect shadowRect = {
                rect.x - 5,
                rect.y + 5,
                rect.w,
                rect.h
        };
        SDL_RenderFillRoundedRect(renderer, shadowRect.x, shadowRect.y, shadowRect.w, shadowRect.h, borderRadius, corners);

        //card background
        if (this->isHovered) SDL_SetRenderDrawColor(renderer, this->hoverColor);
        else SDL_SetRenderDrawColor(renderer, this->color);
        SDL_RenderFillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, borderRadius, corners);
    }

    if (icon != nullptr) { 
        SDL_SetRenderDrawColor(renderer, iconColor);
        if (drawInCircle) {
            renderIconInCircle(renderer, rect.x + rect.w - 100, rect.y + rect.h / 2, 40, 10, 5, changeColorOfTexture(icon, &iconColor));
        } else {
            SDL_Rect iconRect = {rect.x + rect.w - 100 - 35, rect.y + rect.h / 2 - 35, 70, 70};
            SDL_RenderCopy(renderer, changeColorOfTexture(icon, &iconColor), nullptr, &iconRect);
        }
    }

    SDL_Rect textRect = {
            rect.x + 40,
            rect.y + rect.h / 2 - textH / 2,
            textW,
            textH,
    };
    SDL_RenderCopy(renderer, text, nullptr, &textRect);
}

void IconCard::hover(int mouseX, int mouseY) {
    if (this->isClicked(mouseX, mouseY)) { isHovered = true; } 
    else { isHovered = false; }
    SDL_Log("%i", isHovered);
}

BigValueCard::BigValueCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* mainFont, TTF_Font* valueFont, int x, int y, int w, int h, int borderRadius, SDL_Color color, SDL_Color iconColor, SDL_Texture* plusIcon, SDL_Texture* minusIcon, const std::string& title, const std::string& value) 
: UIElement(id, x, y, w, h), borderRadius(borderRadius), color(color), iconColor(iconColor), minusIcon(minusIcon), plusIcon(plusIcon) {
    this->title = getTextureFromText(renderer, mainFont, title, &black, &titleW, &titleH);
    this->value = getTextureFromText(renderer, valueFont, value, &gray, &valueW, &valueH);
    this->min = getTextureFromText(renderer, mainFont, "Min", &black, &minW, &minH);
    this->max = getTextureFromText(renderer, mainFont, "Max", &black, &maxW, &maxH);
}

BigValueCard::~BigValueCard() {
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(value);
    SDL_DestroyTexture(min);
    SDL_DestroyTexture(max);
}

void BigValueCard::render(SDL_Renderer* renderer) {
    bool corners[4] = {true, true, true, true};

    // scuffed drop shadow
    SDL_SetRenderDrawColor(renderer, 183, 174, 169, 255);
    SDL_Rect shadowRect = {
            rect.x - 5,
            rect.y + 5,
            rect.w,
            rect.h
    };
    SDL_RenderFillRoundedRect(renderer, shadowRect.x, shadowRect.y, shadowRect.w, shadowRect.h, borderRadius, corners);

    //base
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, borderRadius, corners);

    //title
    SDL_Rect titleRect = {
            rect.x + 40,
            rect.y + 40 ,
            titleW,
            titleH,
    };
    SDL_RenderCopy(renderer, title, nullptr, &titleRect);

    //bar
    SDL_Rect bar = {
            rect.x + 40,
            rect.y + titleH + 2 * 40,
            rect.w - 2 * 40,
            2,
    };
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &bar);

    //first value
    SDL_Rect firstValueRect = {
            rect.x + 40,
            rect.y + titleH + bar.h + 3 * 40,
            minW,
            minH,
    };
    SDL_RenderCopy(renderer, min, nullptr, &firstValueRect);

    //second value
    SDL_Rect secondValueRect = {
            rect.x + 40,
            rect.y + titleH + minH + bar.h + 4 * 40,
            maxW,
            maxH,
    };
    SDL_RenderCopy(renderer, max, nullptr, &secondValueRect);

    //changing value
    SDL_Rect valueRect = {
            rect.x + rect.w - valueH - 80,
            titleRect.y + titleH - valueH,
            valueW,
            valueH,

    };

    SDL_RenderCopy(renderer, value, nullptr, &valueRect);

    //icons
    int iconR = 38;
    SDL_Rect iconRects[4];
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            iconRects[x * 2 + y].x = rect.x + rect.w - iconR - 40 * (x + 1) - 80 * x;
            if (y == 0) iconRects[x * 2 + y].y = firstValueRect.y + minH / 2;
            else iconRects[x * 2 + y].y = secondValueRect.y + maxH / 2;
            iconRects[x * 2 + y].w = iconR * 2;
            iconRects[x * 2 + y].h = iconR * 2;
        }
    }

   // for (int i = 0; i < 4; i++) {
   //     hitboxes[i] = iconRects[i];
   // }

    SDL_Texture* icons[4] ={
          plusIcon , plusIcon, minusIcon, minusIcon,
    };
    SDL_SetRenderDrawColor(renderer, iconColor);
    for (int i = 0; i < 4; i++) {
        renderIconInCircle(renderer, iconRects[i].x, iconRects[i].y, iconR, 10, 5, changeColorOfTexture(icons[i], &iconColor));
    }
}

Text::Text(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, const std::string& text, SDL_Color color) 
    : UIElement(id, x, y, 0, 0) {
    int w, h;
    this->texture = getTextureFromText(renderer, font, text, &color, &w, &h);
    this->rect.w = w;
    this->rect.h = h;
}

Text::~Text() {
    SDL_DestroyTexture(texture);
}

void Text::changeText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text) {
    SDL_DestroyTexture(texture);
    int w, h;
    this->texture = getTextureFromText(renderer, font, text, &color, &w, &h);
    this->rect.w = w;
    this->rect.h = h;
}

void Text::render(SDL_Renderer* renderer) {
    SDL_Rect renderRect = {
        this->rect.x - this->rect.w / 2,
        this->rect.y - this->rect.h / 2,
        this->rect.w,
        this->rect.h,
    };
    SDL_RenderCopy(renderer, this->texture, nullptr, &renderRect);
}

Image::Image(const std::string& id, SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Texture* texture, SDL_Color color)
    : UIElement(id, x, y, w, h), color(color), texture(texture) { }

Image::~Image() {}

void Image::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, changeColorOfTexture(this->texture, &this->color), nullptr, &this->rect);
}

void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect) {
    SDL_RenderCopy(renderer, buttonImage, nullptr, buttonRect);
}

void drawSettingsButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect) {
    SDL_RenderCopy(renderer, buttonImage, nullptr, buttonRect);
}

void drawGenerateButton(SDL_Renderer* renderer, int windowW, SDL_Texture* buttonImage, SDL_Rect* imageRect) {
    bool corners[4] = {true, false, true, false};
    SDL_RenderFillRoundedRect(renderer, windowW- 170, 150, 170, 130, 40, corners);
    SDL_RenderCopy(renderer, buttonImage, nullptr, imageRect);
}

void drawMainImage(SDL_Renderer* renderer, int windowH, int windowW, SDL_Texture* mainImage, SDL_Rect* mainImageRect, SDL_Texture* mainImageMask) {
    if (mainImage != nullptr) {
        renderImageWithMask(renderer, mainImage, mainImageRect, mainImageMask); // render main image
    }
}

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage) {
    bool corners[4] = {true, false, true, false};
    SDL_RenderFillRoundedRect(renderer, x, y, w, h, 50, corners);
    for(int i = 0; i < 5; i++) {
        SDL_SetRenderDrawColor(renderer, solidColors[i]);
        SDL_RenderDrawCircle(renderer, x + w / 2, y + 30 + holdRadius + (30 + 76) * i, holdRadius, holdWidth);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawCircle(renderer, x + w / 2, y + 30 + holdRadius + (30 + 76) * 5, holdRadius, holdWidth);
    SDL_Rect rect = {
            x + w / 2 - 18,
            y + h - w / 2 - 25,
            36,
           36
    };
    SDL_RenderCopy(renderer, deleteImage, nullptr, &rect);
}

UIHandler::UIHandler() {}

UIHandler::~UIHandler() {
    for (auto& element : elements) {
        delete element;
    }
}

void UIHandler::addElement(UIElement* element) {
    elements.push_back(element);
}

void UIHandler::render(SDL_Renderer* renderer, const std::vector<std::string>& ids) {
    if (ids.empty()) {
        for (auto& element : elements) {
            element->applyScroll();
            element->render(renderer);
            element->revertScroll();
        }
    } else {
        for (auto& element : elements) {
            if (std::find(ids.begin(), ids.end(), element->getId()) != ids.end()) {
                element->applyScroll();
                element->render(renderer);
                element->revertScroll();
            }
        }
    }
}

void UIHandler::removeElement(const std::string& id) {
        elements.erase(std::remove_if(elements.begin(), elements.end(), 
                    [&id](UIElement* element) {
                        return element->getId() == id;
                    }), elements.end());
}

std::string UIHandler::handleClick(int mouseX, int mouseY, const std::vector<std::string>& ids) {
    if (ids.empty()) {
        for (auto& element : elements) {
            if (element-> isClicked(mouseX, mouseY)) {
                return element->getId();
            }
        }
    } else {
        for (auto& element : elements) {
            if (std::find(ids.begin(), ids.end(), element->getId()) != ids.end()) {
                if (element->isClicked(mouseX, mouseY)) {
                    return element->getId();
                }
            }
        }
    }
    return "";
}

void UIHandler::scroll(int scroll, const std::vector<std::string>& ids) {
    if (ids.empty()) {
        for (auto& element : elements) {
            element->setScroll(scroll);
        }
    } else {
        for (auto& element : elements) {
            if (std::find(ids.begin(), ids.end(), element->getId()) != ids.end()) {
                element->setScroll(scroll);
            }
        }
    }
}

void UIHandler::hover(int mouseX, int mouseY, const std::vector<std::string>& ids) {
    if (ids.empty()) {
        for (auto& element : elements) {
            element->hover(mouseX, mouseY);
        }
    } else {
        for (auto& element : elements) {
            if (std::find(ids.begin(), ids.end(), element->getId()) != ids.end()) {
                element->hover(mouseX, mouseY);
            }
        }
    }
}
