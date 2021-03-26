# cli build
# cli run
# cli gen
# cli version
# cli gen build run

import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCommand(cmd):
    subprocess.call(["python3", "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmd)])

for i in range(1, len(sys.argv)):
    cmd = sys.argv[i]
    
    print("\n------------------------------")
    print("Executing: ", cmd)

    RunCommand(cmd)
