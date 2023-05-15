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

SDL_Texture* LoadTexture(const char* text)      // Load texture
{
    SDL_Surface * tempSurface = IMG_Load(text);
    SDL_Texture * tex =  SDL_CreateTextureFromSurface(renderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

bool checkCollision(SDL_Rect a, SDL_Rect b )   // hàm kiểm tra va chạm
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

class Game                              // class cho game chính
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
        bool one_player = true;
        bool run_player_arrow = false;
        bool run_enemy_arrow = false;
        bool run_bullet_player = false;
        bool run_ice_bullet_3_1 = false;
        bool run_ice_bullet_3_3 = false;
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

class Castle                            //class cho thành
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

class Cannon                            // class cho pháo
{
    public:
        Cannon(const char* texturesheet,int x,int y,string z);
        ~Cannon();
        void update_cannon(string y);
        void render_cannon();
        int xpos;
        int ypos;
        int cannon_now = 1;
        SDL_Rect destRect;
        SDL_Rect destRect_ice_cannon[11];
        string type = "";
    private:
        SDL_Texture *cannon_Texture;
};

class PlayerBullet                      // class cho đạn của người chơi
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
        int damage = 10;
        int type = -1;
        int count_2 = 8;
        int count_3 = 5;
        int count_4 = 2;
        bool can_count = false;
        SDL_Rect destRect;
    private:
        SDL_Texture *bullet_Texture;
};

class EnemyBullet                       // class cho đạn của đối thủ
{
     public:
        EnemyBullet(const char* texturesheet,double x,double y);
        ~EnemyBullet();
        void update_bullet();
        void render_bullet();
        void reset();
        double xpos;
        double ypos;
        double veloc = 400;
        double max_veloc = 1000;
        double min_veloc = 1;
        int angle = -1;
        int t = 0;
        int type = 4;
        int damage=  20;
        int count_2 = 8;
        int count_3 = 5;
        int count_4 = 2;
        bool can_count = false;
        bool collision = false;
        bool out = false;
        SDL_Rect destRect;

    private:
        SDL_Texture *bullet_Texture;
};

class PickBullet                        // class chọn đạn
{
    public:
        PickBullet(const char* texturesheet,int x,int y);
        ~PickBullet();
        void update_pick();
        void render_pick();
        int xpos;
        int ypos;
        int pick_now = 1;
        string type = "";
        SDL_Rect destRect_ice[11];
        SDL_Rect destRect_earth[11];
        SDL_Texture *pick_Texture;
};

class AnimationBullet                   // class hiệu ứng đạn va chạm
{
    public:
        AnimationBullet(const char* texturesheet,int x,int y);
        ~AnimationBullet();
        SDL_Texture *animation_Texture;
        int animation_now = 1;
        void update_position(int x,int y);
        void update_animation(string y);
        void render_animation(string y);
        SDL_Rect destRect_bullet_1[8];
        SDL_Rect destRect_ice_bullet_4[12];
        SDL_Rect destRect_ice_bullet_2[10];
        SDL_Rect destRect_earth_bullet_2[8];
        SDL_Rect destRect_earth_bullet_3[5];
        SDL_Rect destRect_earth_bullet_4[10];
};

class Arrow                             // class mũi tên
{
    public:
        Arrow(const char* texturesheet,int x,int y);
        ~Arrow();
        void update_arrow();
        void render_arrow();
        int xpos;
        int ypos;
        int degrees = 0;
        int arrow_now = 1;
        int d = 1;
        SDL_Rect destRect;
        SDL_Point center;
    private:
        SDL_Texture *arrow_Texture;
};

class Hp                                // class máu
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

Castle *player_castle;                  // khởi tạo biến
Castle *enemy_castle;
Cannon *player_cannon;
Cannon *enemy_cannon;
PlayerBullet *player_bullet;
PlayerBullet *ice_bullet_3_1;
PlayerBullet *ice_bullet_3_3;
EnemyBullet *enemy_bullet;
Hp *player_hp;
Hp *enemy_hp;
Arrow *player_arrow;
Arrow *enemy_arrow;
Game *game = nullptr;
PickBullet *pick_bullet;
AnimationBullet *animation_bullet;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int width, int height, bool fullscreen)          // khởi tạo game chính
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
    player_cannon =     new Cannon("image/animation_ice_cannon_1.png",210,540,"ice");
    enemy_cannon =      new Cannon("image/earth_cannon_right.png",1110,540,"earth");
    player_arrow =      new Arrow("image/arrow_1.png",280,500);
    enemy_arrow =       new Arrow("image/arrow_1.png",1100,500);
    player_bullet =     new PlayerBullet("image/ice_bullet_3.png",0,0);
    enemy_bullet=       new EnemyBullet("image/earth_bullet_3.png",0,0);
    player_hp =         new Hp("image/hp_100.png",30,350);
    enemy_hp =          new Hp("image/hp_100.png",1250,350);
    pick_bullet =       new PickBullet("image/pick_1.png",100,100);
    animation_bullet =  new AnimationBullet("image/animation_bullet_1.png",100,100);
    ice_bullet_3_1 =    new PlayerBullet("image/ice_bullet_3.png",0,0);
    ice_bullet_3_3 =    new PlayerBullet("image/ice_bullet_3.png",0,0);
}

void Game::handleEvents()                               // xử lí bàn phím
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if( event.type == SDL_QUIT )
    {
        isRunning = false;
    }
    else if( step == 4 && event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT:
                if (player_cannon->destRect_ice_cannon[1].x >100)
                {
                    player_cannon->destRect_ice_cannon[1].x -= 5;

                    player_arrow->destRect.x -=5;
                    player_bullet->xpos -=5;
                    ice_bullet_3_1->xpos -=5;
                    ice_bullet_3_3->xpos -=5;
                }
                break;
            case SDLK_RIGHT:
                if (player_cannon->destRect_ice_cannon[1].x < 600)
                {
                    player_cannon->destRect_ice_cannon[1].x += 5;
                    player_arrow->destRect.x +=5;
                    player_bullet->xpos +=5;
                    ice_bullet_3_1->xpos +=5;
                    ice_bullet_3_3->xpos +=5;
                }

                break;
        }
    }
    else if( step == 9 && game->one_player == false && event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT:
                if (enemy_cannon->destRect.x >800 )
                {
                    enemy_cannon->destRect.x -= 5;
                    enemy_arrow->destRect.x -=5;
                    enemy_bullet->xpos -=5;
                }
                break;
            case SDLK_RIGHT:
                if (enemy_cannon->destRect.x <1300 )
                {
                    enemy_cannon->destRect.x += 5;
                    enemy_arrow->destRect.x +=5;
                    enemy_bullet->xpos +=5;
                }
                break;
        }
    }
}

void Game::render()                 // hàm render chính
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
    if (run_player_arrow)player_arrow->render_arrow();
    if (run_enemy_arrow)enemy_arrow->render_arrow();
    if (run_bullet_player)player_bullet->render_bullet();
    if (run_bullet_enemy)enemy_bullet->render_bullet();
    if (run_ice_bullet_3_1)ice_bullet_3_1->render_bullet();
    if (run_ice_bullet_3_3)ice_bullet_3_3->render_bullet();
    if (pick_done == true) pick_bullet->render_pick();
}

void Game::update()                 // hàm update
{
    player_castle->update_castle();
    ice_bullet_3_1->update_bullet();
    ice_bullet_3_3->update_bullet();
    enemy_castle->update_castle();
    player_cannon->update_cannon("image/animation_ice_cannon_");
    enemy_cannon->update_cannon("image/animation_earth_cannon_");
    player_bullet->update_bullet();
    enemy_bullet->update_bullet();
    player_arrow->update_arrow();
    enemy_arrow->update_arrow();
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

Cannon::Cannon(const char* texturesheet,int x,int y, string z)
{
    cannon_Texture=LoadTexture(texturesheet);
    type = z;

    destRect_ice_cannon[1].w = 60;
    destRect_ice_cannon[1].h = 60;

    destRect_ice_cannon[2].w = 64;
    destRect_ice_cannon[2].h = 61;

    destRect_ice_cannon[3].w = 68;
    destRect_ice_cannon[3].h = 58;

    destRect_ice_cannon[4].w = 72;
    destRect_ice_cannon[4].h = 53;

    destRect_ice_cannon[5].w = 74;
    destRect_ice_cannon[5].h = 54;

    destRect_ice_cannon[6].w = 71;
    destRect_ice_cannon[6].h = 53;

    destRect_ice_cannon[7].w = 62;
    destRect_ice_cannon[7].h = 69;

    destRect_ice_cannon[8].w = 65;
    destRect_ice_cannon[8].h = 77;

    destRect_ice_cannon[9].w = 53;
    destRect_ice_cannon[9].h = 79;

    destRect_ice_cannon[10].w = 58;
    destRect_ice_cannon[10].h = 53;

    destRect_ice_cannon[1].x = 210;
    destRect_ice_cannon[1].y = 540;

    destRect.x = x;
    destRect.y = y;
    destRect.w = 60;
    destRect.h = 60;
}

void Cannon::update_cannon(string y)
{
    string s = y + to_string(cannon_now) + ".png";
    cannon_Texture = LoadTexture(s.c_str());
    for (int i=2;i<=10;i++)
    {
        destRect_ice_cannon[i].x = destRect_ice_cannon[1].x+60 - destRect_ice_cannon[i].w;
        destRect_ice_cannon[i].y = destRect_ice_cannon[1].y+60 - destRect_ice_cannon[i].h;
    }
}

void Cannon::render_cannon()
{
    if (type == "ice" ) SDL_RenderCopy(renderer,cannon_Texture,NULL,&destRect_ice_cannon[cannon_now]);
    if (type == "earth" ) SDL_RenderCopy(renderer,cannon_Texture,NULL,&destRect);
}

Arrow::Arrow(const char* texturesheet,int x,int y)
{
    arrow_Texture=LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 60;
    destRect.h = 60;
    center.x = 0;
    center.y = 30;
}

void Arrow::update_arrow()
{
    string s = "image/arrow_" + to_string(arrow_now)+ ".png";
    arrow_Texture = LoadTexture(s.c_str());
}

void Arrow::render_arrow()
{
    SDL_RenderCopyEx(renderer,arrow_Texture,NULL,&destRect,-degrees,&center,SDL_FLIP_NONE);
}

PlayerBullet::PlayerBullet(const char* texturesheet,double x,double y)
{
    bullet_Texture=LoadTexture(texturesheet);
    xpos = 262;
    ypos = 526;
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
        destRect.w = 20;
        destRect.h = 17;

    }
    else if (type== 2)
    {
        damage = 20;
        destRect.w = 20;
        destRect.h = 19;
    }
    else if (type== 3)
    {
        damage = 30;
        destRect.w = 45;
        destRect.h = 8;
    }
    else if (type==4)
    {
        damage=40;
        destRect.w = 22;
        destRect.h = 20;
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
    xpos = 1113;
    ypos = 526;
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
        destRect.w = 20;
        destRect.h = 17;

    }
    else if (type== 2)
    {
        damage = 20;
        destRect.w = 20;
        destRect.h = 17;
    }
    else if (type== 3)
    {
        damage=30;
        destRect.w = 55;
        destRect.h = 5;
    }
    else if (type==4)
    {
        damage=40;
        destRect.w = 32;
        destRect.h = 22;
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
    destRect_ice[1].h = 10;
    destRect_ice[2].h = 22;
    destRect_ice[3].h = 32;
    destRect_ice[4].h = 46;
    destRect_ice[5].h = 62;
    destRect_ice[6].h = 85;
    destRect_ice[7].h = 100;
    destRect_ice[8].h = 123;
    destRect_ice[9].h = 141;
    destRect_ice[10].h = 152;
    for (int i=1;i<11;i++)
    {
        destRect_ice[i].w = 466;
        destRect_ice[i].x = 0;
        destRect_ice[i].y = 700 - destRect_ice[i].h;
    }

    for (int i=1;i<11;i++)
    {
        destRect_earth[i].h = 15*i;
        destRect_earth[i].w = 466;
        destRect_earth[i].x = 934;
        destRect_earth[i].y = 700 - destRect_earth[i].h;
    }

}

void PickBullet::update_pick()
{
    string s ="";
    if (type == "ice")  s = "image/ice_pick_"+to_string(pick_now)+".png";
    if (type == "earth") s = "image/earth_pick_"+to_string(pick_now)+".png";
    pick_Texture = LoadTexture(s.c_str());
}

void PickBullet::render_pick()
{
    if (type == "ice")SDL_RenderCopy(renderer, pick_Texture,NULL,&destRect_ice[pick_now]);
    if (type == "earth")SDL_RenderCopy(renderer, pick_Texture,NULL,&destRect_earth[pick_now]);
}

AnimationBullet::AnimationBullet(const char* texturesheet,int x,int y)
{
    destRect_bullet_1[1].w = 26;
    destRect_bullet_1[1].h = 40;

    destRect_bullet_1[2].w = 53;
    destRect_bullet_1[2].h = 55;

    destRect_bullet_1[3].w = 64;
    destRect_bullet_1[3].h = 63;

    destRect_bullet_1[4].w = 98;
    destRect_bullet_1[4].h = 116;

    destRect_bullet_1[5].w = 83;
    destRect_bullet_1[5].h = 97;

    destRect_bullet_1[6].w = 92;
    destRect_bullet_1[6].h = 92;

    destRect_bullet_1[7].w = 85;
    destRect_bullet_1[7].h = 89;

    destRect_ice_bullet_4[1].w = 57;
    destRect_ice_bullet_4[1].h = 69;

    destRect_ice_bullet_4[2].w = 59;
    destRect_ice_bullet_4[2].h = 64;

    destRect_ice_bullet_4[3].w = 57;
    destRect_ice_bullet_4[3].h = 59;

    destRect_ice_bullet_4[4].w = 76;
    destRect_ice_bullet_4[4].h = 57;

    destRect_ice_bullet_4[5].w = 73;
    destRect_ice_bullet_4[5].h = 54;

    destRect_ice_bullet_4[6].w = 82;
    destRect_ice_bullet_4[6].h = 47;

    destRect_ice_bullet_4[7].w = 98;
    destRect_ice_bullet_4[7].h = 32;

    destRect_ice_bullet_4[8].w = 100;
    destRect_ice_bullet_4[8].h = 32;

    destRect_ice_bullet_4[9].w = 110;
    destRect_ice_bullet_4[9].h = 38;

    destRect_ice_bullet_4[10].w = 99;
    destRect_ice_bullet_4[10].h = 31;

    destRect_ice_bullet_4[11].w = 88;
    destRect_ice_bullet_4[11].h = 21;

    destRect_ice_bullet_2[1].w = 18;
    destRect_ice_bullet_2[1].h = 27;

    destRect_ice_bullet_2[2].w = 42;
    destRect_ice_bullet_2[2].h = 67;

    destRect_ice_bullet_2[3].w = 68;
    destRect_ice_bullet_2[3].h = 103;

    destRect_ice_bullet_2[4].w = 58;
    destRect_ice_bullet_2[4].h = 100;

    destRect_ice_bullet_2[5].w = 70;
    destRect_ice_bullet_2[5].h = 104;

    destRect_ice_bullet_2[6].w = 47;
    destRect_ice_bullet_2[6].h = 95;

    destRect_ice_bullet_2[7].w = 37;
    destRect_ice_bullet_2[7].h = 85;

    destRect_ice_bullet_2[8].w = 38;
    destRect_ice_bullet_2[8].h = 73;

    destRect_ice_bullet_2[9].w = 37;
    destRect_ice_bullet_2[9].h = 70;

    destRect_earth_bullet_2[1].w = 14;
    destRect_earth_bullet_2[1].h = 10;

    destRect_earth_bullet_2[2].w = 28;
    destRect_earth_bullet_2[2].h = 24;

    destRect_earth_bullet_2[3].w = 60;
    destRect_earth_bullet_2[3].h = 35;

    destRect_earth_bullet_2[4].w = 98;
    destRect_earth_bullet_2[4].h = 48;

    destRect_earth_bullet_2[5].w = 94;
    destRect_earth_bullet_2[5].h = 53;

    destRect_earth_bullet_2[6].w = 87;
    destRect_earth_bullet_2[6].h = 48;

    destRect_earth_bullet_2[7].w = 92;
    destRect_earth_bullet_2[7].h = 45;

    destRect_earth_bullet_3[1].w = 63;
    destRect_earth_bullet_3[1].h = 56;

    destRect_earth_bullet_3[2].w = 80;
    destRect_earth_bullet_3[2].h = 63;

    destRect_earth_bullet_3[3].w = 65;
    destRect_earth_bullet_3[3].h = 50;

    destRect_earth_bullet_3[4].w = 63;
    destRect_earth_bullet_3[4].h = 60;

    destRect_earth_bullet_4[1].w = 23;
    destRect_earth_bullet_4[1].h = 24;

    destRect_earth_bullet_4[2].w = 31;
    destRect_earth_bullet_4[2].h = 34;

    destRect_earth_bullet_4[3].w = 45;
    destRect_earth_bullet_4[3].h = 48;

    destRect_earth_bullet_4[4].w = 54;
    destRect_earth_bullet_4[4].h = 62;

    destRect_earth_bullet_4[5].w = 63;
    destRect_earth_bullet_4[5].h = 70;

    destRect_earth_bullet_4[6].w = 68;
    destRect_earth_bullet_4[6].h = 69;

    destRect_earth_bullet_4[7].w = 65;
    destRect_earth_bullet_4[7].h = 62;

    destRect_earth_bullet_4[8].w = 63;
    destRect_earth_bullet_4[8].h = 64;

    destRect_earth_bullet_4[9].w = 58;
    destRect_earth_bullet_4[9].h = 62;
}

void AnimationBullet::update_position(int x,int y)
{
    for (int i=1;i<=7;i++)
    {
        destRect_bullet_1[i].x = x - (destRect_bullet_1[i].w/2) + 12;
        destRect_bullet_1[i].y = y - (destRect_bullet_1[i].h/2) + 15;
    }
    for (int i=1;i<=11;i++)
    {
        destRect_ice_bullet_4[i].x = x;
        destRect_ice_bullet_4[i].y = y + 18 - destRect_ice_bullet_4[i].h;
    }
    destRect_ice_bullet_4[10].x = x + 110 - destRect_ice_bullet_4[10].w;
    destRect_ice_bullet_4[11].x = x + 110 - destRect_ice_bullet_4[11].w;

    for (int i=1;i<=9;i++)
    {
        destRect_ice_bullet_2[i].x = x - (destRect_ice_bullet_2[i].w/2) + 13;
        destRect_ice_bullet_2[i].y = y - (destRect_ice_bullet_2[i].h) + 19;
    }

    for (int i=1;i<=7;i++)
    {
        destRect_earth_bullet_2[i].x = x - (destRect_earth_bullet_2[i].w / 2) + 10;
        destRect_earth_bullet_2[i].y = y - (destRect_earth_bullet_2[i].h ) + 17;
    }

    for (int i=1;i<=4;i++)
    {
        destRect_earth_bullet_3[i].x = x;
        destRect_earth_bullet_3[i].y = y;
    }

    for (int i=1;i<=9;i++)
    {
        destRect_earth_bullet_4[i].x = x - (destRect_earth_bullet_4[i].w / 2) + 16;
        destRect_earth_bullet_4[i].y = y - (destRect_earth_bullet_4[i].h / 2) + 11;
    }
}

void AnimationBullet::update_animation(string y)
{
    string s= y+to_string(animation_now)+".png";
    animation_Texture = LoadTexture(s.c_str());
}

void  AnimationBullet::render_animation(string y)
{
    if (y == "image/animation_ice_bullet_4_")   SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_ice_bullet_4[animation_now]);
    if (y == "image/animation_bullet_1_")       SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_bullet_1[animation_now]);
    if (y == "image/animation_ice_bullet_2_")   SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_ice_bullet_2[animation_now]);
    if (y == "image/animation_earth_bullet_3_") SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_earth_bullet_3[animation_now]);
    if (y == "image/animation_earth_bullet_2_") SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_earth_bullet_2[animation_now]);
    if (y == "image/animation_earth_bullet_4_") SDL_RenderCopy(renderer, animation_Texture,NULL,&destRect_earth_bullet_4[animation_now]);
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
    SDL_Rect intro_rect = {0,0,150,300};
    bool menu_show = false;
    bool menu_expand_show = false;
    bool help_show = false;
    bool quit_show = false;
    bool begin_show = true;
    bool isMute = false;
    bool can_update = true;
    bool check_bullet = false;
    int rand_angle = 30+rand()%40;
    int rand_veloc = 1+rand()%1000;
    int rand_bullet = 1+rand()%4;
    int power = 10;
    int between_step = 0;
    int count_number = 0;
    int d = 1;
    bool between_step_count = false;
    string type_animation = "";
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        if (can_update)game->update();
        game->render();
        if (menu_show)          SDL_RenderCopy(renderer,LoadTexture("image/menu.png"),NULL,&menu_rect);
        if (menu_expand_show)   SDL_RenderCopy(renderer,LoadTexture("image/menu_expand.png"),NULL,&menu_expand_rect);
        if (help_show)          SDL_RenderCopy(renderer,LoadTexture("image/help.png"),NULL,&help_rect);
        if (quit_show)          SDL_RenderCopy(renderer,LoadTexture("image/quit.png"),NULL,&quit_rect);
        frameTime=SDL_GetTicks()-frameStart;
        int x,y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        if ( game->step == 1 )          // bước 1 : chọn số lượng người chơi (1 hoặc 2)
        {
                if  (y>641 && y<685 && x>312 && x<544)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/begin_2.png"),NULL,NULL);
                        if  (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) game->check = true;
                        between_step_count = true;
                }
                else if (y>641 && y<685 &&x>588 && x<815)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/begin_3.png"),NULL,NULL);
                        if  (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) game->check = true;
                        between_step_count = true;
                        game->one_player = false;
                }
                else if  (y>641 && y<685 &&x>859 && x<1084)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/begin_4.png"),NULL,NULL);
                        if  (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) game->check = true;
                        between_step_count = true;
                }
                else SDL_RenderCopy(renderer,LoadTexture("image/begin_1.png"),NULL,NULL);
        }
        else if (game->step==2)         // bước 2 : chọn nation ( chưa làm )
        {
                if  (y>617 && y<675 && x>48 && x<368)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/chose_nation_2.png"),NULL,NULL);
                        if  ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT))&&(between_step >20))
                        {
                            game->check = true;
                            between_step_count =false;
                        }
                }
                else if (y>617 && y<675 &&x>552 && x<872)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/chose_nation_3.png"),NULL,NULL);
                        if  ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT))&&(between_step >20))
                        {
                            game->check = true;
                            between_step_count =false;
                        }
                }
                else if  (y>617 && y<675 &&x>1058 && x<1372)
                {
                        SDL_RenderCopy(renderer,LoadTexture("image/chose_nation_4.png"),NULL,NULL);
                        if  ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT))&&(between_step >20))
                        {
                            game->check = true;
                            between_step_count =false;
                        }
                }
                else SDL_RenderCopy(renderer,LoadTexture("image/chose_nation_1.png"),NULL,NULL);
        }
        else
        {
            if (game->step==3)          // bước 3 : chọn đan của người chơi
            {
                pick_bullet->type = "ice";
                game->pick_done = true;
                if (pick_bullet->pick_now <=9)
                {
                    count_number ++;
                    pick_bullet->pick_now = 1 + (count_number/4);
                }
                else
                {
                    count_number = 0;
                    intro_rect.x = x;
                    intro_rect.y = y-300;
                    if (sqrt((x-59)*(x-59)+(y-639)*(y-639))<=26)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/ice_intro_1.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            player_bullet->type=1;
                            game->check = true;
                            game->pick_done = false;
                            between_step_count = true;
                            type_animation = "bullet_1";
                        }
                    }
                    else if (sqrt((x-159)*(x-159)+(y-636)*(y-636))<=30 && player_bullet->count_2 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/ice_intro_2.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            player_bullet->type = 2;
                            game->check = true;
                            game->pick_done = false;
                            player_bullet->count_2 --;
                            between_step_count = true;
                            type_animation = "ice_bullet_2";
                        }
                    }
                    else if (sqrt((x-411)*(x-411)+(y-634)*(y-634))<=24 && player_bullet->count_4 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/ice_intro_4.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            player_bullet->type = 4;
                            game->check = true;
                            game->pick_done = false;
                            player_bullet->count_4--;
                            between_step_count = true;
                            type_animation = "ice_bullet_4";
                        }
                    }
                    else if (x>=232 && x<=359&&y>=623&&y<=651 && player_bullet->count_3 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/ice_intro_3.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            player_bullet->type = 3;
                            ice_bullet_3_1->type = 3;
                            ice_bullet_3_3->type = 3;
                            game->check = true;
                            game->pick_done = false;
                            player_bullet->count_3--;
                            between_step_count = true;
                            type_animation = "ice_bullet_3";
                        }
                    }
                }
            }
            else if (game->step == 4)   // bước 4 : di chuyển pháo và điều chỉnh lực
            {
                game->handleEvents();
                game->run_player_arrow = true;
                if (x<393)player_arrow->degrees = 90;
                else if (y>542)player_arrow->degrees = 30;
                else
                {
                    player_arrow->degrees=atan((abs(y-player_bullet->destRect.y))*1.0/(abs(x-player_bullet->destRect.x)))*180/3.14;
                    if (player_arrow->degrees < 30 ) player_arrow->degrees = 30;
                }
                if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && between_step >10 )
                {
                    player_arrow->arrow_now = 1 + (count_number / 4);
                    power = power + 12*d;
                    count_number = count_number + d;
                    if (player_arrow->arrow_now == 13) d = 0;
                    if (player_arrow->arrow_now == 1) d = 1;
                    player_bullet->angle = player_arrow->degrees;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_1->angle = player_bullet->angle - 5;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_3->angle = player_bullet->angle + 5;
                }
                else if (power>10)
                {
                    player_bullet->can_count = true;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_1->can_count = true;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_3->can_count = true;
                    game->run_player_arrow = false;
                    player_bullet->veloc = power;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_1->veloc = power*0.95;
                    if (type_animation == "ice_bullet_3")ice_bullet_3_3->veloc = power*1.05;
                    between_step_count = false;
                    player_arrow->arrow_now = 1;
                    count_number = 0;
                    game->check = true;
                }
            }
            else if (game->step == 5)   // bước 5 : animation của pháo
            {
                count_number++;
                player_cannon->cannon_now = 1 + count_number / 6;
                if (player_cannon->cannon_now == 10) game->check = true;
            }
            else if (game->step == 6)   // bước 6 : update vị trí và xử lí va chạm đạn của người chơi
            {
                player_cannon->cannon_now = 1;
                count_number = 0;
                if (player_bullet->can_count)
                {
                    if (can_update)player_bullet->t++;
                    if (checkCollision(player_bullet->destRect,enemy_castle->destRect))
                    {
                        enemy_hp->hp_now = enemy_hp->hp_now - player_bullet->damage;
                        game->run_bullet_player = false;
                        player_bullet->reset();
                        player_bullet->can_count = false;
                        Mix_PlayChannel(-1, explosion, 0);
                        animation_bullet->update_position(player_bullet->destRect.x,player_bullet->destRect.y);
                    }
                    else if (player_bullet->destRect.x>1400||player_bullet->destRect.y>580)
                    {
                        game->run_bullet_player = false;
                        player_bullet->reset();
                        Mix_PlayChannel(-1, explosion, 0);
                        animation_bullet->update_position(player_bullet->destRect.x,player_bullet->destRect.y);
                    }
                }
                if (ice_bullet_3_1->can_count && type_animation == "ice_bullet_3")
                {
                    if (can_update)ice_bullet_3_1->t++;
                    if (checkCollision(ice_bullet_3_1->destRect,enemy_castle->destRect))
                    {
                        enemy_hp->hp_now = enemy_hp->hp_now - player_bullet->damage;
                        game->run_ice_bullet_3_1 = false;
                        ice_bullet_3_1->reset();
                        ice_bullet_3_1->can_count = false;
                        Mix_PlayChannel(-1, explosion, 0);

                    }
                    else if (ice_bullet_3_1->destRect.x>1400||ice_bullet_3_1->destRect.y>580)
                    {
                        game->run_ice_bullet_3_1 = false;
                        ice_bullet_3_1->reset();
                        Mix_PlayChannel(-1, explosion, 0);
                        ice_bullet_3_1->can_count = false;
                    }
                }
                if (ice_bullet_3_3->can_count  && type_animation == "ice_bullet_3")
                {
                    if (can_update)ice_bullet_3_3->t++;
                    if (checkCollision(ice_bullet_3_3->destRect,enemy_castle->destRect))
                    {
                        enemy_hp->hp_now = enemy_hp->hp_now - player_bullet->damage;
                        game->run_ice_bullet_3_3 = false;
                        ice_bullet_3_3->reset();
                        ice_bullet_3_3->can_count = false;
                        Mix_PlayChannel(-1, explosion, 0);

                    }
                    else if (ice_bullet_3_3->destRect.x>1400||ice_bullet_3_3->destRect.y>580)
                    {
                        game->run_ice_bullet_3_3 = false;
                        ice_bullet_3_3->reset();
                        Mix_PlayChannel(-1, explosion, 0);
                        ice_bullet_3_3->can_count = false;
                    }
                }
                if (game->run_ice_bullet_3_3 == false && game->run_ice_bullet_3_1 == false && game->run_bullet_player == false) game->check = true;
            }
            else if (game-> step == 7)  // bước 7 : animation của đạn khi va chạm
            {
                count_number ++;
                animation_bullet->animation_now = 1 + count_number/6;
                if (type_animation == "bullet_1")
                {
                    animation_bullet->update_animation("image/animation_bullet_1_");
                    animation_bullet->render_animation("image/animation_bullet_1_");
                    if ( animation_bullet->animation_now == 7 )game->check = true;
                }
                else if (type_animation == "ice_bullet_4")
                {
                    animation_bullet->update_animation("image/animation_ice_bullet_4_");
                    animation_bullet->render_animation("image/animation_ice_bullet_4_");
                    if ( animation_bullet->animation_now == 11 )game->check = true;
                }
                else if (type_animation == "ice_bullet_2")
                {
                    animation_bullet->update_animation("image/animation_ice_bullet_2_");
                    animation_bullet->render_animation("image/animation_ice_bullet_2_");
                    if ( animation_bullet->animation_now == 9 )game->check = true;
                }
                else game->check = true;
            }
            if (game->step == 8 && game->one_player)    // bước 8 ( 1 người chơi ) : update vị trí và xử lí va chạm đạn của đối thủ
            {
                count_number = 0;
                game->run_bullet_enemy = true;
                enemy_bullet->angle = rand_angle;
                enemy_bullet->veloc = rand_veloc;
                enemy_bullet->type = rand_bullet;
                enemy_bullet->can_count=true;
                if (rand_bullet == 1 ) type_animation = "bullet_1";
                if (rand_bullet == 2 ) type_animation = "earth_bullet_2";
                if (rand_bullet == 3 ) type_animation = "earth_bullet_3";
                if (rand_bullet == 4 ) type_animation = "earth_bullet_4";
                if (enemy_bullet->can_count && can_update)enemy_bullet->t++;
                if (checkCollision(enemy_bullet->destRect,player_castle->destRect))
                {
                    game->check = true;
                    player_hp->hp_now = player_hp->hp_now-enemy_bullet->damage;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                    animation_bullet->update_position(enemy_bullet->destRect.x,enemy_bullet->destRect.y);
                    enemy_bullet->collision = true;
                }
                if (enemy_bullet->destRect.x<0||enemy_bullet->destRect.y>580)
                {
                    game->check = true;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                    animation_bullet->update_position(enemy_bullet->destRect.x,enemy_bullet->destRect.y);
                    if (enemy_bullet->destRect.x<0)enemy_bullet->out = true;
                }
            }
            else if (game->step == 9 && game->one_player) // bước 9 ( 1 người chơi ) : animation đạn
            {
                    count_number ++;
                    animation_bullet->animation_now = 1 + count_number/8;
                    if (type_animation == "bullet_1")
                    {
                        animation_bullet->update_animation("image/animation_bullet_1_");
                        animation_bullet->render_animation("image/animation_bullet_1_");
                        if ( animation_bullet->animation_now == 7 )game->check = true;
                    }
                    else if (type_animation == "earth_bullet_2")
                    {
                        animation_bullet->update_animation("image/animation_earth_bullet_2_");
                        animation_bullet->render_animation("image/animation_earth_bullet_2_");
                        if ( animation_bullet->animation_now == 7 )game->check = true;
                    }
                    else if (type_animation == "earth_bullet_3")
                    {
                        animation_bullet->update_animation("image/animation_earth_bullet_3_");
                        animation_bullet->render_animation("image/animation_earth_bullet_3_");
                        if ( animation_bullet->animation_now == 4 )game->check = true;
                    }
                    else if (type_animation == "earth_bullet_4")
                    {
                        animation_bullet->update_animation("image/animation_earth_bullet_4_");
                        animation_bullet->render_animation("image/animation_earth_bullet_4_");
                        if ( animation_bullet->animation_now == 9 )game->check = true;
                    }
                    else game->check = true;
            }
            else if(game->step == 8 && game->one_player == false) // bước 8 ( 2 người chơi ) : chọn đạn của đối thủ
            {
                pick_bullet->type = "earth";
                game->pick_done = true;
                if (pick_bullet->pick_now <=9)
                {
                    count_number ++;
                    pick_bullet->pick_now = 1 + (count_number/4);
                }
                else
                {
                    count_number = 0;
                    intro_rect.x = x-150;
                    intro_rect.y = y-300;
                    if (x>=973 && x<=1014 &&y>=622 && y<=656)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/earth_intro_1.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            enemy_bullet->type=1;
                            game->check = true;
                            game->pick_done = false;
                            between_step_count = true;
                            type_animation = "bullet_1";
                        }
                    }
                    else if (x>=1087 && x<=1131 && y>=617 && y<=656 && enemy_bullet->count_2 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/earth_intro_2.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            enemy_bullet->type = 2;
                            game->check = true;
                            game->pick_done = false;
                            enemy_bullet->count_2 --;
                            between_step_count = true;
                            type_animation = "earth_bullet_2";
                        }
                    }
                    else if (x>=1165 && x<=1286 && y>=621 && y<=658 && enemy_bullet->count_3 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/earth_intro_4.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            enemy_bullet->type = 3;
                            game->check = true;
                            game->pick_done = false;
                            enemy_bullet->count_3--;
                            between_step_count = true;
                            type_animation = "earth_bullet_3";
                        }
                    }
                    else if (x>=1313 && x<=1377 && y>=614 && y<=663 && enemy_bullet->count_4 >=1)
                    {
                        SDL_RenderCopy(renderer,LoadTexture("image/earth_intro_3.png"),NULL,&intro_rect);
                        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                        {
                            enemy_bullet->type = 4;
                            game->check = true;
                            game->pick_done = false;
                            enemy_bullet->count_4--;
                            between_step_count = true;
                            type_animation = "earth_bullet_4";
                        }
                    }
                }
            }
            else if (game->step == 9 && game->one_player == false) // bước 9 ( 2 người chơi ) : di chuyển pháo và điều chỉnh lực
            {
                game->handleEvents();
                game->run_enemy_arrow = true;
                if (x>1200)enemy_arrow->degrees = 90;
                else if (y>460)enemy_arrow->degrees = 150;
                else
                {
                    enemy_arrow->degrees= 180- atan((abs(y-enemy_bullet->destRect.y))*1.0/(abs(x-enemy_bullet->destRect.x)))*180/3.14;
                    if (enemy_arrow->degrees < 30 ) enemy_arrow->degrees = 30;
                }
                if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && between_step >10 )
                {
                    enemy_arrow->arrow_now = 1 + (count_number / 4);
                    power = power + 12*d;
                    count_number = count_number + d;
                    if (enemy_arrow->arrow_now == 13) d = 0;
                    if (enemy_arrow->arrow_now == 1) d = 1;
                    enemy_bullet->angle = 180-enemy_arrow->degrees;
                }
                else if (power>10)
                {
                    enemy_bullet->can_count = true;
                    game->run_enemy_arrow = false;
                    game->run_bullet_enemy = true;
                    enemy_bullet->veloc = power;
                    between_step_count = false;
                    enemy_arrow->arrow_now = 1;
                    count_number = 0;
                    game->check = true;
                }
            }
            else if (game->step == 10 && game->one_player == false) //bước 10 (2 người chơi ) : update vị trí và xử lí va chạm đạn của đối thủ
            {

                if (enemy_bullet->can_count && can_update)enemy_bullet->t++;
                if (checkCollision(enemy_bullet->destRect,player_castle->destRect))
                {
                    game->check = true;
                    player_hp->hp_now = player_hp->hp_now-enemy_bullet->damage;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                    animation_bullet->update_position(enemy_bullet->destRect.x,enemy_bullet->destRect.y);
                }
                if (enemy_bullet->destRect.x<0||enemy_bullet->destRect.y>580)
                {
                    game->check = true;
                    game->run_bullet_enemy = false;
                    enemy_bullet->reset();
                    Mix_PlayChannel(-1, explosion, 0);
                    animation_bullet->update_position(enemy_bullet->destRect.x,enemy_bullet->destRect.y);
                }
            }
            else if (game->step == 11 && game->one_player == false) // bước 11 (2 người chơi ) : animation đạn
            {
                count_number ++;
                animation_bullet->animation_now = 1 + count_number/8;
                if (type_animation == "bullet_1")
                {
                    animation_bullet->update_animation("image/animation_bullet_1_");
                    animation_bullet->render_animation("image/animation_bullet_1_");
                    if ( animation_bullet->animation_now == 7 )game->check = true;
                }
                else if (type_animation == "earth_bullet_2")
                {
                    animation_bullet->update_animation("image/animation_earth_bullet_2_");
                    animation_bullet->render_animation("image/animation_earth_bullet_2_");
                    if ( animation_bullet->animation_now == 7 )game->check = true;
                }
                else if (type_animation == "earth_bullet_3")
                {
                    animation_bullet->update_animation("image/animation_earth_bullet_3_");
                    animation_bullet->render_animation("image/animation_earth_bullet_3_");
                    if ( animation_bullet->animation_now == 4 )game->check = true;
                }
                else if (type_animation == "earth_bullet_4")
                {
                    animation_bullet->update_animation("image/animation_earth_bullet_4_");
                    animation_bullet->render_animation("image/animation_earth_bullet_4_");
                    if ( animation_bullet->animation_now == 9 )game->check = true;
                }
                else game->check = true;
            }
            if  (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) // menu
            {
                if ( menu_show == true && x>=665 && x<=735 && y>=670 && y<=700)
                {
                    menu_show = false;
                    menu_expand_show = true;
                    can_update = false;
                }
                else if (menu_expand_show == true)
                {
                    if (x>=644 && x<=746 && y>=273 && y<=316)
                    {
                        menu_expand_show = false;
                        menu_show = true;
                        can_update = true;
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
                        can_update = true;
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
                        can_update = true;
                    }
                }
                else if (quit_show == true)
                {
                    if (x>=615 && x<=704 && y>=404 && y<=440)
                    {
                        quit_show = false;
                        menu_show = true;
                        can_update = true;
                    }
                    else if (x>=715 && x<=804 && y>=404 && y<=440) game->isRunning = false;
                }
            }
        }
        if (between_step_count)between_step++;
        else between_step = 0;
        if (game->check)        // đặt lại 1 số biến sau mỗi bước
        {
            if (game->step == 3)
            {
                pick_bullet->pick_now =1;
                count_number = 0;
            }
            game->step = game->step+1;
            if (game->step == 4)
            {
                count_number = 0;
                d=0;
                power=0;
            }
            if (game->step == 5) count_number = 0;
            if (game->step == 6)
            {
                game->run_bullet_player = true;
                if (type_animation == "ice_bullet_3") game->run_ice_bullet_3_1 = true;
                if (type_animation == "ice_bullet_3") game->run_ice_bullet_3_3 = true;
            }
            if (game->step == 8)
            {
                type_animation = "";
                animation_bullet->animation_now = 1;
                if (game->one_player == false) count_number = 0;
            }
            if (game->step == 9 && game->one_player == false)
            {
                count_number = 0;
                d=0;
                power=0;
            }
            if (game->step == 11 && game->one_player == false) count_number =0;

            game->check = false;
            if (game->step == 3) menu_show = true;
        }
        if (game->step == 10 && game->one_player) // đặt lại 1 số biến sau 1 vòng ( 1 người chơi )
        {
            game->step = 3;                                                                    // Do thành cửa người chơi ở sát biên
            if (enemy_bullet->out == true) enemy_bullet->max_veloc = rand_veloc;               // Nếu đạn bắn ra ngoài thì gán lại lực bắn max
            else if (enemy_bullet->collision == false) enemy_bullet->min_veloc = rand_veloc;   // Ngược lại nếu đạn bắn vào trong và không va chạm với thành thì gán lại lực bắn min
            rand_veloc = (enemy_bullet->max_veloc + enemy_bullet->min_veloc) / 2;               // Gán lực bắn
            enemy_bullet->out = false;
            rand_bullet = 1+rand()%4;
            power = 10;
            type_animation = "";
            count_number = 0;
            player_bullet->reset();
            ice_bullet_3_1->reset();
            ice_bullet_3_3->reset();
        }
        if (game->step == 12 && game->one_player == false) //đặt lại 1 số biến sau 1 vòng ( 2 người chơi )
        {
            player_bullet->reset();
            ice_bullet_3_1->reset();
            ice_bullet_3_3->reset();
            game->step = 3;
            type_animation = "";
            count_number = 0;
        }
        if (frameDelay>frameTime) SDL_Delay(frameDelay-frameTime);
        explosion = Mix_LoadWAV("image/explosion.wav");
        if (player_hp->hp_now <= 0 )game->lose = true;
        if (enemy_hp->hp_now <=0 ) game->win = true;
        if (game->win == true)SDL_RenderCopy(renderer,LoadTexture("image/winner.png"),NULL,NULL);
        if (game->lose == true)SDL_RenderCopy(renderer,LoadTexture("image/lose.png"),NULL,NULL);
        if (game->lose == true || game->win == true ) // xử lí khi thắng hoặc thua
            if  (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
            {
                game->step = 1 ;
                count_number = 0;
                quit_show = false;
                fight = Mix_LoadWAV("image/fight.wav");
                explosion = Mix_LoadWAV("image/explosion.wav");
                menu_show = false;
                player_hp->hp_now = 100;
                enemy_hp->hp_now = 100;
                game->run_bullet_player = false;
                game->run_bullet_enemy = false;
                game->run_player_arrow = false;
                game->run_enemy_arrow = false;
                game->win = false;
                game->lose = false;
                player_bullet->count_2 = 8;
                player_bullet->count_3 = 5;
                player_bullet->count_4 = 2;
                player_bullet->can_count = false;
                ice_bullet_3_1->can_count = false;
                ice_bullet_3_3->can_count = false;
                enemy_bullet->can_count = false;
                player_bullet->reset();
                enemy_bullet->reset();
                between_step = 0;
                between_step_count = false;
                type_animation = "";
                game->check = false;
                game->one_player = false;
            }
        SDL_RenderPresent(renderer);
    }
    game->clean();
    return 0;
}

