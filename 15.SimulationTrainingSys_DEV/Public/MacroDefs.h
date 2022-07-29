#pragma once
#include <qvariant.h>
#include <string>
using namespace std;

// 缓存区大小
#define ORIGIN_BUFFER_SIZE 10485760
#define MAX_NET_FRAME_LENGTH 65535

/**
* @brief 参数处理结果的值。联合类型 \n
*
*/
union ValueType
{
	double d;				/*!< double类型 */
	unsigned long long ull;	/*!< unsigned long long类型 */
	long long ll;			/*!< long long类型 */
	unsigned long ul;		/*!< unsigned long类型 */
	long l;					/*!< long类型 */
	float f;				/*!< float类型 */
	unsigned int ui;		/*!< unsigned int类型 */
	int i;					/*!< int类型 */
	unsigned short us;		/*!< unsigned short类型 */
	short s;				/*!< short类型 */
	unsigned char uc;		/*!< unsigned char类型 */
	char c;					/*!< char类型 */
};

struct CacheValue
{
	double t;
	ValueType v;
};

struct ResultData
{
	unsigned int key;
	//ValueType value;
	CacheValue value;
};

Q_DECLARE_METATYPE(ResultData)

//根据表示一天内的微秒数的unsigned long long变量获取对应的小时、分钟、秒和毫秒
#define HOUR(x)				((x)/3600000000)
#define MINUTE(x)			(((x) - (HOUR(x))*3600000000)/60000000)
#define SECOND(x)			(((x) - (HOUR(x))*3600000000 - (MINUTE(x))*60000000)/1000000)
#define MILLISECOND(x)		((double)(((x) - (HOUR(x))*3600000000 - (MINUTE(x))*60000000 - (SECOND(x))*1000000)/1000.0))

//计算结果
#define RESULT_TYPE_DOUBLE      1
#define RESULT_TYPE_ULONGLONG   2 
#define RESULT_TYPE_LONGLONG    3 
#define RESULT_TYPE_FLOAT       4
#define RESULT_TYPE_UINT        5
#define RESULT_TYPE_INT         6
#define RESULT_TYPE_USHORT      7
#define RESULT_TYPE_SHORT       8
#define RESULT_TYPE_UCHAR       9
#define RESULT_TYPE_CHAR		10
 
 