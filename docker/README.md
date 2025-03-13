# Ruqola Setup on WSL or WINDOWS

This guide provides steps to set up and run **Ruqola** on **WSL (Windows Subsystem for Linux)**.

## Prerequisites

- **Windows Subsystem for Linux (WSL openSUSE)**
- **Git** (with SSH configured)
- **Visual Studio Code** with the **IntelliSense extension**
- **Qt5/Qt6 and KF6 installed** on WSL

---

## **Part 1: Setup Without Docker on WSL**

### **0. Install WSL from PowerShell**

Run the following command in PowerShell to install WSL:

```powershell
wsl --install -d openSUSE-Tumbleweed
```

Once installed, launch WSL from the Start menu.

---

### **1. Install VcXsrv and Set Up X Server**

1. Download and install **VcXsrv** from [SourceForge](https://sourceforge.net/projects/vcxsrv/).
2. Run **XLaunch** and configure it with the following settings:
   - **Multiple windows** option
   - **Start no client**
   - **Disable access control**
   - Click **Next** and **Save the configuration** for future use.
3. In WSL, check if `DISPLAY` is set to `0:` by running:
   ```sh
   echo $DISPLAY
   ```
   If not, add the following line to your **\~/.bashrc** file:
   ```sh
   echo 'DISPLAY=0:' >> ~/.bashrc
   source ~/.bashrc
   ```
4. Test the setup by running:
   ```sh
   xclock
   ```
   You should see a clock window appear.

---

### **2. Run WSL**

Open **WSL** and run the following commands:

```sh
chmod +x init.sh
./init.sh
```

---

### **3. Clone the Git Repository with SSH**

First, set up your SSH keys for authentication if not already done:

```sh
ssh-keygen -t ed25519 -C "your-email@example.com"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
cat ~/.ssh/id_ed25519.pub
```

Copy the output and add it to your **GitHub SSH keys**:

- Go to **GitHub → Settings → SSH and GPG keys**
- Click **"New SSH key"**, paste the key, and save it

Then, **clone the repository**:

```sh
git clone git@github.com:your-username/ruqola.git
cd ruqola
```

---

### **4. Start VS Code & Install IntelliSense**

1. Open **VS Code** inside your project directory:
   ```sh
   code . /your/path/to/ruqola
   ```
2. Install the **C/C++ IntelliSense extension** from the Extensions Marketplace.
3. Open the C/C++ configuration by pressing:\
   **`Ctrl + Shift + P`** → Search for "C/C++ Edit Configurations".
4. Set the `includePath` in `.vscode/c_cpp_properties.json` as follows:
   ```json
   {
       "name": "Linux",
       "includePath": [
           "${workspaceFolder}/**",
           "/usr/include/qt5/**",
           "/usr/include/KF6/**",
           "/usr/include/qt6/**"
       ],
       "defines": [],
       "compilerPath": "/usr/bin/gcc",
       "cStandard": "c17",
       "cppStandard": "gnu++17",
       "intelliSenseMode": "linux-gcc-x64"
   }
   ```
5. **Create a workspace** in VS Code:
   - Go to **File → Save Workspace As…**
   - Save the workspace inside `.vscode` where other configuration files are stored.

---

### **5. Build and Run Ruqola**

1. Create a `build` directory and navigate to it:
   ```sh
   mkdir build
   cd build
   ```
2. Run CMake:
   ```sh
   cmake ..
   ```
3. Compile the project:
   ```sh
   make
   ```
4. Run Ruqola from the `build` directory:
   ```sh
   ./bin/ruqola
   ```
5. Save your current workspace in VS Code within `.vscode` and always start VS Code from the same location.

---

If you experience any issues (all text appearing as squares), or something else, just google the missing package for openSUSE-Tumbleweed:

```sh
sudo zypper search your_package
sudo zypper install your_package
```

---

## **Part 2: Setup with Docker on WSL or WINDOWS**

If you prefer to run Ruqola inside a **Docker container**, follow these steps:

### **1. Install Docker**

Ensure you have Docker installed and running on Windows:

if you want to code from within WSL then and you didn't install it
```powershell
wsl --install -d openSUSE-Tumbleweed 
```

### **2. Build the Docker Container**

1. Clone the Ruqola repository with SSH, if not already done:
   ```sh
   git clone git@github.com:your-username/ruqola.git
   cd ruqola
   ```
2. Build Docker:
   ```sh
   docker build -it ruqola .
   ```

### **3. Run Docker**

1. Start the new container and open a shell:

   This is the local folder that you are going to mount to your container 
   ```sh
   -v /your/local/path:/path/within/container 
   ```
   This is the actual command to run, -it stands for interactive+terminal,
   try to setup your local/folder:container/folder to match, that way its going to be easy to switch between container and local.
   ```sh
   docker run -it --name ruqola-container -v /your/local/path:/path/within/container ruqola-image /bin/bash
   ```
   Now you can always start, stop, and check running containers with next commands
   ```sh
   docker stop container-name
   ```
   ```sh
   docker start -i container-name
   ```
   ```sh
   docker ps -a 
   ```
3. From inside the container, navigate to the build directory and run Ruqola:
   ```sh
   cd build
   cmake ..
   make 
   ./bin/ruqola
   ```
4. Now your workflow is next, you cd into your folder, code code code code, then when you want to build you
 ```sh
    docker start -i container-name
    ./bin/ruqola
   ```
   After that you can code either within container or within wsl, the changes are two way binded


If you code in vscode and exprience any issues with intellisense you could download qt:

```sh
sudo zypper search your_package
sudo zypper install your_package
```


HAPPY CODING, YOU ARE DONE !
