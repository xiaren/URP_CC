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
	cout << "**QUT_URP 自动评价老师** " << endl;
	cout << "************************\n" << endl;


	if(GetImageAndCookie(URL,VALIDCODE_URL,IMAGE,COOKIE) <0)
	{
	cout << "获取失败" << endl;
	return 0;
	}

	cout << "\n请先记住验证码，关闭验证码窗口，再进行填写，谢谢 \n" << endl;

	ShowImage(IMAGE);
	cout << "请输入验证码:" << endl;
	cin >> vcode;
	cout << "请输入用户名:" << endl;
	cin >> user;
	cout << "请输入密码:" << endl;
	cin >> passwd;
	

	if (urp_login(main_curl, main_res, user, passwd, vcode) < 0)
	{
		cout << "login failure\nprogram exit\n";
		return -1;
	}
	cout << "自动评估教师ing.....\n\n";

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



