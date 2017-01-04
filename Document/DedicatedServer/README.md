# 専用サーバ

## コンテンツをパッケージ化
* File - Package Project - Package Settings でパッケージ化の設定
* File - Package Project - Windows - Windows (64bit) - 出力先を指定して出力

## ビルド
* UE4Shooter**Server**.Target.cs を作成 (基本UE4Shooter**Editor**.Target.cs をコピーして作成)
~~~
...
  Type = TargetType.Server;
  bUsesSteam = true;
...
public override bool GetSupportedPlatforms(ref List<UnrealTargetPlatform> OutPlatforms)
{
  return UnrealBuildTool.UnrealBuildTool.GetAllServerPlatforms(ref OutPlatforms, false);
}
...
~~~
  * .uproject 右クリック - Generate Visual Studio project files
  * Visual Studio を開いてコンフィギュレーションを Development Server にしてビルド
  * 実行ファイルが出力される Binaries/Win64/UE4ShooterServer.exe
  * 実行ファイルをパッケージ化先ディレクトリへコピーする
  * 実行ファイルを起動する (-log をつけるとログウィンドウが出る)
  * 確認
    * DevelopmentEditor で起動して、コンソールから `open 127.0.0.1:7777`。サーバに入れたら成功