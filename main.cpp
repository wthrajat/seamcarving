#include <opencv2/opencv.hpp>
#include "sc.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv ){
    if(argc!=5){
        cout<<"Usage: ../sc input_image new_width new_height output_image"<<endl;
        return -1;
    }

    // Load the input image
    // the image should be a 3 channel image by default but we will double check that in the seam_carving
    Mat in_image;
    in_image = imread(argv[1]/*, CV_LOAD_IMAGE_COLOR*/);

    if(!in_image.data){
        cout<<"Could not load input image!!!"<<endl;
        return -1;
    }

    if(in_image.channels()!=3){
        cout<<"Image does not have 3 channels!!! "<<in_image.depth()<<endl;
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

    // write it on disk
    imwrite( argv[4], out_image);

    namedWindow( "Original image", WINDOW_AUTOSIZE );
    namedWindow( "Seam Carved Image", WINDOW_AUTOSIZE );
    imshow( "Original image", in_image );
    imshow( "Seam Carved Image", out_image );
    waitKey(0);
    return 0;
}