# VGS-Zero SDK for Steam

Steam で VGS-Zero のゲームを販売するための SDK です

## Support Platform

- [ ] Windows
- [x] Linux
- [x] macOS

## How to Build

1. Steamworks で App クレジットを購入して AppID を入手
2. Steamworks SDK をダウンロードして `public` と `redistributable_bin` を [`./sdk`](./sdk) 以下にコピー
3. 販売する `game.pkg` をこのリポジトリ直下に配置
4. `steam_appid.txt` を作成して AppID を記述
5. `./game_actions_X.vdf` を Steam クライアントのインストール先の `controller_config` にコピーして `X` の箇所を AppID にリネーム
6. `make` (Windows の場合は `make.bat`) を実行

## Game Contorller Setup

ビルドが完了した時点では SteamInput のレイアウト情報が設定されていないため、ゲームコントローラでの入力ができません。

以下の手順を実施することで、ローカルビルドでもゲームコントローラでの入力ができるようになります。

1. ビルドが成功後 Steamworks のデポへ `./release` 以下を zip で固めたビルドをアップロード
2. Steam クライアントのライブラリからゲームのページを開く
3. ゲームコントローラのアイコンをクリックしてレイアウトを適切に編集
4. レイアウトを共有保存
5. Steamworks でデフォルトレイアウトを設定

## Licenses

販売時に同梱する README.txt や製品のランディングページ等に次のライセンス情報を必ず記載してください。

- [PicoJSON - a C++ JSON parser / serializer](./LICENSE-PICOJSON.txt) ※Windowsのみ
- [Simple DirectMedia Layer](./LICENSE-SDL.txt) ※Linux/macOSのみ
- [SUZUKIPLAN - Z80 Emulator](./LICENSE-Z80.txt)
- [VGS-Zero](./LICENSE-VGS0.txt)
- [VGS-Zero Library for Z80](./LICENSE-VGS0LIB.txt)
