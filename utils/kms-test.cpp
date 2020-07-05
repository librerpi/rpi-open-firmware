#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

using namespace std;

void fatal(const char *str) {
  fprintf(stderr, "%s\n", str);
  exit(1);
}


string connectorTypeToStr(uint32_t type) {
  switch (type) {
  case DRM_MODE_CONNECTOR_HDMIA: // 11
    return "HDMIA";
  case DRM_MODE_CONNECTOR_DSI: // 16
    return "DSI";
  }
  return "unknown";
}

void printDrmModes(int fd) {
  drmVersionPtr version = drmGetVersion(fd);
  printf("version %d.%d.%d\nname: %s\ndate: %s\ndescription: %s\n", version->version_major, version->version_minor, version->version_patchlevel, version->name, version->date, version->desc);
  drmFreeVersion(version);
  drmModeRes * modes = drmModeGetResources(fd);
  for (int i=0; i < modes->count_fbs; i++) {
    printf("FB#%d: %x\n", i, modes->fbs[i]);
  }
  for (int i=0; i < modes->count_crtcs; i++) {
    printf("CTRC#%d: %d\n", i, modes->crtcs[i]);
    drmModeCrtcPtr crtc = drmModeGetCrtc(fd, modes->crtcs[i]);
    printf("  buffer_id: %d\n", crtc->buffer_id);
    printf("  position: %dx%d\n", crtc->x, crtc->y);
    printf("  size: %dx%d\n", crtc->width, crtc->height);
    printf("  mode_valid: %d\n", crtc->mode_valid);
    printf("  gamma_size: %d\n", crtc->gamma_size);
    printf("  Mode\n    clock: %d\n", crtc->mode.clock);
    drmModeModeInfo &mode = crtc->mode;
    printf("    h timings: %d %d %d %d %d\n", mode.hdisplay, mode.hsync_start, mode.hsync_end, mode.htotal, mode.hskew);
    printf("    v timings: %d %d %d %d %d\n", mode.vdisplay, mode.vsync_start, mode.vsync_end, mode.vtotal, mode.vscan);
    printf("    vrefresh: %d\n", mode.vrefresh);
    printf("    flags: 0x%x\n", mode.flags);
    printf("    type: %d\n", mode.type);
    printf("    name: %s\n", mode.name);
    drmModeFreeCrtc(crtc);
  }
  for (int i=0; i < modes->count_connectors; i++) {
    printf("Connector#%d: %d\n", i, modes->connectors[i]);
    drmModeConnectorPtr connector = drmModeGetConnector(fd, modes->connectors[i]);
    if (connector->connection == DRM_MODE_CONNECTED) puts("  connected!");
    string typeStr = connectorTypeToStr(connector->connector_type);
    printf("  ID: %d\n  Encoder: %d\n  Type: %d %s\n  type_id: %d\n  physical size: %dx%d\n", connector->connector_id, connector->encoder_id, connector->connector_type, typeStr.c_str(), connector->connector_type_id, connector->mmWidth, connector->mmHeight);
    for (int j=0; j < connector->count_encoders; j++) {
      printf("  Encoder#%d:\n", j);
      drmModeEncoderPtr enc = drmModeGetEncoder(fd, connector->encoders[j]);
      printf("    ID: %d\n    Type: %d\n    CRTCs: 0x%x\n    Clones: 0x%x\n", enc->encoder_id, enc->encoder_type, enc->possible_crtcs, enc->possible_clones);
      drmModeFreeEncoder(enc);
    }
    printf("  Modes: %d\n", connector->count_modes);
    for (int j=0; j < connector->count_modes; j++) {
      printf("  Mode#%d:\n", j);
      if (j > 1) break;
      drmModeModeInfo &mode = connector->modes[j];
      printf("    clock: %d\n", mode.clock);
      printf("    h timings: %d %d %d %d %d\n", mode.hdisplay, mode.hsync_start, mode.hsync_end, mode.htotal, mode.hskew);
      printf("    v timings: %d %d %d %d %d\n", mode.vdisplay, mode.vsync_start, mode.vsync_end, mode.vtotal, mode.vscan);
      printf("    vrefresh: %d\n", mode.vrefresh);
      printf("    flags: 0x%x\n", mode.flags);
      printf("    type: %d\n", mode.type);
      printf("    name: %s\n", mode.name);
    }
    drmModeFreeConnector(connector);
  }
  for (int i=0; i < modes->count_encoders; i++) {
    printf("Encoder#%d: %d\n", i, modes->encoders[i]);
  }
  printf("min size: %dx%d\n", modes->min_width, modes->min_height);
  printf("max size: %dx%d\n", modes->max_width, modes->max_height);
  drmModeFreeResources(modes);
}

void *setupFrameBuffer(int fd, int crtc_id, uint32_t conn_id, uint32_t *width, uint32_t *heigth, uint32_t *pitch) {
  struct drm_mode_create_dumb creq;
  struct drm_mode_map_dumb mreq;
  uint32_t fb_id;
  void *buf;
  drmModeCrtcPtr crtc_info = drmModeGetCrtc(fd, crtc_id);

  memset(&creq, 0, sizeof(struct drm_mode_create_dumb));
  creq.width = crtc_info->mode.hdisplay;
  creq.height = crtc_info->mode.vdisplay;
  creq.bpp = 24;
  printf("%d x %d\n", creq.width, creq.height);

  if (drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
    fatal("drmIoctl DRM_IOCTL_MODE_CREATE_DUMB failed");
  }

  if (drmModeAddFB(fd, crtc_info->mode.hdisplay, crtc_info->mode.vdisplay, 24, 24, creq.pitch, creq.handle, &fb_id)) {
    fatal("drmModeAddFB failed");
  }
  printf("fb_id: %d\n", fb_id);
  printf("pitch: %d\n", creq.pitch);

  memset(&mreq, 0, sizeof(struct drm_mode_map_dumb));
  mreq.handle = creq.handle;

  if (drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
    fatal("drmIoctl DRM_IOCTL_MODE_MAP_DUMB failed");
  }

  buf = (void *) mmap(0, creq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
  if (buf == MAP_FAILED) {
    perror("cant mmap fb");
    exit(2);
  }


  if (drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &crtc_info->mode)) {
    perror("drmModeSetCrtc");
    fatal("drmModeSetCrtc() failed");
  }
  drmModeFreeCrtc(crtc_info);
  *width = creq.width;
  *heigth = creq.height;
  *pitch = creq.pitch;
  return buf;
}

/* emits uart data over DPI
 * config.txt contents:
dpi_output_format=0x17
dpi_group=2
dpi_mode=87
dpi_timings=50 0 0 1 0 2000 0 0 1 0 0 0 0 30 0 5000000 6
dtoverlay=dpi24
enable_dpi_lcd=1
 */
void emitUartData(void *buf, const char *msg, uint32_t width, uint32_t height, uint32_t pitch) {
  int len = strlen(msg);
  for (int y=0; y < height; y++) {
    for (int x=0; x < width; x++) {
      uint8_t *pixel = reinterpret_cast<uint8_t*>(buf + (y * pitch) + (x * 3));
      if ((x/10) < len) {
        if (x % 10 == 0) { // start bit
          pixel[0] = 0;
        } else if (x % 10 == 9) { // stop bit
          pixel[0] = 255;
        } else { // data bits
          int databit = (msg[x/10] >> ((x%10)-1)) & 1;
          pixel[0] = databit ? 0xff : 0x00;
        }
      } else {
        pixel[0] = 0xff;
      }
      pixel[1] = 0; // green
      pixel[2] = 0; // blue
    }
  }
  printf("%d x %d\n", width, height);
}

void emitSineData(void *buf, uint32_t width, uint32_t height, uint32_t pitch) {
  for (int y=0; y < height; y++) {
    for (int x=0; x < width; x++) {
      uint8_t *pixel = reinterpret_cast<uint8_t*>(buf + (y * pitch) + (x * 3));
      double dx = x;
      pixel[0] = (sin((dx / 10) * 3.14) * 0x80) + 0x80;
      pixel[1] = 0;
      pixel[2] = 0;
    }
  }
}

int main(int argc, char **argv) {
  int fd = open("/dev/dri/card1", O_RDWR);
  uint32_t crtc_id, connector_id;
#if 0
  // pi4 DPI
  crtc_id = 52;
  connector_id = 54;
#endif
#if 1
  // pi4 hdmi0
  crtc_id = 81;
  connector_id = 83;
#endif
  if (fd < 0) {
    perror("unable to open /dev/dri/card1");
    exit(3);
  }
  uint64_t has_dumb;
  if (drmGetCap(fd, DRM_CAP_DUMB_BUFFER, &has_dumb) < 0 || has_dumb == 0) {
    fatal("drmGetCap DRM_CAP_DUMB_BUFFER failed or doesn't have dumb buffer");
  }
  drmSetMaster(fd);
  printDrmModes(fd);
  uint32_t width,height,pitch;
  void *buf = setupFrameBuffer(fd, crtc_id, connector_id, &width, &height, &pitch);
  //emitUartData(buf,"Uart test data", width, height, pitch);
  emitSineData(buf, width, height, pitch);
  sleep(100);
  return 0;
}
