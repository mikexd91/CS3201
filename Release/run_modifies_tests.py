import os

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-ss1.txt SystemTests/Results/modifies.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-MultiClause-ss1.txt SystemTests/Results/modifies_multiclause.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-StmtOverflow-ss1.txt SystemTests/Results/modifies_stmtoverflow.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-Misc-ss1.txt SystemTests/Results/modifies_misc.xml")