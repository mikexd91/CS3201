import os

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/SingleProc-ss1.txt SystemTests/Results/singleproc-ss1.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/MultiProc-ss2.txt SystemTests/Results/multiproc-ss2.xml")


# EXTENSION
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/NextBIP-ss3.txt SystemTests/Reuslts/nextbip-ss3.xml")
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/NextBIPStar-ss3.txt SystemTests/Reuslts/nextbipstar-ss3.xml")