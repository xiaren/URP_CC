#include "init.h"
#include "regex.hpp"

#include <stdio.h>


CURL* main_curl = NULL;
CURLcode main_res;

int main()
{
	curl_init();/* Do noe delete it*/
	main_curl = curl_easy_init();
	/*-------------------------------Begin-------------------------------*/

	
	
	string user, passwd, vcode;
	string isprintText;


	cout << "************************" << endl;
	cout << "**QUT_URP �Զ�������ʦ** " << endl;
	cout << "************************\n" << endl;


	if(GetImageAndCookie(URL,VALIDCODE_URL,IMAGE,COOKIE) <0)
	{
	cout << "��ȡʧ��" << endl;
	return 0;
	}

	cout << "\n���ȼ�ס��֤�룬�ر���֤�봰�ڣ��ٽ�����д��лл \n" << endl;

	ShowImage(IMAGE);
	cout << "��������֤��:" << endl;
	cin >> vcode;
	cout << "�������û���:" << endl;
	cin >> user;
	cout << "����������:" << endl;
	cin >> passwd;
	

	if (urp_login(main_curl, main_res, user, passwd, vcode) < 0)
	{
		cout << "login failure\nprogram exit\n";
		return -1;
	}
	cout << "�Զ�������ʦing.....\n\n";

	CommentTeacher(main_curl, TEACHURE_COMMENT_1);
	
	cout << "\nCreated by zhw ~~~~~~\n"<<endl;
	system("pause");
	/*-------------------------------End-------------------------------*/
	curl_easy_cleanup(main_curl);
	curl_cleanup();/* Do noe delete it*/
	system("del GdeImageAndCookie.log");
	system("del validcode.jpg");
	system("del cookie.txt");

	return 0;
}



