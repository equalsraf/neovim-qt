# How to Build neovim-qt on Ubuntu

## 1. Install dependencies

```bash
sudo apt update
sudo apt install -y git build-essential cmake ninja-build pkg-config neovim libqt6svg6 qt6-svg-dev libqt6svgwidgets6 qt6-base-dev qt6-base-dev-tools qtchooser qmake6
```

## 2. Clone repository

```bash
git clone https://github.com/equalsraf/neovim-qt.git
cd neovim-qt
```

## 3. Configure

```bash
cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=ON
```

## 4. Build

```bash
cmake --build build -j"$(nproc)"
```

## 5. (Optional) Run tests

```bash
ctest --test-dir build -VV --output-on-failure
```

## 6. (Optional) Run nvim-qt

```bash
NVIM_QT_RUNTIME_PATH=src/gui/runtime ./build/bin/nvim-qt
```

## If `neovim` package is not found

```bash
sudo add-apt-repository -y ppa:neovim-ppa/stable
sudo apt update
sudo apt install -y neovim
```
