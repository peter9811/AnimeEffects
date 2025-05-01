# AnimeEffects

🇺🇸 [English](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README.md)

レイヤー構造や設計を気にすることなく、手軽に制作できる 2D アニメーションツールです。ポリゴンメッシュの変形をベースとした様々な機能により、アニメーション制作を簡素化することができます。
もともと Hidefuku によって開発されていましたが、現在は有志コミュニティによって開発およびメンテナンスされています。

- 公式サイト:<br>

  - https://animeeffectsdevs.github.io/<br>

- 公式 SNS:<br>
  - Discord: <a href='https://discord.gg/sKp8Srm'>AnimeEffects コミュニティサーバー</a> (@Jose-Moreno の厚意により)<br>
  - Twitter: <a href='https://twitter.com/anime_effects'>AnimeEffects</a> (@p_yukusai によって運営されています)<br>

注意: 今後、互換性のない変更が行われる可能性があります。影響を受ける変更があった場合は、リリースの時に通知されます。<br>
**_もし、何か問題があったり、新しい機能を提案したい場合は、公式 SNS にて連絡してください。_**

## ダウンロード

- AnimeEffects の[最新リリース](https://github.com/AnimeEffectsDevs/AnimeEffects/releases) からダウンロードしてください。任意のフォルダに解凍し、実行ファイルを実行するだけです。<br>

## 要件

- Windows/Linux/Mac
  - 具体的な対応要件については、下記の詳細をご覧ください。
- OpenGl 3.3 以上
  - Linux の場合、コマンドラインにて`glxinfo | grep "OpenGL core profile version`を実行し、グラフィックカードが OpenGL3.3 の CoreProfile をサポートしているかどうかを確認してください。
- [FFmpeg](https://ffmpeg.org/download.html) (動画出力に必要です。任意の実行パスに配置するか、「/tools」フォルダーに実行ファイルを配置してください。)

## OS ターゲット

#### これらの OS は、私たちがソフトウェアをコンパイルし、テストしているバージョンです。これより古いバージョンでも動作する場合がありますが非推奨です。

- Windows 10 以降。
- Ubuntu LTS 以降。
  - 提供された AppImage は、glibc の関係で古いバージョンでは*動作しません*。
- macOS Big Sur 以降。
  - 私たちは誰も AnimeEffects をテストするための Mac を持っていませんが、コンパイルエラーとアーティファクトを記録しています。

## 開発要件

- Qt5.14 以上
- MSVC2015/MinGW/GCC/Clang (32-bit または 64-bit)

## Linux

### 依存関係のインストール

#### Debian / Ubuntu

- 最初に更新してから、依存関係をインストールします:

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git gcc libglib2.0-0 qt5-default make
```

#### Arch / Manjaro

- 最初に更新してから、依存関係をインストールします:

```
sudo pacman -Syu
sudo pacman -S git gcc glib2 qt5 make
```

### 複製 / コンパイル

- AnimeEffects の git リポジトリを複製し、「/src」フォルダに移動します:

```
git clone https://github.com/herace/AnimeEffects
cd AnimeEffects/src
qmake AnimeEffects.pro
make
```

- ビルドが完了したら、AnimeEffects を実行します:

```
./AnimeEffects
```

## Windows

- コンパイルには QtCreator を使用することを推奨します:

```
QtCreatorを使用していない場合は、選択したコンパイラとそのツールの「/bin」フォルダをパスに追加し、
ビルドとデプロイに利用できるPowershell Scriptをチェックアウトすることをお勧めします(MinGW 推奨です)。
プロジェクトをクローンし、QtCreatorで 「AnimeEffects.pro」を開きます。
リリースプロファイルでプロジェクトをコンパイルしてください。
コンソールを開き、「windeployqt.exe --release "実行ファイルパス"」を実行します
```

- デブロイが完了したら、AnimeEffects.exe を実行してください。
