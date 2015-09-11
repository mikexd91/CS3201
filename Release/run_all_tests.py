import os

# For original query files
os.system("AutoTester SystemTests/Sources/Source-File-1.txt SystemTests/Query-File-1.txt SystemTests/Results/out1.xml")
os.system("AutoTester SystemTests/Sources/Source-File-2.txt SystemTests/Query-File-2.txt SystemTests/Results/out2.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Query-File-3.txt SystemTests/Results/out3.xml")

# For underscore
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Follows-Underscore-s3.txt SystemTests/Results/follows_underscore.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/FollowsStar-Underscore-s3.txt SystemTests/Results/followsstar_underscore.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Parent-Underscore-s3.txt SystemTests/Results/parent_underscore.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/ParentStar-Underscore-s3.txt SystemTests/Results/parentstar_underscore.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Modifies-Underscore-s3.txt SystemTests/Results/modifies_underscore.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Uses-Underscore-s3.txt SystemTests/Results/uses_underscore.xml")

# For overflow
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Follows-Overflow.txt SystemTests/Results/follows_overflow.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/FollowsStar-Overflow.txt SystemTests/Results/followsstar_overflow.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Parent-Overflow-s3.txt SystemTests/Results/parent_overflow.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/ParentStar-Overflow-s3.txt SystemTests/Results/parentstar_overflow.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Modifies-StmtOverflow-s3.txt SystemTests/Results/modifies_overflow.xml")
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/Uses-StmtOverflow-s3.txt SystemTests/Results/uses_overflow.xml")

# For multi procedures
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Modifies-MultiProc-s5.txt SystemTests/Results/modifies_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Uses-MultiProc-s5.txt SystemTests/Results/uses_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-6.txt SystemTests/Calls-MultiProc-s6.txt SystemTests/Results/calls_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Parent-MultiProc-s5.txt SystemTests/Results/parent_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/ParentStar-MultiProc-s5.txt SystemTests/Results/parentstar_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/Follows-MultiProc-s5.txt SystemTests/Results/follows_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-5.txt SystemTests/FollowsStar-MultiProc-s5.txt SystemTests/Results/followsstar_multiproc.xml")
os.system("AutoTester SystemTests/Sources/Source-File-7.txt SystemTests/Pattern-s7.txt SystemTests/Results/pattern.xml")

# For other miscellaneous crashes
os.system("AutoTester SystemTests/Sources/Source-File-3.txt SystemTests/miscellaneous_crashes.txt SystemTests/Results/miscellaneous_crashes.xml")