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