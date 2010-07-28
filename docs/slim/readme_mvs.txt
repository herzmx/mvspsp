----------------------------------------------------------------------

              NEOGEO Emulator for PSP 2.2.1 for PSP Slim

                                 NJ (http://nj-emu.hp.infoseek.co.jp)
----------------------------------------------------------------------

<概要>

PSP用のNEOGEO(MVS/AES)エミュレータです。

----------------------------------------------------------------------

     このプログラムは PSP-2000 + CFW 3.71 M33以降専用です。

----------------------------------------------------------------------
2.0.6以前をお使いの方へ

  rominfo.mvsの構造が変更したため、2.0.6のものはすべて破棄してこの
  ファイルに付属するものを使用してください。
  また、2.0.6のキャッシュファイルは使用できないため、全て作り直して
  ください。

  キャッシュ不要なゲームがいくつか増えたとは思いますが、どのゲームが
  キャッシュ不要になったかは確認していないため、まずはキャッシュなしで
  起動するかどうか試し、エラーが表示されたらそのゲーム用のキャッシュを
  作成してください。

  また、一部のゲームのクローンセットは、親セットのキャッシュが使用可能
  な場合にはそちらを共有出来るようにしましたので、下記の一覧を参考に
  してください。

----------------------------------------------------------------------
対応ROMセットについて

  MAME 0.119に準拠させているため、zipファイル名はMAME 0.119が
  要求するROMセット名と一致させる必要があります。

  基本的にNeoRageX用のROMセットは使用できないと思ってください。
  Kawaks、FBA、NebulaのROMセットはほぼそのまま利用できるでしょう。
  (NeoRageXは速度は速いですが、他のNEOGEOエミュレータと比較すると、
  再現度の点では劣ります。いまだに推奨しているサイトもあるようですが、
  古いエミュレータなので、はっきり言って使用すべきではないと思います。)

  全てのROMイメージファイルはzipファイルに圧縮する必要があります。
  フォルダに展開したファイルを扱えないことを除けば、基本的にMAMEと
  全く同じです。また、MAMEが対応していないROMセットには対応しません。

  ファイルブラウザ上で白く表示されているゲームは全て動作します。
  動作しない場合はROMセットが要求するものと異なっているということです。
  ClrMame ProやRomCenter等のツールを使って、MAME 0.119のROMセット
  に一致させてください。

  どうしても他のエミュレータや古いMAMEのROMセットを動作させたいと
  いうのであれば、rominfo.datをnotepad等で書き換えればいいのですが、
  お勧めはしません。rominfo.datの内容を見て意味がわからなければ
  変更しないでください。

  ファイルブラウザ上でグレーで表示されているゲームは暗号化キーが解読
  されていないもので、現状ではこれらは起動できません。

----------------------------------------------------------------------
ディレクトリ設定

ディレクトリは全て初回起動時に自動的に作成されます。

 /PSP/GAME/  (または/PSP/GAME371/)
      |
      +- MVSPSP/  (root directory)
         |  |
         |  +- EBOOT.PBP    MVSPSP本体
         |  +- mvspsp.ini   MVSPSP設定情報記録ファイル (自動的に作成されます)
         |  +- command.dat  MAME Plus!用command.dat (コマンドリスト用/任意)
         |
         +- cache/    キャッシュファイル用ディレクトリ(romcnv_mvs.exeで作成)
         |  |
         |  +- mslug_cache/   (例: Metal Slug) ※作成されたフォルダをコピー
         |
         +- config/   ゲーム個別設定ファイル用ディレクトリ
         |
         +- memcard/  メモリカード用ディレクトリ
         |
         +- nvram/    SRAMデータ保存用ディレクトリ
         |
         +- snap/     スクリーンショット用ディレクトリ
         |
         +- state/    ステートデータ用ディレクトリ
         |
         +- roms/     ROMイメージファイル用ディレクトリ (ZIP形式で圧縮すること)
         |    |
         |    +- neogeo.zip   (NEOGEO BIOS)
         |    +- samsho.zip   (例: サムライスピリッツ)
         |    +- ...

・BIOSはneogeo.zipというファイルにまとめ、romsフォルダに置いてください。

・各ゲームのROMファイル名はどんな名前でも構いませんが、"CRCはMAME 0.119
  のROMセットのCRCと一致"している必要があります。

----------------------------------------------------------------------
resource_jp.zipについて

  英語版のresource_jp.zipに含まれるファイルは、ゲームリストとコマンド
  リストで日本語を使用する場合に必要なファイルです。

  日本語を使用しないのであれば不要ですので削除してください。

  日本語で表示を行う場合は、resource_jp.zipに含まれるファイルを
  /PSP/GAME/MVSPSP/にそのままコピーしてください。

----------------------------------------------------------------------
ROMセットについての補足

・bootleg setについては、bootleg対応版のみで動作します。
  通常版と分けているのは、bootleg対応版は余分な処理が増えるため、確保
  できるメモリが減り、実行速度も若干落ちるためです。

・以下のゲームはMAMEでは動作しますが、このエミュレータではメモリ不足に
  より起動できないため未対応です。


・以下のROMセットは、MAMEのROMセットと親子関係が異なるため注意して
  ください。(※いずれもbootleg対応版のみ動作可能)

   親セット    クローンセット

   garoup   --- garoubl

   svcboot  -+- svcplus
             +- svcplusa
             +- svcsplus

   kof2k4se --- kf2k4pls

   kof10th  -+- kf10thep
             +- kf2k5uni

   kf2k3bl  -+- kf2k3bla
             +- kf2k3pl
             +- kf2k3upl

----------------------------------------------------------------------
キャッシュファイルの作成

・ROM読み込み時に"メモリが足りない"というエラーが表示される場合は、
  グラフィックデータのキャッシュを作成する必要があります。
  付属のromcnv_mvs.exeで作成してください。使い方はromcnv_mvs.exeの
  readme_mvs.txtを参照してください。

・以下のクローンセットは親セットのキャッシュがあればそちらを使用する
  ので、親セットのみ作成すればOKです。
  (括弧内が親セット名、bootlegはbootleg対応版でのみ動作)

   aof2a     (aof2)
   fatfursa  (fatfursp)
   kof95a    (kof95)
   samsho3a  (samsho3)
   fswords   (samsho3)
   aof3k     (aof3)
   kof96h    (kof96)
   kof96ep   (kof96)      ※bootleg
   kof97a    (kof97)
   kof97pls  (kof97)      ※bootleg
   lastbldh  (lastblad)
   lastsold  (lastblad)
   shocktra  (shocktro)
   rbff2h    (rbff2)
   rbff2k    (rbff2)
   kof98k    (kof98)
   kof98n    (kof98)
   kof99a    (kof99)
   kof99e    (kof99)
   kof99n    (kof99)
   garouo    (garou)
   mslug3n   (mslug3)
   kof2000n  (kof2000)
   kof2001h  (kof2001)
   kf2k1pls  (kof2001)    ※bootleg
   kf2k1pa   (kof2001)    ※bootleg
   ms4plus   (mslug4)     ※bootleg
   kf2k2pls  (kof2002)    ※bootleg
   kf2k2pla  (kof2002)    ※bootleg
   kf2k2plb  (kof2002)    ※bootleg
   mslug5b   (mslug5)     ※bootleg
   svcpcba   (svcpcb)
   samsho5h  (samsho5)
   samsh5sh  (samsh5sp)
   samsh5sn  (samsh5sp)
   kf2k5uni  (kof10th)    ※bootleg / MAMEと親セットが異なる
   svcplusa  (svcboot)    ※bootleg / MAMEと親セットが異なる
   kf2k3bla  (kf2k3bl)    ※bootleg / MAMEと親セットが異なる

・以下のクローンセットは一部のキャッシュファイルを親セットの
  キャッシュと共有します。[]内のファイルが親セットのキャッシュに
  あればそちらを使用します。
  (括弧内が親セット名、bootkegはbootleg対応版でのみ動作)

   kof97pla  (kof97)    [crom/vrom] ※bootleg
   kog       (kof97)    [vrom]      ※bootleg
   kof99p    (kof99)    [vrom]
   garoubl   (garoup)   [crom]      ※bootleg / MAMEと親セットが異なる
   mslug3b6  (mslug3)   [crom/vrom] ※bootleg
   cthd2003  (kof2001)  [vrom]      ※bootleg
   ct2k3sp   (kof2001)  [vrom]      ※bootleg
   kof2002b  (kof2002)  [srom/vrom] ※bootleg
   kf2k2plc  (kof2002)  [crom/vrom] ※bootleg
   kf2k2mp   (kof2002)  [crom/vrom] ※bootleg
   kf2k2mp2  (kof2002)  [crom/vrom] ※bootleg
   matrimbl  (matrim)   [crom/srom] ※bootleg
   ms5plus   (mslug5)   [crom/srom] ※bootleg
   kf2k4pls  (kof2k4se) [crom/srom] ※bootleg / MAMEと親セットが異なる
   kf10thep  (kof10th)  [vrom]      ※bootleg / MAMEと親セットが異なる
   svcplus   (svcboot)  [crom/vrom] ※bootleg / MAMEと親セットが異なる
   svcsplus  (svcboot)  [crom/vrom] ※bootleg / MAMEと親セットが異なる
   kf2k3pl   (kf2k3bl)  [crom/vrom] ※bootleg / MAMEと親セットが異なる
   kf2k3upl  (kf2k3bl)  [crom/vrom] ※bootleg / MAMEと親セットが異なる

----------------------------------------------------------------------
操作方法

※BIOSの設定画面はファイルブラウザ実行中に"START"を押すことで表示
  されます。"必ずここで設定してから"ゲームを起動してください。

・ゲーム実行中の画面とメインメニューを除き、全てのメニューで"Rトリガ"
  を押すことで操作ヘルプが表示されるようになっています。
  わからなければとにかく"Rトリガ"を押してください。
  見ればわかると思うので、詳細は割愛します。

・ゲームの設定等を変更するメニューは、ゲーム実行中に"HOME"を押すことで
  表示されます。(ユーザーモードでコンパイルした場合は"START + SELECT"に
  なります。)

・ゲーム中のボタン操作
  ボタンの割り当ては変更可能です。以下にデフォルトの設定を書いておきます。
  (※ver.2.1.0より初期ボタン配置が変更されています。)

    Up       - Up or Analog Up
    Down     - Down or Analog Down
    Left     - Left or Analog Left
    Right    - Right or Analog Right
    Start    - Start
    Coin     - Select
    Button A - Square
    Button B - Triangle
    Button C - Cross
    Button D - Circle

  特殊操作
    START + SELECT: メニューを開く
    L + R + SELECT: サービススイッチ (特定のボタンに割り当ても可能)

----------------------------------------------------------------------
BIOSのRegion/Machine Modeの変更について

・MVS BIOSを使用している場合は、ゲーム設定メニュー内で変更できるように
  していますが、完全に動作するわけではありません。
  後期のゲームの場合、この設定を変更するとプロテクトに引っかかり動作
  しないものがあります。
  また、AESのBIOSでMVSのゲームを動作させようとした場合も、同様に
  プロテクトに引っかかって動作しない場合があります。

・確実に変更したいのであれば、uni-biosを使用してください。
  uni-biosはそれ自体が言語/モード変更機能を備えており、他のBIOSのように
  エミュレータから変更することは出来ません。
  起動直後のuni-biosのロゴが表示されている間に"A+B+C"を押すと、uni-bios
  のメニューが開きますので、このメニュー内で設定を変更してください。
  なお、v2.0以降はスキャンライン割り込みを処理しないと動作しないため、
  実行速度が少し低下します。

・ASIA-AES及びDEBUG BIOSではこれらの設定は表示されません。

----------------------------------------------------------------------
その他

・メモリカードのファイルはゲームごとに作成されます。
  また、メモリカードは常に認識した状態になっています。

・uni-biosのゲームメニューはゲームプレイ中に"START+COIN"で表示されます。
  チート機能等が使用可能です(ユーザーモードでコンパイルした場合は
  ボタンがかぶるため、使用できません)。

・PCB版のゲームはNEOGEOのデータを流用した専用基板であり、MVSではあり
  ません。当たり前ですが、uni-bios等も使用できません。
