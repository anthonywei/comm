// =====================================================================================
// 
//       Filename:  test.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2014-03-18 18时44分45秒
//       Revision:  none
// 
// =====================================================================================

#include "MsgQueue.h"
#include <unistd.h>


int main()
{
    while(1)
    {

        MsgQueue oQueue;

        oQueue.InitQueue(12345);
 
        std::string strInData = "1231231";

        int iret = oQueue.PutOne(strInData);
        printf("Put data into vmsg ret:%d Errmsg:%s\n", iret, oQueue.GetLastErrMsg().c_str());
        std::string strData;
        iret = oQueue.GetOne(strData, 0);

        printf("Get data from vmsgq ret: %d Errmsg:%s\n", iret, oQueue.GetLastErrMsg().c_str());
        sleep(1);
    }
    return 0;
}
