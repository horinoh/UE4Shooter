# 専用サーバ

## [パッケージ化](https://github.com/horinoh/UE4Shooter/tree/master/Document/Packaging)する

## ビルド
* UE4Shooter**Server**.Target.cs を作成 (基本UE4Shooter**Editor**.Target.cs をコピーして作成)
  * .uproject 右クリック - Generate Visual Studio project files
  * Visual Studio を開いてコンフィギュレーションを Development Server にしてビルド
    * Binaries/Win64/UE4ShooterServer.exe へ実行ファイルが作成される
  * Visual Studio を開いてコンフィギュレーションを Development Client にしてビルド
    * Binaries/Win64/UE4Shooter.exe へ実行ファイルが作成される
  * 実行ファイルを**パッケージ化先ディレクトリ**へコピーする
  * サーバ実行ファイルを起動する
    * -log をつけるとログウィンドウが出る
  * クライアント実行ファイルを起動する
    * コンソール(@)から `open 127.0.0.1:7777`。サーバに入れたら成功

* DefaultEngine.ini に URLの項目を追加しないとダメ？
~~~
[URL]
GameName=UE4Shooter
~~~