#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <regex>    /*正则表达式*/
#include <vector>
#include <windows.h>


#include "curl.h"
#include "multi.h"
#include "highgui.h"

#pragma comment(lib,"libcurl.lib")

#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")

/*-----------------------    包含依赖    -----------------------*/

#define _Global_ 
#define IMAGE "validcode.jpg"
#define COOKIE "cookie.txt"

/* 问一下别的ip,做一下ping 找个最小的登陆 */
#define LOGIN_PATH		"http://222.195.242.240:8080/loginAction.do"
#define REFERER			"http://222.195.242.240:8080/loginAction.do"
#define VALIDCODE_URL   "http://222.195.242.240:8080/validateCodeAction.do?random=0.07051449670442017"
#define URL             "http://222.195.242.240:8080/" 
#define COMMENT_URL		"http://222.195.242.240:8080/jxpgXsAction.do?oper=listWj"
#define EVERY_TEACHER_COMMENT_URL_POST "http://222.195.242.240:8080/jxpgXsAction.do"
#define FINAL_COMMENT_POST_URL "http://222.195.242.240:8080/jxpgXsAction.do?oper=wjpg"


#define TEACHURE_COMMENT_1 "课堂内容充实，简单明了，使学生能够轻轻松松掌握知识。"
#define TEACHURE_COMMENT_2 "教学内容丰富有效，教学过程中尊重学生，有时还有些幽默，很受大家欢迎。"
#define TEACHURE_COMMENT_3 "老师上课诙谐有趣，善于用凝练的语言将复杂难于理解的过程公式清晰的表达出来。"

#define USER_AGENT      "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)"

/* 正则表达式部分 */



using namespace std;
/*   ----------------------申请变量----------------------------*/




/*             ---------------------------- 结构体，类变量---------------------------        */
/*
	func_name : curl_init
	func_param: NULL
	func_describe : init the global curl environment!!!! ,when you use any curl func,this func must be exec before!!! 

*/
int curl_init(_Global_);


/*
func_name : curl_cleanup
func_param: NULL
func_describe : cleanup the global curl environment!!!! ,this func must not be exec , before you want to exit curl environment

*/
void curl_cleanup(_Global_);

/*
func_name : write_data
func_param: 
		@param1: ptr ,a pointer to save the data
		@param2: size  @param3: nmemb    the size 
		@param4: stream   ,the data stream
func_describe : defined as the offical recommend form for the <curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)file);>
*/
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

/*
func_name : GetImageAndCookie
func_param: 
		@param1  url  ,the url_image_path you wang to download
		@param2  vcodeurl ,the validcode path 
		@param3  filename  ,the save path
		@param4  cookie  ,the save path

func_describe :Input a url_path , it will download as a file in <filename> path,and cookiie
*/
int GetImageAndCookie(char* url, char* vcodeurl, char* filename, char* cookie);

/*
func_name : ShowImage
func_param: 
		@param :ImagePath ,the image path to show
func_describe : use the <sextremaly> old ver opencv to show the Image 
*/
int ShowImage(string ImagePath);


/*
func_name :  urp_login
func_param:
	@param1: curl
	@param2: res   src string url
	@param3: user  user string 
	@param4: passwd   passwd string 
	@param5: vcode  vaildcode string
func_describe : login the urp_system
*/
int urp_login(CURL* curl, CURLcode res, string user, string passwd, string vcode);

/*
*
func_name: urlcode
func_param:
	@param : src_str : string need to url code
	@return: char* urlcode string 
*/
const char* urlcode(string src_str);

/*
*
func_name: urlcode
func_param:
	@param1: curl 
	@param2: src_str : string need to url code
	@return: char* urlcode string
*/
const char* urlcode(CURL* curl ,string src_str);

int printComment(const string& str, vector<string>& v_str, bool isCommended = false);

int ProcessCommentText(const string& post, vector<string>& Message, CURL* curl);

int CommentTeacher(CURL* curl, char* commentText);