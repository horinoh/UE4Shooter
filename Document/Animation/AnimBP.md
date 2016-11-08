# アニメーションBP

## ABP_UE4Mannequin を作成

* AnimGraph にステートマシン(Locomotion)を作成
* ステートを追加、接続を行う (条件式はC++からやるので書かない)
    * Stand ... BS_Stand をアサイン、Speed, Direction を接続 
        * (変数はShow Inherited Variablesにチェックを入れないと出てこないので注意)
    * Crouch ... BS_Crouch をアサイン、Speed, Direction を接続
    * JumpStart, JumpLoop, JumpEnd
    * Sprint

![画像](AnimBP_StateMachine.png)

* 
    * AnimGraph 内で New Saved cache pose ノードを作成、入力に Locomotion の出力を接続
    * Layered blend per bone ノードを作成
        * Config - Layer Setup - 0 - Branch Filters - + で増やす
        * 0 - Bone Name に "spine_01"
    * Use cached pose 'SavePose' ノードを作成、Layered blend per bone の Base Pose へ接続
    * Slot 'DefaultSlot' ノードを作成
        * Use cached pose 'SavePose' ノードを作成、Slot 'DefaultSlot' の入力へ接続
        * 出力を Layered blend per bone の Blend Poses0 へ接続
        * Slot Name を UpperBody へ変更 (UpperBody スロットが無い場合は追加する)

![画像](AnimBP_LayeredBLend.png)

* ループの設定は、ステートに付けたアニメーションを選択して Detail - Settings - LoopAnimation で行う