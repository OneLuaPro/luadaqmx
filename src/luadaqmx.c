/*
--------------------------------------------------------------------------------
MIT License

luadaqmx - Copyright (c) 2024 Kritzel Kratzel.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

--------------------------------------------------------------------------------

National Instruments / Data Acquisition (NI-DAQmx) SDK
Copyright (c) National Instruments 2003-2024.  All Rights Reserved.     
https://www.ni.com/en/about-ni/legal/software-license-agreement.html

--------------------------------------------------------------------------------
*/

#ifdef _WINDLL
#include <windows.h>
#define DLL __declspec(dllexport)
#else
#define DLL //empty
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <NIDAQmx.h>

#define TRUE 1
#define FALSE 0
#define LUADAQMX_VERSION "luadaqmx 0.1"

#ifdef _WINDLL
static int captureLastError(lua_State *L, TaskHandle taskHandle) {
  // Capture and push on stack latest NI-DAQmx error message
  // Call DAQmxGetExtendedErrorInfo twice for dynamic memory alloc
  size_t sz = (size_t)DAQmxGetExtendedErrorInfo(NULL, 0);
  char *buf = (char *)malloc(sz+1);
  DAQmxGetExtendedErrorInfo(buf, (uInt32)sz);
  // Try clean NI-DAQ exit
  DAQmxStopTask(taskHandle);
  DAQmxClearTask(taskHandle);
  lua_pushboolean(L,FALSE);	// command failed
  lua_pushstring(L, buf); 	// detailed error message
  free(buf);
  return 2;	// two items pushed on stack, return this from caller
}

static int luadaqmx_getNIDAQVersion(lua_State *L) {
  // returns NI-DAQ version info - no error handling here
  uInt32 major = 0;
  uInt32 minor = 0;
  uInt32 update = 0;
  DAQmxGetSysNIDAQMajorVersion(&major);
  DAQmxGetSysNIDAQMinorVersion(&minor);
  DAQmxGetSysNIDAQUpdateVersion(&update);
  lua_pushinteger(L, (lua_Integer)major);	// major version
  lua_pushinteger(L, (lua_Integer)minor);	// minor version
  lua_pushinteger(L, (lua_Integer)update);	// update version
  return 3;
}

static int luadaqmx_writeDigitalLine(lua_State *L) {
  // arguments: device, port, line, value, timeout

  // Check number of arguments
  if (lua_gettop(L) != 5) {
    // bailing out
    return luaL_error(L,"Wrong number of arguments.");
  }
  // check arguments, type and data range
  const char *device = luaL_checkstring(L,1);
  lua_Integer port   = luaL_checkinteger(L,2);
  lua_Integer line   = luaL_checkinteger(L,3);
  lua_Integer value  = luaL_checkinteger(L,4);
  lua_Number timeout = luaL_checknumber(L,5);
  if (port < 0 || port > 11) {
    return luaL_error(L,"Invalid port number (0 <= port <= 11).");
  }
  if (line < 0 || line > 7) {
    return luaL_error(L,"Invalid line number (0 <= line <= 7).");
  }
  if (value != 0 && value != 1) {
    return luaL_error(L,"Invalid line value (0 or 1).");
  }
  
  // Create task
  TaskHandle taskHandle = 0;
  int32 status = DAQmxCreateTask("writeDigitalLine",&taskHandle);
  if (status != 0) return captureLastError(L, taskHandle);

  // Configure digital output channel
  size_t sz = snprintf(NULL,0,"%s/port%d/line%d",device,(int)port,(int)line);
  char *buf = (char *)malloc(sz+1);
  snprintf(buf,sz+1,"%s/port%d/line%d",device,(int)port,(int)line);
  // printf("DEBUG:writeDigitalLine(): buf = %s\n",buf);
  status = DAQmxCreateDOChan(taskHandle,buf,"",DAQmx_Val_ChanForAllLines);
  free(buf);
  if (status != 0) return captureLastError(L, taskHandle);
  
  // Start code
  status = DAQmxStartTask(taskHandle);
  if (status != 0) return captureLastError(L, taskHandle);

  // Write digital data to line
  uInt8 data[1]={(uInt8)value};
  status = DAQmxWriteDigitalLines(taskHandle,1,1,(float64)timeout,
				  DAQmx_Val_GroupByChannel,data,NULL,NULL);
  if (status != 0) return captureLastError(L, taskHandle);

  // Done here - stop Code
  DAQmxStopTask(taskHandle);
  DAQmxClearTask(taskHandle);
  lua_pushboolean(L,TRUE);	// command succeeded
  lua_pushnil(L);		// no error description
  return 2;
}

static int luadaqmx_readDigitalLine(lua_State *L) {
  // arguments: device, port, line, timeout
  // returns  : nil or 0 or 1 + errmsg, if any

  // Check number of arguments
  if (lua_gettop(L) != 4) {
    // bailing out
    return luaL_error(L,"Wrong number of arguments.");
  }
  // check arguments, type and data range
  const char *device = luaL_checkstring(L,1);
  lua_Integer port   = luaL_checkinteger(L,2);
  lua_Integer line   = luaL_checkinteger(L,3);
  lua_Number timeout = luaL_checknumber(L,4);
  if (port < 0 || port > 11) {
    return luaL_error(L,"Invalid port number (0 <= port <= 11).");
  }
  if (line < 0 || line > 7) {
    return luaL_error(L,"Invalid line number (0 <= line <= 7).");
  }
  
  // Create task
  TaskHandle taskHandle = 0;
  int32 status = DAQmxCreateTask("readDigitalLine",&taskHandle);
  if (status != 0) return captureLastError(L, taskHandle);

  // Configure digital input channel
  size_t sz = snprintf(NULL,0,"%s/port%d/line%d",device,(int)port,(int)line);
  char *buf = (char *)malloc(sz+1);
  snprintf(buf,sz+1,"%s/port%d/line%d",device,(int)port,(int)line);
  status = DAQmxCreateDIChan(taskHandle,buf,"",DAQmx_Val_ChanForAllLines);
  free(buf);
  if (status != 0) return captureLastError(L, taskHandle);
  
  // Start code
  status = DAQmxStartTask(taskHandle);
  if (status != 0) return captureLastError(L, taskHandle);

  // Read digital data from line
  uInt8 data[1]={0};
  int32	read, bytesPerSamp;
  status = DAQmxReadDigitalLines(taskHandle,1,(float64)timeout,
				 DAQmx_Val_GroupByChannel,data,1,
				 &read,&bytesPerSamp,NULL);  
  if (status != 0) return captureLastError(L, taskHandle);

  // Done here - stop Code
  DAQmxStopTask(taskHandle);
  DAQmxClearTask(taskHandle);
  // Finally check if data is either 0 or 1
  if (data[0] > 2) {
    lua_pushnil(L);				// no value
    lua_pushstring(L,"readDigitalLine() did not return 0 or 1."); // errmsg
  }
  else {
    lua_pushinteger(L,(lua_Integer)data[0]);	// valid digital value
    lua_pushnil(L);				// no error
  }
  return 2;
}
#else
// FIXME - non-_WINDLL not yet implemented
#endif

static const struct luaL_Reg luadaqmx_metamethods [] = {
  {"__call", luadaqmx_getNIDAQVersion},
  {"__call", luadaqmx_writeDigitalLine},
  {"__call", luadaqmx_readDigitalLine},
  {NULL, NULL}
};

static const struct luaL_Reg luadaqmx_funcs [] = {
  {"getNIDAQVersion", luadaqmx_getNIDAQVersion},
  {"writeDigitalLine", luadaqmx_writeDigitalLine},
  {"readDigitalLine", luadaqmx_readDigitalLine},
  {NULL, NULL}
};

DLL int luaopen_luadaqmx(lua_State *L){
  luaL_newlib(L, luadaqmx_funcs);
  luaL_newlib(L, luadaqmx_metamethods);
  lua_setmetatable(L, -2);
  lua_pushliteral(L,LUADAQMX_VERSION);
  lua_setfield(L,-2,"_VERSION");
  return 1;
}
