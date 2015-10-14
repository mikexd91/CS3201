import os

# USES CLAUSE
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-ss1.txt SystemTests/Results/uses.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-MultiClause-ss1.txt SystemTests/Results/uses_multiclause.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-StmtOverflow-ss1.txt SystemTests/Results/uses_stmtoverflow.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-Misc-ss1.txt SystemTests/Results/uses_misc.xml")

# MODIFIES CLAUSE
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-ss1.txt SystemTests/Results/modifies.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-MultiClause-ss1.txt SystemTests/Results/modifies_multiclause.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-StmtOverflow-ss1.txt SystemTests/Results/modifies_stmtoverflow.xml")
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-Misc-ss1.txt SystemTests/Results/modifies_misc.xml")

# PARENT(*) CLAUSE
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Parent-ss1.txt SystemTests/Results/parent.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/ParentStar-ss1.txt SystemTests/Results/parentstar.xml")

# FOLLOWS(*) CLAUSE
# os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Follows-ss1.txt SystemTests/Results/follows.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/FollowsStar-ss1.txt SystemTests/Results/followsstar.xml")
