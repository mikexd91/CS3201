import os

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/SingleProc-ss1.txt SystemTests/Results/singleproc-ss1.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/MultiProc-ss2.txt SystemTests/Results/multiproc-ss2.xml")

os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/Next-ss3.txt SystemTests/Results/next-ss3.xml")
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/Affects-ss3.txt SystemTests/Results/affects-ss3.xml")
os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/AffectsStar-ss3.txt SystemTests/Results/affectsstar-ss3.xml")

# EXTENSION
# os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/NextBIP-ss3.txt SystemTests/Reuslts/nextbip-ss3.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/NextBIPStar-ss3.txt SystemTests/Reuslts/nextbipstar-ss3.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/AffectsBIP-ss3.txt SystemTests/Reuslts/affectsbip-ss3.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-3.txt SystemTests/AffectsBIPStar-ss3.txt SystemTests/Reuslts/affectsbipstar-ss3.xml")