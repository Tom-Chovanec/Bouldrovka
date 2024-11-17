#include "../headers/files.h"
#include "../headers/holds.h"
#include "../headers/rendering.h"
#include "../headers/ui.h"
#include "../headers/utility.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "cmath"
#include "iostream"
#include "memory"
#include "string"
#include "unordered_map"
#include "vector"
#include <android/log.h>
#include <fcntl.h>
#include <jni.h>

bool changeImage = true;

void openImagePicker() {
  auto *env = static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv());
  auto activity = static_cast<jobject>(SDL_AndroidGetActivity());
  jclass clazz = env->FindClass("com/bouldrovka/app/Bouldrovka");
  jmethodID methodId = env->GetMethodID(clazz, "openImagePicker", "()V");
  env->CallVoidMethod(activity, methodId);
  env->DeleteLocalRef(clazz);
}

SDL_Texture *loadTexture(const std::string &path, SDL_Renderer *renderer) {
  // Load image at specified path
  SDL_Texture *newTexture = IMG_LoadTexture(renderer, path.c_str());
  if (newTexture == nullptr) {
    SDL_Log("Unable to load image %s ! SDL_image Error: %s ", path.c_str(),
            IMG_GetError());
  }
  return newTexture;
}

extern "C" JNIEXPORT void JNICALL
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

SDL_Window *createWindow(int width, int height) {
  SDL_Window *window = nullptr;
  window = SDL_CreateWindow("Boulder gen", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_FULLSCREEN);
  if (window == nullptr) {
    std::cout << "Error while creating window: " << SDL_GetError();
    return nullptr;
  }
  return window;
}

SDL_Renderer *createRenderer(SDL_Window *window) {
  SDL_Renderer *renderer = nullptr;
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

  TTF_Font *gFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 56);
  if (gFont == nullptr) {
    printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  TTF_Font *gSmallFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 34);
  if (gSmallFont == nullptr) {
    printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }
  TTF_Font *gBigFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 78);
  if (gBigFont == nullptr) {
    printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    TTF_CloseFont(gFont);
    TTF_CloseFont(gSmallFont);
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
    GENERATION_OPTIONS,
    PROBLEMS,
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
  colors["primary"] = {255, 106, 20, 255};
  colors["secondary"] = {255, 241, 233, 255};
  colors["blueAccent"] = {151, 153, 219, 255};
  colors["orangeAccent"] = {255, 192, 174, 255};
  colors["redAccent"] = {221, 43, 29, 255};
  colors["darkRedAccent"] = {158, 13, 13, 255};
  colors["black"] = {0, 0, 0, 255};
  colors["gray"] = {81, 81, 81, 255};
  colors["lightGray"] = {249, 249, 249, 255};
  colors["white"] = {255, 255, 255, 255};
  colors["null"] = {0, 0, 0, 0};

  std::unordered_map<std::string, SDL_Texture *> textures;

  textures["mainImage"] = nullptr;
  textures["logoImage"] = loadTexture("images/logo.png", gRenderer);
  textures["whiteLogoImage"] = loadTexture("images/white_logo.png", gRenderer);
  textures["threeBarsImage"] = loadTexture("images/3_bars.png", gRenderer);
  textures["whiteThreeBarsImage"] = loadTexture("images/white_3_bars.png", gRenderer);
  textures["leftArrowImage"] = loadTexture("images/left_arrow.png", gRenderer);
  textures["whiteCrossImage"] = loadTexture("images/white_cross.png", gRenderer);
  textures["crossImage"] = loadTexture("images/cross.png", gRenderer);
  textures["topRightArrowImage"] = loadTexture("images/top_right_arrow.png", gRenderer);
  textures["whiteTopRightArrowImage"] = loadTexture("images/white_top_right_arrow.png", gRenderer);
  textures["penImage"] = loadTexture("images/pen.png", gRenderer);
  textures["whitePenImage"] = loadTexture("images/white_pen.png", gRenderer);
  textures["backgroundBlobsImage"] = loadTexture("images/background_blobs.png", gRenderer);
  textures["blackMinusImage"] = loadTexture("images/black_minus.png", gRenderer);
  textures["blackPlusImage"] = loadTexture("images/black_plus.png", gRenderer);
  textures["whiteMinusImage"] = loadTexture("images/white_minus.png", gRenderer);
  textures["whitePlusImage"] = loadTexture("images/white_plus.png", gRenderer);
  textures["whiteSaveImage"] = loadTexture("images/white_save.png", gRenderer);
  textures["whiteCogwheel"] = loadTexture("images/white_cogwheel.png", gRenderer);

  float ratio = 9.0f / 16.0f;
  SDL_Rect mainImageRect;
  mainImageRect.w = WINDOW_WIDTH;
  mainImageRect.h = static_cast<int>((float)mainImageRect.w / ratio);
  mainImageRect.x = 0;
  mainImageRect.y = WINDOW_HEIGHT - mainImageRect.h;

  SDL_SetRenderDrawColor(gRenderer, colors["white"]);
  textures["mainImageMask"] =
      createRoundedRectMask(gRenderer, mainImageRect.w, mainImageRect.h, 100);

  SDL_Rect topBackground = {
      0,
      0,
      WINDOW_WIDTH,
      150,
  };

  SDL_Rect generateButtonRect = {WINDOW_WIDTH - 145, 170, 90, 90};

  SDL_Rect settingsButtonRect = {WINDOW_WIDTH - 120, 150 / 2 - 30, 60, 60};

  SDL_Rect backButtonRect = {60, 150 / 2 - 30, 60, 60};

  int selectMenuX = WINDOW_WIDTH - 120;
  int selectMenuY = 500;
  int selectMenuW = 120;
  int selectMenuH = 666;
  int selectMenuHoldRadius = 38;
  int selectMenuHoldWidth = 10;
  SDL_Rect selectMenuRect = {selectMenuX, selectMenuY, selectMenuW,
                             selectMenuH};
  SDL_Rect selectMenuHitBoxes[6];

  for (int i = 0; i < 6; i++) {
    int x = selectMenuX + (selectMenuW / 2) - selectMenuHoldRadius;
    int y = selectMenuY + 30 + (selectMenuHoldRadius * 2 + 30) * i;
    int w = selectMenuHoldRadius * 2;
    selectMenuHitBoxes[i] = {x, y, w, w};
  }

  std::vector<std::unique_ptr<Hold>> holds;
  std::vector<std::unique_ptr<Hold>> generatedHolds;
  twoNum holdTypeCount[6] = {
    {1, 2},
    {2, 6},
    {2, 6},
    {1, 2},
    {1, 2},
    {1, 2},
  };

  int holdCount = 0;

  UIHandler uiHandler;


  IconCard* boulderTitle = new IconCard("boulderTitle", gRenderer, gBigFont, 50, 175, WINDOW_WIDTH - 100, 100, 50, colors["null"], colors["null"], "Drevený boulder", textures["whitePenImage"], colors["primary"]);
  IconCard* boulderDescription = new IconCard("boulderDescription", gRenderer, gSmallFont, 50, 300, WINDOW_WIDTH - 100, 200, 50, colors["lightGray"], colors["lightGray"], "popis", textures["whitePenImage"], colors["primary"]);
  IconCard* changePhoto = new IconCard("changePhoto", gRenderer, gFont, 50, 650, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Vymeniť fotku", textures["whiteTopRightArrowImage"], colors["primary"]);
  IconCard* generationOptions = new IconCard("generationOptions", gRenderer, gFont, 50, 875, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Nastaviene generovania", textures["whiteCogwheel"], colors["primary"]);
  IconCard* numberOfHolds = new IconCard("numberOfHolds", gRenderer, gFont, 50, 1100, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Počet chytov", nullptr, colors["primary"]);
  Text* numberOfHoldsText = new Text("numberOfHoldsText", gRenderer, gFont, WINDOW_WIDTH - 150, 1200, std::to_string(holdCount), colors["primary"]);
  IconCard* clearHolds = new IconCard("clearHolds", gRenderer, gFont, 50, 1325, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Vyčistiť chyty", textures["whiteCrossImage"], colors["primary"]);
  Text* myProblemsText = new Text("myProblemsText", gRenderer, gFont, WINDOW_WIDTH / 2, 1680, "Moje bouldrové cesty", colors["black"]);
  //change !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  IconCard* loadProblem = new IconCard("loadProblem", gRenderer, gFont, 50, 1750, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Načítať cestu", textures["whiteThreeBarsImage"], colors["primary"], false);
  IconCard* saveProblem = new IconCard("saveProblem", gRenderer, gFont, 50, 1975, WINDOW_WIDTH - 100, 200, 50, colors["white"], colors["lightGray"], "Uložiť cestu", textures["whiteSaveImage"], colors["primary"], false);
  Image* logo = new Image("logo", gRenderer, WINDOW_WIDTH / 2 - 64, 2225, 128, 128, textures["whiteLogoImage"], colors["primary"]);
  Image* introLogo = new Image("introLogo", gRenderer, WINDOW_WIDTH / 2 - 128, 300, 256, 256, textures["whiteLogoImage"], colors["primary"]);
  Button* introButton = new Button("introButton", gRenderer, gBigFont, 150, WINDOW_HEIGHT - 700, WINDOW_WIDTH - 300, 300, "Ideme na to!", colors["primary"], colors["white"], 150);

  std::vector<std::string> introUIElements = {
    "introButton",
    "introLogo"
  };

  std::vector<std::string> generalOptionUIElements = {
    "boulderTitle",
    "boulderDescription",
    "changePhoto",
    "generationOptions",
    "numberOfHolds",
    "numberOfHoldsText",
    "clearHolds",
    "myProblemsText",
    "loadProblem",
    "saveProblem",
    "logo",
  };


  uiHandler.addElement(boulderTitle);
  uiHandler.addElement(boulderDescription);
  uiHandler.addElement(changePhoto);
  uiHandler.addElement(generationOptions);
  uiHandler.addElement(numberOfHolds);
  uiHandler.addElement(numberOfHoldsText);
  uiHandler.addElement(clearHolds);
  uiHandler.addElement(myProblemsText);
  uiHandler.addElement(loadProblem);
  uiHandler.addElement(saveProblem);
  uiHandler.addElement(logo);
  uiHandler.addElement(introLogo);
  uiHandler.addElement(introButton);

  // --------------------------------------------------------------- main loop ------------------------------------------------------------

  SDL_Event e;
  bool running = true;
  bool pressed = false;
  float touchY = 0.0f;
  float touchDy = 0.0f;
  float scroll = 0.0f;
  Uint8 imageAlpha = 0;
  SDL_Point pressMousePos;
  textures["backgroundImage"] = nullptr;

  while (running) {
    if (changeImage) {
      textures["mainImage"] = loadTexture(
          "/data/user/0/com.bouldrovka.app/files/boulder/background.jpg",
          gRenderer);
      changeImage = false;
    }
    placeHolds = true;

    while (SDL_PollEvent(&e) != 0) {
      SDL_TouchFingerEvent tf = e.tfinger;
      float x = tf.x * (float)WINDOW_WIDTH;
      float y = tf.y * (float)WINDOW_HEIGHT;
      SDL_Point mousePos = {(int)x, (int)y};
      uiHandler.hover(mousePos.x, mousePos.y);
      if (e.type == SDL_QUIT) {
        running = false;
      }

      if (e.type == SDL_FINGERDOWN) {
        pressed = true;
        touchY = y;
        pressMousePos = {(int)x, (int)y};

      } else if (e.type == SDL_FINGERMOTION) {
        uiHandler.hover(-1, -1); //reset hover
        if (not isWithinRadius(&pressMousePos, &mousePos, 20)) {
          pressed = false;
        }
        if (scene == OPTIONS) {
          float dY = y - touchY;
          touchY = y;
          touchDy = dY;
          scroll += dY;
        }

      } else if (e.type == SDL_FINGERUP) {
        uiHandler.hover(-1, -1); //reset hover
        if (pressed) {

          if (scene == MAIN || scene == OPTIONS) {
            if (SDL_PointInRect(&mousePos, &settingsButtonRect)) {
              if (scene == MAIN) {
                scene = OPTIONS;
              } else if (scene == OPTIONS) {
                scene = MAIN;
                scroll = 0;
              }
            }

            if (SDL_PointInRect(&mousePos, &backButtonRect)) {
              if (scene == OPTIONS) {
                scene = MAIN;
                scroll = 0;
              } else if (scene == MAIN) {
                scene = INTRO;
                scroll = 0;
              }

            }
          }

          if (scene == INTRO) {
            std::string element = uiHandler.handleClick(mousePos.x, mousePos.y, introUIElements);
            if (element == "introButton") {
              scene = MAIN;
            }

          }

          if (scene == MAIN) {
            if (SDL_PointInRect(&mousePos, &generateButtonRect)) {
              state = GENERATING;
              generatedHolds = getGeneratedHolds(holds, holdTypeCount);
            }

            if (SDL_PointInRect(&mousePos, &selectMenuRect) ||
                not SDL_PointInRect(&mousePos, &mainImageRect)) {
              placeHolds = false;
            }

            if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[0])) {
              state = PLACING;
              holdType = TOP;
            } else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[1])) {
              state = PLACING;
              holdType = UPPER;
            } else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[2])) {
              state = PLACING;
              holdType = LOWER;
            } else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[3])) {
              state = PLACING;
              holdType = START;
            } else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[4])) {
              state = PLACING;
              holdType = FOOT;
            } else if (SDL_PointInRect(&mousePos, &selectMenuHitBoxes[5])) {
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
              numberOfHoldsText->changeText(gRenderer, gFont, colors["primary"], std::to_string(++holdCount));
            }

            if (state == DELETING) {
              auto it = std::remove_if(
                  holds.begin(), holds.end(),
                  [&](const std::unique_ptr<Hold> &hold) {
                    auto dx = static_cast<float>((int)mousePos.x - hold->x);
                    auto dy = static_cast<float>((int)mousePos.y - hold->y);
                    bool shouldRemove =
                        std::sqrt(dx * dx + dy * dy) <= (float)hold->radius;
                    if (shouldRemove) {
                      numberOfHoldsText->changeText(gRenderer, gFont, colors["primary"], std::to_string(--holdCount));
                    }
                    return shouldRemove;
                  });
              holds.erase(it, holds.end());
            }

            if (state == GENERATING) {
              if (SDL_PointInRect(&mousePos, &mainImageRect)) {
                if (imageAlpha == 0)
                  imageAlpha = 75;
                else if (imageAlpha == 75)
                  imageAlpha = 150;
                else if (imageAlpha == 150)
                  imageAlpha = 0;

                SDL_DestroyTexture(textures["backgroundImage"]);
                textures["backgroundImage"] = getDarkenImage(
                    gRenderer, textures["mainImage"], imageAlpha);
              }
            }
          }

          if (scene == OPTIONS) {

            if (not SDL_PointInRect(&mousePos, &topBackground)) {
              std::string element = uiHandler.handleClick(mousePos.x, mousePos.y, generalOptionUIElements);
              if (element == "changePhoto") {openImagePicker();}
              else if (element == "saveProblem") {saveHoldsToFile( holds, "/data/user/0/com.bouldrovka.app/files/boulder/holds.txt");}
              else if (element == "loadProblem") {
                  holds = readHoldsFromFile( "/data/user/0/com.bouldrovka.app/files/boulder/holds.txt");
                  holdCount = (int)holds.size();
                  numberOfHoldsText->changeText(gRenderer, gFont, colors["primary"], std::to_string(holdCount));
              }
              else if (element == "clearHolds") {
                  holds.clear();
                  holdCount = 0;
                  numberOfHoldsText->changeText(gRenderer, gFont, colors["primary"], std::to_string(holdCount));
              }
            }
          }
        }
      }
    }

    if (scroll < WINDOW_HEIGHT - 2403) scroll = WINDOW_HEIGHT - 2403;
    if (scroll > 0) scroll = 0;

    uiHandler.scroll(scroll, generalOptionUIElements);
    // -----------------------------------------------  rendering ------------------------------------------

    // set background
    SDL_SetRenderDrawColor(gRenderer, colors["white"]);
    SDL_RenderClear(gRenderer);

    if (scene == INTRO) {
      SDL_RenderCopy(gRenderer, textures["backgroundBlobsImage"], nullptr, nullptr);

      uiHandler.render(gRenderer, introUIElements);
    }

    if (scene == MAIN) {

      // main image
      if (textures["backgroundImage"] != nullptr)
        drawMainImage(gRenderer, WINDOW_HEIGHT, WINDOW_WIDTH,
                      textures["backgroundImage"], &mainImageRect,
                      textures["mainImageMask"]);
      else
        drawMainImage(gRenderer, WINDOW_HEIGHT, WINDOW_WIDTH,
                      textures["backgroundBlobsImage"], &mainImageRect,
                      textures["mainImageMask"]);
      // holds
      if (state == GENERATING)
        drawHolds(gRenderer, generatedHolds);
      else
        drawHolds(gRenderer, holds);

      // buttons
      SDL_SetRenderDrawColor(gRenderer, colors["secondary"]);
      drawGenerateButton(gRenderer, WINDOW_WIDTH, textures["logoImage"],
                         &generateButtonRect);
      drawSelectHoldMenu(gRenderer, selectMenuX, selectMenuY, selectMenuW,
                         selectMenuH, selectMenuHoldRadius, selectMenuHoldWidth,
                         textures["crossImage"]);

      // title
    }

    if (scene == OPTIONS) {
      bool corners[4] = {true, true, false, false};

      // background
      SDL_SetRenderDrawColor(gRenderer, 255, 241, 233, 255);
      SDL_RenderFillRoundedRect(gRenderer, 0, getScrolled(575, scroll),
                                mainImageRect.w, WINDOW_HEIGHT * 2, 100,
                                corners);

      uiHandler.render(gRenderer, generalOptionUIElements);
    }


    if (scene == MAIN || scene == OPTIONS) {
      SDL_SetRenderDrawColor(gRenderer, colors["white"]);
      SDL_RenderFillRect(gRenderer, &topBackground);
      drawButton(gRenderer, textures["threeBarsImage"], &settingsButtonRect);
      drawButton(gRenderer, textures["leftArrowImage"], &backButtonRect);
    }

    SDL_RenderPresent(gRenderer);
  }

  // --------------------------------------------------- clean up -----------------------------------------------
  for (auto &pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
  textures.clear();

  TTF_CloseFont(gFont);
  TTF_CloseFont(gSmallFont);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gRenderer = nullptr;
  gWindow = nullptr;

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
  return 0;
}
