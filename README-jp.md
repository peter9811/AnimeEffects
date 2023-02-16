# AnimeEffects

🇺🇸 [English](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README.md)

慎重に検討した計画を必要としない2Dアニメーションツールでは、ポリゴンメッシュの変形に基づいてさまざまな機能を提供することにより、アニメーションを簡素化します。
もともとHidefukuによって開発されていましたが、現在はコミュニティによって開発および維持されています。

* 公式サイト:<br>
  * *工事中* <br>

* 公式ソーシャル:<br>
  * Discord: <a href='https://discord.gg/sKp8Srm'>AnimeEffects コミュニティサーバー</a> (@Jose-Moreno の厚意により)<br>
  * Twitter: <a href='https://twitter.com/anime_effects'>AnimeEffects</a> (@p_yukusai が維持)<br>

注：現在の場合、互換性のない変更が行われる可能性があります。これらは、発生した場合に影響を受けるリリースで知られるようになります。<br>
***もし、何か問題があったり、新しい機能を提案したい場合は、私たちのソーシャルに連絡してください。***

## ダウンロード
* AnimeEffectsの最新リリースを[公開しましたちら](https://github.com/AnimeEffectsDevs/AnimeEffects/releases) 、使用方法は、任意のフォルダに解凍し、実行ファイルを実行するだけです。<br>

## 要件
* Windows/Linux/Mac
  * 下記の対応バージョンをご覧ください。
* OpenGl 3.3 以上
  * Linuxでは、お使いのグラフィックカードがOpenGL3.3のCoreProfileをサポートしているかどうかを確認するために、ターミナル上で「glxinfo | grep "OpenGL core profile version"」と実行します。
* [FFmpeg](https://ffmpeg.org/download.html) (ビデオエクスポートに必要な場合、パスに配置したり、「/tools」フォルダーにコピーしたりできます。)

## OSターゲット
#### これらのOSは、私たちがソフトウェアをコンパイルし、テストしているバージョンであり、古いバージョンでも動作する場合がありますが、これは推奨されません。
* Windows 10 以降。
* Ubuntu LTS 以降。
  * 提供されたAppImageは、glibcの関係で古いバージョンでは*動作しない*。
* macOS Big Sur 以降。
  * 私たちは誰もアニメエフェクトをテストするためのMacを持っていませんが、コンパイルエラーとアーティファクトを記録しています。

## 開発要件
* Qt5.14 以上
* MSVC2015/MinGW/GCC/Clang (32-bitまたは64-bit)

## Linux
### 依存関係のインストール
#### Debian / Ubuntu

* 最初に更新してから、依存関係をインストールします:

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git gcc libglib2.0-0 qt5-default make
```

#### Arch / Manjaro
* 最初に更新してから、依存関係をインストールします:  

```
sudo pacman -Syu
sudo pacman -S git gcc glib2 qt5 make
```

### 複製 / コンパイル
* AnimeEffectsのgitリポジトリを複製し、「/src」フォルダに移動します: 

```
git clone https://github.com/herace/AnimeEffects  
cd AnimeEffects/src
qmake AnimeEffects.pro
make
```
* ビルドが完了したら、AnimeEffectsを実行します:
```
./AnimeEffects  
```

## Windows
* コンパイルにはQtCreatorを使用することを推奨します:
```
QtCreatorを使用していない場合は、選択したコンパイラとそのツールの「/bin」フォルダをパスに追加し、ビルドとデプロイに利用できるパワーシェルスクリプトをチェックアウトすることをお勧めします「MinGWを推奨します」
プロジェクトをクローンし、QtCreatorで 「AnimeEffects.pro」を開きます
リリースプロファイルでプロジェクトをコンパイルする 
お好みのコンソールを開く
「windeployqt.exe --release "実行_可能_パス"」を実行します
```

* 展開が完了したら、「AnimeEffects.exe」を実行するだけでよいでしょう。
