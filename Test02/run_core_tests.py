import os

# SYSTEM SOURCES
os.system("..\Release\AutoTester Sources\System-Source-1.txt SingleProc-ss1.txt Results\singleproc-ss1.xml")
os.system("..\Release\AutoTester Sources\System-Source-2.txt MultiProc-ss2.txt Results\multiproc-ss2.xml")
os.system("..\Release\AutoTester Sources\System-Source-3.txt MultiProc-ss3.txt Results\multiproc-ss3.xml")

# EXTENSIONS
os.system("..\Release\AutoTester Sources\System-Source-3.txt test_bonus\Extensions-ss3.txt Results\extensions-ss3.xml")