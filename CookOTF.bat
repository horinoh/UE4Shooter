@rem Saved/Cooked/<Platform> に出力される -iterate 指定が無い場合は、削除再クックされる

@rem -targetplatform=WindowsNoEditor+WindowsServer+LinuxServer+PS4+XboxOne+IOS+Android

@rem If using --cookonthefly we need -filehostip= in Visual Studio
@rem Project Right Click - Properties - Debugging - Command Arguments に -filehostip=127.0.0.1

D:\GitHub\UnrealEngine\Engine\Binaries\Win64\UE4Editor.exe D:\GitHub\UE4Shooter\UE4Shooter.uproject -run=cook -cookonthefly -iterate