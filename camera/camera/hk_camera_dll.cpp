#include "hk_camera_dll.h"
#include "camera.h"

namespace HK
{
	DLL_EXPORT void videoshow()
	{
		//---------------------------------------
		// 初始化
		NET_DVR_Init();
		//设置连接时间与重连时间
		NET_DVR_SetConnectTime(2000, 1);
		NET_DVR_SetReconnect(10000, true);


		//---------------------------------------
		// 注册设备
		LONG lUserID;
		NET_DVR_DEVICEINFO_V30 struDeviceInfo;
		lUserID = NET_DVR_Login_V30("192.168.0.31", 8000, "admin", "12345", &struDeviceInfo);
		if (lUserID < 0)
		{
			printf("Login error, %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return;
		}

		//---------------------------------------
		//设置异常消息回调函数
		NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

		//---------------------------------------
		//启动预览并设置回调数据流
		LONG lRealPlayHandle;
		//cvNamedWindow("Mywindow", 0);
		cvNamedWindow("IPCamera", 0);

		/*HWND  h = (HWND)cvGetWindowHandle("Mywindow");
		if (h == 0)
		{
			std::cout << "窗口创建失败" << std::endl;
		}*/


		NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
		//struPlayInfo.hPlayWnd = h;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
		struPlayInfo.lChannel = 1;           //预览通道号
		struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
		struPlayInfo.dwLinkMode = 0;         //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

		lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, fRealDataCallBack, NULL);

		if (lRealPlayHandle < 0)
		{
			printf("NET_DVR_RealPlay_V40 error\n");
			printf("%d\n", NET_DVR_GetLastError());
			NET_DVR_Logout(lUserID);
			NET_DVR_Cleanup();
			return;
		}
		cv::waitKey();
		while (true)
		{

		}
		Sleep(-1);
		//---------------------------------------
		//关闭预览
		NET_DVR_StopRealPlay(lRealPlayHandle);
		//注销用户
		NET_DVR_Logout(lUserID);
		//释放SDK资源
		NET_DVR_Cleanup();

		return;
	}
}