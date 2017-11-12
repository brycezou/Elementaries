#include <cv.h>
#include <highgui.h>
using namespace cv;


int main(int argc, char** argv)
{	
	IplImage * src=cvLoadImage(argv[1], 0);
	IplImage* paintx=cvCreateImage( cvGetSize(src),IPL_DEPTH_8U, 1 );
	IplImage* painty=cvCreateImage( cvGetSize(src),IPL_DEPTH_8U, 1 );
	cvZero(paintx);
	cvZero(painty);
	int* v=new int[src->width];
	int* h=new int[src->height];
	memset(v, 0, src->width*4);
	memset(h, 0, src->height*4);

	int x,y;
	CvScalar s,t;
	for(x=0; x<src->width; x++)
	{
		for(y=0; y<src->height; y++)
		{
			s=cvGet2D(src, y, x);			
			if(s.val[0]==255)
				v[x]++;					
		}		
	}

	for(x=0; x<src->width; x++)
	{
		for(y=0; y<v[x]; y++)
		{		
			t.val[0]=255;
			cvSet2D(paintx, y, x, t);		
		}		
	}

	for(y=0; y<src->height; y++)
	{
		for(x=0; x<src->width; x++)
		{
			s=cvGet2D(src, y, x);			
			if(s.val[0]==255)
				h[y]++;		
		}	
	}
	for(y=0; y<src->height; y++)
	{
		for(x=0; x<h[y]; x++)
		{			
			t.val[0]=255;
			cvSet2D(painty, y, x, t);			
		}		
	}


	for (int i=0; i<src->width; i++)
	{
		std::cout<<v[i]<<"\t";
	}
	std::cout<<std::endl<<std::endl;
	for (int i=0; i<src->height; i++)
	{
		std::cout<<h[i]<<"\t";
	}
	std::cout<<std::endl<<std::endl;

	int minX, maxX, minY, maxY;
	for (int i=0; i<src->width; i++)
	{
		if(v[i]!=0)
		{
			minX=i;
			break;
		}
	}
	for (int i=src->width-1; i>0; i--)
	{
		if(v[i]!=0)
		{
			maxX=i;
			break;
		}
	}
	for (int i=0; i<src->height; i++)
	{
		if(h[i]!=0)
		{
			minY=i;
			break;
		}
	}
	for (int i=src->height-1; i>0; i--)
	{
		if(h[i]!=0)
		{
			maxY=i;
			break;
		}
	}
	std::cout<<minX<<std::endl<<maxX<<std::endl<<minY<<std::endl<<maxY<<std::endl;

	Mat rectImage;
	rectImage.create(cvGetSize(src), CV_8UC3); 
	for (int i=0; i<rectImage.rows; i++)
	{
		uchar *depthhead = rectImage.ptr<uchar>(i);//point of the row i , one uchar indicates one color
		for (int j=0; j<rectImage.cols; j++)
		{
			s=cvGet2D(src, i, j);		
			depthhead[3*j] = s.val[0];
			depthhead[3*j+1] = s.val[0];
			depthhead[3*j+2] = s.val[0];
		}
	}
	IplImage ipl_rectImage=rectImage;
	cvRectangle(&ipl_rectImage, cvPoint(minX, minY), cvPoint(maxX, maxY), cvScalar(0, 0, 255), 3);

	cvShowImage("二值图像", src);
	cvSaveImage("二值图像.jpg", src);
	cvShowImage("ipl_rectImage", &ipl_rectImage);
	cvSaveImage("ipl_rectImage.jpg", &ipl_rectImage);
	cvShowImage("垂直积分投影", paintx);
	cvSaveImage("垂直积分投影.jpg", paintx);
	cvShowImage("水平积分投影", painty);
	cvSaveImage("水平积分投影.jpg", painty);

	CvRect rect;
	rect.x=minX;
	rect.y=minY;
	rect.width=maxX-minX;
	rect.height=maxY-minY;
	cvSetImageROI(src, rect);
	cvShowImage("结果", src);
	cvSaveImage("结果.jpg", src);
	cvResetImageROI(src);

	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&paintx);
	cvReleaseImage(&painty);

	return 0;
}