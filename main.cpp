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
vector<Point2f> points;

void menu();
void L1_norm();
void L2_norm();
void mahalanobis();
void knn();
void sel_methods();
static void mouseCallback(int event, int x, int y, int, void*);


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
    setMouseCallback(window, mouseCallback, &img);

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
    Vec3b black(0, 0, 0);

    Mat imgL1;

    th = 90;

    cout << "Select one point and press some key" << endl;
    waitKey(0);

    img.copyTo(imgL1);

    color = imgL1.at<Vec3b>(Point(pt.x, pt.y));

    Mat imgColor(100, 100, CV_8UC3, cv::Scalar(color.val[0], color.val[1], color.val[2]));
    imshow("Color", imgColor);

    for(int i = 0; i < imgL1.rows; i ++){
        for (int j = 0; j < imgL1.cols; j++){
            pixel = imgL1.at<Vec3b>(Point(j, i));
            if((color.val[0] + th > pixel.val[0]) && (color.val[0] - th < pixel.val[0])){
                if ((color.val[1] + th > pixel.val[1]) && (color.val[1] - th < pixel.val[1])){
                    if ((color.val[2] + th > pixel.val[2]) && (color.val[2] - th < pixel.val[2])){
                        imgL1.at<Vec3b>(Point(j, i)) = white;
                    }
                    else imgL1.at<Vec3b>(Point(j, i)) = black;
                }
                else imgL1.at<Vec3b>(Point(j, i)) = black;
            }
            else imgL1.at<Vec3b>(Point(j, i)) = black;
        }
    }

    cout << "L1-norm image" << endl;

    //imwrite("L1-norm.jpg", imgL1);
    namedWindow("L1-norm", WINDOW_NORMAL);
    imshow("L1-norm", imgL1);
}

void L2_norm(){
    int th;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Vec3b black(0, 0, 0);

    Mat imgL2;

    th = 120;

    cout << "Select one point and press some key" << endl;
    waitKey(0);

    img.copyTo(imgL2);

    color = imgL2.at<Vec3b>(Point(pt.x, pt.y));

    for(int i = 0; i < imgL2.rows; i ++){
        for (int j = 0; j < imgL2.cols; j++) {
            pixel = imgL2.at<Vec3b>(Point(j, i));

            float de = (pixel.val[0] - color.val[0]) * (pixel.val[0] - color.val[0]) +
                       (pixel.val[1] - color.val[1]) * (pixel.val[1] - color.val[1]) +
                       (pixel.val[2] - color.val[2]) * (pixel.val[2] - color.val[2]);

            if(sqrt(de) < th) imgL2.at<Vec3b>(Point(j, i)) = white;
            else imgL2.at<Vec3b>(Point(j, i)) = black;

        }
    }

    cout << "L2-norm image" << endl;

    //imwrite("L2-norm.jpg", imgL2);
    namedWindow("L2-norm", WINDOW_NORMAL);
    imshow("L2-norm", imgL2);

}

void mahalanobis() {

    Mat imgMh;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Vec3b black(0, 0, 0);
    double th = 3;

    Mat MatA(3, 3, CV_64F);
    Mat MatAInv(3, 3, CV_64F);

    double b_ = 0.0, g_ = 0.0, r_ = 0.0;

    img.copyTo(imgMh);
    cout << "Select the points and press some key" << endl;
    waitKey(0);

    /// Mahalanobis code here
    for (auto &point : points) {
        color = imgMh.at<Vec3b>(Point(static_cast<int>(point.x), static_cast<int>(point.y)));
        b_ += color.val[0];
        g_ += color.val[1];
        r_ += color.val[2];
    }

    b_ /= points.size();
    g_ /= points.size();
    r_ /= points.size();

    for (auto &point : points) {
        color = imgMh.at<Vec3b>(Point(static_cast<int>(point.x), static_cast<int>(point.y)));

        MatA.at<double>(2, 2) += (color.val[0] - b_) * (color.val[0] - b_);     // bb
        MatA.at<double>(1, 1) += (color.val[1] - g_) * (color.val[1] - g_);     // gg
        MatA.at<double>(0, 0) += (color.val[2] - r_) * (color.val[2] - r_);     // rr

        MatA.at<double>(1, 2) += (color.val[1] - g_) * (color.val[0] - b_);     // gb
        MatA.at<double>(0, 1) += (color.val[2] - r_) * (color.val[1] - g_);     // rg
        MatA.at<double>(0, 2) += (color.val[2] - r_) * (color.val[0] - b_);     // rb
    }


    MatA.at<double>(2, 2) /= (points.size() - 1);
    MatA.at<double>(1, 1) /= (points.size() - 1);
    MatA.at<double>(0, 0) /= (points.size() - 1);

    MatA.at<double>(1, 2) /= (points.size() - 1);
    MatA.at<double>(0, 1) /= (points.size() - 1);
    MatA.at<double>(0, 2) /= (points.size() - 1);

    MatA.at<double>(1, 0) = MatA.at<double>(0, 1);      // gr
    MatA.at<double>(2, 0) = MatA.at<double>(0, 2);      // br
    MatA.at<double>(2, 1) = MatA.at<double>(1, 2);      // bg

    MatAInv = MatA.inv();

    for (int i = 0; i < imgMh.rows; i++) {
        for (int j = 0; j < imgMh.cols; j++) {
            pixel = imgMh.at<Vec3b>(Point(j, i));

            double mt0 = (pixel.val[2] - r_) * MatAInv.at<double>(0, 0) +
                         (pixel.val[1] - g_) * MatAInv.at<double>(1, 0) +
                         (pixel.val[0] - b_) * MatAInv.at<double>(2, 0);

            double mt1 = (pixel.val[2] - r_) * MatAInv.at<double>(0, 1) +
                         (pixel.val[1] - g_) * MatAInv.at<double>(1, 1) +
                         (pixel.val[0] - b_) * MatAInv.at<double>(2, 1);

            double mt2 = (pixel.val[2] - r_) * MatAInv.at<double>(0, 2) +
                         (pixel.val[1] - g_) * MatAInv.at<double>(1, 2) +
                         (pixel.val[0] - b_) * MatAInv.at<double>(2, 2);

            double d = (mt0 * (pixel.val[2] - r_)) + (mt1 * (pixel.val[1] - g_)) + (mt2 * (pixel.val[0] - b_));

            if (sqrt(d) < th)
                imgMh.at<Vec3b>(Point(j, i)) = white;
            else imgMh.at<Vec3b>(Point(j, i)) = black;

        }
    }

    namedWindow("Mahalanobis", WINDOW_NORMAL);
    imshow("Mahalanobis", imgMh);

    waitKey(0);
}

void knn(){
    int th;
    Vec3b color, pixel;
    Vec3b white(255, 255, 255);
    Vec3b black(0, 0, 0);
    Mat imgKnn;

    th = 75;

    img.copyTo(imgKnn);

    cout << "Select one point and press some key" << endl;
    cout << "Press ESC to stop and return to menu" << endl;

    namedWindow("K-nearest neighbors", WINDOW_NORMAL);

    while(waitKey() != 27){

        color = imgKnn.at<Vec3b>(Point(pt.x, pt.y));

        while(color == white){
            cout << "Select another point and press some key" << endl;
            waitKey(0);
            color = imgKnn.at<Vec3b>(Point(pt.x, pt.y));
        }

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
                points.clear();
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


static void mouseCallback(int event, int x, int y, int, void* img_){
    Vec3b white(0, 255, 0);
    Mat &img = *(Mat*) img_;

    if(event == CV_EVENT_LBUTTONDOWN && (k == 49 || k == 50 || k == 52)){
        pt.x = x;
        pt.y = y;
        return;
    }

    if (event == CV_EVENT_LBUTTONDOWN)
        leftButtonDown = true;
    else if(event == CV_EVENT_MOUSEMOVE && leftButtonDown){
        img.at<Vec3b>(Point(x, y)) = white;
        points.emplace_back(x,y);
    }
    else if(event == CV_EVENT_LBUTTONUP)
        leftButtonDown = false;

    imshow(window, img);

}