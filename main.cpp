#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
const int SCREEN_WIDTH = 800; // Chiều rộng màn hình
const int SCREEN_HEIGHT = 600; // Chiều cao màn hình
const int FPS = 60; // Số khung hình trên giây
SDL_Window* window = NULL; // Cửa sổ chương trình
SDL_Renderer* renderer = NULL; // Đối tượng vẽ đồ họa
SDL_Texture* background = NULL; // Hình nền
SDL_Texture* ally_castle,enemy_castlte = NULL; // Hình tháp
SDL_Texture* ally_catapult,enemy_catapult = NULL; // Máy ném đá
SDL_Texture* ally_bullet,enemy_bullet = NULL; // Hình đạn
Mix_Music* music = NULL; // Âm nhạc nền
Mix_Chunk* explode_sound = NULL; // Âm thanh nổ
TTF_Font* font = NULL; // Font chữ
SDL_Color white = {255, 255, 255}; // Màu trắng
SDL_Rect ally_rect,enemy_rect; // Hình chữ nhật bao quanh tàu
int score = 0; // Điểm số

// Hàm khởi tạo SDL và các thành phần liên quan
bool init();

// Hàm giải phóng các tài nguyên đã sử dụng
void close();

// Hàm tải một hình ảnh từ file và trả về texture tương ứng
SDL_Texture* loadTexture(string path);

// Hàm vẽ một texture lên renderer tại vị trí x, y với kích thước w, h
void renderTexture(SDL_Texture* texture, int x, int y, int w, int h);

// Hàm vẽ một texture lên renderer theo hình chữ nhật cho trước
void renderTexture(SDL_Texture* texture, SDL_Rect rect);

// Hàm vẽ một chuỗi lên renderer tại vị trí x, y với font và màu cho trước
void renderText(string text, int x, int y, TTF_Font* font, SDL_Color color);

// Hàm xử lý các sự kiện từ bàn phím và chuột
void handleEvents(bool& quit);

// Hàm cập nhật trạng thái của game
void update();

// Hàm vẽ các đối tượng lên màn hình
void render();

// Hàm kiểm tra hai hình chữ nhật có giao nhau không
bool checkCollision(SDL_Rect a, SDL_Rect b);

// Hàm tạo một điểm ngẫu nhiên ở phía trên màn hình
void createAsteroid();

// Hàm chính
int main(int argc, char* argv[])
{
    // Khởi tạo SDL và các thành phần liên quan
    if (!init())
    {
        cout << "Không thể khởi tạo SDL!\n";
        return -1;
    }

    // Biến lưu trạng thái thoát chương trình
    bool quit = false;

    // Vòng lặp chính của game
    while (!quit)
    {

        // Xử lý các sự kiện từ bàn phím và chuột
        handleEvents(quit);

        // Cập nhật trạng thái của game
        update();

        // Vẽ các đối tượng lên màn hình
        render();

        // Nếu thời gian vòng lặp nhỏ hơn 1000 / FPS ms thì đợi cho đủ
        Uint32 end_time = SDL_GetTicks();
        if (end_time - start_time < 1000 / FPS)
        {
            SDL_Delay(1000 / FPS - (end_time - start_time));
        }

        // Cập nhật thời gian kể từ lần xuất hiện thiên thạch gần nhất
        asteroid_time += end_time - start_time;

        // Nếu đã đủ khoảng thời gian quy định thì tạo một thiên thạch mới
        if (asteroid_time >= ASTEROID_INTERVAL)
        {
            createAsteroid();
            asteroid_time = 0;
        }
    }

    // Giải phóng các tài nguyên đã sử dụng
    close();

    return 0;
}
