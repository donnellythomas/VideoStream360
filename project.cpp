#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace cv;

static float *quaternion_mult(const float q[4], const float r[4]) {
    static float ret[4];
    // printf("before1 w: %f, X: %f, Y: %f, Z: %f\n", q[0], q[1], q[2], q[3]);
    float q0 = q[0];
    float q1 = q[1];
    float q2 = q[2];
    float q3 = q[3];
    float r0 = r[0];
    float r1 = r[1];
    float r2 = r[2];
    float r3 = r[3];
    ret[0] = r0 * q0 - r1 * q1 - r2 * q2 - r3 * q3;
    ret[1] = r0 * q1 + r1 * q0 - r2 * q3 + r3 * q2;
    ret[2] = r0 * q2 + r1 * q3 + r2 * q0 - r3 * q1;
    ret[3] = r0 * q3 - r1 * q2 + r2 * q1 + r3 * q0;
    return ret;
}

float *new_rotation(float u, float v, float *rotation, float inHeight,
                    float inWidth) {
    // Helpful resource for this function
    // https://github.com/DanielArnett/360-VJ/blob/d50b68d522190c726df44147c5301a7159bf6c86/ShaderMaker.cpp#L678
    float latitude = v * M_PI - M_PI / 2.0;
    float longitude = u * 2.0 * M_PI - M_PI;
    // Create a ray from the latitude and longitude
    float p[4];
    p[0] = 0;
    p[1] = cos(latitude) * sin(longitude);
    p[2] = sin(latitude);
    p[3] = cos(latitude) * cos(longitude);

    // Rotate the ray based on the user input
    float rotationInv[4] = {rotation[0], -rotation[1], -rotation[2],
                            -rotation[3]};
    float *p_ret = quaternion_mult(
        quaternion_mult((float *)rotation, (float *)p), (float *)rotationInv);

    float x = p_ret[1];
    float y = p_ret[2];
    float z = p_ret[3];
    // Convert back to latitude and longitude
    latitude = asin(y);
    longitude = atan2(x, z);
    // Convert back to the normalized M_PIxel coordinate
    x = (longitude + M_PI) / (2.0 * M_PI);
    y = (latitude + M_PI / 2.0) / M_PI;
    static float uv[2];
    // Convert to xy source M_PIxel coordinate
    uv[1] = y * inHeight;
    uv[0] = x * inWidth;

    return uv;
}

// createCubeMapFace implemented by https://stackoverflow.com/a/34720686

// Define our six cube faces.
// 0 - 3 are side faces, clockwise order
// 4 and 5 are top and bottom, respectively
float faceTransform[6][2] = {{0, 0},         {M_PI / 2, 0},  {M_PI, 0},
                             {-M_PI / 2, 0}, {0, -M_PI / 2}, {0, M_PI / 2}};


inline void splitFace(const Mat &full, Mat &top, Mat &mid, Mat &bott, const int width, const int height){
    top = full(Range(0, full.rows/4), Range(0,full.cols));
    resize(top, top, Size(), .5, .5, INTER_CUBIC); // upscale 2x

//    cv::normalize(leftROI, leftROI, 0, 1, cv::NORM_MINMAX);  //to view
    mid = full(Range(full.rows/4, 3*full.rows/4), Range(0,full.cols));
    bott = full(Range(3*full.rows/4, full.rows), Range(0,full.cols));
    resize(bott, bott, Size(), .5, .5, INTER_CUBIC); // upscale 2x

}

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
            float *uv;
            uv = new_rotation(u, v, (float *)rotation, inHeight - 1,
                              inWidth - 1);
            u = uv[0];
            v = uv[1];

            // Save the result for this pixel in map

            mapx.at<float>(x, y) = u;
            mapy.at<float>(x, y) = v;
            // exit(1);
        }
    }

    // // Recreate output image if it has wrong size or type.
    // if(face.cols != width || face.rows != height ||
    //     face.type() != in.type()) {
    //     face = Mat(width, height, in.type());
    // }

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

float *CreateFromYawPitchRoll(float yaw, float pitch,
                              float roll)  // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);

    float m_w = cy * cp * cr + sy * sp * sr;
    // printf("%f\n", m_w);
    float m_x = cy * cp * sr - sy * sp * cr;
    // printf("%f\n", m_x);

    float m_y = sy * cp * sr + cy * sp * cr;
    // printf("%f\n", m_y);

    float m_z = sy * cp * cr - cy * sp * sr;
    // printf("%f\n", m_z);

    static float rotation[4];
    rotation[0] = m_w;
    rotation[1] = m_x;
    rotation[2] = m_y;
    rotation[3] = m_z;
    return rotation;
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
                            { 2.5 * cubeSize, 0},
                            {2.5*cubeSize, cubeSize/2}};
   
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
                            { 2.5 * cubeSize, 0, cubeSize/2, cubeSize/2}, //top
                            {2.5*cubeSize, cubeSize/2, cubeSize/2, cubeSize/2}}; //bottom
    // coord order 
    // backMid,frontMid,leftMid,rightMid,backTop,backBott,frontTop,frontBott,leftTop,leftBott,rightTop,rightBott,top,bottom             
    float twoResCubeCoords[14][2] = { // x, y
                                    {3*cubeSize, cubeSize+cubeSize/4}, //back
                                    {cubeSize, cubeSize+cubeSize/4}, //front
                                    {0,cubeSize+cubeSize/4}, //left
                                    {2*cubeSize, cubeSize+cubeSize/4}, //right
                                    {3*cubeSize,cubeSize},//backtopp
                                    {3*cubeSize, cubeSize*1.75},//backbott
                                  {cubeSize,cubeSize},//frontop
                                    {cubeSize, cubeSize*1.75},//frontbott
                                    {0,cubeSize}, //lefttop
                                    {0,cubeSize*1.75}, //leftbott
                                    
                                      {2*cubeSize,cubeSize}, //righttop
                                    {2*cubeSize, cubeSize*1.75}, //rightbott
                                    {cubeSize,0},//top
                                    {cubeSize,cubeSize*1.99}//bottom
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
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: ./project <Input_Path> <Output_Path>\n");
        return -1;
    }

    Mat in = importImage(argv[1]);
    float roll = 0;
    float rotation[4] = {1, 0, 0, 0};
    int cubeSize = in.cols/4;
    Mat out2 = assembleCubemap(in, cubeSize, rotation);
    imwrite(argv[2], out2);
    Mat packedIn  = importImage(argv[2]);
    Mat repacked(cubeSize*3, cubeSize * 4, in.type());
    assemblePacked(packedIn, repacked, cubeSize);
    return 0;
}
