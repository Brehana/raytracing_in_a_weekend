del a.exe
g++ main.cpp
IF EXIST a.exe (
    .\a.exe > image.ppm
    .\image.ppm
)
