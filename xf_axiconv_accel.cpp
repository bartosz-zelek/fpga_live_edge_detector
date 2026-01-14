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

#include "xf_axiconv_accel_config.h"

void axiconv_accel(hls::stream<ap_axiu<_W, 1, 1, 1> >& src, hls::stream<ap_axiu<_W, 1, 1, 1> >& dst, int rows, int cols) {
    #pragma HLS INTERFACE axis port=src
    #pragma HLS INTERFACE axis port=dst
    #pragma HLS INTERFACE s_axilite port=rows               
    #pragma HLS INTERFACE s_axilite port=cols               
    #pragma HLS INTERFACE s_axilite port=return

    xf::cv::Mat<RGB, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> src_mat(rows, cols);

    xf::cv::Mat<GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> gray_mat(rows, cols);

    xf::cv::Mat<GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> _dstgx(rows, cols);
    xf::cv::Mat<GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> _dstgy(rows, cols);

    xf::cv::Mat<GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> _dst_combined(rows, cols);

    xf::cv::Mat<RGB, XF_HEIGHT, XF_WIDTH, NPPCX, XF_CV_DEPTH> dst_mat(rows, cols);

    #pragma HLS dataflow

    xf::cv::AXIvideo2xfMat(src, src_mat);

    xf::cv::rgb2gray<RGB, GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, 
                     XF_CV_DEPTH, XF_CV_DEPTH>(src_mat, gray_mat);

    xf::cv::Sobel<XF_BORDER_CONSTANT, FILTER_WIDTH, GRAY, GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, XF_USE_URAM,
                  XF_CV_DEPTH, XF_CV_DEPTH, XF_CV_DEPTH>(gray_mat, _dstgx, _dstgy);
                  
    xf::cv::magnitude<NORM_TYPE, GRAY, GRAY, XF_HEIGHT, XF_WIDTH, NPPCX, 
                  XF_CV_DEPTH, XF_CV_DEPTH, XF_CV_DEPTH>(
                  _dstgx, _dstgy, _dst_combined);

    xf::cv::gray2rgb<GRAY, RGB, XF_HEIGHT, XF_WIDTH, NPPCX,
                     XF_CV_DEPTH, XF_CV_DEPTH>(_dst_combined, dst_mat);

    xf::cv::xfMat2AXIvideo(dst_mat, dst);

    return;
}
