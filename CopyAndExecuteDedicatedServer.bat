@set SRC=Binaries\Win64
@set DST=Package\WindowsNoEditor\UE4Shooter\Binaries\Win64
@set EXE=UE4ShooterServer.exe

@xcopy %SRC%\%EXE% %DST% /Y

@%DST%\%EXE% -log

@pause