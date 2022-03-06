##############################################################################
#
#                     nmake file for sql_LV.dll
#
##############################################################################

OBJ = obj
SUFFIX = dll

CC = cl
# CFLAGS = /TC -DWIN -D__LCC__ /wd4047 -D_CRT_SECURE_NO_DEPRECATE -DHAVE_ORACLE /MT
CFLAGS = -DWIN -DHAVE_MYSQL -DHAVE_ORACLE /MD
DLLFLAGS = /DLL $(LIB) /DEF:exports.def
LINKER = link.exe

#	Windows
SDK_ROOT = "C:\Program Files (x86)\Windows Kits\10
SDK_VER = 10.0.17134.0
MSVC_ROOT = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726
INCLUDES = -I$(SDK_ROOT)\Include\$(SDK_VER)\um"\
		   -I$(SDK_ROOT)\Include\$(SDK_VER)\shared"\
		   -I$(SDK_ROOT)\Include\$(SDK_VER)\ucrt"\
		   -I$(MSVC_ROOT)\include"
LIB=	/LIBPATH:$(SDK_ROOT)\Lib\$(SDK_VER)\um\x86"\
		/LIBPATH:$(SDK_ROOT)\Lib\$(SDK_VER)\ucrt\x86"\
		/LIBPATH:$(MSVC_ROOT)\lib\x86"
LIBS = "msvcrt.lib" "advapi32.lib" "user32.lib" "odbc32.lib"

#	LabView
INCLUDES = $(INCLUDES) \
           -I"C:\Program Files (x86)\National Instruments\LabVIEW 2014\cintools"
LIB= $(LIB) \
    /LIBPATH:"C:\Program Files (x86)\National Instruments\LabVIEW 2014\cintools"
LIBS = $(LIBS) "labview.lib"

#	MySQL
INCLUDES =  $(INCLUDES) \
           -I"C:\Program Files (x86)\MySQL\MySQL Connector.C 6.1\include"
LIB= $(LIB) \
    /LIBPATH:"C:\Program Files (x86)\MySQL\MySQL Connector.C 6.1\lib\vs14"
LIBS = $(LIBS) "mysqlclient.lib"

#	OCI/Oracle *** make sure the OS can find OCI.DLL once you've completed the build ***
ORA_VER = instantclient_12_2
INCLUDES =  $(INCLUDES) \
           -I"C:\Oracle\$(ORA_VER)\sdk\include"
LIB= $(LIB) \
    /LIBPATH:"C:\Oracle\$(ORA_VER)\sdk\lib\msvc\vc9"\
    /LIBPATH:"C:\Oracle\$(ORA_VER)\sdk\lib\msvc"
LIBS = $(LIBS) "oci.lib"
DLLFLAGS = $(DLLFLAGS) /nod:libc 
		   

all: sql_LV.$(SUFFIX)

.c.obj:
  $(CC) $(INCLUDES) -c $(CFLAGS) $*.c

sql_LV.dll: sql_LV.obj
  $(LINKER) /out:$@ $(DLLFLAGS) $*.obj $(LIBS)

clean:
  del *.obj *.dll *.lib *.exp

test:    sql_LV.$(SUFFIX)
  $(CC) $(INCLUDES) -c $(CFLAGS) $@.c
  $(LINKER) /out:$@.exe $(DLLFLAGS) $@.obj sql_LV.lib
  $@.exe

sample:    sample.obj
  $(LINKER) /out:$@.$(SUFFIX)  /DLL $(LIB)  /DEF:export_open.def $*.obj $(LIBS)


