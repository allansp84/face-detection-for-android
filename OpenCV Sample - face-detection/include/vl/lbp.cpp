#include "lbp.hpp"

#include <math.h>
#include <stdlib.h>
#include <string.h>


long int vl_floor_f (float x) {
  long int xi = (long int) x ;
  if (x >= 0 || (float) xi == x) return xi ;
  else return xi - 1 ;
}

static void _vl_lbp_init_uniform(VlLbp * self) {
  int i, j ;

  self->dimension = 58 ;

  for (i = 0 ; i < 256 ; ++i) {
    self->mapping[i] = 57 ;
  }

  self->mapping[0x00] = 56 ;
  self->mapping[0xff] = 56 ;

  for (i = 0 ; i < 8 ; ++i) {
    for (j = 1 ; j <= 7 ; ++j) {
      int ip ;
      int unsigned string ;
      if (self->transposed) {
        ip = (- i + 2 - (j - 1) + 16) % 8 ;
      } else {
        ip = i ;
      }

      string = (1 << j) - 1 ;
      string <<= ip ;
      string = (string | (string >> 8)) & 0xff ;

      self->mapping[string] = i * 7 + (j-1) ;
    }
  }
}

/* ---------------------------------------------------------------- */

VlLbp* vl_lbp_new(VlLbpMappingType type, vl_bool transposed) {
  VlLbp* self = new VlLbp;
  if (self == NULL) {
    return NULL ;
  }
  self->transposed = transposed ;
  switch (type) {
    case VlLbpUniform: _vl_lbp_init_uniform(self) ; break ;
    default: exit(1) ;
  }
  return self ;
}

void
vl_lbp_delete(VlLbp * self) {
  free(self) ;
}

vl_size vl_lbp_get_dimension(VlLbp * self) {
  return self->dimension ;
}


void vl_lbp_process (VlLbp * self,
                float * features,
                float * image, vl_size width, vl_size height,
                vl_size cellSize) {
  vl_size cwidth = width / cellSize;
  vl_size cheight = height / cellSize ;
  vl_size cstride = cwidth * cheight ;
  vl_size cdimension = vl_lbp_get_dimension(self) ;
  vl_index x,y,cx,cy,k,bin ;

#define at(u,v) (*(image + width * (v) + (u)))
#define to(u,v,w) (*(features + cstride * (w) + cwidth * (v) + (u)))

  memset(features, 0, sizeof(float)*cdimension*cstride);

  for (y = 1 ; y < (signed)height - 1 ; ++y) {
    float wy1 = (y + 0.5f) / (float)cellSize - 0.5f ;
    int cy1 = (int) vl_floor_f(wy1) ;
    int cy2 = cy1 + 1 ;
    float wy2 = wy1 - (float)cy1 ;
    wy1 = 1.0f - wy2 ;
    if (cy1 >= (signed)cheight) continue ;

    for (x = 1 ; x < (signed)width - 1; ++x) {
      float wx1 = (x + 0.5f) / (float)cellSize - 0.5f ;
      int cx1 = (int) vl_floor_f(wx1) ;
      int cx2 = cx1 + 1 ;
      float wx2 = wx1 - (float)cx1 ;
      wx1 = 1.0f - wx2 ;
      if (cx1 >= (signed)cwidth) continue ;

      {
        int unsigned bitString = 0 ;
        float center = at(x,y) ;
        if(at(x+1,y+0) > center) bitString |= 0x1 << 0; /*  E */
        if(at(x+1,y+1) > center) bitString |= 0x1 << 1; /* SE */
        if(at(x+0,y+1) > center) bitString |= 0x1 << 2; /* S  */
        if(at(x-1,y+1) > center) bitString |= 0x1 << 3; /* SW */
        if(at(x-1,y+0) > center) bitString |= 0x1 << 4; /*  W */
        if(at(x-1,y-1) > center) bitString |= 0x1 << 5; /* NW */
        if(at(x+0,y-1) > center) bitString |= 0x1 << 6; /* N  */
        if(at(x+1,y-1) > center) bitString |= 0x1 << 7; /* NE */
        bin = self->mapping[bitString] ;
      }

      if ((cx1 >= 0) & (cy1 >=0)) {
        to(cx1,cy1,bin) += wx1 * wy1;
      }
      if ((cx2 < (signed)cwidth)  & (cy1 >=0)) {
        to(cx2,cy1,bin) += wx2 * wy1 ;
      }
      if ((cx1 >= 0) & (cy2 < (signed)cheight)) {
        to(cx1,cy2,bin) += wx1 * wy2 ;
      }
      if ((cx2 < (signed)cwidth) & (cy2 < (signed)cheight)) {
        to(cx2,cy2,bin) += wx2 * wy2 ;
      }
    }
  }

  for (cy = 0 ; cy < (signed)cheight ; ++cy) {
    for (cx = 0 ; cx < (signed)cwidth ; ++ cx) {
      float norm = 0 ;
      for (k = 0 ; k < (signed)cdimension ; ++k) {
        norm += features[k * cstride] ;
      }
      norm = sqrtf(norm) + 1e-10f; ;
      for (k = 0 ; k < (signed)cdimension ; ++k) {
        features[k * cstride] = sqrtf(features[k * cstride]) / norm  ;
      }
      features += 1 ;
    }
  }
}
