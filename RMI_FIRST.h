#include "MRI.h"

#define MAKESTRING(str) #str
#define JOINSTRING4(str1, str2, str3, str4) MAKESTRING(str1##str2##str3##str4)

template<typename classname>
class ClientCalculate : public RMIClientBase
{
public:
	int sum(int p1, int p2)
	{
		return FunctionObject<int, int, int>()(JOINSTRING4(int, sum, int, int), this, p1, p2);
	}
};
