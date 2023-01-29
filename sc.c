#include "sc.h"
#include <math.h>
#include <vector>
#include <memory>
using namespace cv;
using namespace std;


bool validation(int rows, int cols,int newWidth,int newHeight){

    if(newWidth>cols){
        cout<<"Invalid request!!! new Width has to be smaller than the current size!"<<endl;
        return false;
    }
    if(newHeight>rows){
        cout<<"Invalid request!!! new Height has to be smaller than the current size!"<<endl;
        return false;
    }
    if(newWidth<=0){
        cout<<"Invalid request!!! newWidth has to be positive!"<<endl;
        return false;
    }
    if(newHeight<=0){
        cout<<"Invalid request!!! newHeight has to be positive!"<<endl;
        return false;
    }
	return true;
}

// seam carves by removing trivial seams
bool seam_carving_trivial(Mat& inImage, Mat& outImage, int newWidth, int newHeight){

    Mat iimage = inImage.clone();
    Mat oimage = inImage.clone();

	while(iimage.rows!=newHeight || iimage.cols!=newWidth){

	 if(iimage.rows>newHeight){
		reduce_horizontal_seam_trivial(iimage, oimage);
	    cout<<"Horizontal Seam Removed, New height is "<<oimage.rows<<endl;
		iimage = oimage.clone();
        }

        if(iimage.cols>newWidth){
    	reduce_vertical_seam_trivial(iimage, oimage);
	    cout<<"Vertical Seam Removed, New Width is "<<oimage.cols<<endl;
        iimage = oimage.clone();
     }
    }

    outImage = oimage.clone();
    return true;
}

double findPixelEnergy(Mat& inImage,int i,int j)
{    Vec3b pixelNx, pixelNy;
	 Vec3b pixelPx, pixelPy;
     int rows = inImage.rows;
     int cols = inImage.cols;

	//pixel (x + 1, y) 	N-Next	 X gradient
     if(i!=rows-1)
	     pixelNx  = inImage.at<Vec3b>(i+1, j);
	else
	     pixelNx  = inImage.at<Vec3b>(0, j);

	//pixel (x, y+1) 	N-Next	 Y gradient
	if(j!=cols-1)
         pixelNy  = inImage.at<Vec3b>(i, j+1);
    else
         pixelNy  = inImage.at<Vec3b>(i, 0);

	//pixel (x − 1, y)  P-Previous X gradient
	if(i!=0)
		pixelPx  = inImage.at<Vec3b>(i-1, j);
	else
		pixelPx  = inImage.at<Vec3b>(rows-1, j);

	//pixel (x − 1, y)  P-Previous  Y gradient
    if (j!=0)
	      pixelPy  = inImage.at<Vec3b>(i, j-1);
    else
         pixelPy  = inImage.at<Vec3b>(i, cols-1);

	   double xGradient = pow(pixelNx[0] - pixelPx[0],2) + pow(pixelNx[1] - pixelPx[1],2) + pow(pixelNx[2] - pixelPx[2],2);
	   double yGradient = pow(pixelNy[0] - pixelPy[0],2) + pow(pixelNy[1] - pixelPy[1],2) + pow(pixelNy[2] - pixelPy[2],2);

	   return xGradient + yGradient;
}

// horizontl trivial seam is a seam through the center of the image
 bool reduce_horizontal_seam_trivial(Mat& inImage, Mat& outImage){

	int cols = inImage.cols;
	int rows = inImage.rows;

	double  pixelEnergy = 0;
    vector<vector<int> > cumulativeEnergy;
	cumulativeEnergy.resize(rows,vector<int>(cols));

	int prevPixelEnergy = 0,i,j,k;

   //Cumulative energy
    for(j=0;j<cols;++j)
     for(i=0;i<rows;++i){
       pixelEnergy = findPixelEnergy(inImage,i,j);
    	if(j==0)
			prevPixelEnergy = 0;
		else if (i==0)
		    prevPixelEnergy = min(cumulativeEnergy[i][j-1],cumulativeEnergy[i+1][j-1]);
		else if(i==rows-1)
            prevPixelEnergy =  min(cumulativeEnergy[i-1][j-1],cumulativeEnergy[i][j-1]);
        else
            prevPixelEnergy = min(min(cumulativeEnergy[i-1][j-1],cumulativeEnergy[i][j-1]),cumulativeEnergy[i+1][j-1]);

	    cumulativeEnergy[i][j] = pixelEnergy + prevPixelEnergy;
	}

	int minEnergyPixel = cumulativeEnergy[0][cols-1], horizontalSeamX = 0;

	//Find optimal Horizontal Seam
 	 for(j=1;j<rows;j++)
		  {  if (minEnergyPixel > cumulativeEnergy[j][cols-1])
			  { minEnergyPixel = cumulativeEnergy[j][cols-1];
			    horizontalSeamX = j;
			  }
	  }

	vector<int> horizontalSeamValues(cols); //X values for each Y
	horizontalSeamValues[cols-1] = horizontalSeamX;

	// find pixel positions in optimal Seam path
	for(j = cols-1; j>0; j--){

	  if(horizontalSeamX == 0)
	   {    if(cumulativeEnergy[horizontalSeamX][j-1] > cumulativeEnergy[horizontalSeamX+1][j-1])
		        horizontalSeamX++;
	   }
	  else if (horizontalSeamX ==(rows-1))
	  {     if(cumulativeEnergy[horizontalSeamX-1][j-1] < cumulativeEnergy[horizontalSeamX][j-1])
				horizontalSeamX--;
	  }
	  else if(cumulativeEnergy[horizontalSeamX-1][j-1] < cumulativeEnergy[horizontalSeamX][j-1])
		{   if(cumulativeEnergy[horizontalSeamX-1][j-1] < cumulativeEnergy[horizontalSeamX+1][j-1])
				horizontalSeamX--;
			else
              horizontalSeamX++;
		}
	   else
		{   if(cumulativeEnergy[horizontalSeamX][j-1] > cumulativeEnergy[horizontalSeamX+1][j-1])
                horizontalSeamX++;
		}
       horizontalSeamValues[j-1] = horizontalSeamX;
	}

	 // create an image slighly smaller
    outImage.create(rows-1, cols, CV_8UC3);

	int iPrev,jPrev;

	 //taking pixels from input image and copying to output image
	for(jPrev = 0; jPrev < cols; ++jPrev){
	  for(iPrev = 0,i = 0; iPrev < rows && i<(rows-1); ++iPrev){
           if(iPrev == horizontalSeamValues[jPrev])
              continue;
	       else
     	     { outImage.at<Vec3b>(i,jPrev) = inImage.at<Vec3b>(iPrev, jPrev);
			   i++;
             }
		}
     }

	 horizontalSeamValues.clear();
	 cumulativeEnergy.clear();

    return true;
}

// vertical trivial seam is a seam through the center of the image
bool reduce_vertical_seam_trivial(Mat& inImage, Mat& outImage){
    // retrieve the dimensions of the image
    int rows = inImage.rows;
    int cols = inImage.cols;
    double  pixelEnergy = 0;
    vector<vector<int> > cumulativeEnergy;
	cumulativeEnergy.resize(rows,vector<int>(cols));
	int prevPixelEnergy = 0, i,j,k;

   // create an image slighly smaller
	outImage.create(rows, cols-1, CV_8UC3);

   // Cumulative energy
   for(i=0;i<rows;++i)
    for(j=0;j<cols;++j){
       pixelEnergy = findPixelEnergy(inImage,i,j);
	   if(i==0)
			prevPixelEnergy = 0;
	   else if (j==0)
		    prevPixelEnergy = min(cumulativeEnergy[i-1][j],cumulativeEnergy[i-1][j+1]);
	   else if(j==cols-1)
            prevPixelEnergy =  min(cumulativeEnergy[i-1][j-1],cumulativeEnergy[i-1][j]);
       else
            prevPixelEnergy = min(min(cumulativeEnergy[i-1][j-1],cumulativeEnergy[i-1][j]),cumulativeEnergy[i-1][j+1]);

	    cumulativeEnergy[i][j] = pixelEnergy + prevPixelEnergy;
	}


	   int minEnergyPixel = cumulativeEnergy[rows-1][0], verticalSeamY = 0;

	 //Find optimal Vertical Seam
	for(j=1;j<cols;j++)
		  {  if (minEnergyPixel > cumulativeEnergy[rows-1][j])
			  {  minEnergyPixel = cumulativeEnergy[rows-1][j];
			     verticalSeamY = j;
			  }
	  }

	vector<int> verticalSeamValues(rows);
	verticalSeamValues[rows-1] = verticalSeamY;

	//save the pixels of the optimal path
	for(i = rows-1;i>0;i--){
	  if(verticalSeamY == 0)
	   {  if(cumulativeEnergy[i-1][verticalSeamY] > cumulativeEnergy[i-1][verticalSeamY+1])
			  verticalSeamY++;
	   }
	  else if (verticalSeamY == rows-1)
	  {  if(cumulativeEnergy[i-1][verticalSeamY-1] < cumulativeEnergy[i-1][verticalSeamY])
					 verticalSeamY--;
	  }
	  else if(cumulativeEnergy[i-1][verticalSeamY-1] < cumulativeEnergy[i-1][verticalSeamY])
	  {  if(cumulativeEnergy[i-1][verticalSeamY-1] < cumulativeEnergy[i-1][verticalSeamY+1])
					 verticalSeamY--;
			   else
                     verticalSeamY++;
	  }
	  else
			{    if(cumulativeEnergy[i-1][verticalSeamY] > cumulativeEnergy[i-1][verticalSeamY+1])
               		    verticalSeamY++;
			}
       verticalSeamValues[i-1] = verticalSeamY;
	}

	int  iPrev, jPrev;

    for(iPrev = 0; iPrev < rows; ++iPrev){
        for(jPrev = 0,j = 0; jPrev < cols && j<cols-1; ++jPrev){
            if(jPrev == verticalSeamValues[iPrev])
             { continue;
			}
	       else
     	     { outImage.at<Vec3b>(iPrev,j) = inImage.at<Vec3b>(iPrev, jPrev);
			   j++;
             }
		}
	}

	verticalSeamValues.clear();
	cumulativeEnergy.clear();
	return true;
}