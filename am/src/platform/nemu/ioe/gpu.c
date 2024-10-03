#include <am.h>
#include <nemu.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  uint32_t size = inl(VGACTL_ADDR);
  uint32_t w = size >> 16;
  uint32_t h = size & 0x0000ffff;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t size = inl(VGACTL_ADDR);
  uint32_t w = size >> 16;
  uint32_t h = size & 0x0000ffff;
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t size = inl(VGACTL_ADDR);
  int width = size >> 16;
  int height = size & 0x0000ffff;
  if (w == 0 || h == 0 || w > width || h > height)
  {
    printf("|%d|%d|%d|%d|%d|%d|\n",x, y, w, h, width, height);
  }

  uint32_t *pixels = ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j)
    {
      uint32_t index = x + (y + i) * width + j;
      uint32_t value = pixels[i*w + j];
      // TODO: outl seems werid, but I not find why
      // outl(FB_ADDR + index, value);
      fb[index] = value;
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
