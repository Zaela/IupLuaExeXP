
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <luajit.h>
#include <iup.h>
#include <iuplua.h>
#include <iupcontrols.h>
#include <iupluacontrols.h>
#include <stdio.h>
#include <zlib.h>

#ifdef _WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

//#define CONSOLE
#define INCLUDE_IUP_GL 1

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "zlib1.lib")
#pragma comment(lib, "cd.lib")
#pragma comment(lib, "cdlua51.lib")
#pragma comment(lib, "freetype6.lib")
#pragma comment(lib, "iup.lib")
#pragma comment(lib, "iupcd.lib")
#pragma comment(lib, "iupcontrols.lib")
#pragma comment(lib, "iuplua51.lib")
#pragma comment(lib, "iupluacontrols51.lib")

#ifdef INCLUDE_IUP_GL
#include <iupgl.h>
#include <iupglcontrols.h>
#include <iupluagl.h>
#include <iupluaglcontrols.h>

#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "iupgl.lib")
#pragma comment(lib, "iupluagl51.lib")
#ifdef INCLUDE_IUP_GL_CONTROLS
#pragma comment(lib, "cdluagl51.lib")
#pragma comment(lib, "ftgl.lib")
#pragma comment(lib, "iupglcontrols.lib")
#pragma comment(lib, "iupluaglcontrols51.lib")
#endif
#endif

#define ENTRY_FILE_PATH "scripts/gui/main.lua"

void ShowErrMsg(const char* msg);

#if defined(_WIN32) && !defined(CONSOLE)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
	lua_State* L = luaL_newstate();
    int rc = 0;

	if (L == NULL)
	{
		ShowErrMsg("Could not initialize Lua interpreter");
		return 2;
	}

	luaL_openlibs(L);

	IupOpen(NULL, NULL);
	IupControlsOpen();

#ifdef INCLUDE_IUP_GL
	IupGLCanvasOpen();
#ifdef INCLUDE_IUP_GL_CONTROLS
	IupGLControlsOpen();
#endif
#endif

	iuplua_open(L);
	iupcontrolslua_open(L);

#ifdef INCLUDE_IUP_GL
	iupgllua_open(L);
#ifdef INCLUDE_IUP_GL_CONTROLS
	iupglcontrolslua_open(L);
#endif
#endif

	if (luaL_loadfile(L, ENTRY_FILE_PATH) || lua_pcall(L, 0, 0, 0))
	{
		ShowErrMsg(lua_tostring(L, -1));
		rc = 1;
	}

	lua_close(L);
#ifndef EXCLUDE_IUP
	//IupControlsClose();
#ifndef CONSOLE
	// Crashes in console mode
	//IupClose();
#endif
#endif

	return rc;
}

void ShowErrMsg(const char* msg)
{
#if defined(_WIN32) && !defined(CONSOLE)
	MessageBoxA(NULL, msg, NULL, MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
	printf("%s\n", msg);
#endif
}

#ifdef _WIN32
#define EXPORT extern __declspec(dllexport)
#else
#define EXPORT extern
#endif

// Re-export Zlib functions, not exported in the static build
EXPORT unsigned long zlib_compressBound(unsigned long len)
{
	return compressBound(len);
}

EXPORT int zlib_compress(unsigned char* dst, unsigned long* dlen, const unsigned char* src, unsigned long slen)
{
	return compress2(dst, dlen, src, slen, 9);
}

EXPORT int zlib_uncompress(unsigned char* dst, unsigned long* dlen, const unsigned char* src, unsigned long slen)
{
	return uncompress(dst, dlen, src, slen);
}
