SET CUBEMX_FIRMWARE_LOCATION=F:\STM32Cube\Repository
SET ROBOSD=E:\SOURCETREE\OMEGA\ROBOTICS\robosd-5
SET CUBEMX_FIRMWARE_LOCATION=F:\STM32Cube\Repository

chcp 1251 


PUSHD 

set settings=%COMPUTERNAME%
set settings=%settings::=,%
set settings=%settings:.=,%
set settings=%CD%\settings\%settings%




cd /D "%~dp0"

if not EXIST reference (
	MD   reference 
)


IF EXIST ".\reference\robosd-ref"  RMDIR ".\reference\robosd-ref"
MKLINK /J ".\reference\robosd-ref"  "%ROBOSD%"  

if not EXIST settings (
	MD   settings 
)


if not EXIST "%settings%" (
	MD   "%settings%" 
)


IF EXIST ".\reference\settings-ref"  RMDIR ".\reference\settings-ref"
MKLINK /J ".\reference\settings-ref"  "%settings%"  


echo [LOCAL] > ~robosd.ini
echo CATALOG=%settings%\ >> ~robosd.ini
COPY    ~robosd.ini "%settings%"\robosd-local.ini  /Y 
DEL ~robosd.ini



cd /D "C:"
if not EXIST reference (
	MD   reference 
)


IF EXIST ".\reference\cubmx-fm-location"  RMDIR ".\reference\cubmx-fm-location"
MKLINK /J ".\reference\cubmx-fm-location"  "%CUBEMX_FIRMWARE_LOCATION%"  


POPD