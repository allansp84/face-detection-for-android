#ifndef VL_LBP_H
#define VL_LBP_H

#include "host.hpp"

typedef enum _VlLbpMappingType {
  VlLbpUniform
} VlLbpMappingType ;

typedef struct VlLbp_ {
  vl_size dimension ;
  vl_uint8 mapping [256] ;
  vl_bool transposed ;
} VlLbp ;

VlLbp * vl_lbp_new(VlLbpMappingType type, vl_bool transposed) ;
void vl_lbp_delete(VlLbp * self) ;
void vl_lbp_process(VlLbp * self,
                            float * features,
                            float * image, vl_size width, vl_size height,
                            vl_size cellSize) ;
vl_size vl_lbp_get_dimension(VlLbp * self) ;

#endif
