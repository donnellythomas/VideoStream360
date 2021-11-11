#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace cv;


// createCubeMapFace implemented by https://stackoverflow.com/a/34720686

// Define our six cube faces.
// 0 - 3 are side faces, clockwise order
// 4 and 5 are top and bottom, respectively


inline void splitFace(const Mat &full, Mat &top, Mat &mid, Mat &bott, const int width, const int height){
    top = full(Range(0, full.rows/4), Range(0,full.cols));
    resize(top, top, Size(), .5, .5, INTER_CUBIC); // upscale 2x

//    cv::normalize(leftROI, leftROI, 0, 1, cv::NORM_MINMAX);  //to view
    mid = full(Range(full.rows/4, 3*full.rows/4), Range(0,full.cols));
    bott = full(Range(3*full.rows/4, full.rows), Range(0,full.cols));
    resize(bott, bott, Size(), .5, .5, INTER_CUBIC); // upscale 2x

}
float faceTransform[6][2] = {{0, 0},         {M_PI / 2, 0},  {M_PI, 0},
                             {-M_PI / 2, 0}, {0, -M_PI / 2}, {0, M_PI / 2}};

// Map a part of the equirectangular panorama (in) to a cube face
// (face). The ID of the face is given by faceId. The desired
// width and height are given by width and height.
inline void createCubeMapFace(const Mat &in, Mat &face, float rotation[4],
                              int faceId = 0, const int width = -1,
                              const int height = -1) {
    float inWidth = in.cols;
    float inHeight = in.rows;

    // Allocate map
    Mat mapx(height, width, CV_32F);
    Mat mapy(height, width, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    // to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    float ftu = faceTransform[faceId][0];
    float ftv = faceTransform[faceId][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Map face pixel coordinates to [-1, 1] on plane
            float nx = ((float)(y) / (float)height - 0.5f);
            float ny = ((float)(x) / (float)width - 0.5f);

            // printf("test");
            nx *= 2;
            ny *= 2;
            // printf("nx: %f, ny %f\n", nx, ny);

            // Map [-1, 1] plane coords to [-an, an]
            // thats the coordinates in respect to a unit sphere
            // that contains our box.
            nx *= an;
            ny *= an;
            // printf("Mapped to unit circle\n");
            // printf("nx: %f, ny %f\n", nx, ny);

            float u, v;

            // Project from plane to sphere surface.
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

            // Map from angular coordinates to [-1, 1], respectively.

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

            // Map from [-1, 1] to in texture space
            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            // once in texture space replace current point with point after
            // rotation
            // float *uv;
            // uv = new_rotation(u, v, (float *)rotation, inHeight - 1,
            //                   inWidth - 1);
            // u = uv[0];
            // v = uv[1];
            u = u * (inWidth - 1);
            v = v * (inHeight - 1);

            // Save the result for this pixel in map

            mapx.at<float>(x, y) = u;
            mapy.at<float>(x, y) = v;
            // exit(1);
        }
    }

    // Do actual resampling using OpenCV's remap
    remap(in, face, mapx, mapy, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));
}

Mat importImage(String path) {
    Mat image = imread(path, CV_32F);
    if (!image.data) {
        printf("No image data \n");
    }
    return image;
}


Mat assembleCubemap(Mat &in, float cubeSize, float rotation[4]) {
    Mat left, leftTop, leftMid, leftBott; 
    Mat front, frontTop, frontMid, frontBott;
    Mat right, rightTop, rightMid, rightBott;
    Mat back, backTop, backMid, backBott;
    Mat top, bottom;

    // Generate each face of the cubemap
    createCubeMapFace(in, front, rotation, 0, cubeSize, cubeSize);
    splitFace(front, frontTop, frontMid, frontBott, cubeSize, cubeSize);

    createCubeMapFace(in, right, rotation, 1, cubeSize, cubeSize);
    splitFace(right, rightTop, rightMid, rightBott, cubeSize, cubeSize);

    createCubeMapFace(in, back, rotation, 2, cubeSize, cubeSize);
    splitFace(back, backTop, backMid, backBott, cubeSize, cubeSize);

    createCubeMapFace(in, left, rotation, 3, cubeSize, cubeSize);
    splitFace(left, leftTop, leftMid, leftBott, cubeSize, cubeSize);

    createCubeMapFace(in, top, rotation, 4, cubeSize, cubeSize);
    createCubeMapFace(in, bottom, rotation, 5, cubeSize, cubeSize);

    //resize top and bottom areas
    resize(top, top, Size(), .5, .5, INTER_CUBIC); // downscale 2x
    resize(bottom, bottom, Size(), .5, .5, INTER_CUBIC); // downscale 2x
    
    Mat img(cubeSize, cubeSize * 3, front.type());
    Mat faces[14] = {backMid,frontMid,leftMid,rightMid,backTop,backBott,frontTop,frontBott,leftTop,leftBott,rightTop,rightBott,top,bottom};
    // organize different faces into cubemap
    // can probably fix rotation within createCubeMapFace
    rotate(top, top, ROTATE_90_CLOCKWISE);
    rotate(bottom, bottom, ROTATE_90_COUNTERCLOCKWISE);
    float faceCoords[14][2] = {
                            {0,0},{cubeSize,0},
                            {0,cubeSize/2},
                            {cubeSize, cubeSize/2},
                            {2*cubeSize, 0},
                            {2*cubeSize, cubeSize/8},
                            {2*cubeSize, 2* cubeSize/8},
                            {2*cubeSize, 3* cubeSize/8},
                            {2*cubeSize, 4* cubeSize/8},
                            {2*cubeSize, 5* cubeSize/8},
                            {2*cubeSize, 6* cubeSize/8},
                            {2*cubeSize, 7* cubeSize/8},
                            { 2.5f * cubeSize, 0},
                            {2.5f *cubeSize, cubeSize/2}};
   
    // TODO: Use pointers and see if its faster 
    for(int i = 0; i < 14; i++){
        faces[i].copyTo(img(Rect(faceCoords[i][0],  faceCoords[i][1],faces[i].cols, faces[i].rows)));
    }

    return img;
}
void assemblePacked(Mat &in, Mat &out, float cubeSize){
    //back
    float twoResCoords[14][4] = { //x, y, width, height
                            {0,0,cubeSize,cubeSize/2}, //back
                            {cubeSize,0,cubeSize,cubeSize/2}, //front
                            {0,cubeSize/2,cubeSize,cubeSize/2}, //left
                            {cubeSize, cubeSize/2,cubeSize,cubeSize/2}, //right
                            {2*cubeSize, 0, cubeSize/2, cubeSize/8}, //backTop
                            {2*cubeSize, cubeSize/8, cubeSize/2, cubeSize/8}, //backbott
                            {2*cubeSize, 2* cubeSize/8,cubeSize/2, cubeSize/8}, //front top
                            {2*cubeSize, 3* cubeSize/8,cubeSize/2, cubeSize/8}, //front bott
                            {2*cubeSize, 4* cubeSize/8,cubeSize/2, cubeSize/8}, //left top
                            {2*cubeSize, 5* cubeSize/8,cubeSize/2, cubeSize/8}, //left bott
                            {2*cubeSize, 6* cubeSize/8,cubeSize/2, cubeSize/8}, //right top
                            {2*cubeSize, 7* cubeSize/8,cubeSize/2, cubeSize/8}, //right bott
                            { 2.5f * cubeSize, 0, cubeSize/2, cubeSize/2}, //top
                            {2.5f*cubeSize, cubeSize/2, cubeSize/2, cubeSize/2}}; //bottom
    // coord order 
    // backMid,frontMid,leftMid,rightMid,backTop,backBott,frontTop,frontBott,leftTop,leftBott,rightTop,rightBott,top,bottom             
    float twoResCubeCoords[14][2] = { // x, y
                                    {3*cubeSize, cubeSize+cubeSize/4}, //back
                                    {cubeSize, cubeSize+cubeSize/4}, //front
                                    {0,cubeSize+cubeSize/4}, //left
                                    {2*cubeSize, cubeSize+cubeSize/4}, //right
                                    {3*cubeSize,cubeSize},//backtopp
                                    {3*cubeSize, cubeSize*1.75f},//backbott
                                  {cubeSize,cubeSize},//frontop
                                    {cubeSize, cubeSize*1.75f},//frontbott
                                    {0,cubeSize}, //lefttop
                                    {0,cubeSize*1.75f}, //leftbott
                                    
                                      {2*cubeSize,cubeSize}, //righttop
                                    {2*cubeSize, cubeSize*1.75f}, //rightbott
                                    {cubeSize,0},//top
                                    {cubeSize,cubeSize*1.99f}//bottom
                                    };
    Mat face;
    for(int i = 0; i < 14; i++){
        face = in(Range(twoResCoords[i][1],twoResCoords[i][1]+twoResCoords[i][3]),Range(twoResCoords[i][0],twoResCoords[i][0]+twoResCoords[i][2]));
        if(i > 3){
            resize(face, face, Size(), 2, 2, INTER_CUBIC);
        }
        face.copyTo(out(Rect(twoResCubeCoords[i][0],  twoResCubeCoords[i][1],face.cols, face.rows)));
        // imshow("Face:"+i, out);
        // waitKey(0);
    }
}
void front(Mat &in, Mat &top, Mat &mid, Mat &bott, int cubeSize){
    // imshow("Ref", ref);
    // imshow("in", in);
    // waitKey(0);
    printf("IN: %d X %d\n", in.cols, in.rows);

    printf("TOP: %d X %d\n", top.cols, top.rows);
    printf("MID: %d X %d\n", mid.cols, mid.rows);
    printf("BOTT: %d X %d\n", bott.cols, bott.rows);
    float inWidth = in.cols;
    float inHeight = in.rows;

    int height = cubeSize;
    int width = cubeSize;
   
    // Allocate map
    Mat tx(top.rows, top.cols, CV_32F);
    Mat ty(top.rows, top.cols, CV_32F);
    
    Mat mx(mid.rows, mid.cols, CV_32F);
    Mat my(mid.rows, mid.cols, CV_32F);

    Mat bx(bott.rows, bott.cols, CV_32F);
    Mat by(bott.rows, bott.cols, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    // to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    float ftu = faceTransform[0][0];
    float ftv = faceTransform[0][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Map face pixel coordinates to [-1, 1] on plane
            float nx = ((float)(y) / (float)height - 0.5f);
            float ny = ((float)(x) / (float)width - 0.5f);

            // printf("test");
            nx *= 2;
            ny *= 2;
            // printf("nx: %f, ny %f\n", nx, ny);

            // Map [-1, 1] plane coords to [-an, an]
            // thats the coordinates in respect to a unit sphere
            // that contains our box.
            nx *= an;
            ny *= an;
            // printf("Mapped to unit circle\n");
            // printf("nx: %f, ny %f\n", nx, ny);

            float u, v;

             // Project from plane to sphere surface.
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

            // Map from angular coordinates to [-1, 1], respectively.

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

            // Map from [-1, 1] to in texture space
            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            // once in texture space replace current point with point after
            // rotation
            // float *uv;
            // uv = new_rotation(u, v, (float *)rotation, inHeight - 1,
            //                   inWidth - 1);
            // u = uv[0];
            // v = uv[1];
            u = u * (inWidth - 1);
            v = v * (inHeight - 1);
            int x2,x3;
            // Save the result for this pixel in map
            if(x >=0 && x < cubeSize/4){
                if(x % 2 == 0){
                // circle(in, Point(u,v), 2,Scalar(255,0,0),-1, 8,0);
                tx.at<float>(x, y) = u;
                ty.at<float>(x, y) = v;
                }
            }
            else if(x >= cubeSize/4 &&x < 3*cubeSize/4){
                // circle(in, Point(u,v), 2,Scalar(0,255,0),-1, 8,0);
                x2 = x - cubeSize/4 + 1;
                mx.at<float>(x2, y) = u;
                my.at<float>(x2, y) = v;
            }
            else if(x >= 3*cubeSize/4 && x < cubeSize-1){
                if(x % 2 == 0){
                // circle(in, Point(u,v), 2,Scalar(0,0,255),-1, 8,0);
                x3 = x - 3*cubeSize/4 + 1;
                bx.at<float>(x3, y) = u;
                by.at<float>(x3, y) = v;}
            }
           
            
            // exit(1);
        }
    }

    // Do actual resampling using OpenCV's remap]
    // TODO: Figure out remap function
    remap(in, top, tx, ty, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));
    remap(in, mid, mx, my, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));
    remap(in, bott, bx, by, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));
   
} 
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: ./project <Input_Path> <Output_Path>\n");
        return -1;
    }

    // Mat in = importImage(argv[1]);
    // float roll = 0;
    // float rotation[4] = {1, 0, 0, 0};
    // int cubeSize = in.cols/4;
    // Mat out2 = assembleCubemap(in, cubeSize, rotation);
    // imwrite(argv[2], out2);
    // Mat packedIn  = importImage(argv[2]);
    // Mat repacked(cubeSize*3, cubeSize * 4, in.type());
    // assemblePacked(packedIn, repacked, cubeSize);

    Mat in = importImage(argv[1]);
    Mat ref = importImage(argv[2]);
    printf("X:%d, Y:%d\n", in.cols, in.rows);
    printf("X:%d, Y:%d\n", ref.cols, ref.rows);
    resize(ref, ref, Size(), .2, .2, INTER_CUBIC);
    int cubeSize = in.cols/4;
    printf("Type: %d\n",in.type());
    Mat top(cubeSize/8,cubeSize/2, in.type());
    Mat mid(cubeSize/2,cubeSize, in.type());
    Mat bott(cubeSize/8 + 1,cubeSize/2, in.type());

    front(in,top,mid,bott,cubeSize);
    imshow("top",top);
    imshow("mid",mid);
    imshow("bott", bott);
    waitKey(0);
    return 0;
}
