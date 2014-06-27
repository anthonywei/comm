#ifndef _CHECK_UNIQ_PROCESS_H_
#define _CHECK_UNIQ_PROCESS_H_

//anthonywei 判断进程个数，保证进程在机器上只有一个进程在运行
//因为错误码组件是操作共享内存，所以多进程的话会导致内存
//错乱，而且错误码不多，但进程完全可以处理，所以限制单进程
//即可
class CheckUniqProcess
{
/*
* 根据进程名判断进程个数，这里参数是进程的名字
*/
public:
static int CheckProcessNum(char* processName); 
};

#endif

