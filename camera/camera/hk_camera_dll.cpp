#include "hk_camera_dll.h"
#include "camera.h"

namespace HK
{
	DLL_EXPORT void videoshow()
	{
		//---------------------------------------
		// ��ʼ��
		NET_DVR_Init();
		//��������ʱ��������ʱ��
		NET_DVR_SetConnectTime(2000, 1);
		NET_DVR_SetReconnect(10000, true);


		//---------------------------------------
		// ע���豸
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
		//�����쳣��Ϣ�ص�����
		NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

		//---------------------------------------
		//����Ԥ�������ûص�������
		LONG lRealPlayHandle;
		//cvNamedWindow("Mywindow", 0);
		cvNamedWindow("IPCamera", 0);

		/*HWND  h = (HWND)cvGetWindowHandle("Mywindow");
		if (h == 0)
		{
			std::cout << "���ڴ���ʧ��" << std::endl;
		}*/


		NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
		//struPlayInfo.hPlayWnd = h;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
		struPlayInfo.lChannel = 1;           //Ԥ��ͨ����
		struPlayInfo.dwStreamType = 0;       //0-��������1-��������2-����3��3-����4���Դ�����
		struPlayInfo.dwLinkMode = 0;         //0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP

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
		//�ر�Ԥ��
		NET_DVR_StopRealPlay(lRealPlayHandle);
		//ע���û�
		NET_DVR_Logout(lUserID);
		//�ͷ�SDK��Դ
		NET_DVR_Cleanup();

		return;
	}
}