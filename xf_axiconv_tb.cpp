/*
 * Copyright 2022 Xilinx, Inc.
 * ... (License header) ...
 */

#include "common/xf_headers.hpp"
#include "xf_axiconv_tb_config.h"
#include "common/xf_axi.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Invalid Number of Arguments!\nUsage: <executable> <image> <ref>\n");
        return -1;
    }

    cv::Mat img, ref_img, diff; // Added ref_img variable

    // 1. Load Input Image
    img = cv::imread(argv[1], 0);
    if (img.data == NULL) {
        fprintf(stderr, "Cannot open image at %s\n", argv[1]);
        return 0;
    }

    // 2. Load Reference Image (Golden Output)
    ref_img = cv::imread(argv[2], 0);
    if (ref_img.data == NULL) {
        fprintf(stderr, "Cannot open image at %s\n", argv[2]);
        return 0;
    }

    int rows = img.rows;
    int cols = img.cols;
    cv::Mat out_img(rows, cols, GRAY);

    // convert input to axiStream
    hls::stream<ap_axiu<_W, 1, 1, 1> > _src;
    xf::cv::cvMat2AXIvideoxf<NPPCX, _W>(img, _src);

    // output axiStream
    hls::stream<ap_axiu<_W, 1, 1, 1> > _dst;

    // Launch the kernel
    // NOTE: Ensure your kernel fixes the 'dst_gy' deadlock mentioned in the previous step!
    axiconv_accel(_src, _dst, rows, cols);

    xf::cv::AXIvideo2cvMatxf<NPPCX>(_dst, out_img);

    // Write output image
    cv::imwrite("output.png", out_img);

    /**** validation ****/
    // 3. Compute absolute difference: |Hardware Output - Reference Image|
    // Fixed: Comparing out_img vs ref_img (instead of img)
    cv::absdiff(out_img, ref_img, diff); 
    
    cv::imwrite("error.png", diff); 

    float err_per;
    xf::cv::analyzeDiff(diff, 0, err_per);

    if (err_per > 0.0f) {
        fprintf(stderr, "ERROR: Test Failed. Error percentage: %f\n", err_per);
        return 1;
    } else {
        std::cout << "Test Passed " << std::endl;
    }
    /**** end of validation ****/

    return 0;
}