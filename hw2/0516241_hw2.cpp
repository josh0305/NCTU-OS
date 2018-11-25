#include "bmpReader.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

const int BILINEAR_RATIO = 7;
/*float ROTATION_ANGLE = 45;
float SHEAR_VERTICAL_DEGREE = 0.45;
float SHEAR_HORIZONTAL_DEGREE = 0.45;*/

const char* INPUT_IMAGE = "input.bmp";
const char* OUT_IMAGE[5] = {"output1.bmp", "output2.bmp", "output3.bmp", "output4.bmp", "output5.bmp"};

BmpImage bilinear(BmpImage pic, int nw, int nh);
BmpImage rotation(BmpImage pic, int nw,int nh, float ROTATION_ANGLE);
BmpImage shear_vertical(BmpImage pic, int nw, int nh, float SHEAR_VERTICAL_DEGREE);
BmpImage shear_horizontal(BmpImage pic, int nw, int nh, float SHEAR_HORIZONTAL_DEGREE);

// TODO: add your pthread codes to speed up the program
#define TNUM 5

BmpReader* bmpReader = new BmpReader();
BmpImage picture;

struct variable{
public:
	float ROTATION_ANGLE, SHEAR_VERTICAL_DEGREE, SHEAR_HORIZONTAL_DEGREE;
	int id;
};

void *runner(void *arg)
{
	variable* data = (variable *)arg;
	int nw = (int)(picture.width);
	int nh = (int)picture.height;

	
	if(data->id == 1)
	{
		BmpImage pic = rotation(picture, nw, nh, data->ROTATION_ANGLE);
		bmpReader->WriteBMP(OUT_IMAGE[data->id], pic);
		free(pic.data);
	}
	else if(data->id == 2)
	{
		BmpImage pic = shear_vertical(picture, nw, nh, data->SHEAR_VERTICAL_DEGREE);
		bmpReader->WriteBMP(OUT_IMAGE[data->id], pic);
		free(pic.data);
	}
	else if(data->id == 3)
	{
		BmpImage pic = shear_horizontal(picture, nw, nh, data->SHEAR_HORIZONTAL_DEGREE);
		bmpReader->WriteBMP(OUT_IMAGE[data->id], pic);
		free(pic.data);
	}
	else if(data->id == 4)
	{
		BmpImage pic1 = shear_vertical(picture, nw, nh, data->SHEAR_VERTICAL_DEGREE);
		BmpImage pic2 = rotation(pic1, nw, nh, data->ROTATION_ANGLE);
		BmpImage pic3 = shear_horizontal(pic2, nw, nh, data->SHEAR_HORIZONTAL_DEGREE);
		bmpReader->WriteBMP(OUT_IMAGE[data->id], pic3);
		free(pic1.data);
		free(pic2.data);
		free(pic3.data);
	}

	pthread_exit(NULL);
}


int main(){

	double time1, time2;
	
	struct timeval  tv1, tv2;

	printf("without using pthread\n");
	
	// timing function 1
	gettimeofday(&tv1, NULL);
	
	//BmpReader* bmpReader = new BmpReader();
	BmpImage input_pic = bmpReader->ReadBMP(INPUT_IMAGE);

	int newWidth = (int)(input_pic.width * BILINEAR_RATIO);
	int newHeight = (int)(input_pic.height * BILINEAR_RATIO);
	
	//all image processing
	BmpImage bilinear_pic = bilinear(input_pic, newWidth, newHeight);
	BmpImage rotation_pic = rotation(bilinear_pic, newWidth, newHeight, 45);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic, newWidth, newHeight, 0.45);
	BmpImage shearHori_pic = shear_horizontal(bilinear_pic, newWidth, newHeight, 0.45);

	BmpImage tmp_pic = rotation(shearVert_pic, newWidth, newHeight, 45);
	BmpImage mix_pic = shear_horizontal(tmp_pic, newWidth, newHeight, 0.45);
	
	//Write bmp files
	bmpReader->WriteBMP(OUT_IMAGE[0], bilinear_pic);
	bmpReader->WriteBMP(OUT_IMAGE[1], rotation_pic);
	bmpReader->WriteBMP(OUT_IMAGE[2], shearVert_pic);
	bmpReader->WriteBMP(OUT_IMAGE[3], shearHori_pic);
	bmpReader->WriteBMP(OUT_IMAGE[4], mix_pic);
	
	//free memory
	free(input_pic.data);
	free(bilinear_pic.data);
	free(rotation_pic.data);
	free(shearVert_pic.data);
	free(shearHori_pic.data);
	free(tmp_pic.data);
	free(mix_pic.data);	
	
	// timing function 2
	gettimeofday(&tv2, NULL);
	
	// TODO: modify (ID) to your student ID
	printf("0516241%f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	time1 = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	printf("using pthread\n");
	
	gettimeofday(&tv1, NULL);
	// TODO: add your pthread codes to speed up the program
	BmpImage newinput = bmpReader->ReadBMP(INPUT_IMAGE);
	picture = bilinear(newinput, newWidth, newHeight);
	bmpReader->WriteBMP(OUT_IMAGE[0], picture);


	variable* data[TNUM];
	for(int i = 0; i < TNUM; i++)
		data[i] = new variable;
	data[1]->ROTATION_ANGLE = 45;
	data[1]->SHEAR_VERTICAL_DEGREE = 0;
	data[1]->SHEAR_HORIZONTAL_DEGREE = 0;

	data[2]->ROTATION_ANGLE = 0;
	data[2]->SHEAR_VERTICAL_DEGREE = 0.3;
	data[2]->SHEAR_HORIZONTAL_DEGREE = 0;

	data[3]->ROTATION_ANGLE = 0;
	data[3]->SHEAR_VERTICAL_DEGREE = 0;
	data[3]->SHEAR_HORIZONTAL_DEGREE = 0.6;
	
	
	data[4]->SHEAR_VERTICAL_DEGREE = 0.45;
	data[4]->ROTATION_ANGLE = 60;
	data[4]->SHEAR_HORIZONTAL_DEGREE = 0.45;

	for(int i = 0; i < TNUM; i++)
		data[i]->id = i;

	pthread_t pid[TNUM];
	for(int i = 0; i < TNUM; i++)
	{
		pthread_create(&pid[i], NULL, runner, (void *)data[i]);
	}

	for(int i = 0; i < TNUM; i++)
		pthread_join(pid[i], NULL);

	free(newinput.data);
	free(picture.data);
	for(int i = 0; i < TNUM; i++)
		free(data[i]);



	gettimeofday(&tv2, NULL);
	time2 = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	printf("0516241%f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	printf("speed up rate = %f\n", (double)(time1/time2));
} 

BmpImage bilinear(BmpImage pic, int newWidth, int newHeight)
{
	printf("bilinear\n");
	unsigned char* data	= (unsigned char*)malloc(3 * newWidth * newHeight * sizeof(unsigned char));

	for(int i = 0 ; i < newWidth; i++){
		for(int j = 0;j < newHeight; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				int relevant_w = (int)(i / BILINEAR_RATIO);
				int relevant_h = (int)(j / BILINEAR_RATIO);
				int reference_points[4];
				
				// the four reference points of this pixel
				reference_points[0] = pic.data[3 * (relevant_h * pic.width + relevant_w) + k];
				reference_points[1] = pic.data[3 * (relevant_h * pic.width + (relevant_w + 1)) + k];
				reference_points[2] = pic.data[3 * ((relevant_h + 1) * pic.width + relevant_w) + k];
				reference_points[3] = pic.data[3 * ((relevant_h + 1) * pic.width + (relevant_w + 1)) + k];
				
				// TODO: assign a new value to this pixel
				int x = i % 7, y = j % 7;
				pixel = reference_points[0]*(7-x)*(7-y)/(7*7) + reference_points[1]*x*(7-y)/49 + reference_points[2]*(7-x)*y/49 + reference_points[3]*x*y/49;
				
				if (pixel < 0) pixel = 0;
				if (pixel > 255) pixel = 255;	
				
				data[3 * (j * newWidth + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = newWidth;
	newBmpImage.height = newHeight;
	
	return newBmpImage;
}

BmpImage rotation(BmpImage pic,int nw,int nh, float ROTATION_ANGLE)
{
	printf("rotation\n");
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	
	int w_offset = (int)(nw / 2);
	int h_offset = (int)(nh / 2);
	
	for(int i = 0 ; i < nw; i++){
		for(int j = 0; j < nh; j++){
			for(int k = 0 ; k < 3 ; k++){
				
				double angle = (double)ROTATION_ANGLE * M_PI / 180.0;
				double cos_theta = cos(angle);    
				double sin_theta = sin(angle);
				int relevant_w = i - w_offset;
				int relevant_h = j - h_offset;
				int trans_w = (int)(relevant_w * cos_theta - relevant_h * sin_theta) + w_offset;
				int trans_h = (int)(relevant_h * cos_theta + relevant_w * sin_theta) + h_offset;
				
				int pixel;
				
				if (trans_w >= 0 && trans_w < w_offset * 2 && trans_h >= 0 && trans_h < h_offset * 2){
					pixel = pic.data[3 * (trans_h * 2 * w_offset + trans_w) + k];
				}else{
					pixel = 0;
				}
				
				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}
	
	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;
	
	return newBmpImage;
}


BmpImage shear_vertical(BmpImage pic, int nw, int nh, float SHEAR_VERTICAL_DEGREE)
{
	printf("shear vertical\n");
	
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				int h_offset = (int)(nw * SHEAR_VERTICAL_DEGREE / 2);
				int trans_w = i;
				int trans_h = (int)(j + i * SHEAR_VERTICAL_DEGREE) - h_offset;
				
				if (trans_h >= 0 && trans_h < nh){
					pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
				}else{
					pixel = 0;
				}
				
				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}
	
	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}

BmpImage shear_horizontal(BmpImage pic, int nw, int nh, float SHEAR_HORIZONTAL_DEGREE)
{
	printf("shear horizontal\n");
	
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				
				// TODO: refer to shear_vertical, try to write shear_horizontal on your own
				int w_offset = (int)(nw * SHEAR_HORIZONTAL_DEGREE / 2);
				int trans_w = (int)(i + j * SHEAR_HORIZONTAL_DEGREE) - w_offset;
				int trans_h = j;
				
				if (trans_w >= 0 && trans_w < nw){
					pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
				}else{
					pixel = 0;
				}
				
				
				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}
	
	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}

