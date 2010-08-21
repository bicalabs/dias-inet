
dias-develps.dll: dlldata.obj dias-devel_p.obj dias-devel_i.obj
	link /dll /out:dias-develps.dll /def:dias-develps.def /entry:DllMain dlldata.obj dias-devel_p.obj dias-devel_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0500 /DREGISTER_PROXY_DLL \
		$<
# _WIN32_WINNT=0x0500 is for Win2000, change it to 0x0400 for NT4 or Win95 with DCOM

clean:
	@del dias-develps.dll
	@del dias-develps.lib
	@del dias-develps.exp
	@del dlldata.obj
	@del dias-devel_p.obj
	@del dias-devel_i.obj
