#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"


#define file_NULL (struct fsdata_file *) NULL


static const unsigned int dummy_align__config_shtml = 0;
static const unsigned char data__config_shtml[] = {
/* /config.shtml (14 chars) */
0x2f,0x63,0x6f,0x6e,0x66,0x69,0x67,0x2e,0x73,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-type: text/html
Expires: Fri, 10 Apr 2008 14:00:00 GMT
Pragma: no-cache

" (85 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x45,0x78,0x70,0x69,0x72,0x65,0x73,
0x3a,0x20,0x46,0x72,0x69,0x2c,0x20,0x31,0x30,0x20,0x41,0x70,0x72,0x20,0x32,0x30,
0x30,0x38,0x20,0x31,0x34,0x3a,0x30,0x30,0x3a,0x30,0x30,0x20,0x47,0x4d,0x54,0x0d,
0x0a,0x50,0x72,0x61,0x67,0x6d,0x61,0x3a,0x20,0x6e,0x6f,0x2d,0x63,0x61,0x63,0x68,
0x65,0x0d,0x0a,0x0d,0x0a,
/* raw file data (224 bytes) */
0x3c,0x21,0x44,0x4f,0x43,0x54,0x59,0x50,0x45,0x20,0x68,0x74,0x6d,0x6c,0x3e,0x0d,
0x0a,0x3c,0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,
0x76,0x3d,0x22,0x72,0x65,0x66,0x72,0x65,0x73,0x68,0x22,0x20,0x63,0x6f,0x6e,0x74,
0x65,0x6e,0x74,0x3d,0x22,0x31,0x22,0x3e,0x0d,0x0a,0x3c,0x62,0x6f,0x64,0x79,0x3e,
0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x54,0x69,0x6d,0x65,0x73,0x74,0x61,0x6d,0x70,0x20,0x3a,0x20,0x3c,
0x21,0x2d,0x2d,0x23,0x74,0x73,0x2d,0x2d,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x4c,0x65,0x64,0x30,0x3a,0x20,0x3c,0x21,0x2d,
0x2d,0x23,0x6c,0x65,0x64,0x30,0x2d,0x2d,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x4c,0x65,0x64,0x31,0x3a,0x20,0x3c,0x21,0x2d,
0x2d,0x23,0x6c,0x65,0x64,0x31,0x2d,0x2d,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x4c,0x65,0x64,0x32,0x3a,0x20,0x3c,0x21,0x2d,
0x2d,0x23,0x6c,0x65,0x64,0x32,0x2d,0x2d,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,
};

static const unsigned int dummy_align__index_html = 1;
static const unsigned char data__index_html[] = {
/* /index.html (12 chars) */
0x2f,0x69,0x6e,0x64,0x65,0x78,0x2e,0x68,0x74,0x6d,0x6c,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
/* raw file data (184 bytes) */
0x3c,0x21,0x44,0x4f,0x43,0x54,0x59,0x50,0x45,0x20,0x68,0x74,0x6d,0x6c,0x3e,0x0d,
0x0a,0x3c,0x68,0x65,0x61,0x64,0x65,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,
0x74,0x69,0x74,0x6c,0x65,0x3e,0x48,0x6f,0x6d,0x65,0x3c,0x2f,0x74,0x69,0x74,0x6c,
0x65,0x3e,0x0d,0x0a,0x3c,0x2f,0x68,0x65,0x61,0x64,0x65,0x72,0x3e,0x0d,0x0a,0x3c,
0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x68,0x31,0x3e,0x48,
0x6f,0x6d,0x65,0x3c,0x2f,0x68,0x31,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x70,
0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x54,0x68,0x69,0x73,0x20,
0x69,0x73,0x20,0x48,0x6f,0x6d,0x65,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x70,
0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x54,0x68,0x69,0x73,0x20,0x69,0x73,0x20,0x50,0x61,0x72,0x61,
0x67,0x72,0x61,0x70,0x68,0x0d,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,
0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,};



const struct fsdata_file file__config_shtml[] = { {
file_NULL,
data__config_shtml,
data__config_shtml + 16,
sizeof(data__config_shtml) - 16,
1,
}};

const struct fsdata_file file__index_html[] = { {
file__config_shtml,
data__index_html,
data__index_html + 12,
sizeof(data__index_html) - 12,
1,
}};

#define FS_ROOT file__index_html
#define FS_NUMFILES 2

