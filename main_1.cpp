#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
using namespace std;
SDL_Renderer *renderer;
SDL_Texture *background;

SDL_Texture* LoadTexture(const char* text)
        {
            SDL_Surface * tempSurface = IMG_Load(text);
            SDL_Texture * tex=  SDL_CreateTextureFromSurface(renderer,tempSurface);
            SDL_FreeSurface(tempSurface);
            return tex;
        }
class Game
{
    public:
        Game();
        ~Game();
        void init(const char*title, int width, int height, bool fullscreen);
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running (){return isRunning;};

        SDL_Rect scrR, destR;
    private:
        bool isRunning;
        SDL_Window *window;

        SDL_Texture *texture;

        int cnt;

};
class Castle
{
   public:
        Castle(const char* texturesheet,int x,int y);
        ~Castle();
        void update_castle();
        void render_castle();
        int xpos;
        int ypos;
    private:
        SDL_Texture *castle_Texture;
        SDL_Rect destRect;
};
class Cannon
{
    public:
        Cannon(const char* texturesheet,int x,int y);
        ~Cannon();
        void update_cannon();
        void render_cannon();
        int xpos;
        int ypos;
    private:
        SDL_Texture *cannon_Texture;
        SDL_Rect destRect;

};
class Bullet
{
     public:
        Bullet(const char* texturesheet,int x,int y);
        ~Bullet();
        void update_bullet();
        void render_bullet();
        int xpos;
        int ypos;
    private:
        SDL_Texture *bullet_Texture;
        SDL_Rect destRect;
};
class Arrow
{
    public:
        Arrow(const char* texturesheet,int x,int y);
        ~Arrow();
        void update_arrow(bool stop);
        void render_arrow();
        int xpos;
        int ypos;
        int degrees=-50;
        SDL_Rect destRect;
        int d=1;
        SDL_Point center;
        bool stop = false;
    private:
        SDL_Texture *arrow_Texture;

};
class Health
{
    public:
        Bullet(const char* texturesheet,int x,int y);
        ~Bullet();
        void update_bullet();
        void render_bullet();
        int xpos;
        int ypos;
    private:
        SDL_Texture *bullet_Texture;
        SDL_Rect destRect;
};
Castle *player_castle;
Castle *enemy_castle;
Cannon *player_cannon;
Cannon *enemy_cannon;
Bullet *player_bullet;
Bullet *enemy_bullet;
Arrow *arrow;
Game *game=nullptr;
Game::Game()
{

}
Game::~Game()
{

}
void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags=0;
    if (fullscreen)
    {
        flags=SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        cout<<"Subsystems...."<<endl;
        window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,flags);
        if (window)
        {
            cout<<"Window created"<<endl;
        }
        renderer = SDL_CreateRenderer(window,-1,0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            cout<<"Renderer created"<<endl;
        }
        isRunning=true;
    }
    background=LoadTexture("image/bg1.png");
    player_castle=new Castle("image/ice_castle_left.png",110,410);
    enemy_castle=new Castle("image/earth_castle_right.png",1130,400);
    player_cannon=new Cannon("image/ice_cannon_left.png",280,510);
    enemy_cannon=new Cannon("image/earth_cannon_right.png",1065,511);
    arrow =new Arrow("image/arrow.png",330,490);
}
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            isRunning=false;
            break;
        default:
            break;
    }
}
void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,background,NULL,NULL);
    player_castle->render_castle();
    enemy_castle->render_castle();
    player_cannon->render_cannon();
    enemy_cannon->render_cannon();
    arrow->render_arrow();
    SDL_RenderPresent(renderer);
}
void Game::update()
{
    player_castle->update_castle();
    enemy_castle->update_castle();
    player_cannon->update_cannon();
    enemy_cannon->update_cannon();
    arrow->update_arrow(arrow->stop);
}
void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout<<"closed"<<endl;
}
Castle::Castle(const char* texturesheet,int x,int y)
{
   castle_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Castle::update_castle()
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=150;
    destRect.h=150;
}
void Castle::render_castle()
{
    SDL_RenderCopy(renderer, castle_Texture,NULL,&destRect);
}
Cannon::Cannon(const char* texturesheet,int x,int y)
{
    cannon_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Cannon::update_cannon()
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=40;
    destRect.h=40;
}
void Cannon::render_cannon()
{
    SDL_RenderCopy(renderer,cannon_Texture,NULL,&destRect);
}
Arrow::Arrow(const char* texturesheet,int x,int y)
{
    arrow_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Arrow::update_arrow(bool stop)
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=40;
    destRect.h=40;
    center.x=0;
    center.y=20;
    if (!stop)
    {
        degrees=degrees+1*d;
        if (degrees>20)d=-1;
        if (degrees<-65)d=+1;
    }

}
void Arrow::render_arrow()
{
    SDL_RenderCopyEx(renderer,arrow_Texture,NULL,&destRect,degrees,&center,SDL_FLIP_NONE);
}
int main(int argc, char *argv[])
{
    const int FPS=60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    game= new Game();
    game->init("Avatar",1400,600,false);
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        game->render();
        frameTime=SDL_GetTicks()-frameStart;
        int x, y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if (!arrow->stop)arrow->stop=true;
            else arrow->stop=false;
            cout<<arrow->degrees<<endl;
        }
        if (frameDelay>frameTime)
        {
            SDL_Delay(frameDelay-frameTime);
        }
    }
    game->clean();
    return 0;

}
