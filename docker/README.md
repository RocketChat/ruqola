# Ruqola Setup on WSL or WINDOWS

This guide provides steps to set up and run **Ruqola** on **WSL (Windows Subsystem for Linux)**.

## Prerequisites

- **Windows Subsystem for Linux (WSL)**
- **Qt6 and KF6 installed** on WSL

---
### **1. Install Docker**

Ensure you have Docker installed and running on Windows:

if you want to code from within WSL then and you didn't install it
```powershell
wsl --install -d Ubuntu 
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
   cmake -DCMAKE_INSTALL_PREFIX=install ..
   make 
   ./bin/ruqola
   ```
   If you want to install then use
    ```sh
   make install
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
