#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <xf86drm.h>
#include <xf86drmMode.h>

using namespace std;

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

int main(int argc, char **argv) {
  int fd = open("/dev/dri/card1", O_RDWR);
  if (fd < 0) {
    perror("unable to open /dev/dri/card1");
    exit(1);
  }
  drmSetMaster(fd);
  printDrmModes(fd);
  return 0;
}
