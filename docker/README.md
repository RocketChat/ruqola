# Ruqola Setup on WSL or WINDOWS 

This guide provides steps to set up and run **Ruqola** on **WINDOWS**.

## Prerequisites

- **Windows**
- **Docker**
- **VcXsrv**
- **Gitbash**
---

### **1. Install Docker**

Ensure you have Docker installed and running on Windows:

If you want to code from within WSL and haven't installed it yet(optional):

```powershell
wsl --install -d Ubuntu
```

---

### **2. Install X Server**

You need this to forward GUI from Docker back to WINDOWS:

#### **Install an X Server on Windows**

1. **Download and install an X Server:**
   - **VcXsrv**: [Download here](https://sourceforge.net/projects/vcxsrv/)

2. **Run the X server:**
   - Start **XLaunch**, set it to multiple windows -> start to client -> enable "Disable access control".

---

### **3. Build the Docker Container**

0. Start gitbash:

1. Clone the Ruqola repository with SSH, if not already done:

   ```sh
   git clone git@github.com:your-username/ruqola.git
   cd ruqola
   ```

2. Build Docker:

   ```sh
   cd ruqola
   cd docker
   docker build -t ruqola .
   ```

---

### **4. Run Docker**

1. Start the new container and open a shell:

   This is the local folder that you are going to mount to your container:

   ```sh
   -v /your/local/path:/path/within/container
   ```

   This is the actual command to run (`-it` stands for interactive + terminal). Try to set up your local-folder:container-folder mapping to match, making it easier to switch between container and local:

   ```sh
   docker run -it --name ruqola-container -v /your/local/path:/path/within/container ruqola /bin/bash
   ```

   Now you are within a new created container:

   check 
   ```sh
   cat /etc/os-release | grep "opensuse-tumbleweed" | awk -F='{print $2}'
   ```
   you should see 
    ```sh
   'opensuse-tumbleweed'
   ```
   then 
   ```sh
   echo "export DISPLAY=host.docker.internal:0" >> ~/.bashrc
   source ~/.bashrc
   ```
   Test the installation with `xclock`:

   ```sh
   xclock
   ```
   
   If a clock window appears, your X server is working correctly.

   Now you can always start, stop, and check running containers with these commands:

   ```sh
   docker stop ruqola-container
   ```

   ```sh
   docker start -i ruqola-container
   ```

   ```sh
   docker ps -a
   ```

2. From inside the container, navigate to the build directory and run Ruqola:

   ```sh
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=install ..
   make
   ./bin/ruqola
   ```

   If you want to install, use:

   ```sh
   make install
   ```

3. Now your workflow is:
   - Navigate to your folder, code as usual.
   - When you want to build, run:

     ```sh
     docker start -i ruqola-container
     ./bin/ruqola
     ```

---

### **5. Troubleshooting **

If you experience issues with IntelliSense in VS Code, install Qt packages from the official ![website](https://www.qt.io/download-qt-installer-oss) and set your Qt6 path in `includePath`.

```json
"includePath": [
    "/path/to/qt6/include"
]
```

If you want to code from withing WSL, install WSL Ubuntu and clone again the ruqola repo into Ubuntu and start from there.

---

### **HAPPY CODING! YOU ARE DONE!** 🚀

