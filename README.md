# UE4Shooter

## 使用アセット

* Starter Content
  * エディタから Add New - Add Feature or Content Pack ... を選択してインストール可能
* Animation Starter Pack
    * https://www.unrealengine.com/marketplace/animation-starter-pack
* Prototype Weapons
    * https://www.unrealengine.com/marketplace/prototype-weapons-01
* Crosshair
    * https://wiki.unrealengine.com/First_Person_Shooter_C%2B%2B_Tutorial

## [Default Class](https://github.com/horinoh/UE4Shooter/tree/master/Document/DefaultClass)
## [Steam](https://github.com/horinoh/UE4Steam/tree/master/)
## [インプット](https://github.com/horinoh/UE4Shooter/tree/master/Document/Input)
## [コリジョン](https://github.com/horinoh/UE4Shooter/tree/master/Document/Collision)
## [キャラクタ](https://github.com/horinoh/UE4Shooter/tree/master/Document/Character)
## [アニメーション](https://github.com/horinoh/UE4Shooter/tree/master/Document/Animation)
## [フォースフィードバック](https://github.com/horinoh/UE4Shooter/tree/master/Document/ForceFeedback)
## [パッケージ化](https://github.com/horinoh/UE4DedicatedServer/tree/master/Document/Packaging)
## [専用サーバ](https://github.com/horinoh/UE4DedicatedServer/tree/master/Document/DedicatedServer)

<!--
Edit - Editor Preference - General - Global - Derived Data - Shared を設定することによりシェーダコンパイルを共有できる
    http://historia.co.jp/archives/9294/

GenerateProjectFiles.bat で「タスククラスが見つかりません」と怒られる場合
	VSインストーラ - 変更 - 個別のコンポーネント - コートツール - NuGetパッケージマネージャ

Visual Assist X の設定
	https://docs.wholetomato.com/default.asp?W804
UE4用の設定
    VAssistX - Visual Assist Options - Unreal Engine - Enable supoort for Unreal Engine4 にチェック
デフォルトのインテリセンスをやめる
    VAssistX - Visual Assist Options - Enhanced Listboxes - Source of C/C++ content - Visual Assit を選択

nldef.h が無いと言われてコンパイルが通らない
    vs2015をアンインストールしたせい、vs2017を使用する場合でも、(無駄に)vs2015をインストールしておかないとダメみたい…

Visual Studio(VS)を英語版にする
    インストーラを起動 - 変更 - 言語パックタブ - 英語にチェック - 変更
    VSを起動 - ツール - オプション - 環境 - 国際対応の設定 - English - OK - VSを再起動

GenerateProjectFiles.bat 次のようにで怒られる ".NETFramework,Version=v4.6.2" の参照アセンブリが見つかりませんでした。
    VisualStudioインストーラ - 変更からインストールする
    .NET desktop development にチェックが入っていても、右側の 4.6.2 にチェックが入っていないとダメなので注意

UE4初期設定でやること
    Edit - Editor Preference - General
        Region & Language : 言語を英語にする
        Misclleraneous - Hot Reload - Automatically Compile Newly...のチェックを外す
        Global - Derived Data - Shared Derived Data Cache : フォルダを指定する
        Live Coding - Enable Live Codingにチェックを入れる
-->

<!--
TODO

○ オンラインサブシステム対応
○ 軌跡エフェクト対応
○ AnimInstance 条件遷移調査
    立ち - しゃがみ
    立ち - ほふく(Prone対応する場合)

○ エイム(Ironsight)対応する?
○ 非専用サーバ対応する？
○ Prone 対応する？
○ Knife(Melee)対応する？
-->



<!--
○ WIKI 覚書

[リンク](https://github.com/horinoh/UE4Shooter/Document/XXX.md)
![画像](Document/XXX.png)

__強調__
___強い強調___

~~打ち消し~~

`void main()/*コード*/`
~~~
//!< コード
void main()
~~~

| テーブル | YYY | ZZZ |
|:-:|:-:|:-:|
| aaa | bbb | ccc |
| ddd | eee | fff |
-->