# 専用サーバ

## [パッケージ化](https://github.com/horinoh/UE4Shooter/tree/master/Document/Packaging)する

## ビルド
* UE4Shooter**Server**.Target.cs を作成 (基本UE4Shooter**Editor**.Target.cs をコピーして作成)
  * .uproject 右クリック - Generate Visual Studio project files
  * Visual Studio を開いてコンフィギュレーションを XXXServer(Development Server等) にしてビルド
  * Binaries/Win64/UE4ShooterServer.exe へ実行ファイルが作成される
  * 実行ファイルを**パッケージ化先ディレクトリ**へコピーする
  * 実行ファイルを起動する (-log をつけるとログウィンドウが出る)
  * 確認
    * コンソール(@)から `open 127.0.0.1:7777`。サーバに入れたら成功