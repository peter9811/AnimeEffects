# 角度特效

## 下载

|                                                                                                                                                               稳定的 (v1.6)                                                                                                                                                               |                                                                                                                                                                                     每晚一次                                                                                                                                                                                     |                                           源代码                                           |
| :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------: |
| [Windows](https://github.com/AnimeEffectsDevs/AnimeEffects/releases/download/v1.6/AnimeEffects-Installer-Windows.exe) - [MacOS](https://github.com/AnimeEffectsDevs/AnimeEffects/releases/download/v1.6/AnimeEffects-MacOS.zip) - [Linux](https://github.com/AnimeEffectsDevs/AnimeEffects/releases/download/v1.6/AnimeEffects-Linux.zip) | [Windows](https://nightly.link/AnimeEffectsDevs/AnimeEffects/workflows/build-windows.yaml/master/AnimeEffects-Windows-x64.zip) - [MacOS](https://nightly.link/AnimeEffectsDevs/AnimeEffects/workflows/build_mac_intel.yaml/master/AnimeEffects-MacOS.zip) - [Linux](https://nightly.link/AnimeEffectsDevs/AnimeEffects/workflows/build_linux.yaml/master/AnimeEffects-Linux.zip) | [下载 ZIP](https://github.com/AnimeEffectsDevs/AnimeEffects/archive/refs/heads/master.zip) |

## 🌐 README 🌐

[English](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README.md) - Up-to-date <br>
[日本語](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README-ja.md) - 時代遅れ <br>
[简体中文](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README-zh.md) - 尚未提供 <br>
[正體中文](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README-zh-t.md) - 尚未提供 <br>
[Español](https://github.com/AnimeEffectsDevs/AnimeEffects/blob/master/README-es.md) - Actualizado <br>

## 描述

一个 2D 动画工具不需要经过仔细思考的计划，它通过提供基于多边形网格变形的各种函数来简化动画。<br>
它最初是由藏身者开发的，现在正由其社区开发和维护。

- 官方网站：<br>

  - <https://animeeffectsdevs.github.io/>

- 官方社团：<br>

  - Discord: <a href='https://discord.gg/sKp8Srm'>AnimeEffects</a> (礼遇[Joseassociated Moreno](https://github.com/Jose-Moreno))<br>
  - X (Twitter): <a href='https://x.com/anime_effects'>@anime_effect</a> ( [p_yukusai](https://github.com/p-yukusai))<br>
  - 红： <a href='https://www.reddit.com/r/AnimeEffects/'>r/AnimeEffects</a> (礼遇 [visterical](https://www.tumblr.com/visterical))<br>

- 您可以通过以下方式支持我们：<br><br>
  [！[ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/V7V04YLC3) &nbsp;&nbsp; <a href="https://yukusai.itch.io/animeeffects" target="_blank"> <img src="https://static.itch.io/images/badge-color.svg" alt="itch.io" style="width:100px" /> </a>

注意：目前可能有不兼容的更改，如果发生这些更改，将会在受影响的版本中公布。<br>
**_如果你有任何问题或希望建议新的功能，请随时联系我们我们的社会！_**

## 发布

AnimeEffects 一旦出现，将立即通知您可用的稳定释放。

- 我们的稳定版本可用的 [here](https://github.com/AnimeEffectsDevs/AnimeEffects/releases)。<br>
- 我们的不稳定版本可用的 [here](https://github.com/p-yukusai/AnimeEffects/releases)。<br>
- 我们的夜间构建可用的 [here](https://github.com/AnimeEffectsDevs/AnimeEffects/actions)。

## B. 所需经费

- Windows/Linux/Mac
  - 查看下面的兼容版本
- 处理器： 64 位 CPU
- RAM: 4 GB
- 图像卡：GPU/iGPU 支持 OpenGL 4.0 或更高版本
- [FFmpeg](https://ffmpeg.org/download.html) (视频导出的节能)。 您可以将其放置在您的路径上，或者复制到"/tools"文件夹 - 如果不存在它，请在可执行文件的同时创建此文件夹)

## 操作系统目标

#### 这是我们正在编译和测试软件，它可能会在旧版本上运行，但是这不是劝阻的。

- Windows 10 或更高版本
- Ubuntu LTS 或类似磁盘。
- MacOS 大南方或较新的

## B. 发展要求

- Qt 6.6.X
- Vulkan 头部
- CMake 3.16 或更高版本
- MSVC/GCC/CLang (64 位)

## Linux (Debian)

### 创建编译和应用图像

- 这些依赖关系大多是不必要的，因为它们带着您的迪特罗，因此请检查您自己的软件包：

```
sudo apt update && sudo apt upgrade -y
sudo apt install -y software-properties-common g++ make cmake ninja-build wget rsync build-essential libglib2.0-0
sudo apt install -y libgl1-mesa-dev file libvulkan-dev openssl python3 python3-pip libxcb-cursor0 libxrandr2 wget
pip install -U pip
pip install aqtinstall
aqt install-qt linux desktop 6.6.2 gcc_64 -m qtimageformats qtmultimedia qt5compat
git clone https://github.com/AnimeEffectsDevs/AnimeEffects
cd AnimeEffects
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
cd build/src/gui/Release
mkdir -p appdir
cp AnimeEffects appdir
cp -R ../data appdir/data
cp -R ../../../../dist appdir/dist
cp ../../../../dist/AnimeEffects.png appdir
find appdir/
export APPIMAGE_EXTRACT_AND_RUN=1
wget -c -nv "https://github.com/p-yukusai/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
./linuxdeployqt-continuous-x86_64.AppImage appdir/dist/AnimeEffects.desktop -extra-plugins=imageformats,multimedia,core5compat -appimage -verbose=2
chmod a+x AnimeEffects-x86_64.AppImage
```

## 窗口

### 编译和文件夹创建

- 安装步骤假定您已经通过安装程序安装了所有的要求，您的路径也有这些要求

```powershell
git clone https://github.com/AnimeEffectsDevs/AnimeEffects
cd AnimeEffects
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
cd build/src/gui/Release
mkdir .\AnimeEffects-Windows-x64
windeployqt --dir .\AnimeEffects-Windows-x64 .\AnimeEffects.exe
Copy-Item -Path "..\data" -Destination ".\AnimeEffects-Windows-x64\" -recurse -Force
Copy-Item ".\AnimeEffects.exe" ".\AnimeEffects-Windows-x64\"
```

## MacOS

### 编译和创建应用

- 这些步骤假设您的系统上安装了 xcode、brew、wget、python 3 和 pip

```bash
brew install cmake ninja vulkan-headers
pip install -U pip
pip install aqtinstall
aqt install-qt mac desktop 6.6.2 clang_64 -m qtimageformats qtmultimedia qt5compat
git clone https://github.com/AnimeEffectsDevs/AnimeEffects
cd AnimeEffects
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
cd build/src/gui/Release
mkdir -p appdir/usr/lib
cp -R AnimeEffects.app appdir/AnimeEffects.app
cp -R ../data appdir/data
cp -R ../../../../dist appdir/dist
find appdir/
cd appdir
macdeployqt AnimeEffects.app
wget https://raw.githubusercontent.com/OpenZWave/ozw-admin/master/scripts/macdeployqtfix.py && chmod a+x macdeployqtfix.py
./macdeployqtfix.py AnimeEffects.app /usr/local/Cellar/qt/*/
```
