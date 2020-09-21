#ifndef __BASICDRAW_H
#define __BASICDRAW_H 	


//21*41
void draw_my_plane(int x, int y);
void draw_enemy_plane(int x ,int y);

void draw_my_bullet(int x, int y);

void draw_cross(int x, int y);
void draw_heart(int x, int y);

void init_play_interface(void);
void paintnum(int x, int y,int size,int score);
void draw_play_title(int score, int life);
void draw_all_my_buttles(int my_buttles[100][2]);
void draw_all_enemy_planes(int enemy_planes[10][2]);
void draw_play_all(int score,int life, int my_plane[2], int my_buttles[100][2], int enemy_planes[10][2]);
#endif
