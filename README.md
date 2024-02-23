# VGS-Zero SDK for Steam

Steam で VGS-Zero のゲームを販売するための SDK です

## Support Platform

- Windows
- Linux
- macOS

（補足事項）

- ビルドと動作確認には各 OS の実機 PC が必要です（仮想環境については SUZUKIPLAN では検証していません）
- 最低でも Windows と Linux に対応することを推奨します。
  - Windows は利用ユーザーが多いため対応することで売上が向上するものと思われます
  - Linux に対応することで SteamDeck 互換性審査をパスできる可能性があがるものと想定
  - Windows バイナリのみでも Proton で動作できる可能性がありますが、SUZUKIPLAN では Linux ネイティブ・バイナリでの SteamDeck 対応しか想定していないため、VGS-Zero の Proton での動作検証を実施していません
- macOS は余力がある場合にのみ対応で問題無いものと思われます

## Prerequest

### Windows

- Visual Studio (なるべく新しいバージョン)
  - コマンドライン環境のみ利用します（Visual Studio の IDE は使いません）
- git (なるべく新しいバージョン)

### Linux

```bash
# install GNU Make, GCC and other
sudo apt install build-essential

# install SDL2
sudo apt-get install libsdl2-dev

# install ALSA
sudo apt-get install libasound2
sudo apt-get install libasound2-dev
```

### macOS

- HomeBrew
- XCODE Command Line Tools

```bash
# install SDL2
brew install sdl2
```

## Setup Project

ターミナルで次のコマンドを実行して Steam 対応する game.pkg のリポジトリを準備してください。

```bash
# 本リポジトリを mygame-steam として取得
git clone https://github.com/suzukiplan/vgszero-steam mygame-steam

# ディレクトリ移動
cd mygame-steam

# .git を削除
rm -rf .git

# submodule を一旦削除
rm .gitmodules
rm -rf vgszero

# README.md を削除
rm README.md

# .git を初期化
git init

# サブモジュールを追加
git submodule add https://github.com/suzukiplan/vgszero

# commit
git add -A
git commit -m "initial commit"
```

> 上記は `bash` 前提で記述しています。
> Windows の場合は `rm` を `DEL /S /Q` に置き換えれば同様の操作ができる筈です。

（Remarks）

- 作成したリポジトリには OSS 化の義務が生じるため、必ず GitHub 等で public リポジトリとして公開してください
- リポジトリ内に game.pkg のバイナリは含めなくても良い形になっています
- リポジトリは OS 毎に分割する必要がありません

## How to Build

1. Steamworks で App クレジットを購入して AppID を入手
2. Steamworks SDK をダウンロードして `public` と `redistributable_bin` を [`./sdk`](./sdk) 以下にコピー
3. 販売する `game.pkg` をこのリポジトリ直下に配置
4. `steam_appid.txt` を作成して AppID を記述
5. [`./game_actions_X.vdf`](./game_actions_X.vdf) を Steam クライアントのインストール先の `controller_config` にコピーして `X` の箇所を AppID にリネーム
6. Windows の場合、アイコンファイルを差し替え（[./src/icon016.ico](./src/icon016.ico), [./src/icon032.ico](./src/icon032.ico), [./src/icon256.ico](./src/icon256.ico)）
7. `make` (Windows の場合は `make.bat`) を実行

## Game Contorller Setup

ビルドが完了した時点では SteamInput のレイアウト情報が設定されていないため、ゲームコントローラでの入力ができません。

以下の手順を実施することで、ローカルビルドでもゲームコントローラでの入力ができるようになります。

1. ビルドが成功後 Steamworks のデポへ `./release` 以下を zip で固めたビルドをアップロード
2. Steamworks で起動オプションを設定
3. Steam クライアントのライブラリからゲームのページを開く
4. ゲームコントローラのアイコンをクリックしてレイアウトを適切に編集
5. レイアウトを共有保存
6. Steamworks でデフォルトレイアウトを設定

Steamworks で設定する起動オプションは次の通りです。

- Windows
  - 実行ファイル: `GAME.exe`
  - 引数: なし
- Linux
  - 実行ファイル: `game`
  - 引数: なし
- macOS
  - 実行ファイル: `game`
  - 引数: `-g Metal`

> Linux は SteamDeck 限定で対応する場合、起動オプションに `-g Vulkan` を指定することでパフォーマンスが良くなります。

## FAQ

- Q. ビルドが成功したが起動しない
  - A. steam_appid.txt が正しいかご確認ください
- Q. 落ちるetc
  - A. log.txt をご確認ください
- Q. ジョイパッドが効かない
  - A. ジョイパッドの入力は Steam クライアントから起動して SteamInput の設定でレイアウトを指定することで利用できるようになります。詳細は Steamworks で公開されている SteamInput のマニュアルをご確認ください。
- Q. なにゆえ SteamInput？ (XInputｶﾞﾖｶｯﾀﾉﾆ...)
  - A1. SteamDeck の互換性審査を通すため
  - A2. OS 毎のゲームパッド対応が魔窟
- Q. バグを見つけた
  - A. [issues](https://github.com/suzukiplan/vgszero-steam/issues) でチケットを切って [twitter@suzukiplan](https://twitter.com/suzukiplan) までご連絡ください
- Q. VisualStudio の IDE で動かしたい
  - A. (　･ω･)もきゅ？
  - _訳: Windows, Linux, macOS でリポジトリ共通化するには CLI ビルド環境がベストなため、Visual Studio Code などのプラットフォームフリーな IDE を利用してくだしあ_
- Q. SteamCloud対応したい
  - A. Steamworks の SteamCloud で サブディレクトリ `save`、パターン `*`、 OS `すべて` のルートパス設定をすれば、Windows、Linux、macOS の全ての OS で共通のセーブデータ（`save.dat`）がクラウドセーブされる形になります
- Q. アチーブメント対応したい
  - A. Steamworks でアチーブメントを登録後、[./src/winmain.cpp](./src/winmain.cpp) と [./src/sdlmain.cpp](./src/sdlmain.cpp) にアチーブメント送信のためのフック処理を実装してください
  - アチーブメント送信のためのフック処理はセーブデータ保存のコールバックでセーブデータの変化内容をバイナリチェックして送信する形（セーブデータとアチーブメント実績を一致させる形）が望ましいと考えられます
  - アチーブメントは `CSteam::unlock` に Steamworks で設定したアチーブメント ID を指定すれば送信できます。
- Q. リーダーボード対応したい
  - A. アチーブメントとだいたい同じ要領で対応できます
- Q. [Battle Marine のランディングページのようなもの](https://battle-marine.web.app/) をつくりたい
  - A. Battle Marine のランディングページは Firebase Hosting を用いて配信しているスタティック HTML+CSS です
  - 複製リポジトリに `doc` ディレクトリを作成して html や css ファイルを配置して `commit`
  - GitHub Pages で `doc` ディレクトリを配信（※テスト）
  - Firebase Hosting の GitHub Actions で `doc` ディレクトリを CDN 配信（※本番）
  - なお、GitHub Pages での配信は商用配信に利用できないため、必ず Firebase Hosting やさくらインターネットなどの商用利用ができる CDN を利用してください（Firebase Hosting ならゲームが大ヒットしない限り通信コストが発生しないためオススメです）

## Licenses

販売時に同梱する [./README.txt](./README.txt) や製品のランディングページ等に次のライセンス情報を必ず記載してください。

- [PicoJSON - a C++ JSON parser / serializer](./LICENSE-PICOJSON.txt) ※Windowsのみ
- [Simple DirectMedia Layer](./LICENSE-SDL.txt) ※Linux/macOSのみ
- [SUZUKIPLAN - Z80 Emulator](./LICENSE-Z80.txt)
- [VGS-Zero](./LICENSE-VGS0.txt)
- [VGS-Zero Library for Z80](./LICENSE-VGS0LIB.txt)
