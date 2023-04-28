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
            SDL_Texture * tex =  SDL_CreateTextureFromSurface(renderer,tempSurface);
            SDL_FreeSurface(tempSurface);
            return tex;
        }

bool checkCollision(SDL_Rect a, SDL_Rect b )
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA <= topB ) return false;
    if( topA >= bottomB ) return false;
    if( rightA <= leftB ) return false;
    if( leftA >= rightB ) return false;
    return true;
}

class Game
{
    public:
        Game();
        ~Game();
        void init(const char *title, int width, int height, bool fullscreen);
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running (){return isRunning;};
        int step = 1;
        bool run_arrow = false;
        bool run_bullet_player = false;
        bool run_bullet_enemy = false;
        bool check = false;
        bool win = false;
        bool lose = false;
        bool pick_done = false;
        bool isRunning;
        SDL_Rect scrR, destR;
    private:
        SDL_Window *window;
        SDL_Texture *texture;

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
        SDL_Rect destRect;
    private:
        SDL_Texture *cannon_Texture;
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
        double veloc = 400;
        int angle = -1;
        int t = 0;
        int damage = 20;
        int type = -1;
        int count_2 = 3;
        int count_3 = 2;
        int count_4 = 1;
        bool can_count = false;
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
        double veloc = 300;
        int angle = -1;
        int t = 0;
        int type = 4;
        int damage=  20;
        bool can_count = false;
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
        int degrees = 0;
        SDL_Rect destRect;
        int d = 1;
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
Game *game = nullptr;
PickBullet *pick_bullet;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags = 0;
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
        isRunning = true;
    }
    background =        LoadTexture("image/bg1.png");
    player_castle =     new Castle("image/ice_castle_left.png",0,400);
    enemy_castle =      new Castle("image/earth_castle_right.png",1200,400);
    player_cannon =     new Cannon("image/ice_cannon_1.png",210,540);
    enemy_cannon =      new Cannon("image/earth_cannon_right.png",1110,540);
    arrow =             new Arrow("image/arrow.png",280,500);
    player_bullet =     new PlayerBullet("image/ice_bullet_3.png",0,0);
    enemy_bullet=       new EnemyBullet("image/earth_bullet_3.png",0,0);
    player_hp =         new Hp("image/hp_100.png",30,350);
    enemy_hp =          new Hp("image/hp_100.png",1250,350);
    pick_bullet =       new PickBullet("image/pick.png",100,100);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            isRunning = false;
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
    if (pick_done == true)
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
    if (win == true)SDL_RenderCopy(renderer,LoadTexture("image/winner.png"),NULL,NULL);
    if (lose == true)SDL_RenderCopy(renderer,LoadTexture("image/winner.png"),NULL,NULL);

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
    xpos = x;
    ypos = y;
}

void Castle::update_castle()
{
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 200;
    destRect.h = 200;
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
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 60;
    destRect.h = 60;
}

void Cannon::render_cannon()
{
    SDL_RenderCopy(renderer,cannon_Texture,NULL,&destRect);
}

Arrow::Arrow(const char* texturesheet,int x,int y)
{
    arrow_Texture=LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void Arrow::update_arrow()
{
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 60;
    destRect.h = 60;
    center.x = 0;
    center.y = 30;
}

void Arrow::render_arrow()
{
    SDL_RenderCopyEx(renderer,arrow_Texture,NULL,&destRect,-degrees,&center,SDL_FLIP_NONE);
}

PlayerBullet::PlayerBullet(const char* texturesheet,double x,double y)
{
    bullet_Texture=LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void PlayerBullet::update_bullet()
{
    if (angle == -1)
    {
    destRect.x = xpos;
    destRect.y = ypos;
    }
    else
    {
        destRect.x = xpos+veloc*cos(angle*3.14/180)*0.01666*t;
        destRect.y = ypos-veloc*sin(angle*3.14/180)*0.01666*t+5*0.1666*0.01666*t*t;
    }
    string s = "image/ice_bullet_"+to_string(type)+".png";
    bullet_Texture = LoadTexture(s.c_str());
    if (type==1)
    {
        damage=10;
        xpos=245;
        ypos=560;
        destRect.w = 25;
        destRect.h = 30;

    }
    else if (type== 2)
    {
        damage = 15;
        xpos = 245;
        ypos = 560;
        destRect.w = 20;
        destRect.h = 30;
    }
    else if (type== 3)
    {
        damage=25;
        xpos = 245;
        ypos = 565;
        destRect.w = 35;
        destRect.h = 35;
    }
    else if (type==4)
    {
        damage=40;
        xpos = 240;
        ypos = 560;
        destRect.w = 30;
        destRect.h = 30;
    }

}

void PlayerBullet::render_bullet()
{
    SDL_RenderCopyEx(renderer,bullet_Texture,NULL,&destRect,-atan((veloc*sin(angle*3.14/180)-10*0.1666*t)/(veloc*cos(angle*3.14/180)))*180/3.14,NULL,SDL_FLIP_NONE);
}

void PlayerBullet::reset()
{
    angle = -1;
    t = 0;
    can_count = false;
}

EnemyBullet::EnemyBullet(const char* texturesheet,double x,double y)
{
    bullet_Texture = LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void EnemyBullet::update_bullet()
{
    if (angle == -1)
    {
    destRect.x = xpos;
    destRect.y = ypos;
    }
    else
    {
        destRect.x = xpos-veloc*cos(angle*3.14/180)*0.01666*t;
        destRect.y = ypos-veloc*sin(angle*3.14/180)*0.01666*t+5*0.1666*0.01666*t*t;
    }
    string s = "image/earth_bullet_"+to_string(type)+".png";
    bullet_Texture = LoadTexture(s.c_str());
    if (type==1)
    {
        damage=10;
        xpos=1100;
        ypos=560;
        destRect.w = 25;
        destRect.h = 30;

    }
    else if (type== 2)
    {
        damage = 15;
        xpos = 1100;
        ypos = 560;
        destRect.w = 20;
        destRect.h = 30;
    }
    else if (type== 3)
    {
        damage=25;
        xpos = 1100;
        ypos = 565;
        destRect.w = 35;
        destRect.h = 35;
    }
    else if (type==4)
    {
        damage=40;
        xpos = 1100;
        ypos = 560;
        destRect.w = 30;
        destRect.h = 30;
    }
}

void EnemyBullet::render_bullet()
{
    SDL_RenderCopyEx(renderer,bullet_Texture,NULL,&destRect,180+atan((veloc*sin(angle*3.14/180)-10*0.1666*t)/(veloc*cos(angle*3.14/180)))*180/3.14,NULL,SDL_FLIP_NONE);
}

void EnemyBullet::reset()
{
    angle = -1;
    t = 0;
    can_count = false;
}

Hp::Hp(const char* texturesheet,int x,int y)
{
    hp_Texture = LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void Hp::update_hp()
{
    string s = "image/hp_"+to_string(hp_now)+".png";
    hp_Texture = LoadTexture(s.c_str());
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 120;
    destRect.h = 20;
}

void Hp::render_hp()
{
    SDL_RenderCopy(renderer, hp_Texture,NULL,&destRect);
}

PickBullet::PickBullet(const char* texturesheet,int x,int y)
{
    pick_Texture = LoadTexture(texturesheet);
    xpos = 0;
    ypos = 0;

}

void PickBullet::update_pick()
{
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 455;
    destRect.h = 108;
}

void PickBullet::render_pick()
{
    SDL_RenderCopy(renderer, pick_Texture,NULL,&destRect);
}

int main(int argc, char *argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;
    int num = 0;
    game= new Game();
    game->init("Avatar",1400,700,false);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(20000, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* sound = Mix_LoadWAV("image/beat.wav");
    Mix_Chunk* fight = Mix_LoadWAV("image/fight.wav");
    Mix_Chunk* explosion = Mix_LoadWAV("image/explosion.wav");
    Mix_PlayChannel(-1, sound, 10);
    int dem = 1500;
    SDL_Rect menu_rect = {665,670,70,30};
    SDL_Rect menu_expand_rect = {630,200,130,300};
    SDL_Rect help_rect = {350,100,700,500};
    SDL_Rect quit_rect = {350,100,700,500};
    SDL_Rect turn_rect = {680,200,40,80};
    SDL_Rect play_rect = {640,300,100,50};
    bool menu_show = true;
    bool menu_expand_show = false;
    bool help_show = false;
    bool quit_show = false;
    bool begin_show = true;
    bool isMute = false;
    bool start = true;
    bool play = false;
    bool check_bullet = false;
    int rand_angle = 30+rand()%40;
    int rand_power = 200+rand()%400;
    int rand_bullet = 1+rand()%4;
    int power = 10;
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        game->render();
        if (menu_show)          SDL_RenderCopy(renderer,LoadTexture("image/menu.png"),NULL,&menu_rect);
        if (menu_expand_show)   SDL_RenderCopy(renderer,LoadTexture("image/menu_expand.png"),NULL,&menu_expand_rect);
        if (help_show)          SDL_RenderCopy(renderer,LoadTexture("image/help.png"),NULL,&help_rect);
        if (quit_show)          SDL_RenderCopy(renderer,LoadTexture("image/quit.png"),NULL,&quit_rect);
        frameTime=SDL_GetTicks()-frameStart;
        int x,y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        if (player_hp->hp_now == 0) game->lose =true;
        if (enemy_hp->hp_now == 0) game->win = true;
        if ( start )
        {
            if (dem>1300)
            {
                SDL_RenderCopy(renderer,LoadTexture("image/begin.png"),NULL,NULL);
                 dem--;
            }
            else
            {
                if (!play) SDL_RenderCopy(renderer,LoadTexture("image/play.png"),NULL,&play_rect);
                else
                {
                    string y = "image/turn_"+to_string(dem/325)+".png";
                    SDL_RenderCopy(renderer,LoadTexture(y.c_str()),NULL,&turn_rect);
                    dem--;
                    if (dem==0) start = false;
                }
                if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                    if (x>=653 && x<=747 && y>=302 && y<=345 )
                    {
                        play = true;
                        Mix_PlayChannel(-1, fight, 0);
                    }
            }
        }
        else
        {
            if  (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                if ( menu_show == true && x>=665 && x<=735 && y>=670 && y<=700)
                {
                    menu_show = false;
                    menu_expand_show = true;
                }
                else if (menu_expand_show == true)
                {
                    if (x>=644 && x<=746 && y>=273 && y<=316)
                    {
                        menu_expand_show = false;
                        menu_show = true;
                    }
                    else if (x>=644 && x<=746 && y>=331 && y<=374)
                    {
                        menu_expand_show = false;
                        help_show = true;
                    }
                    else if (x>=644 && x<=746 && y>=387 && y<=432)
                    {
                        if( isMute )
                        {
                            Mix_ResumeMusic();
                            Mix_Volume(-1,64);
                        }
                        else
                        {
                            Mix_PauseMusic();
                            Mix_Volume(-1,0);
                        }
                        isMute = !isMute;
                        menu_expand_show = false;
                        menu_show = true;
                    }
                    else if (x>=644 && x<=746 && y>=448 && y<=490)
                    {
                        menu_expand_show = false;
                        quit_show = true;
                    }
                }
                else if (help_show == true)
                {
                    if (x>=943 && x<=984 && y>=133 && y<=150)
                    {
                        help_show = false;
                        menu_show = true;
                    }
                }
                else if (quit_show == true)
                {
                    if (x>=615 && x<=704 && y>=404 && y<=440)
                    {
                        quit_show = false;
                        menu_show = true;
                    }
                    else if (x>=715 && x<=804 && y>=404 && y<=440)
                    {
                        start = true;
                        dem = 1800;
                        quit_show = false;
                        fight = Mix_LoadWAV("image/fight.wav");
                        explosion = Mix_LoadWAV("image/explosion.wav");
                        play = false;
                        menu_show = true;
                        player_hp->hp_now = 100;
                        enemy_hp->hp_now = 100;
                        game->run_bullet_player = false;
                        game->run_bullet_enemy = false;
                        game->run_arrow = false;
                        game->step = 1;
                        game->win = false;
                        game->lose = false;
                        player_bullet->count_2 = 3;
                        player_bullet->count_3 = 2;
                        player_bullet->count_4 = 1;
                    }
                }
            }
            if (game->step==1)
            {
               game->pick_done = true;
               if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
               {
                   if (sqrt((x-59)*(x-59)+(y-83)*(y-83))<=22)
                   {
                       player_bullet->type=1;
                       game->check = true;
                       game->pick_done = false;
                   }
                   else if (sqrt((x-156)*(x-156)+(y-82)*(y-82))<=22 && player_bullet->count_2 >=1)
                   {
                       player_bullet->type = 2;
                       game->check = true;
                       game->pick_done = false;
                       player_bullet->count_2 --;
                   }
                   else if (sqrt((x-402)*(x-402)+(y-81)*(y-81))<=27 && player_bullet->count_4 >=1)
                   {
                       player_bullet->type = 4;
                       game->check = true;
                       game->pick_done = false;
                       player_bullet->count_4--;
                   }
                   else if (x>=229 && x<=350&&y>=70&&y<=98 && player_bullet->count_3 >=1)
                   {
                       player_bullet->type = 3;
                       game->check = true;
                       game->pick_done = false;
                       player_bullet->count_3--;
                   }
               }
            }
            else if (game->step == 2)
            {
                game->run_bullet_player = true;
                game->run_arrow = true;
                if (x<393)arrow->degrees = 90;
                else if (y>542)arrow->degrees = 30;
                else
                {
                    arrow->degrees=atan((abs(y-542))*1.0/(abs(x-393)))*180/3.14;
                    if (arrow->degrees < 30 ) arrow->degrees = 30;
                }
                if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
                {
                    player_bullet->angle = arrow->degrees;
                    power = power + 5;
                }
                else if (power>10)
                {
                    player_bullet->can_count = true;
                    game->run_arrow = false;
                    player_bullet->veloc = power;
                }

                if (player_bullet->can_count)
                {
                        player_bullet->t++;
                        if (checkCollision(player_bullet->destRect,enemy_castle->destRect))
                        {
                            game->check = true;
                            enemy_hp->hp_now = enemy_hp->hp_now-player_bullet->damage;
                            game->run_bullet_player = false;
                            player_bullet->reset();
                            player_bullet->can_count = false;
                            Mix_PlayChannel(-1, explosion, 0);
                        }
                        else if (player_bullet->destRect.x>1400||player_bullet->destRect.y>580)
                        {
                            game->check = true;
                            game->run_bullet_player = false;
                            player_bullet->reset();
                            Mix_PlayChannel(-1, explosion, 0);
                        }
                }
            }
            else if (game->step==3)
            {
                game->run_bullet_enemy = true;
                enemy_bullet->angle = rand_angle;
                enemy_bullet->veloc = rand_power;
                enemy_bullet->type = rand_bullet;
                enemy_bullet->can_count=true;
                if (enemy_bullet->can_count==true)enemy_bullet->t++;
                if (checkCollision(enemy_bullet->destRect,player_castle->destRect))
                {
                    game->check = true;
                    player_hp->hp_now = player_hp->hp_now-enemy_bullet->damage;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                }
                if (enemy_bullet->destRect.x<0||enemy_bullet->destRect.y>580)
                {
                    game->check = true;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                }
            }
            if (game->check)
            {
                game->step = game->step+1;
                game->check = false;
            }
            if (game->step == 4)
            {
                game->step = 1;
                rand_angle = 30+rand()%40;
                rand_power = 200+rand()%400;
                rand_bullet = 1+rand()%4;
                power = 10;
            }

            if (frameDelay>frameTime)
            {
                SDL_Delay(frameDelay-frameTime);
            }
            explosion = Mix_LoadWAV("image/explosion.wav");
        }
        SDL_RenderPresent(renderer);
        cout<<x<<" "<<y<<endl;
    }
    game->clean();
    return 0;
}
