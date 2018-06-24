#include "init.h"
#include "regex.hpp"


int curl_init(_Global_)
{
	curl_global_init(CURL_GLOBAL_ALL);
	return 1;
}

void curl_cleanup(_Global_) 
{
	curl_global_cleanup();
}


size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int written = 0;
	written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

size_t string_write_data(void *ptr, size_t size, size_t nmemb, void *string_stream)
{
	size_t written = size*nmemb;
	*(string *)string_stream += (char *)ptr;
	return written;
}

int GetImageAndCookie(char* url, char* vcodeurl, char* filename, char* cookie)
{
	CURL* curl = NULL;
	CURLcode res;
	FILE* file = NULL;
	FILE* log = NULL;

	file = fopen(filename, "w+b");
	log = fopen("GdeImageAndCookie.log", "w+b");
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);

	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookie);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)log);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	res = curl_easy_perform(curl);
	if ((res != CURLE_OK) && (res != CURLE_WRITE_ERROR))
	{
		printf("%s curl error no=%d \n", __FUNCTION__, res);
		curl_easy_cleanup(curl);
		fclose(file);
		return -1;
	}
	cout << "成功取得Cookie" << endl;

	curl_easy_setopt(curl, CURLOPT_URL, vcodeurl);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)file);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

	res = curl_easy_perform(curl);
	if ((res != CURLE_OK) && (res != CURLE_WRITE_ERROR))
	{
		printf("%s curl error no=%d \n", __FUNCTION__, res);
		curl_easy_cleanup(curl);
		fclose(file);
		return -1;
	}
	cout << "成功取得 验证码" << endl;
	curl_easy_cleanup(curl);
	fclose(file);
	fclose(log);
	return 0;


}




int ShowImage(string ImagePath)
{
	IplImage* img = cvLoadImage(ImagePath.c_str());
	if (img == NULL)
		return -1;

	cvNamedWindow("ValidCode", CV_WINDOW_AUTOSIZE);
	cvShowImage("ValidCode", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("ValidCode");
	return 0;
}


int urp_login(CURL* curl, CURLcode res, string user, string passwd, string vcode)
{
	if (curl == NULL)
		return -1;

	string data;
	string pattern{ "<title>学分制综合教务</title>" };
	string PostData = "zjh1=&tips=&lx=&evalue=&eflag=&fs=&dzslh=&zjh=";
	PostData += user;
	PostData += "&mm=";
	PostData += passwd;
	PostData += "&v_yzm=";
	PostData += vcode;

	curl_easy_setopt(curl, CURLOPT_REFERER, REFERER);       //伪造Referer 
	curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(curl, CURLOPT_URL, LOGIN_PATH);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOKIE); 
	
	curl_easy_setopt(curl, CURLOPT_POST, 1);//采用POST传值方式  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,PostData.c_str() );  //提交Post信息  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,(long)PostData.length() );  //提交Post信息  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);  //防止重定向
	curl_easy_setopt(curl, CURLOPT_COOKIE, COOKIE);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_data);

	res = curl_easy_perform(curl);
	if ((res != CURLE_OK) && (res != CURLE_WRITE_ERROR))
	{
		printf("%s curl error no=%d \n", __FUNCTION__, res);
		return -1;
	}
//	cout << data.c_str() << endl;
	if (my_regex_search(pattern, data))
	{
		cout << "\n**********************\n*      登陆成功      *\n**********************\n";
		return 0;
	}
	
	return -1;
	/*
	post 数据：
	zjh1=&tips=&lx=&evalue=&eflag=&fs=&dzslh=&zjh=1111111111111&mm=2222222222222&v_yzm=afw9
	*/
}


const char* urlcode(string src_str) 
{
	CURL * curl = curl_easy_init();
	string tmp;
	if (curl)
	{
		 tmp = curl_easy_escape(curl, src_str.c_str(), src_str.length());
	}
	curl_easy_cleanup(curl);
	return tmp.c_str();
}


const char* urlcode(CURL* curl, string src_str)
{
	if(curl)
		return curl_easy_escape(curl, src_str.c_str(), src_str.length());
	return "";

}



int printComment(const string& str,vector<string>& v_str ,bool isCommended)
{
	/*  str 应该是  "0000000104#@10451#@张建军#@2017-2018学年第二学期学生问卷#@职业生涯规划#@BK1111020"  */

	size_t offset = 0;
	string str_tmp;

	while ((offset + 1) <= str.length())
	{
		
		size_t tmp = str.find("#@", offset);

		if (tmp < 0 || tmp >= str.length())
			break;
		for (auto i = offset; i < tmp; i++)
			str_tmp += str[i];
		v_str.push_back(str_tmp);
		offset = tmp + 2;
		str_tmp = "";
	}
	str_tmp = "";
	for (auto i = offset; i < (str.length() -1) ; i++)
		str_tmp += str[i];
	v_str.push_back(str_tmp);

	cout << "***************************************************" << endl;
	cout << v_str[3] << " : "<<endl;
	cout << "Teacher Name: "<<v_str[2] << endl;
	cout << "Class Name  : "<< v_str[4] << endl;
	cout << "Class Number: " << v_str[5] << endl;
	if (isCommended)
		cout << "\n   !!!!   已经评论过了   !!!!   \n";
	cout << "***************************************************" << endl;
	cout << "\n\n";

	return 0;
}


int ProcessCommentText(const string& post, vector<string>& Message ,CURL* curl)
{
	/* 使用 已经登陆urp的CURL 指针 */
	/*  POST 方法  url = http://222.195.242.240:8080/jxpgXsAction.do*/

	if (!curl)
		return -1;
	/*----------------------------------------------------------------*/

	string evaluatePattern{ R"(<title>问卷评估页面</title>)" };  /*  是否打开了单个老师的评估页面 */
	string final_evaluate_patern{ R"(alert\("评估成功！"\);)" };

	CURLcode res;
	string data;
	/*   ----------------------申请变量--------------------- */


	curl_easy_setopt(curl, CURLOPT_URL, EVERY_TEACHER_COMMENT_URL_POST);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

	curl_easy_setopt(curl, CURLOPT_COOKIE, COOKIE);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(curl, CURLOPT_POST, 1);//采用POST传值方式  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());  //提交Post信息  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)post.length());  //提交Post信息<length>  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);  //防止重定向

	res = curl_easy_perform(curl);/* Exec */

	if (!my_regex_search(evaluatePattern, data))
	{
		cout << "请求 评价失败 " << endl;
		return -1;
	}

	cout << "\n正在评价"<< Message[2] <<"老师 ing...      ";	/* 打开成功 */

 /*  -------------------- 填写 评价表格 -----------------  */
	int i_tmp = rand() % 2 +1;
	string commentText,post_comment_url;

	switch (i_tmp)
	{
	case 1:
		commentText = TEACHURE_COMMENT_1; break;
	case 2:
		commentText = TEACHURE_COMMENT_2; break;
	case 3:
		commentText = TEACHURE_COMMENT_3; break;
	default:
		commentText = TEACHURE_COMMENT_1;
	}

	char teacher_page_comment[500] = { 0 };
	sprintf_s(teacher_page_comment, sizeof(teacher_page_comment),
		"0000000025=20_1&0000000026=20_1&0000000027=20_1&0000000028=20_1&0000000029	20_1&bpr=%s&pgnr=%s&wjbm=%s&wjbz=&xumanyzg=zg&zgpj=%s",
		Message[1].c_str(),
		Message[5].c_str(),
		Message[0].c_str(),
		urlcode(curl,commentText)
	);
	post_comment_url = teacher_page_comment;

	/*---------------------------post发送评论 -------------------------- */

	data = "";

	curl_easy_setopt(curl, CURLOPT_URL, FINAL_COMMENT_POST_URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

	curl_easy_setopt(curl, CURLOPT_COOKIE, COOKIE);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(curl, CURLOPT_POST, 1);//采用POST传值方式  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_comment_url.c_str());  //提交Post信息  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)post_comment_url.length());  //提交Post信息<length>  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);  //防止重定向

	res = curl_easy_perform(curl);/* Exec */

	if (!my_regex_search(final_evaluate_patern, data))
	{
		cout << " 评价失败 " << endl;
		return -1;
	}

	cout << " 评价成功 \n" << endl; 
	Sleep(1000); 
	return 0;
}



int CommentTeacher(CURL* curl, char* commentText)
{
	/* 使用 已经登陆urp的CURL 指针 */
	/* 使用 Get 方法 ，url=http://222.195.242.240:8080/jxpgXsAction.do?oper=listWj  */


	/*  updata the latest regex expression is  <td align="center">.*</td>\s*<td align="center">\s*<img name=".*?"   */

	/*   and its return  is
	<td align="center">是</td>
	<td align="center">
	<img name="0000000104#@10451#@张建军#@2017-2018学年第二学期学生问卷#@职业生涯规划#@BK1111020"
	*/


	/*
		AllText		是 提取出来的整段信息
		v_message   是 关键行信息 "0000000104#@10451#@张建军#@2017-2018学年第二学期学生问卷#@职业生涯规划#@BK1111020"
		part.._text 是 将关键行信息 拆分成 单独信息的 集合<只包含一个关键行>
	*/

	vector<string> AllText, v_message, part_processed_text;
	if (!curl)
		return -1;
	string data;

	string pattern{ R"(<td align="center">.*</td>\s*<td align="center">\s*<img name=".*?")" };  /* 获取上面的整段信息 */
	string iscommend{ R"(<td align="center">是</td>)" };/* 提取  <td align="center">是</td> */
	string messagePattern{ R"(".*#@.*#@.*#@.*#@.*#.*?")" }; /* 提取 "0000000104#@10451#@张建军#@2017-2018学年第二学期学生问卷#@职业生涯规划#@BK1111020"  == v_message */


	/*----------------------打开总评价页面，获取老师信息----------------------*/
	curl_easy_setopt(curl, CURLOPT_URL, COMMENT_URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_COOKIE, COOKIE);
	/*Exec*/
	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
		cout << "commentTeacher_curl_easy_perform : failure\n";
//	cout << data << endl;
	
	/*-----------提取,生成 打开每个老师页面的post 参数-------------*/
	my_sregex_iterator(pattern,data,AllText);

	for (auto i : AllText)
	{
		my_sregex_iterator(messagePattern, i, v_message);

		v_message[0].erase(v_message[0].begin());
		v_message[0].erase(v_message[0].end());
		/* 将v_message 中多包含的 “” 删去  */

		if (my_regex_search(iscommend, i))
		{
			/* 已经评论过了*/
			printComment(v_message[0], part_processed_text, 1);
			part_processed_text.clear();
			v_message.clear();
			continue;
		}
		/*  没评论过*/
		printComment(v_message[0], part_processed_text);

		char c_tmp[500];/*post申请打开评论页面*/
		sprintf_s(c_tmp, sizeof(c_tmp),
			"bpr=%s&bprm=%s&currentPage=1&oper=wjShow&page=1&pageNo=&pageSize=20&pgnr=%s&pgnrm=%s&wjbm=%s&wjbz=null&wjmc=%s",
			part_processed_text[1].c_str(),
			urlcode(curl, part_processed_text[2]),
			part_processed_text[5].c_str(),
			urlcode(curl, part_processed_text[4]),
			part_processed_text[0].c_str(),
			urlcode(curl, part_processed_text[3])
		);/* 构造post 表格 */

		string post_url = c_tmp;

		ProcessCommentText(post_url, part_processed_text, curl);


		/*--------------清理---------------*/
		part_processed_text.clear();
		v_message.clear();

	}

	/*   v_str 为要 post 的 表单数据   */


	return 0;

}