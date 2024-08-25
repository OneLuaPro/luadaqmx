# MIT License
#
# Copyright (c) 2024 Kritzel Kratzel.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in 
# the Software without restriction, including without limitation the rights to 
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all 
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ------------------------------------------------------------------------------
# luadaqmx DLL
add_library(luadaqmx SHARED)
# setup lua include directory
target_include_directories(luadaqmx PRIVATE ${LIBLUA_INCLUDEDIR})
# setup platform-specific sources, compile and linker options
if(WIN32 AND NOT MinGW)
  target_include_directories(luadaqmx PRIVATE ${NIDAQMX_INCDIR})
  target_compile_options(luadaqmx PRIVATE /D_WINDLL /D_WIN32 /D_CRT_SECURE_NO_WARNINGS)
  target_link_options(luadaqmx PRIVATE /LIBPATH:${LIBLUA_LIBDIR} liblua.lib
    /LIBPATH:${NIDAQMX_LIBDIR} NIDAQmx.lib)
else()
  message(FATAL_ERROR "Not yet fully implemented.")
endif()
# plattform-independend sources
target_sources(luadaqmx PRIVATE luadaqmx.c)
# Install
install(TARGETS luadaqmx RUNTIME DESTINATION ${INSTALL_TOP_CDIR})
