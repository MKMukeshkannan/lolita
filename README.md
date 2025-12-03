![output](https://github.com/user-attachments/assets/9284ebc7-4149-4a0a-8452-494298465d15)# Lolita

Lolita is a lightweight image editing application for adjusting and enhancing pictures in real time. Built with C++23 and ImGui, it focuses on a clean, responsive user experience with essential image processing features.

## EFFECTS
- [x] brighness adjustment
- [x] grayscale
- [ ] sepia
- [ ] halftone
- [ ] crt
- [ ] dithering

## 📸 Screenshot
![output](https://github.com/user-attachments/assets/e5c0b01b-8dbd-4c87-b3c1-690258d90b02)

## 🛠️ How to Build & Run

### Requirements
- C++23 compliant compiler  
- CMake  
- OpenGL & ImGui dependencies

### Steps

Clone the repository:  
```bash
git clone https://github.com/MKMukeshkannan/lolita  
cd lolita
```

Generate build files: 
```bash
cmake -S . -B build
```

Build the executable:  
```bash
cmake --build build
```

Run the application:  
```bash
./build/bin/lolita
```
