#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"

#include <QApplication>
#include <QThread>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "RBOT/object3d.h"
#include "RBOT/pose_estimator6d.h"
//#include "RenderAPI/RenderAPI.h"
//#include "RenderAPI/RenderingPlugin.cpp"

#include <assert.h>
#include <math.h>
#include <vector>

typedef int* RenderAPI;

int frameN = -1;
static IUnityInterfaces* s_UnityInterfaces = nullptr;
static IUnityGraphics* s_Graphics = nullptr;
static RenderAPI* s_CurrentAPI = nullptr;
static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;
std::vector<Object3D*> objects;
PoseEstimator6D* poseEstimator = nullptr;

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType){
	if (eventType == kUnityGfxDeviceEventInitialize){
		assert(s_CurrentAPI == NULL);
		s_DeviceType = s_Graphics->GetRenderer();
		//s_CurrentAPI = CreateRenderAPI(s_DeviceType);
		
		frameN = 0;
	
		// Initialize RBOT
		// QApplication?
	
		// camera image size !!!!!!!!!!
		int width = 640;
		int height = 512;   
		// near and far plane of the OpenGL view frustum
		float zNear = 10.0;
		float zFar = 10000.0;  	
		// camera instrinsics !!!!!!!!!!!!!
		cv::Matx33f K = cv::Matx33f(650.048, 0, 324.328, 0, 647.183, 257.323, 0, 0, 1);
		cv::Matx14f distCoeffs =  cv::Matx14f(0.0, 0.0, 0.0, 0.0);
	
		//Distance?
		// distances for the pose detection template generation
		std::vector<float> distances = {200.0f, 400.0f, 600.0f};
	
		//Create 3d object
		objects.push_back(new Object3D("../data/squirrel_demo_low.obj", 15, -35, 515, 55, -20, 205, 1.0, 0.55f, distances));
		//Create pose estimator	
		poseEstimator = new PoseEstimator6D(width, height, zNear, zFar, K, distCoeffs, objects);
	
		// Thread stuff and OpenGL context?
		// move the OpenGL context for offscreen rendering to the current thread, if run in a seperate QT worker thread (unnessary in this example)
		//RenderingEngine::Instance()->getContext()->moveToThread(this);   
		// active the OpenGL context for the offscreen rendering engine during pose estimation
		//RenderingEngine::Instance()->makeCurrent();
	}


	/*
	extern RenderAPI* CreateRenderAPI_OpenGLCoreES(UnityGfxRenderer apiType);
	s_CurrentAPI = CreateRenderAPI_OpenGLCoreES(apiType);
	*/
	// Let the implementation process the device related events
	if (s_CurrentAPI)
	{
		//s_CurrentAPI->ProcessDeviceEvent(eventType, s_UnityInterfaces);
	}
	
	// Cleanup graphics API implementation upon shutdown
	if (eventType == kUnityGfxDeviceEventShutdown)
	{
		frameN = -1;
		delete s_CurrentAPI;
		s_CurrentAPI = NULL;
		s_DeviceType = kUnityGfxRendererNull;
	}
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces){
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = unityInterfaces->Get<IUnityGraphics>();
	
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload(){
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
	}
	
//static RenderAPI* s_CurrentAPI = NULL;
//static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;

/*static cv::Mat drawResultOverlay(const std::vector<Object3D*>& objects, const cv::Mat& frame){
    // render the models with phong shading
    RenderingEngine::Instance()->setLevel(0);
    
    std::vector<cv::Point3f> colors;
    colors.push_back(cv::Point3f(1.0, 0.5, 0.0));
    //colors.push_back(Point3f(0.2, 0.3, 1.0));
    RenderingEngine::Instance()->renderShaded(std::vector<Model*>(objects.begin(), objects.end()), GL_FILL, colors, true);
    
    // download the rendering to the CPU
    cv::Mat rendering = RenderingEngine::Instance()->downloadFrame(RenderingEngine::RGB);
    
    // download the depth buffer to the CPU
    cv::Mat depth = RenderingEngine::Instance()->downloadFrame(RenderingEngine::DEPTH);
    
    // compose the rendering with the current camera image for demo purposes (can be done more efficiently directly in OpenGL)
    cv::Mat result = frame.clone();
    for(int y = 0; y < frame.rows; y++)
    {
        for(int x = 0; x < frame.cols; x++)
        {
            cv::Vec3b color = rendering.at<cv::Vec3b>(y,x);
            if(depth.at<float>(y,x) != 0.0f)
            {
                result.at<cv::Vec3b>(y,x)[0] = color[2];
                result.at<cv::Vec3b>(y,x)[1] = color[1];
                result.at<cv::Vec3b>(y,x)[2] = color[0];
            }
        }
    }
    return result;
}*/
	
static void UNITY_INTERFACE_API OnRenderEvent(int eventID){
		//frameN==...;
		/*std::string s1 = "data/squirrel/frames/a_regular0";
		if (frameN/10 >= 1){
			s1 = s1 + "0";
			if(frameN/100 >= 1){
				s1 = s1 + "0";
			}
		}
		std::string s = s1 + std::to_string(frameN) + ".png";
		cv::Mat frame = cv::imread(s);*/
		//frame = imread("data/frame.png");
		//poseEstimator->estimatePoses(frame, false, true);
		//cv::Mat result = drawResultOverlay(&objects, &frame);
	}
	
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc(){
		return OnRenderEvent;
	}