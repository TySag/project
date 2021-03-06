/*
 * $File: system.h
 * $Date: Wed Jan 01 23:20:31 2014 +0800
 * $Author: jiakai <jia.kai66@gmail.com>
 * $Mender: XinYao <authwork@163.com>
 */

#ifndef __HEADER_SYSTEM__
#define __HEADER_SYSTEM__

#define CPU_FREQUENCY	12500000

typedef unsigned uint32_t;

typedef volatile unsigned* memio_ptr_t;
typedef const unsigned char* img_ptr_t;
static memio_ptr_t const
	vga_buffer = (memio_ptr_t)0xBA000000,
	com_data = (memio_ptr_t)0xBFD003F8,
	com_stat = (memio_ptr_t)0xBFD003FC,
	keyboard_data = (memio_ptr_t)0xAF000000,
	flash = (memio_ptr_t)0xBE000000;

#define VGA_ROW_SIZE	512

static const char KEYCODE_MAP[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,0,0,0,113,49,0,0,0,122,115,97,119,50,0,0,99,120,100,101,52,51,0,0,32,118,102,116,114,53,0,0,110,98,104,103,121,54,0,0,0,109,106,117,55,56,0,0,44,107,105,111,48,57,0,0,46,47,108,59,112,45,0,0,0,39,0,91,61,0,0,0,0,13,93,0,92,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,126,0,0,0,0,0,0,81,33,0,0,0,90,83,65,87,64,0,0,67,88,68,69,36,35,0,0,32,86,70,84,82,37,0,0,78,66,72,71,89,94,0,0,0,77,74,85,38,42,0,0,60,75,73,79,41,40,0,0,62,63,76,58,80,95,0,0,0,34,0,123,43,0,0,0,0,13,125,0,124,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// some functions copy from ucore

/* compiler provides size of save area */
typedef __builtin_va_list va_list;

#define va_start(ap, last)              (__builtin_va_start(ap, last))
#define va_arg(ap, type)                (__builtin_va_arg(ap, type))
#define va_end(ap)                      /*nothing*/

#define read_c0_status()	__read_32bit_c0_register($12, 0)
#define read_c0_cause()		__read_32bit_c0_register($13, 0)
#define write_c0_status(val)	__write_32bit_c0_register($12, 0, val)

#define __read_32bit_c0_register(reg, sel)				\
({ uint32_t __res;								\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #reg "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #reg ", " #sel "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})

#define __write_32bit_c0_register(reg, sel, value)			\
do {									\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mtc0\t%z0, " #reg "\n\t"			\
			: : "Jr" ((unsigned int)(value)));		\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mtc0\t%z0, " #reg ", " #sel "\n\t"	\
			".set\tmips0"					\
			: : "Jr" ((unsigned int)(value)));		\
} while (0)

#define read_c0_count()		__read_32bit_c0_register($9, 0)

// disable com and keyboard interrupt
static inline void disable_interrupt(uint32_t *saved_value) {
	uint32_t status = read_c0_status();
	*saved_value = status;
	status &= ~0x00005000u;
	write_c0_status(status);
}

static inline void enable_interrupt(uint32_t saved_value) {
	write_c0_status(saved_value);
}

/*
 * get cp0 count register value after specified microseconds (1e-3 s)
 */
static inline uint32_t get_count_after_msec(int micro_sec) {
	return read_c0_count() + micro_sec * (CPU_FREQUENCY / 1000);
}

#define SYSCALL_BASE            0x80
static inline int syscall(int num, ...) {
    va_list ap;
    va_start(ap, num);
    uint32_t arg[4];
    int i, ret;
    for (i = 0; i < 4; i ++) {
        arg[i] = va_arg(ap, uint32_t);
    }
    va_end(ap);
//YX remove the base according to kernel
    //num += SYSCALL_BASE;
//
    asm volatile(
      ".set noreorder;\n"
      "move $v0, %1;\n" /* syscall no. */
      "move $a0, %2;\n"
      "move $a1, %3;\n"
      "move $a2, %4;\n"
      "move $a3, %5;\n"
      "syscall;\n"
      "nop;\n"
      "move %0, $v0;\n"
      : "=r"(ret)
      : "r"(num), "r"(arg[0]), "r"(arg[1]), "r"(arg[2]), "r"(arg[3])
      : "a0", "a1", "a2", "a3", "v0"
    );
    return ret;
}

#define SYS_exit            1
#define SYS_putc            30
#define SYS_redraw_console		242

static inline void sys_exit(int error_code) __attribute__((noreturn));

void sys_exit(int error_code) {
    syscall(SYS_exit, error_code);
	for (; ;);	// should never get here
}

static inline int sys_putc(int c) {
    return syscall(SYS_putc, c);
}

static inline void sys_redraw_console() {
	syscall(SYS_redraw_console);
}

static inline void puts(const char *str) {
	while (*str)
		sys_putc(*(str ++));
}

static inline unsigned int mulu10(unsigned int n)
{
  return (n<<3)+(n<<1);
}

/* __divu* routines are from the book, Hacker's Delight */
//YX |  printf need these in MIPS-FPGA
static inline unsigned int divu10(unsigned int n) {
  unsigned int q, r;
  q = (n >> 1) + (n >> 2);
  q = q + (q >> 4);
  q = q + (q >> 8);
  q = q + (q >> 16);
  q = q >> 3;
  r = n - mulu10(q);
  return q + ((r + 6) >> 4);
}

static inline void print_dec(int v) {
	if (!v) {
		sys_putc('0');
		return;
	}
	static char buf[20];
	char *ptr = buf + 19;
	*ptr = 0;
	if (v < 0) {
		sys_putc('-');
		v = -v;
	}
	while (v) {
		unsigned q = divu10(v),
				 m = v - mulu10(q);
		*(-- ptr) = '0' + m;
		v = q;
	}
	puts(ptr);
}

static inline int atoi(const char *str) {
	int val = 0;
	while (*str)
		val = mulu10(val) + *(str ++) - '0';
	return val;
}

static inline int get_key() {
	if (*com_stat & 2)
		return *com_data;
	if (!(read_c0_cause() & 0x00004000)) // ps2 int
		return 0;
	int c = *keyboard_data;
	if (c >= 0 && c < 256)
		c = KEYCODE_MAP[c];
	else
		c = 0;
	return c;
}

#endif // __HEADER_SYSTEM__
