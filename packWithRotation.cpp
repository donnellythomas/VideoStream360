#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <chrono>
using namespace std::chrono;
using namespace std;
using namespace cv;

const float CUBESIZE = 16*20;
Mat importImage(String path) {
    Mat image = imread(path, CV_32F);
    if (!image.data) {
        printf("No image data \n");
    }
    return image;
}
// float faceTransform[6][2] = {{0, 0},         {M_PI / 2, 0},  {M_PI, 0},
//                              {-M_PI / 2, 0}, {0, -M_PI / 2}, {0, M_PI / 2}}; // front right back left top bottom
float faceTransform[6][2] = {{-M_PI / 2, 0},{0, 0},{M_PI / 2, 0},{M_PI, 0}, {0, -M_PI / 2}, {0, M_PI / 2}}; // left front right back top bottom
float packedCoords[14][2] = {
        {CUBESIZE*4,0},             //leftTop
        {0,0},                      //left                   
        {CUBESIZE*4,CUBESIZE/16},   //leftBottom
        {CUBESIZE*4,CUBESIZE/16*2}, //frontTop
        {CUBESIZE,0},               //front
        {CUBESIZE*4,CUBESIZE/16*3}, //frontBot
        {CUBESIZE*4,CUBESIZE/16*4}, //rightTop
        {CUBESIZE*2,0},             //right
        {CUBESIZE*4,CUBESIZE/16*5}, //rightBott
        {CUBESIZE*4,CUBESIZE/16*6}, //backTop
        {CUBESIZE*3,0},             //back
        {CUBESIZE*4,CUBESIZE/16*7}, //backBott
        {CUBESIZE*4+CUBESIZE/4,0},  //top
        {CUBESIZE*4+CUBESIZE/4,CUBESIZE/4}}; //bott
    float cubeCoords[14][2] = {
        {CUBESIZE*3,CUBESIZE},          //leftTop
        {CUBESIZE*3, CUBESIZE*1.25f},   //left
        {CUBESIZE*3,CUBESIZE*1.75f},    //leftBottom
        {0,CUBESIZE},                   //frontTop
        {0,CUBESIZE*1.25f},             //front
        {0,CUBESIZE*1.75f},             //frontBottom
        {CUBESIZE,CUBESIZE},            //rightTop
        {CUBESIZE, CUBESIZE*1.25f},     //right
        {CUBESIZE,CUBESIZE*1.75f},      //rightBottom
        {CUBESIZE*2,CUBESIZE},          //backTop
        {CUBESIZE*2, CUBESIZE*1.25f},   //back
        {CUBESIZE*2,CUBESIZE*1.75f},    //backBottom
        {CUBESIZE,0},                   //top
        {CUBESIZE,CUBESIZE*2} };        //bottom
inline void computeFaceMap(Mat &in, Mat &face, int faceID, int faceType){
    
    
    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    // to our cube face.
    float inWidth = in.cols;
    float inHeight = in.rows;
    float width = face.cols;
    float height = face.rows;
    Mat mx(height,width,CV_32F);
    Mat my(height,width,CV_32F);
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);
    float ftu = faceTransform[faceID][0];
    float ftv = faceTransform[faceID][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    int start = 0;
    int end = height;
    if(faceType == 2){ //MID
        start = CUBESIZE/4;
        end = height + CUBESIZE/4;
    }
    if(faceType == 3){ //bott
        start = width*0.75;
        end = height + width*0.75;
    }

    for (int y = 0; y < width; y++) {
        for (int x = start; x < end; x++) {
            // Map face pixel coordinates to [-1, 1] on plane
            float nx = ((float)(y) / width - 0.5f);
            float ny = ((float)(x) / width - 0.5f);

            // printf("test");
            nx *= 2;
            ny *= 2;
            nx *= an;
            ny *= an;

            float u, v;

            if (ftv == 0) {
                // Center faces
                u = atan2(nx, ak);
                v = atan2(ny * cos(u), ak);
                u += ftu;
            } else if (ftv > 0) {
                // Bottom face
                float d = sqrt(nx * nx + ny * ny);
                v = M_PI / 2 - atan2(d, ak);
                u = atan2(ny, nx);
            } else {
                // Top face
                float d = sqrt(nx * nx + ny * ny);
                v = -M_PI / 2 + atan2(d, ak);
                u = atan2(-ny, nx);
            }

            u = u / (M_PI);
            v = v / (M_PI / 2);

            // Warp around, if our coordinates are out of bounds.
            while (v < -1) {
                v += 2;
                u += 1;
            }
            while (v > 1) {
                v -= 2;
                u += 1;
            }

            while (u < -1) {
                u += 2;
            }
            while (u > 1) {
                u -= 2;
            }

            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            u = u * (inWidth - 1);
            v = v * (inHeight - 1);
            int x2,x3;
            mx.at<float>(x-start, y) = u;
            my.at<float>(x-start, y) = v;
        }
    }
   
    remap(in, face, mx, my, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));

} 
void unpackAssembleCubemap(Mat &unpacked, Mat &packed){
    Mat face;
    
    for(int i = 0; i < 12; i++){
        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/16),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
        resize(face, face, Size(),4, 4, INTER_CUBIC); //upscale 4
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
        i++;

        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/2),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE));
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
        i++;

        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/16),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
        resize(face, face, Size(),4, 4, INTER_CUBIC); //upscale 4
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
    }

   
    int i = 12;
    Mat top = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/4),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
    resize(top, top, Size(),4, 4, INTER_CUBIC); //upscale 4
    rotate(top, top, ROTATE_90_CLOCKWISE);
    top.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],top.cols, top.rows)));
    
    i = 13;
    Mat bottom = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/4),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
    resize(bottom, bottom, Size(),4, 4, INTER_CUBIC); //upscale 4
    
    rotate(bottom, bottom, ROTATE_90_COUNTERCLOCKWISE);
    bottom.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],bottom.cols, bottom.rows)));
}
Mat unpack(Mat packed){
    Mat unpacked(CUBESIZE * 3, CUBESIZE * 4, 16);
    unpackAssembleCubemap(unpacked,packed);
    return unpacked;
}
Mat pack(Mat &in){
    Mat packedFaces[14];
    int j = 0;
    auto start = high_resolution_clock::now();
    for(int i = 0;i<12;i++){
        packedFaces[i] = Mat(CUBESIZE/16, CUBESIZE/4,in.type());
        computeFaceMap(in, packedFaces[i], j,0);
        i++;

        packedFaces[i] = Mat(CUBESIZE/2, CUBESIZE, in.type());
        computeFaceMap(in, packedFaces[i], j,2);
        i++;

        packedFaces[i] = Mat(CUBESIZE/16, CUBESIZE/4,in.type());
        computeFaceMap(in, packedFaces[i], j,3);
        j++;
    }
    packedFaces[12] = Mat(CUBESIZE/4, CUBESIZE/4,in.type());
    computeFaceMap(in, packedFaces[12], 4,0);
    packedFaces[13] = Mat(CUBESIZE/4, CUBESIZE/4,in.type());
    computeFaceMap(in, packedFaces[13], 5,0);
    Mat packed(CUBESIZE/2, CUBESIZE*4.5, in.type());
    for(int i = 0; i <14; i++){
        packedFaces[i].copyTo(packed(Rect(packedCoords[i][0],  packedCoords[i][1],packedFaces[i].cols, packedFaces[i].rows)));
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << duration.count() << endl;

    return packed;
}
void runImage(char*path){
    Mat in = importImage(path);
    int height = in.rows;
    int width = in.cols;
    Mat packed = pack(in);
    imshow("packed", packed);
    Mat unpacked = unpack(packed);
    imshow("unpacked", unpacked);
    waitKey(0);
}
void runVideo(char* path){
    VideoCapture cap(path);
    if(!cap.isOpened()){
        printf("Error opening video\n");
    }
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);;
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int i = 0;
    VideoWriter video("videotestsrc ! openh264enc ! rtph264pay config-interval=10 pt=96 ! udpsink host=127.0.0.1 port=5000",CAP_GSTREAMER,0, 20, Size(1440,160), true);
    if(!video.isOpened()) {
        printf("Writer not opened\n");
        exit(0);
    }
    //play with vlc with vlc stream.sdp
    // VideoWriter video("hvc1.avi",VideoWriter::fourcc('H','V','C','1'),30, Size(1440,160));
    while(1){
        Mat in;
        cap >> in;
        if(i%2==0){
            if(in.empty()) break;
            Mat packed = pack(in);

            video.write(packed);
                        printf("height: %d, width: %d \n", packed.rows, packed.cols);

            imshow("packed", packed);
        }
        char c=(char)waitKey(25);
        if(c==27) break;
        i++;
    }
    cap.release();
    video.release();
    destroyAllWindows();
}
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: ./project <type> <Input_Path> \n");
        return -1;
    }
    cout<<getBuildInformation()<<endl;
    // runImage(argv[1]);
    runVideo(argv[1]);
   
    return 0;
}

// TODO
// include interface for rotation
// sample from downscaled in
// reimplement video
// streaming
    // Figure out bitrate