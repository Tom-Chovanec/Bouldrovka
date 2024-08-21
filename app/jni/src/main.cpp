#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "iostream"
#include "array"
#include <android/log.h>
#include <jni.h>
#include "string"
#include "vector"
#include "memory"
#include "fstream"
#include "cmath"
#include "filesystem"
#include "unordered_map"
#include "../headers/holds.h"
#include "../headers/ui.h"
#include "../headers/rendering.h"
#include "../headers/utility.h"

bool changeImage = true;

void openImagePicker() {
    auto* env = static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv());
    auto activity = static_cast<jobject>(SDL_AndroidGetActivity());
    jclass clazz = env->FindClass("com/bouldrovka/app/Bouldrovka");
    jmethodID methodId = env->GetMethodID(clazz, "openImagePicker", "()V");
    env->CallVoidMethod(activity, methodId);
    env->DeleteLocalRef(clazz);
}

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer) {
    // Load image at specified path
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == nullptr) {
        SDL_Log("Unable to load image %s ! SDL_image Error: %s ", path.c_str(), IMG_GetError());
    }
    return newTexture;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bouldrovka_app_Bouldrovka_onImagePicked(JNIEnv *env, jobject obj) {
    changeImage = true;
}

int WINDOW_HEIGHT = 1920;
int WINDOW_WIDTH = 1080;

int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error while SDL init: " << SDL_GetError();
        return -1;
    }
    return 0;
}

SDL_Window* createWindow(int width, int height) {
    SDL_Window* window = nullptr;
    window = SDL_CreateWindow("Boulder gen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN);
    if (window == nullptr) {
        std::cout << "Error while creating window: " << SDL_GetError();
        return nullptr;
    }
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        return nullptr;
    }
    return renderer;
}




/*
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------
 */

int main(int argc, char *args[]) {
    if (init() < 0) {
        return -1;
    }
    if (TTF_Init() < 0) {
        SDL_Log("Eror with ttf: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_Window *gWindow = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (gWindow == nullptr) {
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Renderer *gRenderer = createRenderer(gWindow);
    if (gRenderer == nullptr) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* gFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 78);
    if (gFont == nullptr) {
        printf("TTF_OpenFont Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_GetWindowSize(gWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);

    // -----------------------------------------------------------------------------------------------------------
    enum {
        INTRO = 0,
        SELECT,
        MAIN,
        OPTIONS,
    } scene;

    scene = MAIN;

    enum {
        PLACING = 0,
        DELETING,
        OPENING_GALLERY,
        GENERATING,
    } state;

    HoldType holdType;
    bool placeHolds = true;

    std::unordered_map<std::string, SDL_Color> colors;

    colors["primary"] = {255, 106, 20 ,255};
    colors["secondary"] = {255, 241, 233 ,255};
    colors["blueAccent"] = {255, 241, 233 ,255};
    colors["orangeAccent"] = {255, 241, 233 ,255};
    colors["redAccent"] = {255, 241, 233 ,255};
    colors["darkRedAccent"] = {255, 241, 233 ,255};
    colors["black"] = {0, 0, 0 ,255};
    colors["white"] = {255, 255, 255 ,255};

    std::unordered_map<std::string, SDL_Texture*> textures;

    textures["mainImage"] = nullptr;
    textures["logoImage"]= loadTexture("images/logo.png", gRenderer);
    textures["threeBarsImage"]= loadTexture("images/3_bars.png", gRenderer);
    textures["leftArrowImage"]= loadTexture("images/left_arrow.png", gRenderer);
    textures["crossImage "]= loadTexture("images/cross.png", gRenderer);
    textures["topRightArrowImage"] = loadTexture("images/top_right_arrow.png", gRenderer);
    textures["penImage"] = loadTexture("images/pen.png", gRenderer);
    textures["backgroundBlobsImage"] = loadTexture("images/background_blobs.png", gRenderer);
    textures["blackMinusImage"] = loadTexture("images/black_minus.png", gRenderer);
    textures["blackPlusImage"] = loadTexture("images/black_plus.png", gRenderer);
    textures["whiteMinusImage"] = loadTexture("images/white.png", gRenderer);
    textures["whitePlusImage"] = loadTexture("images/black_plus.png", gRenderer);

    SDL_Texture* generalOptionCardIcons[4] = {
            textures["penImage"],
            textures["penImage"],
            textures["topRightArrowImage"],
            nullptr
    };


    float ratio = 9.0f / 16.0f;
    SDL_Rect mainImageRect;
    mainImageRect.w = WINDOW_WIDTH;
    mainImageRect.h = static_cast<int>(mainImageRect.w / ratio);
    mainImageRect.x = 0;
    mainImageRect.y = WINDOW_HEIGHT - mainImageRect.h;

    SDL_SetRenderDrawColor(gRenderer, colors["secondary"]);
    textures["mainImageMask"] = createRoundedRectMask(gRenderer, mainImageRect.w, mainImageRect.h, 60);


    SDL_Rect generateButtonRect = {
            WINDOW_WIDTH - 145,
            170,
            90,
            90
    };

    SDL_Rect settingsButtonRect = {
            WINDOW_WIDTH - 120,
            150 / 2 - 30,
            60,
            60
    };

    SDL_Rect backButtonRect = {
            60,
            150 / 2 - 30,
            60,
            60
    };

    SDL_Rect generalOptionCardRects[4];

    for (int i = 0; i < 4; i++) {
        generalOptionCardRects[i].x = 50;
        generalOptionCardRects[i].h = 200;
        generalOptionCardRects[i].w = WINDOW_WIDTH - 2 * generalOptionCardRects[i].x;
        generalOptionCardRects[i].y = 170 + 155 + (generalOptionCardRects[i].h + generalOptionCardRects[i].x) * i;
    }

    SDL_Rect generalOptionCardIconRect[3];

    for (int i = 0; i < 3; i++) {
        generalOptionCardIconRect[i].w = 76;
        generalOptionCardIconRect[i].h = 76;
        generalOptionCardIconRect[i].x = WINDOW_WIDTH - 150 - generalOptionCardIconRect[i].w / 2;
        generalOptionCardIconRect[i].y = generalOptionCardRects[i].y + generalOptionCardRects[i].h / 2 - generalOptionCardIconRect[i].h / 2;
    }

    SDL_Rect generationOptionCardRects[6];

    for (int i = 0; i < 6; i++) {
        generationOptionCardRects[i].x = 50;
        generationOptionCardRects[i].h = 450;
        generationOptionCardRects[i].w = WINDOW_WIDTH - 2 * generationOptionCardRects[i].x;
        generationOptionCardRects[i].y = generalOptionCardRects[3].y + generalOptionCardRects[3].h + 170 + (generationOptionCardRects[i].h + generationOptionCardRects[i].x) * i;
    }

    int selectMenuX = WINDOW_WIDTH - 120;
    int selectMenuY = 500;
    int selectMenuW = 120;
    int selectMenuH = 666;
    int selectMenuHoldRadius = 38;
    int selectMenuHoldWidth = 10;
    SDL_Rect selectMenuRect = {selectMenuX, selectMenuY, selectMenuW, selectMenuH};
    SDL_Rect selectMenuHitBoxes[6];

    for (int i = 0; i < 6; i++) {
        int x = selectMenuX + (selectMenuW / 2) - selectMenuHoldRadius;
        int y = selectMenuY + 30 + (selectMenuHoldRadius * 2 + 30) * i;
        int w = selectMenuHoldRadius * 2;
        selectMenuHitBoxes[i] = {x, y, w, w};
    }

    std::vector<std::unique_ptr<Hold>> holds;
    std::vector<std::unique_ptr<Hold>> generatedHolds;

    twoNum holdTypeCount[5] = {
            //{1, 2},
            {2, 6},
            {2, 6},
            {1, 2},
            {1, 2},
    };

    std::string generalOptionCardTexts[4] = {
           "title",
           "description",
           "changePhoto",
           "numOfHolds",
    };

    std::string generationOptionCardTexts[6] = {
            //"topHolds",
            "topSection",
            "bottomSection",
            "startHolds",
            "startFootHolds",
            "footHolds",
    };

    std::string generationValues[6] = {
            //"topHoldsValue",
            "topSectionValue",
            "bottomSectionValue",
            "startHoldsValue",
            "startFootHoldsValue",
            "footHoldsValue",
    };

    std::unordered_map<std::string, SDL_Texture*> texts;
    std::unordered_map<std::string, SDL_Rect> textRects;
    texts["title"] = getTextureFromText(gRenderer, gFont, "Drevený boulder", &colors["black"], &textRects["title"].w, &textRects["title"].h);
    texts["generalOptionsTitle"] = getTextureFromText(gRenderer, gFont, "Nastavenia steny", &colors["black"], &textRects["generalOptionsTitle"].w, &textRects["generalOptionsTitle"].h);
    texts["generationOptionsTitle"] = getTextureFromText(gRenderer, gFont, "Nastavenia generácie", &colors["black"], &textRects["generationOptionsTitle"].w, &textRects["generationOptionsTitle"].h);
    texts["description"] = getTextureFromText(gRenderer, gFont, "Popis steny", &colors["black"], &textRects["description"].w, &textRects["description"].h);
    texts["changePhoto"] = getTextureFromText(gRenderer, gFont, "Vymeniť fotku", &colors["black"], &textRects["changePhoto"].w, &textRects["changePhoto"].h);
    texts["numOfHolds"] = getTextureFromText(gRenderer, gFont, "Počet chytov", &colors["black"], &textRects["numOfHolds"].w, &textRects["numOfHolds"].h);
    texts["topSection"] = getTextureFromText(gRenderer, gFont, "Vrchná sekcia", &colors["black"], &textRects["topSection"].w, &textRects["topSection"].h);
    texts["bottomSection"] = getTextureFromText(gRenderer, gFont, "Spodná sekcia", &colors["black"], &textRects["bottomSection"].w, &textRects["bottomSection"].h);
    texts["topHolds"] = getTextureFromText(gRenderer, gFont, "Topove chyty", &colors["black"], &textRects["topHolds"].w, &textRects["topHolds"].h);
    texts["startHolds"] = getTextureFromText(gRenderer, gFont, "Štartovné chyty", &colors["black"], &textRects["startHolds"].w, &textRects["startHolds"].h);
    texts["startFootHolds"] = getTextureFromText(gRenderer, gFont, "Štartovné stupy", &colors["black"], &textRects["startFootHolds"].w, &textRects["startFootHolds"].h);
    texts["footHolds"] = getTextureFromText(gRenderer, gFont, "Stupy v ceste", &colors["black"], &textRects["footHolds"].w, &textRects["footHolds"].h);
    texts["zoneGenerating"] = getTextureFromText(gRenderer, gFont, "Generovanie zón", &colors["black"], &textRects["zoneGenerating"].w, &textRects["zoneGenerating"].h);
    texts["min"] = getTextureFromText(gRenderer, gFont, "Min", &colors["black"], &textRects["min"].w, &textRects["min"].h);
    texts["max"] = getTextureFromText(gRenderer, gFont, "Max", &colors["black"], &textRects["max"].w, &textRects["max"].h);

    for (int i = 0; i < 5; i++) {
        std::string string = std::to_string(holdTypeCount[i].a) + " až " + std::to_string(holdTypeCount[i].b);
        texts[generationValues[i]] = getTextureFromText(gRenderer, gFont, string, &colors["black"], &textRects[generationValues[i]].w, &textRects[generationValues[i]].h);
    }
    for (int i = 0; i < 4; i++) {
        textRects[generalOptionCardTexts[i]].x = 100;
        textRects[generalOptionCardTexts[i]].y = generalOptionCardRects[i].y + generalOptionCardRects[i].h / 2 - textRects[generalOptionCardTexts[i]].h / 2;
    }

    int holdCount = 0;
    texts["holdCount"] = getTextureFromText(gRenderer, gFont, std::to_string(holdCount), &colors["primary"], &textRects["holdCount"].w, &textRects["holdCount"].h);
    textRects["holdCount"].x = generalOptionCardRects[3].x + generalOptionCardRects[3].w - 100 - textRects["holdCount"].w / 2;
    textRects["holdCount"].y = textRects["numOfHolds"].y;

    textRects["title"].x = 100;
    textRects["title"].y = 200;

    textRects["generalOptionsTitle"].x = WINDOW_WIDTH / 2 - textRects["generalOptionsTitle"].w / 2;
    textRects["generalOptionsTitle"].y = generalOptionCardRects[0].y - textRects["generalOptionsTitle"].h - 30;

    textRects["generationOptionsTitle"].x =  WINDOW_WIDTH / 2 - textRects["generationOptionsTitle"].w / 2;



    // --------------------------------------------------------------- main loop ------------------------------------------------------------

    SDL_Event e;
    bool running = true;

    while (running) {
        if (changeImage) {
            textures["mainImage"] = loadTexture("/data/user/0/com.bouldrovka.app/files/boulder/background.jpg", gRenderer);
            changeImage = false;
        }
        placeHolds = true;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            if (e.type == SDL_FINGERDOWN) {
                SDL_TouchFingerEvent tf = e.tfinger;

                int x = static_cast<int>(tf.x * WINDOW_WIDTH);
                int y = static_cast<int>(tf.y * WINDOW_HEIGHT);
                SDL_Point mousePos = {x, y};

                if (SDL_PointInRect(&mousePos, &settingsButtonRect)) {
                    if (scene == MAIN) scene = OPTIONS;
                    else if (scene == OPTIONS) scene = MAIN;
                }

                if(SDL_PointInRect(&mousePos, &backButtonRect)) {
                    if(scene == OPTIONS) scene = MAIN;
                }

                if (scene == MAIN) {
                    if (SDL_PointInRect(&mousePos, &generateButtonRect)) {
                        state = GENERATING;
                        generatedHolds = getGeneratedHolds(holds, holdTypeCount);
                    }

                    if (SDL_PointInRect(&mousePos, &selectMenuRect) || not SDL_PointInRect(&mousePos, &mainImageRect)) {
                        placeHolds = false;
                    }

                    if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[0])) {
                        state = PLACING;
                        holdType = TOP;
                    }
                    else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[1])) {
                        state = PLACING;
                        holdType = UPPER;
                    }
                    else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[2])) {
                        state = PLACING;
                        holdType = LOWER;
                    }
                    else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[3])) {
                        state = PLACING;
                        holdType = START;
                    }
                    else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[4])) {
                        state = PLACING;
                        holdType = FOOT;
                    }
                    else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[5])) {
                        state = DELETING;
                    }

                    if (state == PLACING && placeHolds) {
                        switch (holdType) {
                            case TOP:
                                holds.push_back(std::make_unique<Hold>(x, y, TOP));
                                break;
                            case UPPER:
                                holds.push_back(std::make_unique<Hold>(x, y, UPPER));
                                break;
                            case LOWER:
                                holds.push_back(std::make_unique<Hold>(x, y, LOWER));
                                break;
                            case START:
                                holds.push_back(std::make_unique<Hold>(x, y, START));
                                break;
                            case FOOT:
                                holds.push_back(std::make_unique<Hold>(x, y, FOOT));
                                break;

                        }
                        holdCount++;
                        texts["holdCount"] = getTextureFromText(gRenderer, gFont, std::to_string(holdCount), &colors["primary"], &textRects["holdCount"].w, &textRects["holdCount"].h);
                        textRects["holdCount"].x = generalOptionCardRects[3].x + generalOptionCardRects[3].w - 100 - textRects["holdCount"].w / 2;

                    }

                    if (state == DELETING) {
                        auto it = std::remove_if(holds.begin(), holds.end(),
                         [&](const std::unique_ptr<Hold>& hold) {
                             float dx = mousePos.x - hold->x;
                             float dy = mousePos.y - hold->y;
                             bool shouldRemove = std::sqrt(dx * dx + dy * dy) <= hold->radius;
                             if (shouldRemove) {
                                 holdCount--;
                                 texts["holdCount"] = getTextureFromText(gRenderer, gFont, std::to_string(holdCount), &colors["primary"], &textRects["holdCount"].w, &textRects["holdCount"].h);
                                 textRects["holdCount"].x = generalOptionCardRects[3].x + generalOptionCardRects[3].w - 100 - textRects["holdCount"].w / 2;
                             }
                             return shouldRemove;
                         } );
                        holds.erase(it, holds.end());
                    }

                }

                if (scene == OPTIONS) {
                    if (SDL_PointInRect(&mousePos, &generalOptionCardRects[2])) {
                        openImagePicker();
                    }
                }
            }
        }

        // -----------------------------------------------  rendering   ------------------------------------------

        // set background
        SDL_SetRenderDrawColor(gRenderer, colors["white"]);
        SDL_RenderClear(gRenderer);

        if (scene == MAIN) {

            // image background
            SDL_SetRenderDrawColor(gRenderer, colors["secondary"]);
            bool corners[4] = {true, true, false, false};
            SDL_RenderFillRoundedRect(gRenderer, 0, WINDOW_HEIGHT - mainImageRect.h - 35, mainImageRect.w, mainImageRect.h + 35, 60, corners);

            //main image
            drawMainImage(gRenderer, WINDOW_HEIGHT, WINDOW_WIDTH, textures["mainImage"], &mainImageRect, textures["mainImageMask"]);

            // holds
            if (state == GENERATING) drawHolds(gRenderer, generatedHolds);
            else drawHolds(gRenderer, holds);

            // buttons
            SDL_SetRenderDrawColor(gRenderer, colors["secondary"]);
            drawGenerateButton(gRenderer, WINDOW_WIDTH, textures["logoImage"], &generateButtonRect);
            drawSelectHoldMenu(gRenderer, selectMenuX, selectMenuY, selectMenuW, selectMenuH, selectMenuHoldRadius, selectMenuHoldWidth, textures["crossImage"]);

            // title
            SDL_RenderCopy(gRenderer, texts["title"], nullptr, &textRects["title"]);
        }

        if (scene == OPTIONS) {
            bool corners[4] = {true, true, false, false};

            // background
            SDL_SetRenderDrawColor(gRenderer, 255, 241, 233, 255);
            SDL_RenderFillRoundedRect(gRenderer, 0, 170, mainImageRect.w, WINDOW_HEIGHT * 2, 50, corners);

            //general option cards
            SDL_RenderCopy(gRenderer, texts["generalOptionsTitle"], nullptr, &textRects["generalOptionsTitle"]);
            for (int i = 0; i < 4; i++) {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                textureWithDimensions text = {
                        texts[generalOptionCardTexts[i]],
                        textRects[generalOptionCardTexts[i]].w,
                        textRects[generalOptionCardTexts[i]].h,
                };
                drawCardWithIcon(gRenderer, &generalOptionCardRects[i], 38, text, &colors["primary"], generalOptionCardIcons[i]);

                // number of holds
                if (i == 3) {
                    SDL_RenderCopy(gRenderer, texts["holdCount"], nullptr, &textRects["holdCount"]);
                }
            }

            //generation option cards
            for (int i = 0; i < 1; i++) {
                textureWithDimensions title = {
                        texts[generationOptionCardTexts[i]],
                        textRects[generationOptionCardTexts[i]].w,
                        textRects[generationOptionCardTexts[i]].h,
                };
                textureWithDimensions min = {
                        texts["min"],
                        textRects["min"].w,
                        textRects["min"].h
                };
                textureWithDimensions max = {
                        texts["max"],
                        textRects["max"].w,
                        textRects["max"].h
                };
                textureWithDimensions value = {
                        texts[generationValues[i]],
                        textRects[generationValues[i]].w,
                        textRects[generationValues[i]].h
                };
                textureWithDimensions minus = {
                        texts["blackMinus"],
                        textRects["blackMinus"].w,
                        textRects["blackMinus"].h
                };
                textureWithDimensions plus = {
                        texts["blackPlus"],
                        textRects["blackPlus"].w,
                        textRects["blackPlus"].h
                };
                drawCardWithValue(gRenderer, &generationOptionCardRects[i], 38, title, min, max, value, minus, plus, &colors["black"]);
            }

        }

        drawSettingsButton(gRenderer, textures["threeBarsImage"], &settingsButtonRect);
        drawBackButton(gRenderer, textures["leftArrowImage"], &backButtonRect);
        SDL_RenderPresent(gRenderer);
    }

    // --------------------------------------------------- clean up -----------------------------------------------
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();

    for(auto & pair : texts) {
        SDL_DestroyTexture(pair.second);
    }
    texts.clear();

    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

