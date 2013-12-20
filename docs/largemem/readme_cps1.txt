----------------------------------------------------------------------

  Unofficial CAPCOM CPS1 Emulator for PSP 2.2.1 for Large Memory Mod

                                NJ (http://nj-emu.hp.infoseek.co.jp)
----------------------------------------------------------------------

<概要>

  PSP用のCAPCOM CPS1(Capcom Play System)エミュレータです。

----------------------------------------------------------------------

 このプログラムは PSP-2000,3000,Go,Street + CFW 3.71 M33以降専用です。

----------------------------------------------------------------------
対応ROMセットについて

  MAME 0.119に準拠させているため、zipファイル名はMAME 0.119が
  要求するROMセット名と一致させる必要があります。

  全てのROMイメージファイルはzipファイルに圧縮する必要があります。
  フォルダに展開したファイルを扱えないことを除けば、基本的にMAMEと
  全く同じです。また、MAMEが対応していないROMセットには対応しません。

  ファイルブラウザ上で白く表示されているゲームは全て動作します。
  動作しない場合はROMセットが要求するものと異なっているということです。
  ClrMame ProやRomCenter等のツールを使って、MAME 0.119のROMセットに
  一致させてください。

  どうしても他のエミュレータや古いMAMEのROMセットを動作させたいと
  いうのであれば、rominfo.datをnotepad等で書き換えればいいのですが、
  お勧めはしません。rominfo.datの内容を見て意味がわからなければ
  変更しないでください。

----------------------------------------------------------------------
ディレクトリ設定

ディレクトリは全て初回起動時に自動的に作成されます。

 /PSP/GAME/  (または /PSP/GAME371/)
      |
      +- CPS1PSP/  (root directory)
         |  |
         |  +- EBOOT.PBP    CPS1PSP本体
         |  +- cps1psp.ini  CPS1PSP設定情報記録ファイル (自動的に作成されます)
         |  +- command.dat  MAME Plus!用command.dat (コマンドリスト用/任意)
         |
         +- config/   ゲーム個別設定ファイル用ディレクトリ
         |
         +- nvram/    EEPROMデータ保存用ディレクトリ
         |
         +- snap/     スクリーンショット用ディレクトリ
         |
         +- state/    ステートデータ用ディレクトリ
         |
         +- roms/     ROMイメージファイル用ディレクトリ (ZIP形式で圧縮すること)
         |    |
         |    +- 1941.zip   (例: 1941)
         |    +- sf2.zip    (例: Street Fighter II (parent))
         |    +- sf2j.zip   (例: Street Fighter II (clone: Japanese version))
         |    +- ...
         +- cheats/   チートファイル用フォルダ
         |    |
         |    +- 1941.ini (例: 1941)
         |    +- ...

----------------------------------------------------------------------
resource_jp.zipについて

  英語版のresource_jp.zipに含まれるファイルは、ゲームリストとコマンド
  リストで日本語を使用する場合に必要なファイルです。

  日本語を使用しないのであれば不要ですので削除してください。

  日本語で表示を行う場合は、resource_jp.zipに含まれるファイルを
  /PSP/GAME/CPS1PSP/にそのままコピーしてください。

----------------------------------------------------------------------
操作方法

・ゲーム実行中の画面とメインメニューを除き、全てのメニューで"Rトリガ"
  を押すことで操作ヘルプが表示されるようになっています。
  わからなければとにかく"Rトリガ"を押してください。
  見ればわかると思うので、詳細は割愛します。

・ゲームの設定等を変更するメニューは、ゲーム実行中に"START + SELECT"を
  押すことで表示されます。

・ゲーム中のボタン操作
  ボタンの割り当ては変更可能です。以下にデフォルトの設定を書いておきます。

  以下の場合には、自動的にボタン配置を画面に合わせて反転/回転しますので、
  特に意識して変更する必要はありません。
  ・DIPスイッチでCabinetの項目をCooktailに設定した場合の2 Player操作時
  ・DIPスイッチでFlip Screenの項目をOnにした場合
  ・縦画面のゲームでRotate ScreenをYesにした場合

  共通
    Up    - Up or Analog Up
    Down  - Down or Analog Down
    Left  - Left or Analog Left
    Right - Right or Analog Right
    Start - Start
    Coin  - Select

  2ボタンのゲーム
    Button 1 - Square
    Button 2 - Triangle

  3ボタンのゲーム
    Button 1 - Square
    Button 2 - Triangle
    Button 3 - Cross

  クイズゲーム (方向ボタンは使用しません)
    Button 1 - Square
    Button 2 - Triangle
    Button 3 - Cross
    Button 4 - Circle
    プレイヤー切り替え - L trigger

  Street Fighter II系のゲーム (Street Fighter Zero CPS Changer ver.含む)
    Button 1 - Square
    Button 2 - Triangle
    Button 3 - L trigger
    Button 4 - Cross
    Button 5 - Circle
    Button 6 - R trigger

  Forgotton World / Lost World
    (ループレバーはPSPではどうにもならないので、L/Rトリガで代用)
    Button 1 - Square
    Dial(rotate left) - L trigger
    Dial(rotate right) - R trigger

  特殊操作
    START + SELECT: メニューを開く
    L + R + SELECT: サービススイッチ (特定のボタンに割り当ても可能)
    L + R + START:  1P & 2P START (サービスメニューで使用)
