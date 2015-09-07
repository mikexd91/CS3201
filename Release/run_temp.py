import os

# For original query files
# os.system("AutoTester SystemTests/Sources/Source-File-1.txt SystemTests/Query-File-1.txt SystemTests/Results/out1.xml")
# os.system("AutoTester SystemTests/Sources/Source-File-2.txt SystemTests/Query-File-2.txt SystemTests/Results/out2.xml")
# os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Query-File-3.txt SystemTests/Results/out3.xml")

# For underscore
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-follows.txt SystemTests/Results/underscore_follows.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-followsstar.txt SystemTests/Results/underscore_followsstar.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-modifies.txt SystemTests/Results/underscore_modifies.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-parent.txt SystemTests/Results/underscore_parent.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Debug-Underscore-parentstar.txt SystemTests/Results/underscore_parentstar.xml")
#os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Uses-Underscore-s3.txt SystemTests/Results/uses_underscore.xml")

# For overflow
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Follows.txt SystemTests/Results/overflow_follows.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-FollowsStar.txt SystemTests/Results/overflow_followsstar.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Modifies.txt SystemTests/Results/overflow_modifies.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Parent.txt SystemTests/Results/overflow_parent.xml")
# os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/Stmt-Overflow-Parentstar.txt SystemTests/Results/overflow_parentstar.xml")
# os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Uses-StmtOverflow-s3.txt SystemTests/Results/uses_overflow.xml")

# For multi procedures
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Modifies-MultiProc-s5.txt SystemTests/Results/modifies_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Uses-MultiProc-s5.txt SystemTests/Results/uses_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-6.txt SystemTests/Calls-MultiProc-s6.txt SystemTests/Results/calls_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Parent-MultiProc-s5.txt SystemTests/Results/parent_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/ParentStar-MultiProc-s5.txt SystemTests/Results/parentstar_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-7.txt SystemTests/Pattern-s7.txt SystemTests/Results/pattern.xml")

# For other miscellaneous crashes
#os.system("AutoTester SystemTests/Source-File-3.txt SystemTests/miscellaneous_crashes.txt SystemTests/Results/miscellaneous_crashes.xml")
