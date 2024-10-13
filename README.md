# luadaqmx

OneLuaPro access to NI's DAQmx driver.

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

## License

See https://github.com/OneLuaPro/luadaqmx/blob/master/LICENSE.