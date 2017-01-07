@rem Build "Development Server"
@rem Create coocked data "File - Package Project - Windows - Windows(64bit)"

@rem Executable output directory
@set SRC=Binaries\Win64
@rem Executable file name
@set EXE=UE4ShooterServer.exe

@rem Cooked data output directory
@set DST=Package\WindowsNoEditor\UE4Shooter\Binaries\Win64

xcopy %SRC%\%EXE% %DST% /Y

start %DST%\%EXE% -log

@pause