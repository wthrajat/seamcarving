#include <opencv2/opencv.hpp>
#include "sc.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv ){
    if(argc!=5){
        cout<<"Usage: ../sc input_image new_width new_height output_image"<<endl;
        return -1;
    }

    /* Load the image and it should be a 3 channel image by default but we will double check that in the seam_carving
     3 Channel image: RGB pics are 3 channels
     Grayscales pics are single (1) channel */

    Mat in_image;
    in_image = imread(argv[1]/*, CV_LOAD_IMAGE_COLOR*/);

    if(!in_image.data){
        cout<<"Input image could NOT be loaded! :("<<endl;
        return -1;
    }

    if(in_image.channels()!=3){
        cout<<"The input image is NOT a 3-channel image! :("<<in_image.depth()<<endl;
        return -1;
    }

    // get the new dimensions from the argument list
    int new_width = atoi(argv[2]);
    int new_height = atoi(argv[3]);

    // the output image
    Mat out_image;


	if(!validation(in_image.rows, in_image.cols,new_width, new_height)){
        return -1;
    }

    if(!seam_carving_trivial(in_image,out_image, new_width, new_height)){
        return -1;
    }

    // Write to the disk
    imwrite( argv[4], out_image);

    namedWindow("Original Image", WINDOW_AUTOSIZE );
    namedWindow( "Seam Carved Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", in_image );
    imshow( "Seam Carved Image", out_image );
    waitKey(0);
    return 0;
}