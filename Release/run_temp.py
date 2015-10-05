import os

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-ss1.txt SystemTests/Results/uses.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-MultiClause-ss1.txt SystemTests/Results/uses_multiclause.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-StmtOverflow-ss1.txt SystemTests/Results/uses_stmtoverflow.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Uses-Misc-ss1.txt SystemTests/Results/uses_misc.xml")

os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-ss1.txt SystemTests/Results/modifies.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-MultiClause-ss1.txt SystemTests/Results/modifies_multiclause.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-StmtOverflow-ss1.txt SystemTests/Results/modifies_stmtoverflow.xml")
os.system("AutoTester SystemTests/Sources/System-Source-1.txt SystemTests/Modifies-Misc-ss1.txt SystemTests/Results/modifies_misc.xml")