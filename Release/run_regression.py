import os

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/SingleProc-ss1.txt SystemTests/Results/singleproc-ss1.xml")

# SUCH THAT CLAUSES
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/Uses-ss2.txt SystemTests/Results/uses-ss2.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/Parent-ss2.txt SystemTests/Results/parent-ss2.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/Follows-ss2.txt SystemTests/Results/follows-ss2.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/Calls-ss2.txt SystemTests/Results/calls-ss2.xml")
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/CallsStar-ss2.txt SystemTests/Results/callsstar-ss2.xml")

# PATTERN CLAUSE
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/Pattern-ss2.txt SystemTests/Results/pattern-ss2.xml")

# WITH CLAUSE
os.system("AutoTester SystemTests/Sources/System-Source-2.txt SystemTests/With-ss2.txt SystemTests/Results/with-ss2.xml")