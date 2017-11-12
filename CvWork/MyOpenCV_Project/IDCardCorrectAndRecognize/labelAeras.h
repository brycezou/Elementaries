#include <highgui.h>
#include <math.h>
#include <cv.h>
using namespace std;

#define NAME_SX		93/1.0/498
#define NAME_BX		180/1.0/498
#define NAME_SY		34/1.0/308
#define NAME_BY		75/1.0/308

#define SEX_SX			93/1.0/498
#define SEX_BX			132/1.0/498
#define SEX_SY			77/1.0/308
#define SEX_BY			111/1.0/308

#define MINZU_SX		198/1.0/498
#define MINZU_BX		254/1.0/498
#define MINZU_SY		77/1.0/308
#define MINZU_BY		111/1.0/308

#define YEAR_SX		93/1.0/498
#define YEAR_BX		151/1.0/498
#define YEAR_SY		114/1.0/308
#define YEAR_BY		145/1.0/308

#define MONTH_SX	168/1.0/498
#define MONTH_BX	200/1.0/498
#define MONTH_SY	114/1.0/308
#define MONTH_BY	145/1.0/308

#define DAY_SX			218/1.0/498
#define DAY_BX			248/1.0/498
#define DAY_SY			114/1.0/308
#define DAY_BY			145/1.0/308

#define ADDRESS_SX		93/1.0/498
#define ADDRESS_BX	313/1.0/498
#define ADDRESS_SY		154/1.0/308
#define ADDRESS_BY	239/1.0/308

#define PICTURE_SX		321/1.0/498
#define PICTURE_BX		482/1.0/498
#define PICTURE_SY		34/1.0/308
#define PICTURE_BY		237/1.0/308

#define NUMBER_SX		170/1.0/498
#define NUMBER_BX		460/1.0/498
#define NUMBER_SY		252/1.0/308
#define NUMBER_BY		291/1.0/308

#define NUM_BOX_ORG	9
enum BOX_ORG															//身份证中预定义的9个区域的编号
{
	NAME_ORG, SEX_ORG, MINZU_ORG, YEAR_ORG, MONTH_ORG, DAY_ORG, ADDRESS_ORG, PICTURE_ORG, NUMBER_ORG
};
CvRect gOrgRectBoxArray[NUM_BOX_ORG];			//每个区域的大致位置，绝对坐标
CvRect gAccurateRectBoxArray[NUM_BOX_ORG];	//每个区域中的文字区域的位置，相对坐标
CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];	//其实gAbsoluteBoxRectArray中存储的不是真正的CvRect结构，而是该结构的左上角和右下角的坐标值


void MyLabelAeras(IplImage *corrected_RGB_img, int real_width, int real_height, BOOL whetherDraw)
{
	gOrgRectBoxArray[NAME_ORG].x=real_width*NAME_SX;
	gOrgRectBoxArray[NAME_ORG].y=real_height*NAME_SY;
	gOrgRectBoxArray[NAME_ORG].width=real_width*(NAME_BX-NAME_SX);
	gOrgRectBoxArray[NAME_ORG].height=real_height*(NAME_BY-NAME_SY);

	gOrgRectBoxArray[SEX_ORG].x=real_width*SEX_SX;
	gOrgRectBoxArray[SEX_ORG].y=real_height*SEX_SY;
	gOrgRectBoxArray[SEX_ORG].width=real_width*(SEX_BX-SEX_SX);
	gOrgRectBoxArray[SEX_ORG].height=real_height*(SEX_BY-SEX_SY);

	gOrgRectBoxArray[MINZU_ORG].x=real_width*MINZU_SX;
	gOrgRectBoxArray[MINZU_ORG].y=real_height*MINZU_SY;
	gOrgRectBoxArray[MINZU_ORG].width=real_width*(MINZU_BX-MINZU_SX);
	gOrgRectBoxArray[MINZU_ORG].height=real_height*(MINZU_BY-MINZU_SY);

	gOrgRectBoxArray[YEAR_ORG].x=real_width*YEAR_SX;
	gOrgRectBoxArray[YEAR_ORG].y=real_height*YEAR_SY;
	gOrgRectBoxArray[YEAR_ORG].width=real_width*(YEAR_BX-YEAR_SX);
	gOrgRectBoxArray[YEAR_ORG].height=real_height*(YEAR_BY-YEAR_SY);

	gOrgRectBoxArray[MONTH_ORG].x=real_width*MONTH_SX;
	gOrgRectBoxArray[MONTH_ORG].y=real_height*MONTH_SY;
	gOrgRectBoxArray[MONTH_ORG].width=real_width*(MONTH_BX-MONTH_SX);
	gOrgRectBoxArray[MONTH_ORG].height=real_height*(MONTH_BY-MONTH_SY);

	gOrgRectBoxArray[DAY_ORG].x=real_width*DAY_SX;
	gOrgRectBoxArray[DAY_ORG].y=real_height*DAY_SY;
	gOrgRectBoxArray[DAY_ORG].width=real_width*(DAY_BX-DAY_SX);
	gOrgRectBoxArray[DAY_ORG].height=real_height*(DAY_BY-DAY_SY);

	gOrgRectBoxArray[ADDRESS_ORG].x=real_width*ADDRESS_SX;
	gOrgRectBoxArray[ADDRESS_ORG].y=real_height*ADDRESS_SY;
	gOrgRectBoxArray[ADDRESS_ORG].width=real_width*(ADDRESS_BX-ADDRESS_SX);
	gOrgRectBoxArray[ADDRESS_ORG].height=real_height*(ADDRESS_BY-ADDRESS_SY);

	gOrgRectBoxArray[PICTURE_ORG].x=real_width*PICTURE_SX;
	gOrgRectBoxArray[PICTURE_ORG].y=real_height*PICTURE_SY;
	gOrgRectBoxArray[PICTURE_ORG].width=real_width*(PICTURE_BX-PICTURE_SX);
	gOrgRectBoxArray[PICTURE_ORG].height=real_height*(PICTURE_BY-PICTURE_SY);

	gOrgRectBoxArray[NUMBER_ORG].x=real_width*NUMBER_SX;
	gOrgRectBoxArray[NUMBER_ORG].y=real_height*NUMBER_SY;
	gOrgRectBoxArray[NUMBER_ORG].width=real_width*(NUMBER_BX-NUMBER_SX);
	gOrgRectBoxArray[NUMBER_ORG].height=real_height*(NUMBER_BY-NUMBER_SY);

	if (whetherDraw)
	{
		cvRectangle(corrected_RGB_img, cvPoint(real_width*NAME_SX, real_height*NAME_SY), cvPoint(real_width*NAME_BX, real_height*NAME_BY), cvScalar(0, 0, 255), 1);								//姓名
		cvRectangle(corrected_RGB_img, cvPoint(real_width*SEX_SX, real_height*SEX_SY), cvPoint(real_width*SEX_BX, real_height*SEX_BY), cvScalar(0, 0, 255), 1);													//性别
		cvRectangle(corrected_RGB_img, cvPoint(real_width*MINZU_SX, real_height*MINZU_SY), cvPoint(real_width*MINZU_BX, real_height*MINZU_BY), cvScalar(0, 0, 255), 1);							//民族
		cvRectangle(corrected_RGB_img, cvPoint(real_width*YEAR_SX, real_height*YEAR_SY), cvPoint(real_width*YEAR_BX, real_height*YEAR_BY), cvScalar(0, 0, 255), 1);									//年
		cvRectangle(corrected_RGB_img, cvPoint(real_width*MONTH_SX, real_height*MONTH_SY), cvPoint(real_width*MONTH_BX, real_height*MONTH_BY), cvScalar(0, 0, 255), 1);						//月
		cvRectangle(corrected_RGB_img, cvPoint(real_width*DAY_SX, real_height*DAY_SY), cvPoint(real_width*DAY_BX, real_height*DAY_BY), cvScalar(0, 0, 255), 1);											//日
		cvRectangle(corrected_RGB_img, cvPoint(real_width*ADDRESS_SX, real_height*ADDRESS_SY), cvPoint(real_width*ADDRESS_BX, real_height*ADDRESS_BY), cvScalar(0, 0, 255), 1);		//住址
		cvRectangle(corrected_RGB_img, cvPoint(real_width*PICTURE_SX, real_height*PICTURE_SY), cvPoint(real_width*PICTURE_BX, real_height*PICTURE_BY), cvScalar(0, 0, 255), 1);				//照片
		cvRectangle(corrected_RGB_img, cvPoint(real_width*NUMBER_SX, real_height*NUMBER_SY), cvPoint(real_width*NUMBER_BX, real_height*NUMBER_BY), cvScalar(0, 0, 255), 1);			//身份证号码
	}
}

