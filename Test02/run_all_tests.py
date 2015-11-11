import os

# SYSTEM SOURCES
os.system("..\Release\AutoTester Sources\System-Source-1.txt SingleProc-ss1.txt Results\singleproc-ss1.xml")
os.system("..\Release\AutoTester Sources\System-Source-2.txt MultiProc-ss2.txt Results\multiproc-ss2.xml")
os.system("..\Release\AutoTester Sources\System-Source-3.txt MultiProc-ss3.txt Results\multiproc-ss3.xml")

# STRESS TEST
os.system("..\Release\AutoTester Sources\System-Source-3.txt StressTest-ss3.txt Results\stresstest-ss3.xml")

# EXTENSIONS
os.system("..\Release\AutoTester Sources\System-Source-3.txt test_bonus\Extensions-ss3.txt Results\extensions-ss3.xml")

# INVALIDS
os.system("..\Release\AutoTester Sources\System-Source-3.txt InvalidQueries-ss3.txt Results\invalidqueries-ss3.xml")

# CYCLIC SOURCES
os.system("..\Release\AutoTester Sources\Invalid-Source-Cyclic-1.txt CyclicTest.txt Results\cyclic1.xml")
os.system("..\Release\AutoTester Sources\Invalid-Source-Cyclic-2.txt CyclicTest.txt Results\cyclic2.xml")
os.system("..\Release\AutoTester Sources\Invalid-Source-Cyclic-3.txt CyclicTest.txt Results\cyclic3.xml")