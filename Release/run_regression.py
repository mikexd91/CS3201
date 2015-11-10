import os

# SYSTEM SOURCES
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/SingleProc-ss1.txt SystemTests/Results/singleproc-ss1.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/MultiProc-ss2.txt SystemTests/Results/multiproc-ss2.xml")
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/MultiProc-ss3.txt SystemTests/Results/multiproc-ss3.xml")

# EXTENSIONS
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/Extensions-ss3.txt SystemTests/Results/extensions-ss3.xml")