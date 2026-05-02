# LiberoScroller 2.0

> **Libero** *(adjective)*: Free, independent, unrestricted.

**LiberoScroller** is an accessibility-first, hands-free scrolling utility built specifically to give independence back to individuals with limited physical mobility. For those who rely solely on their eyes to interact with a computer, LiberoScroller provides the freedom to comfortably read books, PDFs, and newsletters without requiring expensive hardware or physical assistance.

By using state-of-the-art **Facial Landmark Detection**, it tracks the user's eyes in real-time. Simply close your eyes for 1.5 seconds, and the computer will smoothly scroll down the active window—allowing for a natural, frictionless reading experience.

---

## Features
* **Accessibility-First Design:** Built specifically for users with limited mobility to ensure a hands-free, zero-touch reading experience.
* **Next-Gen Eye Tracking:** Upgraded from legacy Haar Cascades to a 68-point 3D facial mesh (`FacemarkLBF`).
* **Zero False Positives:** Uses the **Eye Aspect Ratio (EAR)** algorithm. It tracks the exact physical distance between your eyelids, completely ignoring shadows, lighting changes, or background movement.
* **Continuous Smooth Scroll:** Holds a smooth scroll command for as long as your eyes remain closed, mimicking a natural glide down the page.
* **Hyper-Optimized:** Built in C++ Release mode for ultra-low latency, meaning it won't slow down older computers.

---

## The EAR Algorithm
Instead of blindly searching for dark pixels, LiberoScroller 2.0 calculates the physical geometry of your eye using the Eye Aspect Ratio. 

When the eye closes, the top and bottom eyelids touch, causing the numerator to approach zero. This makes the tracking mathematically flawless.

$$ \text{EAR} = \frac{||p_2 - p_6|| + ||p_3 - p_5||}{2 ||p_1 - p_4||} $$

---

## Prerequisites
Before building, ensure you have the following installed on your Windows machine:
1. **C++ Compiler:** Visual Studio 2022 Community (with Desktop development with C++).
2. **CMake:** Version 3.10 or higher.
3. **vcpkg:** The C++ package manager.
4. **OpenCV (with Contrib):** You must install OpenCV with the advanced AI `face` modules.

Run this command in your `vcpkg` directory:
```powershell
.\vcpkg.exe install "opencv[contrib]:x64-windows" --recurse
```

## Building the Project
1. Configure the Project
Open your terminal in the liberoscroller project folder and tell CMake where your vcpkg toolchain is located. (Replace [path-to-vcpkg] with your actual installation path).

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake" -DCMAKE_PREFIX_PATH="[path-to-vcpkg]/installed/x64-windows" -DVCPKG_TARGET_TRIPLET=x64-windows
```

2. Compile in Release Mode
To prevent Debug assertions and unlock maximum performance, compile the project in Release mode:

```powershell
cmake --build build --config Release
```

3. Provide the AI Models & Engine
The .exe needs its "brain" and the OpenCV engine to run. Copy the following files directly into your newly created build\Release\ folder:

     * `lbfmodel.yaml` (The 68-point AI model)

     * `haarcascade_frontalface_default.xml` (The face finder)

     * All OpenCV `.dll` files from your `vcpkg\installed\x64-windows\bin\` directory.

## How to Use
Navigate to the execution folder:

```powershell
cd build\Release
```

Launch the application:

```powershell
.\liberoscroller.exe
```

Look at the camera feed. You should see a green mesh accurately tracking your eyes.

Click on the window you want to scroll (e.g., a PDF reader or web browser).

Close your eyes for 1.5 seconds. The program will automatically begin scrolling down until you open your eyes again. Press ESC in the camera window to exit.
