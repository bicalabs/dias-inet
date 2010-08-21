#if !defined __INCLUDE____OS__
#define __INCLUDE____OS__

/*
#define __OS_UNIX__
*/
#define __OS_WINDOWS__

/*
#define __VER_311__
#define __VER_95__
#define __VER_98__
#define __VER_ME__
#define __VER_2000__
#define __VER_NT4__
#define __VER_NT3__
#define	__VER_XP__
*/

/*
#define	__ARCH_16BIT__
*/
#define	__ARCH_32BIT__
#define	__ARCH_x386__
/*
#define	__ARCH_64BIT__
#define	__ARCH_IA64__
*/

typedef	unsigned char uchar;
typedef	unsigned int uint;
typedef	unsigned long ulong;
typedef	signed char schar;
typedef	signed int sint;
typedef	signed long slong;

#ifdef __ARCH_16BIT__

# error	Unsupproted 16 bit architecture

#elif defined __ARCH_32BIT__

typedef	unsigned char	ubyte;
typedef	unsigned short	uword;
typedef	unsigned int	udword;
typedef	signed char	sbyte;
typedef	signed short	sword;
typedef	signed int	sdword;
typedef float		rdword;
typedef	double		rqword;
typedef	long double	rtbyte;
typedef unsigned short	uvar16_32;
typedef signed short	svar16_32;
typedef	unsigned long	uvar32_64;
typedef	signed long	svar32_64;

#elif defined __ARCH_64BIT__

#else

# error	Unknown architecture

#endif /* __ARCH__ */


#ifdef __OS_UNIX__
// Microsoft-specific C/C++ keywords
# define	__asm asm
# define	__cdecl

// Windows-specific data types
# define	SOCKET		unsigned int
# define	INVALID_SOCKET	(-1)
# define	SOCKET_ERROR	(-1)
#endif  // __OS_UNIX__

#endif
