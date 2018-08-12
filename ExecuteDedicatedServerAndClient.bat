@rem Cooked data output directory
@set DST=WindowsNoEditor\UE4Shooter\Binaries\Win64

@rem Executable file name
@set SERVER=UE4ShooterServer.exe
@set CLIENT=UE4Shooter.exe

@rem On client, input "open 127.0.0.1" from console
start %DST%\%SERVER% -log
start %DST%\%CLIENT% -log
