__constant int TOTAL_LENGTH = WIDTH * HEIGHT;

int get_gid(int x, int y) {
  return (y * WIDTH) + x;
}

__kernel void life(
    __global const char *board_g, __global char *res_g)
{
  int gid = get_global_id(0);
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
