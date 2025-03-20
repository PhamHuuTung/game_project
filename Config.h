#ifndef CONFIG_H
#define CONFIG_H

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

const int enemyWidth = 40;
const int enemyHeight = 40;
const int  shootCooldown = 1000; // bắn mỗi 1000ms (1 giây)
// Add to Config.h
const int PLAY_AREA_SIZE = WINDOW_HEIGHT; // Square with sides equal to window height
const int PLAY_AREA_X = (WINDOW_WIDTH - PLAY_AREA_SIZE) / 2; // Centered horizontally
const int PLAY_AREA_Y = 0; // Starting from top
#endif // CONF

const int health_max_E = 15;
const int health_max_P = 100;
// Thanh máu: màu xanh (tỷ lệ theo health, max = 1)
