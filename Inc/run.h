#ifndef __RUN_H
#define __RUN_H	

#include "basic_draw.h"

void generate_enemy_planes(int level, int enemy_planes[10][2]);

void update_my_buttles(int my_buttles[100][2]);

void update_enemy_planes(int enemy_planes[10][2]);

void check_my_plane_to_enemy_plane_collide(int score,int life,	int my_plane[2], int enemy_planes[10][2]);
void check_my_buttles_to_enemy_plane_collide(int score,	int my_buttles[300][2], int enemy_planes[10][2]);

void clear_all_enemy_plane(int enemy_plane[10][2]);
void arrange_enemy_planes(int enemy_planes[10][2]);
void arrange_my_buttles(int my_buttles[100][2]);
#endif
