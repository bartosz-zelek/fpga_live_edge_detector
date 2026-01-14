/*
 * Copyright 2022 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _XF_AXICONV_CONFIG_H_
#define _XF_AXICONV_CONFIG_H_

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "common/xf_infra.hpp"

#include "ap_int.h"
#include "imgproc/xf_sobel.hpp"

#include "core/xf_magnitude.hpp"

#include "imgproc/xf_cvt_color.hpp"

#define _W 24

/* config width and height */
#define XF_HEIGHT 720
#define XF_WIDTH 1280

#define XF_CV_DEPTH 2

/*  Set Filter size  */
#define FILTER_WIDTH 3

#define XF_USE_URAM 0

#define NPPCX XF_NPPC1

#define RGB XF_8UC3
#define GRAY XF_8UC1

#define INPUT_PTR_WIDTH 8
#define OUTPUT_PTR_WIDTH 8

#define NORM_TYPE XF_L1NORM

void axiconv_accel(hls::stream<ap_axiu<_W, 1, 1, 1> >& _src,
                   hls::stream<ap_axiu<_W, 1, 1, 1> >& _dst,
                   int rows,
                   int cols);

#endif
// _XF_AXICONV_CONFIG_H_
