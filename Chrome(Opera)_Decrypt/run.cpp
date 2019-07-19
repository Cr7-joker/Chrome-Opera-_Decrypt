#include "Chrome(Opera)_Decrypt.h"
#include <Wincrypt.h>
#include "sqlite3.h"
#pragma comment(lib,"crypt32.lib")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	sqlite3* DB = NULL; // һ���򿪵����ݿ�ʵ��
	sqlite3_stmt* stmt = NULL;// sqlite3_stmt Ԥ����������. ��sqlite3_prepare()��������sqlite3_finalize()����
	string LoginDataPath = getProfilePath() + "Login Data";
	const char* path = LoginDataPath.c_str();	// Login Data sql�ļ���·��
	char** dbResult; // �� char ** ���ͣ�����*��
	int nRow, nColumn;// �������������
	int index;
	DATA_BLOB DataOut;
	DATA_BLOB Dataput;
	LPWSTR pbtest = NULL;

	// �����ļ�·�������ݿ����ӡ�������ݿⲻ���ڣ��򴴽���
	// ���ݿ��ļ���·��������C�ַ������롣
	int result = sqlite3_open(path, &DB);

	if (result == SQLITE_OK) {
		std::clog << "�����ݿ����ӳɹ�\n";
		//��ʼ��ѯ������� dbResult �Ѿ��� char **�������ּ���һ�� & ȡ��ַ�������ݽ�ȥ�ľͳ���char ***
		const char* sql = "Select * from logins";
		int result1 = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
		int result2 = sqlite3_get_table(DB, "Select * from logins", &dbResult, &nRow, &nColumn, NULL);
		if (result1 == SQLITE_OK && result2 == SQLITE_OK)
		{
			//��ѯ�ɹ�
			printf("���%d ��¼!\n", nRow);
			printf("���%d ��!\n", nColumn);
			printf("�ֶ���|�ֶ�ֵ\n");
			index = nColumn;
			for (int i = 0; i < nRow; i++)
			{
				printf("�� %d ����¼\n", i);
				for (int j = 0; j < nColumn; j++)
				{
					if (!(strcmp(dbResult[j], "password_value")))//�������Ϊpassword_value������Ҫ�Ը��е���ֵ���н��ܲ���
					{
						if (sqlite3_step(stmt) == SQLITE_ROW) {
							//��ʼ�����ܽṹDataOut
							BYTE* pbDataInput = (BYTE*)sqlite3_column_text(stmt, j);
							DWORD cbDataInput = sqlite3_column_bytes(stmt, j);
							DataOut.pbData = pbDataInput;
							DataOut.cbData = cbDataInput;
						}
						if (CryptUnprotectData(&DataOut, &pbtest, NULL, NULL, NULL, 0, &Dataput))
						{
							string s = Password_Handle((char*)Dataput.pbData);//���ܺ�����봦��
							strcpy(dbResult[index], (char*)s.c_str());
						}
						else
						{
							printf("Decryption error!\n");
						}
					}
					printf("�ֶ���: %s | �ֶ�ֵ: %s\n", dbResult[j], U2G(dbResult[index]));
					index++;
					// dbResult ���ֶ�ֵ�������ģ��ӵ�0�������� nColumn - 1���������ֶ����ƣ��ӵ� nColumn ������ʼ�����涼���ֶ�ֵ������һ����ά�ı���ͳ�����б�ʾ������һ����ƽ����ʽ����ʾ
				}
				printf("--------------------------------------------------------------------------------\n");
			}
		}
		//�����������׼��ִ����һ�����
		sqlite3_finalize(stmt);
		//������������ݿ��ѯ�Ƿ�ɹ������ͷ� char** ��ѯ���dbResult��ʹ�� sqlite �ṩ�Ĺ������ͷ�
		//sqlite3_free_table(dbResult); //ִ�л��׳��ϵ㣬��Ϊ�����й�����dbResult�ĵ�ַ�Ѹı䣬����������������
		//�ر����ݿ�
		sqlite3_close(DB);
	}
	else {
		std::clog << "�����ݿ�����ʧ��\n";
	}
	return 0;
}