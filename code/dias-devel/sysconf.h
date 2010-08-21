#pragma once

#if !defined __ANSI_C__
# define	__ANSI_C__
#  ifndef NULL
#  define NULL (void*)0
# endif
#endif

#define __OS_WINDOWS__

#define	__ARCH_32BIT__


typedef	unsigned char uchar;
typedef	unsigned int uint;
typedef	unsigned short ushort;
typedef	unsigned long ulong;
typedef	signed char schar;
typedef	signed int sint;
typedef	signed short sshort;
typedef	signed long slong;


#if defined __ARCH_16BIT__

# error	Unsupproted 16 bit architecture

#elif defined __ARCH_32BIT__

 typedef unsigned char	ubyte;
 typedef unsigned short	uword;
 typedef unsigned int	udword;
 typedef signed char	sbyte;
 typedef signed short	sword;
 typedef signed int	sdword;
 typedef float		rdword;
 typedef double		rqword;
 typedef long double	rtbyte;
 typedef unsigned long	uvar32_64;
 typedef signed long	svar32_64;

# if defined __OS_WINDOWS__
  typedef unsigned __int64 uqword;
  typedef signed __int64sqword;
# elif defined __OS_POSIX__
  typedef unsigned long long uqword;
  typedef signed long long sqword;
# endif

 typedef enum {
	e_byte = 1,
	e_word = 2,
	e_dword = 4,
	e_qword = 8,
	e_tbyte = 10,
	e_qdword = 16,
	e_tword = 32
 } EData;

 typedef enum {
	e_integer, e_real
 } EType;

#elif defined __ARCH_64BIT__

#else

# error	Unknown architecture

#endif /* __ARCH__ */


#if defined __OS_WINDOWS__
# define	socket_t SOCKET
#else
 typedef int socket_t;
# define	closesocket close
#endif


typedef udword flags_t;
typedef uvar32_64 address_t;

#if defined __ANSI_C__
# define	bool	uint
# define	true	1
# define	false	0
#endif /* __ANSI_C__ */
