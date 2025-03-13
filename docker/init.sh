#Copyright (c) <13.03.25> <Andro Ranogajec edcedcedcedc@github>

#Permission is hereby granted, free of charge, to any person obtaining a copy of 
#this software and associated documentation files (the "Software"), to deal in the 
#Software without restriction, including without limitation the rights to use, copy, 
#modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
#and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
#INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
#PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
#HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
#CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
#OR THE USE OR OTHER DEALINGS IN THE SOFTWARE



#!/bin/bash

# Init Refresh and Install Required Base Tools
sudo zypper --non-interactive refresh
sudo zypper --non-interactive install -y grep nano

# Function to add a repository if missing
add_repo_if_missing() {
    repo_url=$1
    repo_alias=$2

    if sudo zypper lr | grep -q "$repo_alias"; then
        echo "✅ Repository '$repo_alias' already exists. Skipping..."
    else
        echo "➕ Adding repository '$repo_alias'..."
        sudo zypper ar -f "$repo_url" "$repo_alias"
    fi
}

add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Qt6/openSUSE_Tumbleweed/" "KDE-Qt6"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Frameworks/openSUSE_Tumbleweed/" "KDE-Frameworks"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Frameworks5/openSUSE_Factory/" "KDE-Frameworks5"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Applications/KDE_Frameworks5_openSUSE_Tumbleweed/" "KDE-Applications"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Extra/openSUSE_Tumbleweed/" "KDE-Extra"

echo "🔄 Refreshing repositories..."
sudo zypper --non-interactive refresh

if sudo zypper list-updates | grep -q "up"; then
    echo "🚀 System updates are available. Performing upgrade..."
    sudo zypper -v dup --allow-vendor-change
else
    echo "✅ No system updates available. Skipping upgrade..."
fi
 	

if ! sudo zypper info --type pattern kde_plasma | grep -q "Installed: Yes"; then
    echo "📦 Installing KDE Plasma pattern..."
    sudo zypper -v --non-interactive install pattern:kde_plasma
else
    echo "✅ KDE Plasma pattern is already installed. Skipping installation..."
fi

echo "📦 Installing KDE Frameworks 6 development libraries..."
sudo zypper --non-interactive install -y \
    cmake make ninja meson \
    extra-cmake-modules=6.11.0 \
	 clang ccache \
    kf6-kcoreaddons-devel \
    xvfb-run openbox \
    AppStream \
    kf6-ki18n-devel \
    kf6-kcrash-devel \
    kf6-knotifications-devel \
    kf6-kwidgetsaddons-devel \
    kf6-ktextwidgets-devel \
    kf6-kiconthemes-devel \
    kf6-syntax-highlighting-devel \
    kf6-knotifyconfig-devel \
    kf6-kio-devel \
    kf6-kitemviews-devel \
    kf6-kidletime-devel \
    kf6-prison-devel \
    kf6-karchive-devel \
    kf6-kcodecs-devel \
    ktextaddons-devel \
    kf6-kdbusaddons-devel \
    kdbusaddons-devel \
    kdbusaddons-tools \
    kf6-purpose-devel \
    kf6-kstatusnotifieritem-devel \
    xclock \
    qt6-core-devel \
    qt6-charts-imports \
    qt6-declarative-tools \
    qt6-tools-qdbus \
    qt6-qt5compat-imports \
    qt6-networkinformation-nm \
    qt6-multimedia-imports \
    qt6-location \
    qt6-positioning-imports \
    qt6-translations \
    qca-qt6-devel qt6-mqtt-devel qt6-charts-devel qt6-chartsqml-devel \
    qt6-base-devel \
    qt6-websockets-devel \
    qt6-multimedia-devel \
    qt6-networkauth-devel \
    qt6-multimediawidgets-devel \
    qt6-sql-mysql \
    qt6-sql-devel \
    libQt6Sql6 \
    qt6-sql-sqlite \
    qt6-sql-postgresql \
    kf6-kuserfeedback-devel \
    kf6-kuserfeedback \
    kf6-kdoctools-devel \
    kf6-networkmanager-qt-devel \
    libKF6UserFeedbackCore6 \
    libKF6UserFeedbackWidgets6 \
    qtkeychain-qt6-devel \
    qtkeychain-qt6-lang \
    qtkeychain-qt6-debugsource \
    qt6-wayland-devel \
    qt6-wayland \
    libQt5Gui5 \
    libQt5Widgets5 \
    xorgproto-devel 

sudo zypper install libxcb1 libxcb-render0 libxcb-shm0 libxcb-xfixes0 libxcb-shape0 libxcb-glx0 libxcb-icccm4 libxcb-composite0 libxcb-cursor0 libxcb-damage0 libxcb-dbe0 libxcb-devel libxcb-dpms0 libxcb-dri2-0 libxcb-dri3-0 libxcb-res0 libxcb-screensaver0 libxcb-sync1 libxcb-xf86dri0 libxcb-xinerama0 libxcb-xinput0 libxcb-xkb1 libxcb-xtest0 libxcb-xv0 libxcb-xvmc0


RUN zypper --non-interactive install \
  	ffmpeg-6-libavcodec-devel ffmpeg-6-libavfilter-devel \      
  	ffmpeg-6-libavformat-devel ffmpeg-6-libavdevice-devel \ 
	ffmpeg-6-libavutil-devel ffmpeg-6-libswscale-devel ffmpeg-6-libpostproc-devel

sudo zypper --non-interactive install fontconfig fontconfig-devel fonts-KanjiStrokeOrders libkfont0

pip install --break-system-packages python-gitlab gcovr cppcheck_codequality reuse doxyqml cheroot wsgidav check-jsonschema chai pygdbmi \
    yamllint==1.33.0

 					
echo "🔗 Creating symlinks for clang-format versions..."
for version in $(ls /usr/bin/clang-format-[0-9]*.* 2>/dev/null); do
    link_name="/usr/bin/$(basename "$version" | cut -d. -f1)"
    if [ ! -e "$link_name" ]; then
        echo "Creating symlink: $link_name -> $version"
        sudo ln -s "$version" "$link_name"
    fi
done

echo "🆔 Generating D-Bus machine ID..."
sudo dbus-uuidgen > /etc/machine-id

echo "🔧 Fixing X11 permissions..."
sudo mkdir -p /tmp/.X11-unix/
sudo chown root:root /tmp/.X11-unix/
sudo chmod 1777 /tmp/.X11-unix/

# Define the variables
echo 'export LC_ALL=C.UTF-8' >> ~/.bashrc
echo 'export XDG_RUNTIME_DIR=/tmp/runtime-$USER' >> ~/.bashrc
echo 'mkdir -p $XDG_RUNTIME_DIR' >> ~/.bashrc
echo 'chmod 0700 $XDG_RUNTIME_DIR' >> ~/.bashrc
echo 'export $(dbus-launch)' >> ~/.bashrc

# Apply the changes to the current session
source ~/.bashrc

echo -e "
,------.,--. ,--.,------.  ,-----. ,------.   ,-----. ,--.   ,--.     ,---.  ,------.  ,-----.,--.   ,--. ,--.,-----.   
|  .---'|  | |  ||  .--. ''  .-.  '|  .-.  \\ '  .-.  '|  |   |  |    /  O  \\ |  .--. ''  .--./|  |   |  | |  ||  |) /_  
|  \`--, |  | |  ||  '--'.'|  | |  ||  |  \\  :|  | |  ||  |   |  |   |  .-.  ||  '--'.'|  |    |  |   |  | |  ||  .-.  \\ 
|  \`---.'  '-'  '|  |\\  \\ '  '-'  '|  '--'  /'  '-'  '|  '--.|  '--.|  | |  ||  |\\  \\ '  '--'\\|  '--.'  '-'  '|  '--' / 
\`------' \`-----' \`--' '--' \`-----' \`-------'  \`-----' \`-----'\`-----'\`--' \`--'\`--' '--' \`-----'\`-----' \`-----' \`------'  
                                                                                                               ,---.    
,--.  ,--.  ,---.  ,------. ,------.,--.   ,--.    ,--.  ,--.  ,---.   ,-----.,--. ,--.,--.,--.  ,--. ,----.   |   |    
|  '--'  | /  O  \\ |  .--. '|  .--. '\\  \`.'  /     |  '--'  | /  O  \\ '  .--./|  .'   /|  ||  ,'.|  |'  .-./   |  .'    
|  .--.  ||  .-.  ||  '--' ||  '--' | '.    /      |  .--.  ||  .-.  ||  |    |  .   ' |  ||  |' '  ||  | .---.|  |     
|  |  |  ||  | |  ||  | --' |  | --'    |  |       |  |  |  ||  | |  |'  '--'\\|  |\\   \\|  ||  | \`   |'  '--'  |\`--'     
\`--'  \`--'\`--' \`--'\`--'     \`--'        \`--'       \`--'  \`--'\`--' \`--' \`-----'\`--' '--'\`--'\`--'  \`--' \`------' .--.      
"




echo "Setup complete! Happy Hacking! :)"
