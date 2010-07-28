----------------------------------------------------------------------

                  CAPCOM CPS2 Emulator for PSP 2.2.1

                                NJ (http://nj-emu.hp.infoseek.co.jp)
----------------------------------------------------------------------

<概要>

  PSP用のCAPCOM CPS2(Capcom Play System II)エミュレータです。

----------------------------------------------------------------------
対応ROMセットについて

  MAME 0.112u2以降に準拠させているため、zipファイル名はMAME 0.119が
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

  ファイルブラウザ上でグレーで表示されているゲームは起動できません。

----------------------------------------------------------------------
ディレクトリ設定

ディレクトリは全て初回起動時に自動的に作成されます。

 /PSP/GAME/  (CFW3.xxの場合は/PSP/GAME150/)
      |
      +- CPS2PSP/  (root directory)
         |  |
         |  +- EBOOT.PBP    CPS2PSP本体
         |  +- cps2psp.ini  CPS2PSP設定情報記録ファイル (自動的に作成されます)
         |  +- command.dat  MAME Plus!用command.dat (コマンドリスト用/任意)
         |
         +- cache/    キャッシュファイル用ディレクトリ(romcnv_cps2.exeで作成)
         |  |
         |  +- 19xx_cache.zip (例:19XX用キャッシュ/ZIP圧縮)
         |  +- ssf2.cache     (例:ssf2用キャッシュ/無圧縮)
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
         |    +- 19xx.zip   (例: 19xx)
         |    +- ssf2.zip   (例: Super Street Fighter II (parent))
         |    +- ssf2j.zip  (例: Super Street Fighter II (clone: Japanese version))
         |    +- ...

----------------------------------------------------------------------
resource_jp.zipについて

  英語版のresource_jp.zipに含まれるファイルは、ゲームリストとコマンド
  リストで日本語を使用する場合に必要なファイルです。

  日本語を使用しないのであれば不要ですので削除してください。

  日本語で表示を行う場合は、resource_jp.zipに含まれるファイルを
  /PSP/GAME/CPS2PSP/にそのままコピーしてください。

----------------------------------------------------------------------
キャッシュファイルの作成

  全てのゲームでグラフィックデータのキャッシュを作成する必要が
  あります。付属のromcnv_cps2.exeで作成してください。使い方は
  romcnv_cps2.exeのreadme_cps2.txtを参照してください。

  基本的には親セットのキャッシュのみ作成すれば良いです。
  各ゲーム毎に作成しても構いません。

  以下のゲームのみ特殊ですのでご注意ください。

  ・Super Street Fighter II Turbo及びそのクローンセット
    (日本語版はスーパーストリートファイターIIXです。)
    このゲームは親セットがssf2ですが、ssf2にグラフィックを追加して
    いるため、ssf2のキャッシュでは正常に動作しません。
    ssf2tを指定してキャッシュファイルを作成するか、個別にキャッシュを
    作成してください。

  ・Mighty! Pang
    USA版と日本語版ではグラフィックROMが異なるようです。
    日本語版はUSA版のクローンセットになっていますが、キャッシュは
    それぞれ別に作成する必要があります。

----------------------------------------------------------------------
操作方法

・ゲーム実行中の画面とメインメニューを除き、全てのメニューで"Rトリガ"
  を押すことで操作ヘルプが表示されるようになっています。
  わからなければとにかく"Rトリガ"を押してください。
  見ればわかると思うので、詳細は割愛します。

・ゲームの設定等を変更するメニューは、ゲーム実行中に"HOME"を押すことで
  表示されます。(ユーザーモードでコンパイルした場合は"START + SELECT"に
  なります。)

・ゲーム中のボタン操作
  ボタンの割り当ては変更可能です。以下にデフォルトの設定を書いておきます。

  以下の場合には、自動的にボタン配置を画面に合わせて反転/回転しますので、
  特に意識して変更する必要はありません。
  ・サービスメニューでFlip Screenの項目を有効にした場合
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

  6ボタンのゲーム
    Button 1 - Square
    Button 2 - Triangle
    Button 3 - L trigger
    Button 4 - Cross
    Button 5 - Circle
    Button 6 - R trigger

  クイズなないろドリームス専用 (方向ボタンは使用しません)
    Button 1 - Square
    Button 2 - Cross
    Button 3 - Triangle
    Button 4 - Circle
    プレイヤー切り替え - L trigger

  特殊操作
    HOME: メニューを開く
    L + R + SELECT: サービススイッチ (特定のボタンに割り当ても可能)
    L + R + START:  1P & 2P START (サービスメニューで使用)

  AdHoc時
  ※ファイルブラウザで□(Square)でゲームを選択するとAdHocモードで
    ゲームを起動します。
    HOME: ポーズ、切断ダイアログの表示
