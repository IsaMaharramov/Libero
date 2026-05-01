#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp> // The modern facial landmark module
#include <windows.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;
using namespace cv::face;

// Function to calculate Eye Aspect Ratio (EAR)
double calculateEAR(const vector<Point2f> &landmarks, int p1, int p2, int p3, int p4, int p5, int p6)
{
    // Vertical distances (distance between upper and lower eyelids)
    double v1 = norm(landmarks[p2] - landmarks[p6]);
    double v2 = norm(landmarks[p3] - landmarks[p5]);

    // Horizontal distance (distance between the corners of the eye)
    double h = norm(landmarks[p1] - landmarks[p4]);

    // Calculate and return the Eye Aspect Ratio
    return (v1 + v2) / (2.0 * h);
}

// Function to simulate mouse scrolling
void performScroll(int amount)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = amount;
    SendInput(1, &input, sizeof(INPUT));
}

int main()
{
    // 1. Load the Face Detector (Finds the general head area)
    CascadeClassifier faceDetector;
    if (!faceDetector.load("haarcascade_frontalface_default.xml"))
    {
        cout << "Error: Could not load haarcascade_frontalface_default.xml" << endl;
        return -1;
    }

    // 2. Load the Modern Landmark Detector (The 68-point 3D mesh AI)
    Ptr<Facemark> facemark = FacemarkLBF::create();
    try
    {
        facemark->loadModel("lbfmodel.yaml");
    }
    catch (const cv::Exception &e)
    {
        cout << "Error: Could not load lbfmodel.yaml. Did you download it?" << endl;
        return -1;
    }

    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cout << "Error: Could not open the webcam." << endl;
        return -1;
    }

    Mat frame, gray;

    // Timing and state variables
    auto eyesClosedStartTime = chrono::steady_clock::now();
    bool isTrackingClosed = false;

    // EAR Threshold: If EAR drops below this number, the eye is considered closed.
    // 0.20 is a standard baseline, adjust if it's too sensitive or not sensitive enough.
    const double EAR_THRESHOLD = 0.20;

    cout << "LiberoScroller 2.0: NEXT-GEN Landmark Tracker Online" << endl;
    cout << "Close your eyes for 1.5 seconds to start scrolling." << endl;

    while (cap.read(frame))
    {
        flip(frame, frame, 1); // Mirror image for natural feeling
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Rect> faces;
        // Detect faces
        faceDetector.detectMultiScale(gray, faces, 1.1, 5, 0, Size(150, 150));

        if (!faces.empty())
        {
            vector<vector<Point2f>> landmarks;
            // Fit the facial landmarks to the detected faces
            bool success = facemark->fit(gray, faces, landmarks);

            if (success)
            {
                // Get the landmarks for the first detected face (index 0)
                vector<Point2f> &points = landmarks[0];

                // Calculate EAR for Left Eye (OpenCV points 36-41)
                double leftEAR = calculateEAR(points, 36, 37, 38, 39, 40, 41);
                // Calculate EAR for Right Eye (OpenCV points 42-47)
                double rightEAR = calculateEAR(points, 42, 43, 44, 45, 46, 47);

                // Average the two ratios
                double averageEAR = (leftEAR + rightEAR) / 2.0;

                // --- SCROLLING LOGIC ---
                if (averageEAR < EAR_THRESHOLD)
                {
                    // EYES ARE CLOSED
                    if (!isTrackingClosed)
                    {
                        eyesClosedStartTime = chrono::steady_clock::now();
                        isTrackingClosed = true;
                    }

                    auto currentTime = chrono::steady_clock::now();
                    auto duration = chrono::duration_cast<chrono::milliseconds>(currentTime - eyesClosedStartTime).count();

                    // If held closed for 1.5 seconds (1500ms)
                    if (duration >= 1500)
                    {
                        performScroll(-20); // Smooth continuous scroll down
                        putText(frame, "SCROLLING DOWN", Point(10, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
                    }
                    else
                    {
                        // Show timer counting up
                        putText(frame, "HOLDING: " + to_string(duration) + "ms", Point(10, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2);
                    }
                }
                else
                {
                    // EYES ARE OPEN
                    isTrackingClosed = false;
                    putText(frame, "READING...", Point(10, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
                }

                // --- VISUALIZATION ---
                // Draw green dots on the eye landmarks so the user can verify tracking
                for (int i = 36; i <= 47; i++)
                {
                    circle(frame, points[i], 2, Scalar(0, 255, 0), -1);
                }
            }
        }

        imshow("LiberoScroller 2.0", frame);

        // Press 'ESC' to exit
        if (waitKey(5) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}