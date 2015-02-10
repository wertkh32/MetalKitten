#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define CLAMP(n,a,b) MIN(MAX(a,n),b)
#define TO_RAD(a) ((a)*(M_PI/180.))
#define TO_DEG(a) ((a)*(180./M_PI))
#define INF 10000000
#define FLOOR -12
#define LINEAR_BLEND