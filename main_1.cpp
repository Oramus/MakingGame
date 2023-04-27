#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <math.h>
#include <string>
#include <cstdlib>
#include <vector>
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
bool checkCollision(SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
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
        bool run_bullet_player=false;
        bool run_bullet_enemy=false;
        bool check=false;
        bool win=false;
        bool lose=false;
        bool pick_done=false;

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
        SDL_Rect destRect;
    private:
        SDL_Texture *castle_Texture;

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
class PlayerBullet
{
     public:
        PlayerBullet(const char* texturesheet,double x,double y);
        ~PlayerBullet();
        void update_bullet();
        void render_bullet();
        void reset();
        double xpos;
        double ypos;
        const double veloc = 300;
        int angle=-1;
        int t=0;
        int damage=20;
        int type=-1;
        bool can_count=false;
        SDL_Rect destRect;
    private:
        SDL_Texture *bullet_Texture;
};
class EnemyBullet
{
     public:
        EnemyBullet(const char* texturesheet,double x,double y);
        ~EnemyBullet();
        void update_bullet();
        void render_bullet();
        void reset();
        double xpos;
        double ypos;
        const double veloc = 300;
        int angle=-1;
        int t=0;
        int type=0;
        int damage=20;
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
class PickBullet
{
    public:
        PickBullet(const char* texturesheet,int x,int y);
        ~PickBullet();
        void update_pick();
        void render_pick();
        int xpos;
        int ypos;
        SDL_Rect destRect;
    private:
        SDL_Texture *pick_Texture;
};
Castle *player_castle;
Castle *enemy_castle;
Cannon *player_cannon;
Cannon *enemy_cannon;
PlayerBullet *player_bullet;
EnemyBullet *enemy_bullet;
Hp *player_hp;
Hp *enemy_hp;
Arrow *arrow;
Turn *turn;
Game *game=nullptr;
PickBullet *pick_bullet;
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
    player_bullet=new PlayerBullet("image/ice_bullet_3.png",340,614);
    enemy_bullet=new EnemyBullet("image/earth_bullet_3.png",1000,584);
    player_hp=new Hp("image/hp_100.png",150,420);
    enemy_hp=new Hp("image/hp_100.png",1150,420);
    turn=new Turn("image/turn_1.png",700,200);
    pick_bullet=new PickBullet("image/pick.png",100,100);
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
    SDL_Rect show = {100,100,150,300};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,background,NULL,NULL);
    player_castle->render_castle();
    enemy_castle->render_castle();
    player_cannon->render_cannon();
    enemy_cannon->render_cannon();
    player_hp->render_hp();
    enemy_hp->render_hp();
    if (run_arrow)arrow->render_arrow();
    if (run_bullet_player)player_bullet->render_bullet();
    if (run_bullet_enemy)enemy_bullet->render_bullet();
    turn->render_turn();
    if (pick_done==true)
    {
        pick_bullet->render_pick();
        int x,y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        SDL_Rect show={x,y,150,300};
        if (sqrt((x-59)*(x-59)+(y-83)*(y-83))<=22)
            {
                SDL_RenderCopy(renderer,LoadTexture("image/intro_2.png"),NULL,&show);
            }
           else if (sqrt((x-156)*(x-156)+(y-82)*(y-82))<=22)
            {
                SDL_RenderCopy(renderer,LoadTexture("image/intro_2.png"),NULL,&show);

            }
           else if (sqrt((x-402)*(x-402)+(y-81)*(y-81))<=27)
            {
                SDL_RenderCopy(renderer,LoadTexture("image/intro_4.png"),NULL,&show);

            }
           else if (x>=229 && x<=350&&y>=70&&y<=98)
            {
                 SDL_RenderCopy(renderer,LoadTexture("image/intro_3.png"),NULL,&show);

            }
    }
    if (win==true)SDL_RenderCopy(renderer,LoadTexture("image/winner.png"),NULL,NULL);
    if (lose==true)SDL_RenderCopy(renderer,LoadTexture("image/winner.png"),NULL,NULL);
    SDL_RenderPresent(renderer);
}
void Game::update()
{
    player_castle->update_castle();
    enemy_castle->update_castle();
    player_cannon->update_cannon();
    enemy_cannon->update_cannon();
    player_bullet->update_bullet();
    enemy_bullet->update_bullet();
    arrow->update_arrow();
    player_hp->update_hp();
    enemy_hp->update_hp();
    turn->update_turn();
    pick_bullet->update_pick();
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
PlayerBullet::PlayerBullet(const char* texturesheet,double x,double y)
{
    bullet_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void PlayerBullet::update_bullet()
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
    string s="image/ice_bullet_"+to_string(type)+".png";
    bullet_Texture=LoadTexture(s.c_str());

}
void PlayerBullet::render_bullet()
{
    SDL_RenderCopyEx(renderer,bullet_Texture,NULL,&destRect,-atan((veloc*sin(angle*3.14/180)-10*0.1666*t)/(veloc*cos(angle*3.14/180)))*180/3.14,NULL,SDL_FLIP_NONE);
}
void PlayerBullet::reset()
{
    angle=-1;
    t=0;
    can_count=false;
}
EnemyBullet::EnemyBullet(const char* texturesheet,double x,double y)
{
    bullet_Texture=LoadTexture(texturesheet);
    xpos=x;
    ypos=y;
}
void EnemyBullet::update_bullet()
{
    if (angle ==-1)
    {
    destRect.x=xpos;
    destRect.y=ypos;
    }
    else
    {
        destRect.x=xpos-veloc*cos(angle*3.14/180)*0.01666*t;
        destRect.y=ypos-veloc*sin(angle*3.14/180)*0.01666*t+5*0.1666*0.01666*t*t;
    }
    destRect.w=35;
    destRect.h=35;

}
void EnemyBullet::render_bullet()
{
    SDL_RenderCopyEx(renderer,bullet_Texture,NULL,&destRect,180+atan((veloc*sin(angle*3.14/180)-10*0.1666*t)/(veloc*cos(angle*3.14/180)))*180/3.14,NULL,SDL_FLIP_NONE);
}
void EnemyBullet::reset()
{
    angle=-1;
    t=0;
    can_count=false;
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
    string s="image/turn_"+to_string(turn_now)+".png";
    turn_Texture=LoadTexture(s.c_str());
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=40;
    destRect.h=80;
}
void Turn::render_turn()
{
    SDL_RenderCopy(renderer, turn_Texture,NULL,&destRect);
}
PickBullet::PickBullet(const char* texturesheet,int x,int y)
{
    pick_Texture=LoadTexture(texturesheet);
    xpos=0;
    ypos=0;

}
void PickBullet::update_pick()
{
    destRect.x=xpos;
    destRect.y=ypos;
    destRect.w=455;
    destRect.h=108;
}
void PickBullet::render_pick()
{
    SDL_RenderCopy(renderer, pick_Texture,NULL,&destRect);
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
        if (game->step==1)
        {
            int x,y;
            Uint32 buttons = SDL_GetMouseState(&x, &y);
           game->pick_done=true;
           if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
           {
               if (sqrt((x-59)*(x-59)+(y-83)*(y-83))<=22)
               {
                   player_bullet->type=1;
                   game->check=true;
                   game->pick_done=false;
               }
               else if (sqrt((x-156)*(x-156)+(y-82)*(y-82))<=22)
               {
                   player_bullet->type=2;
                   game->check=true;
                   game->pick_done=false;
               }
               else if (sqrt((x-402)*(x-402)+(y-81)*(y-81))<=27)
               {
                   player_bullet->type=4;
                   game->check=true;
                   game->pick_done=false;
               }
               else if (x>=229 && x<=350&&y>=70&&y<=98)
               {
                   player_bullet->type=3;
                   game->check=true;
                   game->pick_done=false;
               }
           }
        }
        else if (game->step==2)
        {
            int x, y;
            Uint32 buttons = SDL_GetMouseState(&x, &y);
            game->run_bullet_player=true;
            if (x<393)arrow->degrees=90;
            else if (y>542)arrow->degrees=0;
            else arrow->degrees=atan((abs(y-542))*1.0/(abs(x-393)))*180/3.14;
            if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
            {
                player_bullet->angle=arrow->degrees;
                player_bullet->can_count=true;

            }
            game->run_arrow=true;
            if (player_bullet->can_count==true)player_bullet->t++;
            if (checkCollision(player_bullet->destRect,enemy_castle->destRect))
            {
                game->check=true;
                enemy_hp->hp_now=enemy_hp->hp_now-player_bullet->damage;
                game->run_bullet_player=false;
                game->run_arrow=false;
                player_bullet->reset();
                player_bullet->can_count=false;
            }
            if (player_bullet->destRect.x>1400||player_bullet->destRect.y>700)
            {
                game->check=true;
                game->run_arrow=false;
                game->run_bullet_player=false;
                player_bullet->reset();
            }
        }
        else if (game->step==3)
        {
            game->run_bullet_enemy=true;
            enemy_bullet->angle=50;//+rand()%10;
            enemy_bullet->can_count=true;
            if (enemy_bullet->can_count==true)enemy_bullet->t++;
            if (checkCollision(enemy_bullet->destRect,player_castle->destRect))
            {
                game->check=true;
                player_hp->hp_now=player_hp->hp_now-enemy_bullet->damage;
                game->run_bullet_enemy=false;
                enemy_bullet->reset();
            }
            if (enemy_bullet->destRect.x<0||enemy_bullet->destRect.y>700)
            {
                game->check=true;
                game->run_bullet_enemy=false;
                enemy_bullet->reset();
            }
        }
        if (game->check)
        {
            game->step=game->step+1;
            game->check=false;
        }
        if (game->step==4)
        {
            game->step=1;
            turn->turn_now++;
        }
        if (player_hp->hp_now==0)game->lose=true;
        if (enemy_hp->hp_now==0)game->win=true;
        if (frameDelay>frameTime)
        {
            SDL_Delay(frameDelay-frameTime);
        }
    }
    game->clean();
    return 0;
}
