# Prerequisites #

  * Windows Driver Kit (https://www.microsoft.com/whdc/devtools/wdk/wdkpkg.mspx)

# Building #

  * Start a WDK build environment
  * Navigate to the location of your vmulti source root directory (eg. `C:\projects\vmulti`)
  * Build the drivers and test program using `build -wgc`

# Installing #

Note: This will not work on 64-bit systems without first properly signing the driver

  * Gather together the `vmulti.sys`/`vmulti.inf`/`hidkmdf.sys` files into single directory
  * Also copy the file `WdfCoInstaller01009.dll` (from the WDK) to your installation directory
  * Also copy the file `devcon.exe` (from the WDK) to your installation directory
  * Run the command: `devcon install vmulti.inf djpnewton\vmulti`

# Testing #

  * Run `testvmulti.exe /multitouch` to move the cursor via virtual multitouch (only available on Win7)
  * Run `testvmulti.exe /mouse` to move the cursor via virtual mouse
  * Run `testvmulti.exe /digitizer` to move the cursor via virtual digitizer