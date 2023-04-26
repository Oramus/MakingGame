#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <math.h>
#include <string>
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
        int step=1;
        bool run_arrow=false;
        bool check=false;
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
        Bullet(const char* texturesheet,double x,double y);
        ~Bullet();
        void update_bullet();
        void render_bullet();
        double xpos;
        double ypos;
        const double veloc = 300;
        int angle=-1;
        int t=0;
        bool can_count=false;
        SDL_Rect destRect;
    private:
        SDL_Texture *bullet_Texture;

};
class Arrow
{
    public:
        Arrow(const char* texturesheet,int x,int y);
        ~Arrow();
        void update_arrow();
        void render_arrow();
        int xpos;
        int ypos;
        int degrees=0;
        SDL_Rect destRect;
        int d=1;
        SDL_Point center;
    private:
        SDL_Texture *arrow_Texture;

};
class Hp
{
    public:
        Hp(const char* texturesheet,int x,int y);
        ~Hp();
        void update_hp();
        void render_hp();
        int xpos;
        int ypos;
        int hp_now = 100;
        SDL_Texture *hp_Texture;
        SDL_Rect destRect;

};
class Turn
{
   public:
        Turn(const char* texturesheet,int x,int y);
        ~Turn();
        void update_turn();
        void render_turn();
        int turn_now=1;
        int xpos;
        int ypos;
    private:
        SDL_Texture *turn_Texture;
        SDL_Rect destRect;
};
Castle *player_castle;
Castle *enemy_castle;
Cannon *player_cannon;
Cannon *enemy_cannon;
Bullet *player_bullet;
Bullet *enemy_bullet;
Hp *player_hp;
Hp *enemy_hp;
Arrow *arrow;
Turn *turn;
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
    player_castle=new Castle("image/ice_castle_left.png",110,443);
    enemy_castle=new Castle("image/earth_castle_right.png",1100,443);
    player_cannon=new Cannon("image/ice_cannon_left.png",320,584);
    enemy_cannon=new Cannon("image/earth_cannon_right.png",1025,584);
    arrow =new Arrow("image/arrow.png",380,550);
    player_bullet=new Bullet("image/ice_bullet_3.png",340,614);
    player_hp=new Hp("image/hp_100.png",150,420);
    enemy_hp=new Hp("image/hp_100.png",1150,420);
    turn=new Turn("image/turn_1.png",700,200);
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
    player_bullet->render_bullet();
    player_cannon->render_cannon();
    enemy_cannon->render_cannon();
    player_hp->render_hp();
    enemy_hp->render_hp();
    if (run_arrow)arrow->render_arrow();
    turn->render_turn();
    SDL_RenderPresent(renderer);
}
void Game::update()
{
    player_castle->update_castle();
    enemy_castle->update_castle();
    player_cannon->update_cannon();
    enemy_cannon->update_cannon();
    player_bullet->update_bullet();
    arrow->update_arrow();
    player_hp->update_hp();
    enemy_hp->update_hp();
    turn->update_turn();
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
    destRect.w=200;
    destRect.h=200;
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
    destRect.w=60;
    destRect.h=60;
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
void Arrow::update_arrow()
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=50;
    destRect.h=50;
    center.x=0;
    center.y=25;
}
void Arrow::render_arrow()
{
    SDL_RenderCopyEx(renderer,arrow_Texture,NULL,&destRect,-degrees,&center,SDL_FLIP_NONE);
}
Bullet::Bullet(const char* texturesheet,double x,double y)
{
   bullet_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Bullet::update_bullet()
{
    if (angle ==-1)
    {
    destRect.x=xpos;
    destRect.y=ypos;
    }
    else
    {
        destRect.x=xpos+veloc*cos(angle*3.14/180)*0.01666*t;
        destRect.y=ypos-veloc*sin(angle*3.14/180)*0.01666*t+5*0.1666*0.01666*t*t;
    }
    destRect.w=35;
    destRect.h=35;
}
void Bullet::render_bullet()
{
    //SDL_RenderCopy(renderer, bullet_Texture,NULL,&destRect);
    SDL_RenderCopyEx(renderer,bullet_Texture,NULL,&destRect,-atan((veloc*sin(angle*3.14/180)-10*0.1666*t)/(veloc*cos(angle*3.14/180)))*180/3.14,NULL,SDL_FLIP_NONE);

}
Hp::Hp(const char* texturesheet,int x,int y)
{
    hp_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Hp::update_hp()
{
    string s="image/hp_"+to_string(hp_now)+".png";
    hp_Texture=LoadTexture(s.c_str());
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=120;
    destRect.h=20;
}
void Hp::render_hp()
{
    SDL_RenderCopy(renderer, hp_Texture,NULL,&destRect);
}
Turn::Turn(const char* texturesheet,int x,int y)
{
    turn_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void Turn::update_turn()
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=40;
    destRect.h=80;
}
void Turn::render_turn()
{
    SDL_RenderCopy(renderer, turn_Texture,NULL,&destRect);
}
int main(int argc, char *argv[])
{
    const int FPS=60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;
    int num=0;
    game= new Game();
    game->init("Avatar",1400,700,false);
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        game->render();
        frameTime=SDL_GetTicks()-frameStart;
        //step 1: chọn đạn
        //step 2: chọn góc + lực
        //step 3: tính hp
        //step 4: chọn đạn ( ngẫu nhiên )
        //step 5: chọn góc + lực ( ngẫu nhiên )
        //step 6: tính hp
        // kết thúc sang round kế
        // nếu round ==10 mà chưa thắng thì game over
        if (game->step==1)
        {
            int x, y;
            Uint32 buttons = SDL_GetMouseState(&x, &y);
            if (x<393)arrow->degrees=90;
            else if (y>542)arrow->degrees=0;
            else arrow->degrees=atan((abs(y-542))*1.0/(abs(x-393)))*180/3.14;
            if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                player_bullet->angle=arrow->degrees;
                player_bullet->can_count=true;
            }
            game->run_arrow=true;
            if (player_bullet->can_count==true)player_bullet->t++;
            if (player_bullet->destRect.x>1400||player_bullet->destRect.y>700)game->check=true;
        }
        if (game->check)
        {
            game->step=game->step+1;
            game->check=false;
            game->run_arrow=false;
        }
        if (frameDelay>frameTime)
        {
            SDL_Delay(frameDelay-frameTime);
        }
    }
    game->clean();
    return 0;
}
