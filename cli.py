# cli build
# cli run
# cli gen
# cli version
# cli gen build run

import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCommand(cmd):
    script = "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmd)
    if os.path.exists(script):
        print("Executing: ", cmd)
        subprocess.call(["python3", script])
    else:
        print("Invalid command: ", cmd)

for i in range(1, len(sys.argv)):
    cmd = sys.argv[i]
    
    print("\n------------------------------")
    RunCommand(cmd)
