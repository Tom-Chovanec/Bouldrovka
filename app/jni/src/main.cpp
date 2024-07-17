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
#include "filesystem"
#include "../headers/holds.h"
#include "../headers/ui.h"
#include "../headers/SDL_primitives.h"

bool changeImage = true;

void openImagePicker() {
    auto* env = static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv());
    auto activity = static_cast<jobject>(SDL_AndroidGetActivity());
    jclass clazz = env->FindClass("org/libsdl/app/SDLActivity");
    jmethodID methodId = env->GetMethodID(clazz, "openImagePicker", "()V");
    env->CallVoidMethod(activity, methodId);
    env->DeleteLocalRef(clazz);
}

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer) {
    // Load image at specified path
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == NULL) {
        SDL_Log("Unable to load image %s ! SDL_image Error: %s ", path.c_str(), IMG_GetError());
    }
    return newTexture;
}

extern "C"
JNIEXPORT void JNICALL
Java_org_libsdl_app_SDLActivity_onImagePicked(JNIEnv *env, jobject obj) {
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
    window = SDL_CreateWindow("Boulder gen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
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

    TTF_Font* gFont = TTF_OpenFont("fonts/Roboto-Regular.ttf", 72);
    if (gFont == NULL) {
        printf("TTF_OpenFont Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_GetWindowSize(gWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);
    std::array<int, 3> backgroundColor = {120, 75, 43};

    HoldType state = NONE;
    bool placeHolds = true;
    SDL_Texture* mainImage = nullptr ;
    std::vector<std::unique_ptr<Button>> buttons;
    buttons.push_back(std::make_unique<Button>(gRenderer, 0, 0, 200, 100, "Gallery", gFont));
    buttons.push_back(std::make_unique<Button>(gRenderer, 200, 0, 200, 100, "START", gFont));
    buttons.push_back(std::make_unique<Button>(gRenderer, 400, 0, 200, 100, "MIDDLE", gFont));
    buttons.push_back(std::make_unique<Button>(gRenderer, 600, 0, 200, 100, "END", gFont));
    buttons.push_back(std::make_unique<Button>(gRenderer, 700, 0, 200, 100, "DELETE", gFont));

    float b = 0;
    float prevX;
    SDL_Rect imageRect = {0 , 100, WINDOW_WIDTH - 100 , WINDOW_HEIGHT - 100};

    std::vector<std::unique_ptr<Hold>> holds;
    SDL_Event e;
    bool running = true;

    while (running) {
        if (changeImage) {
            mainImage = loadTexture("/data/user/0/org.libsdl.app/files/boulder/background.jpg", gRenderer);
            changeImage = false;
        }
        placeHolds = true;
        SDL_SetRenderDrawColor(gRenderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], 255);
        SDL_RenderClear(gRenderer);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }


            if (e.type == SDL_FINGERMOTION) {
            }
            if (e.type == SDL_FINGERUP) {
            }
            if (e.type == SDL_MULTIGESTURE) {
                float pinchDistance = e.mgesture.dDist;
                float pinchCenterX = e.mgesture.x;
                float pinchCenterY = e.mgesture.y;

                if (pinchDistance > 0) {
                    b += 0.02f;
                } else {
                    b -= 0.02f;
                }
                if (b < 0) { b = 0; }
                if (b > 2) { b = 2.0f; }

                imageRect = {static_cast<int>(0 - (WINDOW_WIDTH * (b)) / 2), static_cast<int>(100 - (WINDOW_HEIGHT * (b)) / 2), static_cast<int>(WINDOW_WIDTH * (b + 1) - 100), static_cast<int>(WINDOW_HEIGHT * (b + 1) - 100)};
                break;
            }

            if (e.type == SDL_FINGERDOWN) {
                SDL_Log("???");
                SDL_TouchFingerEvent tf = e.tfinger;

                int x = static_cast<int>(tf.x * WINDOW_WIDTH);
                int y = static_cast<int>(tf.y * WINDOW_HEIGHT);

                if (state == DELETE) {

                }

                for (auto &button : buttons)  {
                    if (button->collides(x, y)) {
                        placeHolds = false;
                        if (button->name == "Gallery") {
                            openImagePicker();
                            break;
                        }
                        else if (button->name == "START") {
                            state = START;
                            break;
                        }
                        else if (button->name == "MIDDLE") {
                            state = MIDDLE;
                            break;
                        }
                        else if (button->name == "END") {
                            state = END;
                            break;
                        }
                        else if (button->name == "DELETE") {
                            state = DELETE;
                            break;
                        }
                    }
                }

                if (placeHolds) {
                    switch (state) {
                        case NONE:
                            break;
                        case START:
                            holds.push_back(std::make_unique<Hold>(x, y, START));
                            break;
                        case MIDDLE:
                            holds.push_back(std::make_unique<Hold>(x, y, MIDDLE));
                            break;
                        case END:
                            holds.push_back(std::make_unique<Hold>(x, y, END));
                            break;
                    }
                }
            }
        }
        if (mainImage != nullptr) {
            SDL_RenderCopy(gRenderer, mainImage, nullptr, &imageRect);
        }
        for (auto &button : buttons) {
            button->draw(gRenderer);
        }
        //for (auto &hold : holds) {
        //    hold->draw(gRenderer);
        //}
        drawHolds(gRenderer, holds);
        SDL_RenderPresent(gRenderer);
    }
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

