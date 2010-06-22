#ifndef _EZCD_H_
#define _EZCD_H_

#define EZCD_VERSION(a,b,c,d) ((((a) << 24) + (((b) << 16) + ((c) <<8))) + (d))
#define EZCD_VERSION_MAJOR(x) (((x) >> 24) & 0xFF)
#define EZCD_VERSION_MINOR(x) (((x) >> 16) & 0xFF)
#define EZCD_VERSION_MICRO(x) (((x) >> 8) & 0xFF)
#define EZCD_VERSION_REV(x)   ((x) & 0xFF)

#define EZCD_HEADER_SIZE  16

#define EZCD_SPACE        0x20000 /* 128K Bytes */

typedef struct ezcd_header_s {
	unsigned char 	magic[4];
	unsigned int 	length;
	unsigned int 	version;
	unsigned int 	checksum;
} ezcd_header_t;

struct ezcd_context;

struct ezcd_context *ezcd_start(void);
void ezcd_stop(struct ezcd_context *ctx);
#endif
