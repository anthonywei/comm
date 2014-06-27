#ifndef _CONF_READER_H_
#define _CONF_READER_H_

//读取配置文件的方法。

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include "inc_comm.h"
#include <tinyxml.h>
#include "config.h"

#include <glog/logging.h>

using namespace google;

class CConfReader
{
public:
	static int LoadConf(std::string strFileName)
	{
		std::ifstream ifile;
		ifile.open(strFileName.c_str());
		if (!ifile.is_open())
		return -1;

		char szBufLine[1024] =
		{	0};
		while (ifile.getline(szBufLine, sizeof(szBufLine)))
		{
			//判断第一个字符，看看是不是注释
			if (szBufLine[0] == '#' || szBufLine[0] == ';')
			continue;

			//过滤掉空格等多余字符
			std::string sContent = Trim(szBufLine);
			if (sContent.length() <= 1)
			continue;

			std::vector<std::string> vecContent;

			if (SplitStringIntoVector(sContent.c_str(), "=", vecContent) == 2)
			{
				//插入到配置中
				mapConf.insert(
						std::make_pair<std::string, std::string>(vecContent[0],
								vecContent[1]));
			}
		}

		return 0;
	}

	static int LoadConf4Cp(const char*cFileName, CpConfig&config)
	{
		std::ifstream ifile;
		ifile.open(cFileName);
		if (!ifile.is_open())
		{
			std::cerr << "Config File Not Exist!!" << std::endl;
			return -1;
		}
		TiXmlDocument* configDocument = new TiXmlDocument();
		
		if(!configDocument->LoadFile(cFileName))
			return -1;

		TiXmlElement* rootElement = configDocument->RootElement(); //ConfigCenter
		TiXmlElement* itemElement = rootElement->FirstChildElement();//Item
		//std::cout << "Config Content:" << std::endl;
		//int i = 0;
		std::string strKey, strValue;

		while (itemElement)
		{
			TiXmlAttribute* attributeOfItem = itemElement->FirstAttribute();
			char const * chkey = itemElement->Attribute("Key");
			if (!chkey)
			{
				LOG(ERROR)<<"not found key in element "
				<<itemElement->Value()<<std::endl;

				itemElement = itemElement->NextSiblingElement();
				continue;
			}

			strKey = chkey;

			char const *chvalue = itemElement->Attribute("Value");
			if (chvalue) strValue = chvalue;

			mapConf[strKey] = strValue;

			itemElement = itemElement->NextSiblingElement();
		}

		delete configDocument;
		configDocument = NULL;

		//std::string strHost = GetStrCont("Host");
		//if (strHost == "")
		//{
		//	std::cerr << "Server Host Must Not Empty!!" << std::endl;
		//	return -2;
		//}

		//<Item Key="LogDir" Value="/tmp/calog"/>

		config.strDbHost = GetStrCont("DBHost", "localhost:3306");
		config.strDbName = GetStrCont("DBName", "configcenter");
		config.strDbUser = GetStrCont("DBUser", "dbadmin");
		config.strDbPassWord = GetStrCont("DBPassword", "dbpasswd");
		config.strDbCharset = GetStrCont("DBCharact", "UTF8");
		config.wKeepLiveTime = GetIntCont("KeepAliveTime", 5);
		config.strHost = GetStrCont("Host", "localhost");
		config.wPort = GetIntCont("Port", 10001);
		config.strLogDir = GetStrCont("LogDir", "/var/log/cc/cp/");

		std::cout << "Config Init Finished!!" << std::endl;

		return 0;
	}

	static int LoadConf4Ca(const char* cFileName, CaConfig&config)
	{
		std::ifstream ifile;
		ifile.open(cFileName);
		if (!ifile.is_open())
		{
			std::cerr << "Config File Not Exist!!" << std::endl;
			return -1;
		}
		TiXmlDocument* configDocument = new TiXmlDocument();
		configDocument->LoadFile(cFileName);
		TiXmlElement* rootElement = configDocument->RootElement(); //ConfigCenter
		TiXmlElement* itemElement = rootElement->FirstChildElement();//Item
		std::cout << "Config Content:" << std::endl;
		int i = 0;
		std::string strKey, strValue;

		while (itemElement)
		{
			TiXmlAttribute* attributeOfItem = itemElement->FirstAttribute();

			while (attributeOfItem)
			{
				if (0 == i)
				{
					strKey = std::string(attributeOfItem->Value());
				}
				else if (1 == i)
				{
					strValue = std::string(attributeOfItem->Value());
					mapConf.insert(
							std::pair<std::string, std::string>(Trim(strKey),
									Trim(strValue)));
					std::cout << strKey << ":" << strValue << std::endl;
					i = 0;
				}
				attributeOfItem = attributeOfItem->Next();
				i++;
			}
			i = 0;
			itemElement = itemElement->NextSiblingElement();
		}

		delete configDocument;
		configDocument = NULL;

		std::string strHost = GetStrCont("Host");
		if (strHost == "")
		{
			std::cerr << "Server Host Must Not Empty!!" << std::endl;
			return -2;
		}

		config.strHost = strHost;
		config.wPort = GetIntCont("Port", 10001);
		config.strLogDir = GetStrCont("LogDir", "/tmp/ca_log/");
		config.wLogLevel = GetIntCont("LogLevel", 2);
		config.wKeepLiveTime = GetIntCont("KeepLiveTime", 30);
		config.wConnectServerTime = GetIntCont("ConnectServerTime", 5);
		config.wInitMemCount = 10000;

		std::cout << "Config Init Finished!!" << std::endl;

		return 0;
	}

	static unsigned int GetIntCont(std::string strKey,
			unsigned int wDefaultValue = 0)
	{
		std::map<std::string, std::string>::iterator iter = mapConf.find(
				strKey);
		if (iter != mapConf.end())
		{
			return atoi(iter->second.c_str());
		}

		return wDefaultValue;
	}

	static std::string GetStrCont(std::string strKey,
			const std::string& strDefaultValue = "")
	{
		std::map<std::string, std::string>::iterator iter = mapConf.find(
				strKey);
		if (iter != mapConf.end())
		{
			return iter->second;
		}

		return strDefaultValue;
	}

private:
	//过滤掉空格和回车以及tab
	static std::string Trim(const std::string& strInput)
	{
		std::stringstream sstr;
		for (int i = 0; i < strInput.length(); ++i)
		{
			if (strInput[i] != ' ' && strInput[i] != '\t' && strInput[i] != '\r'
					&& strInput[i] != '\n')
			{
				sstr << strInput[i];
			}
		}

		return sstr.str();
	}
private:
	static std::map<std::string, std::string> mapConf;
};

#endif
