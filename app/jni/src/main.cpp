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

  TTF_Font *gFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 78);
  if (gFont == nullptr) {
    printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  TTF_Font *gSmallFont = TTF_OpenFont("fonts/JosefinSans-Regular.ttf", 55);
  if (gSmallFont == nullptr) {
    printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    TTF_CloseFont(gFont);
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
  colors["primary"] = {255, 106, 20, 255};
  colors["secondary"] = {255, 241, 233, 255};
  colors["blueAccent"] = {151, 153, 219, 255};
  colors["orangeAccent"] = {255, 192, 174, 255};
  colors["redAccent"] = {221, 43, 29, 255};
  colors["darkRedAccent"] = {158, 13, 13, 255};
  colors["black"] = {0, 0, 0, 255};
  colors["gray"] = {81, 81, 81, 255};
  colors["white"] = {255, 255, 255, 255};

  std::unordered_map<std::string, SDL_Texture *> textures;

  textures["mainImage"] = nullptr;
  textures["logoImage"] = loadTexture("images/logo.png", gRenderer);
  textures["threeBarsImage"] = loadTexture("images/3_bars.png", gRenderer);
  textures["leftArrowImage"] = loadTexture("images/left_arrow.png", gRenderer);
  textures["crossImage "] = loadTexture("images/cross.png", gRenderer);
  textures["topRightArrowImage"] =
      loadTexture("images/top_right_arrow.png", gRenderer);
  textures["penImage"] = loadTexture("images/pen.png", gRenderer);
  textures["backgroundBlobsImage"] =
      loadTexture("images/background_blobs.png", gRenderer);
  textures["blackMinusImage"] =
      loadTexture("images/black_minus.png", gRenderer);
  textures["blackPlusImage"] = loadTexture("images/black_plus.png", gRenderer);
  textures["whiteMinusImage"] =
      loadTexture("images/white_minus.png", gRenderer);
  textures["whitePlusImage"] = loadTexture("images/white_plus.png", gRenderer);

  SDL_Texture *generalOptionCardIcons[7] = {
      textures["penImage"],
      textures["penImage"],
      textures["topRightArrowImage"],
      nullptr,
      nullptr,
      nullptr,
      nullptr,
  };

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

  SDL_Rect generalOptionCardRects[7];

  for (int i = 0; i < 7; i++) {
    generalOptionCardRects[i].x = 50;
    generalOptionCardRects[i].h = 200;
    generalOptionCardRects[i].w =
        WINDOW_WIDTH - 2 * generalOptionCardRects[i].x;
    generalOptionCardRects[i].y =
        170 + 155 +
        (generalOptionCardRects[i].h + generalOptionCardRects[i].x) * i;
  }

  SDL_Rect generalOptionCardIconRect[3];

  for (int i = 0; i < 3; i++) {
    generalOptionCardIconRect[i].w = 76;
    generalOptionCardIconRect[i].h = 76;
    generalOptionCardIconRect[i].x =
        WINDOW_WIDTH - 150 - generalOptionCardIconRect[i].w / 2;
    generalOptionCardIconRect[i].y = generalOptionCardRects[i].y +
                                     generalOptionCardRects[i].h / 2 -
                                     generalOptionCardIconRect[i].h / 2;
  }

  SDL_Rect generationOptionCardRects[6];

  for (int i = 0; i < 6; i++) {
    generationOptionCardRects[i].x = 50;
    generationOptionCardRects[i].h = 450;
    generationOptionCardRects[i].w =
        WINDOW_WIDTH - 2 * generationOptionCardRects[i].x;
    generationOptionCardRects[i].y =
        generalOptionCardRects[6].y + generalOptionCardRects[6].h + 170 +
        (generationOptionCardRects[i].h + generationOptionCardRects[i].x) * i;
  }

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
      {1, 2}, {2, 6}, {2, 6}, {1, 2}, {1, 2}, {1, 2},
  };

  std::string generalOptionCardTexts[7] = {
      "title", "description", "changePhoto", "numOfHolds",
      "save",  "load",        "clear",
  };

  std::string generationOptionCardTexts[6] = {
      "topHolds",   "topSection",     "bottomSection",
      "startHolds", "startFootHolds", "footHolds",
  };

  std::string generationValues[6] = {
      "topHoldsValue",   "topSectionValue",     "bottomSectionValue",
      "startHoldsValue", "startFootHoldsValue", "footHoldsValue",
  };

  SDL_Color *generationIconColors[6] = {
      &colors["primary"],   &colors["orangeAccent"],  &colors["blueAccent"],
      &colors["redAccent"], &colors["darkRedAccent"], &colors["darkRedAccent"],
  };

  std::unordered_map<std::string, SDL_Texture *> texts;
  std::unordered_map<std::string, SDL_Rect> textRects;
  texts["title"] =
      getTextureFromText(gRenderer, gFont, "Drevený boulder", &colors["black"],
                         &textRects["title"].w, &textRects["title"].h);
  texts["generalOptionsTitle"] = getTextureFromText(
      gRenderer, gFont, "Nastavenia steny", &colors["black"],
      &textRects["generalOptionsTitle"].w, &textRects["generalOptionsTitle"].h);
  texts["generationOptionsTitle"] = getTextureFromText(
      gRenderer, gFont, "Nastavenia generácie", &colors["black"],
      &textRects["generationOptionsTitle"].w,
      &textRects["generationOptionsTitle"].h);
  texts["description"] = getTextureFromText(
      gRenderer, gFont, "Popis steny", &colors["black"],
      &textRects["description"].w, &textRects["description"].h);
  texts["changePhoto"] = getTextureFromText(
      gRenderer, gFont, "Vymeniť fotku", &colors["black"],
      &textRects["changePhoto"].w, &textRects["changePhoto"].h);
  texts["numOfHolds"] = getTextureFromText(
      gRenderer, gFont, "Počet chytov", &colors["black"],
      &textRects["numOfHolds"].w, &textRects["numOfHolds"].h);
  texts["topSection"] = getTextureFromText(
      gRenderer, gFont, "Vrchná sekcia", &colors["black"],
      &textRects["topSection"].w, &textRects["topSection"].h);
  texts["bottomSection"] = getTextureFromText(
      gRenderer, gFont, "Spodná sekcia", &colors["black"],
      &textRects["bottomSection"].w, &textRects["bottomSection"].h);
  texts["topHolds"] =
      getTextureFromText(gRenderer, gFont, "Topove chyty", &colors["black"],
                         &textRects["topHolds"].w, &textRects["topHolds"].h);
  texts["startHolds"] = getTextureFromText(
      gRenderer, gFont, "Štartovné chyty", &colors["black"],
      &textRects["startHolds"].w, &textRects["startHolds"].h);
  texts["startFootHolds"] = getTextureFromText(
      gRenderer, gFont, "Štartovné stupy", &colors["black"],
      &textRects["startFootHolds"].w, &textRects["startFootHolds"].h);
  texts["footHolds"] =
      getTextureFromText(gRenderer, gFont, "Stupy v ceste", &colors["black"],
                         &textRects["footHolds"].w, &textRects["footHolds"].h);
  texts["zoneGenerating"] = getTextureFromText(
      gRenderer, gFont, "Generovanie zón", &colors["black"],
      &textRects["zoneGenerating"].w, &textRects["zoneGenerating"].h);
  texts["min"] = getTextureFromText(gRenderer, gFont, "Min", &colors["black"],
                                    &textRects["min"].w, &textRects["min"].h);
  texts["max"] = getTextureFromText(gRenderer, gFont, "Max", &colors["black"],
                                    &textRects["max"].w, &textRects["max"].h);
  texts["save"] =
      getTextureFromText(gRenderer, gFont, "Uložiť chyty", &colors["black"],
                         &textRects["save"].w, &textRects["save"].h);
  texts["load"] =
      getTextureFromText(gRenderer, gFont, "Načítať chyty", &colors["black"],
                         &textRects["load"].w, &textRects["load"].h);
  texts["clear"] = getTextureFromText(
      gRenderer, gFont, "Odstrániť všetky chyty", &colors["black"],
      &textRects["clear"].w, &textRects["clear"].h);

  // generation values texts
  for (int i = 0; i < 5; i++) {
    std::string string = std::to_string(holdTypeCount[i].a) + " až " +
                         std::to_string(holdTypeCount[i].b);
    texts[generationValues[i]] = getTextureFromText(
        gRenderer, gSmallFont, string, &colors["gray"],
        &textRects[generationValues[i]].w, &textRects[generationValues[i]].h);
  }

  // general option title rects
  for (int i = 0; i < 7; i++) {
    textRects[generalOptionCardTexts[i]].x = 100;
    textRects[generalOptionCardTexts[i]].y =
        generalOptionCardRects[i].y + generalOptionCardRects[i].h / 2 -
        textRects[generalOptionCardTexts[i]].h / 2;
  }

  int holdCount = 0;
  texts["holdCount"] = getTextureFromText(
      gRenderer, gFont, std::to_string(holdCount), &colors["primary"],
      &textRects["holdCount"].w, &textRects["holdCount"].h);
  textRects["holdCount"].x = generalOptionCardRects[3].x +
                             generalOptionCardRects[3].w - 100 -
                             textRects["holdCount"].w / 2;
  textRects["holdCount"].y = textRects["numOfHolds"].y;

  textRects["title"].x = 100;
  textRects["title"].y = 200;

  textRects["generalOptionsTitle"].x =
      WINDOW_WIDTH / 2 - textRects["generalOptionsTitle"].w / 2;
  textRects["generalOptionsTitle"].y =
      generalOptionCardRects[0].y - textRects["generalOptionsTitle"].h - 30;

  textRects["generationOptionsTitle"].x =
      WINDOW_WIDTH / 2 - textRects["generationOptionsTitle"].w / 2;

  // 6 cards, 4 buttons each
  SDL_Rect generationOptionHitboxes[6][4];



  // --------------------------------------------------------------- main loop
  // ------------------------------------------------------------

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
      if (e.type == SDL_QUIT) {
        running = false;
      }

      if (e.type == SDL_FINGERDOWN) {
        pressed = true;
        touchY = y;
        pressMousePos = {(int)x, (int)y};
      } else if (e.type == SDL_FINGERMOTION) {
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
        if (pressed) {

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
              holdCount++;
              texts["holdCount"] = getTextureFromText(
                  gRenderer, gFont, std::to_string(holdCount),
                  &colors["primary"], &textRects["holdCount"].w,
                  &textRects["holdCount"].h);
              textRects["holdCount"].x = generalOptionCardRects[3].x +
                                         generalOptionCardRects[3].w - 100 -
                                         textRects["holdCount"].w / 2;
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
                      holdCount--;
                      texts["holdCount"] = getTextureFromText(
                          gRenderer, gFont, std::to_string(holdCount),
                          &colors["primary"], &textRects["holdCount"].w,
                          &textRects["holdCount"].h);
                      textRects["holdCount"].x = generalOptionCardRects[3].x +
                                                 generalOptionCardRects[3].w -
                                                 100 -
                                                 textRects["holdCount"].w / 2;
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
              SDL_Rect scrolledRect =
                  getScrolled(&generalOptionCardRects[2], scroll);
              if (SDL_PointInRect(&mousePos, &scrolledRect)) {
                openImagePicker();
              }
              scrolledRect = getScrolled(&generalOptionCardRects[4], scroll);
              if (SDL_PointInRect(&mousePos, &scrolledRect)) {
                saveHoldsToFile(
                    holds,
                    "/data/user/0/com.bouldrovka.app/files/boulder/holds.txt");
              }
              scrolledRect = getScrolled(&generalOptionCardRects[5], scroll);
              if (SDL_PointInRect(&mousePos, &scrolledRect)) {
                holds = readHoldsFromFile(
                    "/data/user/0/com.bouldrovka.app/files/boulder/holds.txt");
                holdCount = (int)holds.size();
                texts["holdCount"] = getTextureFromText(
                    gRenderer, gFont, std::to_string(holdCount),
                    &colors["primary"], &textRects["holdCount"].w,
                    &textRects["holdCount"].h);
                textRects["holdCount"].x = generalOptionCardRects[3].x +
                                           generalOptionCardRects[3].w - 100 -
                                           textRects["holdCount"].w / 2;
              }
              scrolledRect = getScrolled(&generalOptionCardRects[6], scroll);
              if (SDL_PointInRect(&mousePos, &scrolledRect)) {
                holds.clear();
                holdCount = 0;
                texts["holdCount"] = getTextureFromText(
                    gRenderer, gFont, std::to_string(holdCount),
                    &colors["primary"], &textRects["holdCount"].w,
                    &textRects["holdCount"].h);
                textRects["holdCount"].x = generalOptionCardRects[3].x +
                                           generalOptionCardRects[3].w - 100 -
                                           textRects["holdCount"].w / 2;
              }

              // generation option + / -
              for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 4; j++) {
                  if (SDL_PointInRect(&mousePos,
                                      &generationOptionHitboxes[i][j])) {
                    // increase min
                    if (j == 0) {
                      holdTypeCount[i].a++;
                      if (holdTypeCount[i].a > holdTypeCount[i].b) {
                        holdTypeCount[i].b = holdTypeCount[i].a;
                      }
                    }
                    if (j == 1)
                      holdTypeCount[i].b++;
                    if (j == 2)
                      holdTypeCount[i].a--;
                    if (j == 3) {
                      holdTypeCount[i].b--;
                      if (holdTypeCount[i].a > holdTypeCount[i].b) {
                        holdTypeCount[i].a = holdTypeCount[i].b;
                      }
                    }
                    if (holdTypeCount[i].a < 0)
                      holdTypeCount[i].a = 0;
                    if (holdTypeCount[i].b < 0)
                      holdTypeCount[i].b = 0;
                    std::string string = std::to_string(holdTypeCount[i].a) +
                                         " až " +
                                         std::to_string(holdTypeCount[i].b);
                    SDL_DestroyTexture(texts[generationValues[i]]);
                    texts[generationValues[i]] = getTextureFromText(
                        gRenderer, gSmallFont, string, &colors["gray"],
                        &textRects[generationValues[i]].w,
                        &textRects[generationValues[i]].h);
                  }
                }
              }
            }
          }
        }
      }
    }

    // if (touchDy > 1) scroll += touchDy--;
    // if (touchDy < -1) scroll += touchDy++;
    // if (scroll > 0) scroll = 0;
    if (scroll < -1.5f * (float)WINDOW_HEIGHT)
      scroll = -1.5f * (float)WINDOW_HEIGHT;
    if (scroll > 0)
      scroll = 0;

    // -----------------------------------------------  rendering
    // ------------------------------------------

    // set background
    SDL_SetRenderDrawColor(gRenderer, colors["white"]);
    SDL_RenderClear(gRenderer);

    if (scene == MAIN) {

      // main image
      if (textures["backgroundImage"] != nullptr)
        drawMainImage(gRenderer, WINDOW_HEIGHT, WINDOW_WIDTH,
                      textures["backgroundImage"], &mainImageRect,
                      textures["mainImageMask"]);
      else if (textures["mainImage"] != nullptr)
        drawMainImage(gRenderer, WINDOW_HEIGHT, WINDOW_WIDTH,
                      textures["mainImage"], &mainImageRect,
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
      SDL_RenderCopy(gRenderer, texts["title"], nullptr, &textRects["title"]);
    }

    if (scene == OPTIONS) {
      bool corners[4] = {true, true, false, false};

      // background
      SDL_SetRenderDrawColor(gRenderer, 255, 241, 233, 255);
      SDL_RenderFillRoundedRect(gRenderer, 0, getScrolled(170, scroll),
                                mainImageRect.w, WINDOW_HEIGHT * 2, 50,
                                corners);

      // general option cards
      SDL_Rect scrolledRect =
          getScrolled(&textRects["generalOptionsTitle"], scroll);
      SDL_RenderCopy(gRenderer, texts["generalOptionsTitle"], nullptr,
                     &scrolledRect);
      for (int i = 0; i < 7; i++) {
        textureWithDimensions text = {
            texts[generalOptionCardTexts[i]],
            textRects[generalOptionCardTexts[i]].w,
            textRects[generalOptionCardTexts[i]].h,
        };
        scrolledRect = getScrolled(&generalOptionCardRects[i], scroll);
        drawCardWithIcon(gRenderer, &scrolledRect, 38, text, &colors["primary"],
                         generalOptionCardIcons[i]);

        // number of holds
        if (i == 3) {
          scrolledRect = getScrolled(&textRects["holdCount"], scroll);
          SDL_RenderCopy(gRenderer, texts["holdCount"], nullptr, &scrolledRect);
        }
      }

      // generation option cards
      for (int i = 0; i < 6; i++) {
        textureWithDimensions title = {
            texts[generationOptionCardTexts[i]],
            textRects[generationOptionCardTexts[i]].w,
            textRects[generationOptionCardTexts[i]].h,
        };
        textureWithDimensions min = {texts["min"], textRects["min"].w,
                                     textRects["min"].h};
        textureWithDimensions max = {texts["max"], textRects["max"].w,
                                     textRects["max"].h};
        textureWithDimensions value = {texts[generationValues[i]],
                                       textRects[generationValues[i]].w,
                                       textRects[generationValues[i]].h};
        scrolledRect = getScrolled(&generationOptionCardRects[i], scroll);
        drawCardWithValue(gRenderer, &scrolledRect, 38, title, min, max, value,
                          textures["whiteMinusImage"],
                          textures["whitePlusImage"], generationIconColors[i],
                          generationOptionHitboxes[i]);
      }
    }

    SDL_SetRenderDrawColor(gRenderer, colors["white"]);
    SDL_RenderFillRect(gRenderer, &topBackground);
    drawSettingsButton(gRenderer, textures["threeBarsImage"],
                       &settingsButtonRect);
    drawBackButton(gRenderer, textures["leftArrowImage"], &backButtonRect);
    SDL_RenderPresent(gRenderer);
  }

  // --------------------------------------------------- clean up
  // -----------------------------------------------
  for (auto &pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
  textures.clear();

  for (auto &pair : texts) {
    SDL_DestroyTexture(pair.second);
  }
  texts.clear();

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
