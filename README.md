# luadaqmx

OneLuaPro access to NI's DAQmx driver (v24.5.0 a.k.a. 2024 Q3).

## Usage

### writeDigitalLine(device,port,line,value,timeout)

This function writes either `0` or `1` to a digital line specified by its physical designation `Px.y`, with `x` as the port number and `y` as the line number.

```lua
local daq = require "luadaqmx"

local device = "DIO"	-- device name as string as seen in NI MAX Explorer
local port = 1		-- port number as integer value from 0 to 11
local line = 3		-- line number as integer value from 0 to 7
local timeout = 1.0	-- timeout in seconds as float value, negative for no timeout

local retval, errmsg = daq.writeDigitalLine(device,port,line,value,timeout)

-- on success: 
retval = true
errmsg = nil
-- on failure:
retval = false
errmsg = "Detailed NI-DAQmx error message and error code"
```

### readDigitalLine(device,port,line,timeout)

This function reads a digital line specified by its physical designation `Px.y`, with `x` as the port number and `y` as the line number.

```lua
local daq = require "luadaqmx"

local device = "DIO"	-- device name as string as seen in NI MAX Explorer
local port = 1		-- port number as integer value from 0 to 11
local line = 3		-- line number as integer value from 0 to 7
local timeout = 1.0	-- timeout in seconds as float value, negative for no timeout

local value, errmsg = daq.readDigitalLine(device,port,line,timeout)

-- on success: 
value = 0 or 1
errmsg = nil
-- on failure:
value = nil
errmsg = "Detailed NI-DAQmx error message and error code"
```

### getNIDAQVersion()

This function returns the version information of the NI-DAQmx SDK, to which `luadaqmx` was linked.

```lua
local daq = require "luadaqmx"

local major, minor, update = daq.getNIDAQVersion()
print(string.format("Built with NI-DAQmx version %d.%d.%d.",major, minor, update))

Built with NI-DAQmx version 24.5.0.
```

## Install

- Install OneLuaPro to `c:\Apps`.
- Install MSVC Build Tools as described in https://github.com/OneLuaPro/OneLuaPro?tab=readme-ov-file#building-and-installing-oneluapro-from-source-code
- Download and install https://www.ni.com/de/support/downloads/drivers/download.ni-daq-mx.html#544231
  - Install NI-DAQmx - Support for C
  - NI-Certificates
  - Assumes installation of headers and libraries into `C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C`

```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cmake --install . --config Release

# Available architectures (-A ...) are: Win32, x64, ARM, ARM64
```

## License

See https://github.com/OneLuaPro/luadaqmx/blob/master/LICENSE.