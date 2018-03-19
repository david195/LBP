/*
 * David Ruiz Garcia
 *
*/

#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

Mat LBP(Mat img, int nneighbors, int nspaces, int cw);
string cvD2B(int num);
void load(int cw, string src, int id);
vector <Point> anti_cw(vector<Point> cell);

int main( int argc, char** argv )
{
  for(int i=1; i<=22; i++){
    ostringstream url;
    url << "bcdr_png/" << i << ".png";
    load(1,url.str(), i);
    load(0,url.str(), i);
  }

  return 0;
}

Mat LBP(Mat img, int nneighbors, int nspaces, int cw){
  int rows = img.size().height;
  int cols = img.size().width;
  Mat ILBP = Mat::zeros( img.size(), CV_8UC1 );

  cout << "(" << nneighbors << ":" << nspaces << ")" << endl << "*****************" << endl;
  for(int x=0; x<cols; x++){
    for(int y=0; y<rows; y++){
      Point p = Point(x,y);
      vector<Point> cell;
      int nx,ny;
      int itr=(8*nspaces)/nneighbors;
      for(int i=-nspaces; i<=nspaces; i+=itr){
        nx=x+i;
        ny=y-nspaces;
        cell.push_back(Point(nx,ny));
      }
      for(int i=-(nspaces-itr); i<=nspaces; i+=itr){
        nx=x+nspaces;
        ny=y+i;
        cell.push_back(Point(nx,ny));
      }
      for(int i=nspaces-itr; i>=-nspaces; i-=itr){
        nx=x+i;
        ny=y+nspaces;
        cell.push_back(Point(nx,ny));
      }
      for(int i=nspaces-itr; i>=-(nspaces-1); i-=itr){
        nx = x-nspaces;
        ny = y+i;
        cell.push_back(Point(nx,ny));
      }
      if(cw==0)
        cell = anti_cw(cell);
      int val_LBP=0;
      for(int i=0; i<cell.size(); i++){
        if(cell[i].y > 0 && cell[i].y<rows && cell[i].x > 0 && cell[i].x<cols){
          Scalar intensity = img.at<uchar>(cell[i].y, cell[i].x);
          float v = intensity.val[0];
          intensity = img.at<uchar>(y, x);
          float vp = intensity.val[0];
          if(v>=vp)
            val_LBP += pow(2,i);
        }
      }
      cout << x << "," << y << ": "<< val_LBP << " -> " << cvD2B(val_LBP) << endl;
      ILBP.at<uchar>(Point(x,y)) = val_LBP;
    }
  }
  cout << endl<<endl;
  return ILBP;
}


string cvD2B(int num){
  string bin = "";
  if (num > 0) {
        while (num > 0) {
            if (num%2 == 0) {
                bin = "0"+bin;
            } else {
                bin = "1"+bin;
            }
            num = (int) num/2;
        }
    } else if (num == 0) {
        bin = "0";
    } else {
        bin = "No se puede realizar la conversión. Ingrese solo nums positivos";
    }
  return bin;
}

void load(int cw, string src, int id){
  Mat image;
  image = imread(src, CV_LOAD_IMAGE_GRAYSCALE );
  //cvtColor(image, image, COLOR_BGR2GRAY );
  if(! image.data ) {
    cout <<  "Could not open or find the image" << std::endl ;
    return;
  }

  ostringstream url;
  if(cw)
    url << "out/" << id << "/cw/" << "data.txt";
  else
    url << "out/" << id << "/acw/" << "data.txt";
  url.str("");

  int nneighbors = 8;
  int nspaces = 1;
  Mat img_LBP_8_1 = LBP(image,nneighbors,nspaces, cw);

  nneighbors = 8;
  nspaces = 2;
  Mat img_LBP_8_2 = LBP(image,nneighbors,nspaces, cw);

  nneighbors = 16;
  nspaces = 2;
  Mat img_LBP_16_2 = LBP(image,nneighbors,nspaces, cw);

  if(cw==1){
    url << "out/" << id << "/cw/" << "8_1" << ".png";
    imwrite(url.str(), img_LBP_8_1);
    url.str("");

    url << "out/" << id << "/cw/" << "8_2" << ".png";
    imwrite(url.str(), img_LBP_8_2);
    url.str("");

    url << "out/" << id << "/cw/" << "16_2" << ".png";
    imwrite(url.str(), img_LBP_16_2);
  }
  else{
    url << "out/" << id << "/acw/" << "8_1" << ".png";
    imwrite(url.str(), img_LBP_8_1);
    url.str("");

    url << "out/" << id << "/acw/" << "8_2" << ".png";
    imwrite(url.str(), img_LBP_8_2);
    url.str("");

    url << "out/" << id << "/acw/" << "16_2" << ".png";
    imwrite(url.str(), img_LBP_16_2);
  }

  /*cout << "image: " << id;
  if(cw)
    cout << " clockwise" << endl;
  else
    cout << " anti-clockwise" << endl;*/
}

vector <Point> anti_cw(vector<Point> cell){
  vector <Point> res;
  for(int i=cell.size(); i>1; i--){
    res.push_back(cell[i]);
  }
  res.insert(res.begin(),cell[0]);
  return res;
}
