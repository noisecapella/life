import numpy as np
import pyopencl as cl
import imageio


def main():
    height = 4096 // 2
    width = 2048 // 2

    choices = [0] * 4 + [0xff]
    board_np = np.random.choice(choices, size=height * width).astype(np.uint8)

    ctx = cl.create_some_context(interactive=False)
    queue = cl.CommandQueue(ctx)
    mf = cl.mem_flags

    with open("test.cl") as f:
        source = """
__constant int WIDTH = {width};
__constant int HEIGHT = {height};
""".format(height=height, width=width)
        source += f.read()
    prg = cl.Program(ctx, source).build()

    res_g = cl.Buffer(ctx, mf.WRITE_ONLY, board_np.nbytes)

    #res_np = np.zeros(shape=height * width).astype(np.uint8)
    res_np = np.empty_like(board_np)

    with imageio.get_writer("out.mp4", mode='I') as writer:
        for i in range(500):
            #print("Loop {}".format(i), res_np, board_np)
            board_g = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=board_np)
            prg.life(queue, board_np.shape, None, board_g, res_g)
            cl.enqueue_copy(queue, res_np, res_g)

            #np.reshape(res_np, (width, height))
            res_np.shape = (width, height)
            writer.append_data(res_np)
            #np.reshape(res_np, (height*width,)).astype(np.uint8)
            res_np.shape = width * height
            np.copyto(board_np, res_np)


if __name__ == "__main__":
    main()

    
