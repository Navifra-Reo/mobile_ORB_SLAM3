#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

#define _WEBCAM_BUILD_


using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cerr << endl << "argc:" << argc << "!= 3"<< endl;
    }

    cv::VideoCapture cap(1);
    cv::VideoCapture cap_1(0);

    if (!cap.isOpened() || !cap_1.isOpened()) {
        cerr << endl << "Could not open camera feed." << endl;
        return -1;
    }
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);
    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;

#ifdef COMPILEDWITHC11
    std::chrono::steady_clock::time_point initT = std::chrono::steady_clock::now();
#else
    std::chrono::monotonic_clock::time_point initT = std::chrono::monotonic_clock::now();
#endif
    // int test = 0;
    // Main loop
    while(true)//cv::waitKey(0) != 27)
    {
        //Create a new Mat
        cv::Mat frame, frame_sub;
        //Send the captured frame to the new Mat
        cap >> frame;
        cap_1 >> frame_sub;
        
        if(frame.empty())
            break;
#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point nowT = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point nowT = std::chrono::monotonic_clock::now();
#endif
        // Pass the image to the SLAM system
        cout << "\n\ntrack cam1--------------------------------------------------------------------\n";
        SLAM.TrackMonocular(0, frame, std::chrono::duration_cast<std::chrono::duration<double> >(nowT-initT).count());
        cout << "track cam1--------------------------------------------------------------------\n\n\n\n";



        // cout << "\n\ntrack cam2--------------------------------------------------------------------\n";
        // SLAM.TrackMonocular(1, frame_sub, std::chrono::duration_cast<std::chrono::duration<double> >(nowT-initT).count());
        // cout << "track cam2--------------------------------------------------------------------\n\n\n\n";

    }
    // Stop all threads
    SLAM.Shutdown();

    //slam->SaveSeperateKeyFrameTrajectoryTUM("KeyFrameTrajectory-1.txt", "KeyFrameTrajectory-2.txt", "KeyFrameTrajectory-3.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    // skip first three lines
    string s0;
    getline(f,s0);
    getline(f,s0);
    getline(f,s0);

    while(!f.eof())
    {
        string s;
        getline(f,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}