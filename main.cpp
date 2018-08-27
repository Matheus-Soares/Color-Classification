#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

char *window = (char*) "Original image";
bool leftButtonDown = false;
int k;
Point pt;
Mat img;

void menu();
void L1_norm();
void L2_norm();
void mahalanobis();
void knn();
void sel_methods();
static void points(int event, int x, int y, int, void*);


int main(int argc, char *argv[]){

    /// Read the image
    img = imread(argv[1], IMREAD_COLOR);

    if (img.empty()){
        cout << "Error loading the image" << endl;
        return -1;
    }

    /// Create first image window
    namedWindow(window, WINDOW_NORMAL);
    imshow(window, img);

    /// Set mouse callback
    setMouseCallback(window, points, &img);

    sel_methods();

    waitKey(0);

    return 0;
}

void menu(){
    cout << "-----------------------------" << endl;
    cout << "Method: " << endl;
    cout << "1 - L1-norm (cube)" << endl;
    cout << "2 - L2-norm (sphere)" << endl;
    cout << "3 - Mahalanobis" << endl;
    cout << "4 - k nearest neighbor" << endl;
    cout << "Esc - Exit" << endl;
    cout << "-----------------------------" << endl;
}

void L1_norm(){
    int th;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Mat imgL1;

    th = 50;

    cout << "Select one point and press some key" << endl;
    waitKey(0);

    img.copyTo(imgL1);

    color = imgL1.at<Vec3b>(Point(pt.x, pt.y));

    for(int i = 0; i < imgL1.rows; i ++){
        for (int j = 0; j < imgL1.cols; j++){
            pixel = imgL1.at<Vec3b>(Point(j, i));
            if((color.val[0] + th > pixel.val[0]) && (color.val[0] - th < pixel.val[0])) {
                if ((color.val[1] + th > pixel.val[1]) && (color.val[1] - th < pixel.val[1])) {
                    if ((color.val[2] + th > pixel.val[2]) && (color.val[2] - th < pixel.val[2])) {
                        imgL1.at<Vec3b>(Point(j, i)) = white;
                    }
                }
            }
        }
    }

    cout << "L1-norm image" << endl;

    //imwrite("L1-norm.jpg", imgL1);
    namedWindow("L1-norm");
    imshow("L1-norm", imgL1);
}

void L2_norm(){
    int th;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Mat imgL2;

    th = 50;

    cout << "Select one point and press some key" << endl;
    waitKey(0);

    img.copyTo(imgL2);

    color = imgL2.at<Vec3b>(Point(pt.x, pt.y));

    for(int i = 0; i < imgL2.rows; i ++){
        for (int j = 0; j < imgL2.cols; j++){
            pixel = imgL2.at<Vec3b>(Point(j, i));

            float de =  (pixel.val[0] - color.val[0])*(pixel.val[0] - color.val[0]) +
                        (pixel.val[1] - color.val[1])*(pixel.val[1] - color.val[1]) +
                        (pixel.val[2] - color.val[2])*(pixel.val[2] - color.val[2]);

            if(sqrt(de) < th)
                imgL2.at<Vec3b>(Point(j, i)) = white;
        }
    }

    cout << "L2-norm image" << endl;

    //imwrite("L2-norm.jpg", imgL2);
    namedWindow("L2-norm");
    imshow("L2-norm", imgL2);

}

void mahalanobis(){

}

void knn(){
    int th;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Mat imgKnn;

    th = 50;

    img.copyTo(imgKnn);

    cout << "Select one point and press some key" << endl;
    cout << "Press ESC to stop" << endl;

    namedWindow("K-nearest neighbors");

    while(waitKey() != 27){

        color = imgKnn.at<Vec3b>(Point(pt.x, pt.y));

        for(int i = 0; i < imgKnn.rows; i ++){
            for (int j = 0; j < imgKnn.cols; j++){
                pixel = imgKnn.at<Vec3b>(Point(j, i));

                float de =  (pixel.val[0] - color.val[0])*(pixel.val[0] - color.val[0]) +
                            (pixel.val[1] - color.val[1])*(pixel.val[1] - color.val[1]) +
                            (pixel.val[2] - color.val[2])*(pixel.val[2] - color.val[2]);

                if(sqrt(de) < th)
                    imgKnn.at<Vec3b>(Point(j, i)) = white;
            }
        }

        cout << "Image updated" << endl;

        imshow("K-nearest neighbors", imgKnn);
    }
    //imwrite("knn.jpg", imgKnn);

}

void sel_methods(){

    for(;;){
        menu();
        k = waitKey(0);

        switch(k){
            case 49:        /// Key = 1
                L1_norm();
                break;
            case 50:        /// Key = 2
                L2_norm();
                break;
            case 51:        /// Key = 3
                mahalanobis();
                break;
            case 52:        /// Key = 4
                knn();
                break;
            default:
                break;
        }

        if(k == 27){        ///Key = Esc
            destroyAllWindows();
            break;
        }
    }
}


static void points(int event, int x, int y, int, void* img_){
    Vec3b color(255, 255, 255);
    Mat &img = *(Mat*) img_;

    if(event == CV_EVENT_LBUTTONDOWN && (k == 49 || k == 50 || k == 52)){
        pt.x = x;
        pt.y = y;
        return;
    }

    if (event == CV_EVENT_LBUTTONDOWN)
        leftButtonDown = true;
    else if(event == CV_EVENT_MOUSEMOVE && leftButtonDown)
        img.at<Vec3b>(Point(x, y)) = color;
    else if(event == CV_EVENT_LBUTTONUP)
        leftButtonDown = false;

    imshow(window, img);

}