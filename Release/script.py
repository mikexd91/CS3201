import os

# For original query files
os.system("AutoTester SystemTests/Source-File-1.txt SystemTests/Query-File-1.txt SystemTests/Results/out1.xml")
os.system("AutoTester SystemTests/Source-File-2.txt SystemTests/Query-File-2.txt SystemTests/Results/out2.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Query-File-3.txt SystemTests/Results/out3.xml")

# For underscore
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-follows.txt SystemTests/Results/underscore_follows.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-followsstar.txt SystemTests/Results/underscore_followsstar.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-modifies.txt SystemTests/Results/underscore_modifies.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-parent.txt SystemTests/Results/underscore_parent.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-parentstar.txt SystemTests/Results/underscore_parentstar.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-Uses.txt SystemTests/Results/underscore_uses.xml")

# For overflow
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Follows.txt SystemTests/Results/overflow_follows.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-FollowsStar.txt SystemTests/Results/overflow_followsstar.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Modifies.txt SystemTests/Results/overflow_modifies.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Parent.txt SystemTests/Results/overflow_parent.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Parentstar.txt SystemTests/Results/overflow_parentstar.xml")
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Uses.txt SystemTests/Results/overflow_uses.xml")

# For other miscellaneous crashes
os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/miscellaneous_crashes.txt SystemTests/Results/miscellaneous_crashes.xml")
