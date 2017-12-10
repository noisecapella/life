#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int HEIGHT = 4096 / 2;
const int WIDTH = 2048 / 2;

int get_gid(int x, int y) {
  return (y * WIDTH) + x;
}

void life(
	  int gid, const char *board_g, char *res_g)
{
  int x = gid % WIDTH;
  int y = gid / WIDTH;

  int left = x - 1;
  if (left < 0) {
    left = WIDTH - 1;
  }
  int right = x + 1;
  if (right == WIDTH) {
    right = 0;
  }
  int top = y - 1;
  if (top < 0) {
    top = HEIGHT - 1;
  }
  int bottom = y + 1;
  if (bottom == HEIGHT) {
    bottom = 0;
  }

  int neighbors = (
		   (board_g[get_gid(left, y)] ? 1 : 0) +
		   (board_g[get_gid(right, y)] ? 1 : 0) +
		   (board_g[get_gid(left, top)] ? 1 : 0) +
		   (board_g[get_gid(left, bottom)] ? 1 : 0) +
		   (board_g[get_gid(right, top)] ? 1 : 0) +
		   (board_g[get_gid(right, bottom)] ? 1 : 0) +
		   (board_g[get_gid(x, top)] ? 1 : 0) +
		   (board_g[get_gid(x, bottom)] ? 1 : 0)
	       );

  if (neighbors == 2) {
    res_g[gid] = board_g[gid];
  } else if (neighbors == 3) {
    res_g[gid] = 0xff;
  } else {
    res_g[gid] = 0;
  }
}


int main() {
  char* board = malloc(sizeof(char) * HEIGHT * WIDTH);
  char* res = malloc(sizeof(char) * HEIGHT * WIDTH);

  srand(time(NULL));
  for (int i = 0; i < HEIGHT * WIDTH; ++i) {
    board[i] = (rand() % 5) ? 0xff : 0;
  }
  
  for (int i = 0; i < 500; ++i) {
    for (int x = 0; x < WIDTH; ++x) {
      for (int y = 0; y < HEIGHT; ++y) {
	life(x + (y*WIDTH), board, res);
      }
    }
    for (int j = 0; j < HEIGHT * WIDTH; ++j) {
      res[j] = board[j];
    }
  }

  char* towrite = malloc(sizeof(char) * HEIGHT * WIDTH);
  for (int j = 0; j < HEIGHT * WIDTH; ++j) {
    towrite[j] = res[j] ? '*' : ' ';
  }
  
  FILE* f = fopen("out.txt", "w");
  for (int y = 0; y < HEIGHT; ++y) {
    fwrite(towrite + (y*WIDTH), 1, WIDTH, f);
    fputc('\n', f);
  }
  
  return 0;
}
