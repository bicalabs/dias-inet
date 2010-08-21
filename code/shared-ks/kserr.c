/*******************************************************************************
*  This is a part of aIm SUITE.Internet                                        *
*                                                                              *
*  Kernel Services main functions: error handling subset                       *
*                                                                              *
*  Do not copy, view or modify this code unless your a member of               *
*  aIm SUITe development team                                                  *
*                                                                              *
*  For licensing information see license.txt in the program root directory     *
*                                                                              *
*  Created 06.01.2003 Maxim A. Orlovsky (maxim_orlovsky@yahoo.com)             *
*  Revised 11.01.2003 Maxim A. Orlovsky: added strict function arguments       *
*                     control and any kind of possible checks                  *
*                                                                              *
*******************************************************************************/

#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <winsock2.h>

#include <sysconf.h>
#include <strsafex.h>
#include <libnodes.h>
#include "aimkstp.h"

#include "ks.h"
#include "kserr.h"

int _kserr_elog = 0, _kserr_log = 0;

uchar _kserr_started[] = " *** Log service is started\x0D\x0A";
uchar _kserr_stopped[] = " *** Log service is stopped\x0D\x0A";
uchar _kserr_msg_noclient[] = "KSERROR *** No client\x0D\x0A";
uchar _kserr_msg_nomem[] = "KSERROR *** No memory\x0D\x0A";

bool kserr_init (uchar* error_log, uchar* std_log) {
	if (error_log) {
		if ((_kserr_elog = open (error_log, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, S_IWRITE)) == -1)
			return (false);
		write (_kserr_elog, _kserr_started, strlen (_kserr_started));
	}
	if (std_log) {
		if ((_kserr_log = open (std_log, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, S_IWRITE)) == -1)
			return (false);
		write (_kserr_log, _kserr_started, strlen (_kserr_started));
	}
	return (true);
}

void kserr_stop () {
	if (_kserr_log != 0) {
		write (_kserr_log, _kserr_stopped, strlen (_kserr_stopped));
		close (_kserr_log);
	}
	if (_kserr_elog != 0) {
		write (_kserr_elog, _kserr_stopped, strlen (_kserr_stopped));
		close (_kserr_elog);
	}
}

void kserr_fire (client* c, error_t err, char* func, char* file, int line) {
	uword n;

	if (!c && _kserr_elog)
			write (_kserr_elog, _kserr_msg_noclient, strlen (_kserr_msg_noclient));
	if (!c)
		return;

	n = c->err_finish++;
	if (c->err_finish >= KS_ERRCONT_LEN) c->err_finish = 0;
	if (c->err_start == c->err_finish) c->err_start = c->err_finish + 1;
	if (c->err_start >= KS_ERRCONT_LEN) c->err_start = 0;

	file = strrchr (file, '\\') ? strrchr (file, '\\') + 1 : file;
	file = strrchr (file, '/') ? strrchr (file, '/') + 1 : file;
	if (strlen (func) >= KS_ERRFUNC_LEN) {
		memcpy (c->err_cont[n].func, func, KS_ERRFUNC_LEN - 1);
		c->err_cont[n].func[KS_ERRFUNC_LEN - 1] = '\0';
	} else
		strcpy (c->err_cont[n].func, func);
	if (strlen (file) >= KS_ERRFILE_LEN) {
		memcpy (c->err_cont[n].file, file, KS_ERRFILE_LEN - 1);
		c->err_cont[n].file[KS_ERRFILE_LEN - 1] = '\0';
	} else
		strcpy (c->err_cont[n].file, file);
	c->err_cont[n].err = err;
	c->err_cont[n].line = line;

	if ((err & 0xF0000000) == KSERRT_LOGENTRY) {
		if (_kserr_log)
			_kserr_writelog (_kserr_log, c ? c : clients_head, n);
	} else {
		if (_kserr_elog)
			_kserr_writelog (_kserr_elog, c ? c : clients_head, n);
	}
	if ((err & 0xF0000000) == KSERRT_PANIC)
		exit (1);
}

error* kserr_get (client* c, uword offset) {
	uword n;
	isclientn (c);
	n = c->err_start;
	while (offset--)
		if (n++ >= KS_ERRCONT_LEN) n = 0;
	return (&(c->err_cont[n]));
}

error* kserr_last (client* c) {
	uword n;
	isclientn (c);
	n = c->err_finish;
	if (n == 0) n = KS_ERRCONT_LEN - 1;
	else n--;
	return (&(c->err_cont[n]));
}

void _kserr_writelog (int f, client* c, uword n) {
	uchar* p;
	uchar buff[1024];
	struct timeb tb;

	isclient (c) fail
		return;
	otherwise;

	itoa (c->uid, buff, 10);
	write (f, buff, strlen (buff));
	write (f, ", ", 2);
	write (f, c->ip, strlen (c->ip));
	write (f, "\t", 1);

	ftime (&tb);
	if (p = asctime (gmtime (&(tb.time))))
		write (f, p, strlen (p) - 1);
	write (f, "\t", 1);

	write (f, c->err_cont[n].func, strlen (c->err_cont[n].func));
	write (f, "\t(", 2);
	write (f, c->err_cont[n].file, strlen (c->err_cont[n].file));
	write (f, ", ", 2);
	itoa (c->err_cont[n].line, buff, 10);
	write (f, buff, strlen (buff));
	write (f, ")\t", 2);

	if (c->err_cont[n].err) {
		itoa (c->err_cont[n].err, buff, 10);
		write (f, buff, strlen (buff));
	}
	write (f, "\t", 1);

	if (p = kstp_msg (c->err_cont[n].err))
		write (f, p, strlen (p));

	write (f, "\x0D\x0A", 2);
}
