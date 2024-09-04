# MINIRT
A simple raytracing engine
## Description
![Feature Screenshot](./resources/Screenshot_20240904_152524.png)
![Feature Screenshot](./resources/Screenshot_20240904_152558.png)
## Table of Contents
1. [Requirements](#installation)
2. [Installation](#requirements)
3. [Usage](#usage)
4. [Commands](#commands)

## Requirements
- **libbsd-dev**: Required for building the project on Linux

## Installation
### On Ubuntu
1. **Clone repository:**
```
git clone https://github.com/chalant/miniRT.git
```
2. **Install dependencies:**
```
sudo apt-get install libbsd-dev
```
3. **Build project:**
```
make
```
### On Mac OS

1. **Clone repository:**
```
git clone https://github.com/chalant/miniRT.git
```
2. **Build project:**
```
make
```
## Usage
```
./minirt [file-path]
```
## Example
```
./minirt map.rt
```
## Commands
| **Action**                         | **Windows/Linux**          | **Mac**                 |
|------------------------------------|----------------------------|-------------------------|
| Move up, down, left, right         | `W, S, A, D`               | `W, S, A, D`            |
| Zoom in/out                        | `+/-`                      | ``+/-``                 |
| Rotate Up, Down, Left, Right       | `↑, ↓, ←, →`               | `↑, ↓, ←, →`            |
| Increase/Decrease Elevation        | `Shift + Ctrl + ↑/↓`       | `Shift + Cmd + ↑/↓`     |
| Increase/Decrease Width            | `Shift + Ctrl + →/←`       | `Shift + Cmd + →/←`     |
| Increase/Decrease Height           | `Ctrl + ↑/↓`               | `Cmd + ↑/↓`             |
| Increase/Decrease Color separation | `Alt + ↑/↓`                | `Alt + ↑/↓`             |
| Rotate colors                      | `Alt + →/←`                | `Alt + →/←`             |
| Toggle height lightness on/off     | `Alt + L`                  | `Alt + L`               |
