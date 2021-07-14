import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCommand(cmds):
    ret = 0
    cmds[0] = "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmds[0])
    if os.path.exists(cmds[0]):
        cmds.insert(0, "python3")
        ret = subprocess.call(cmds)
    else:
        print("Invalid command: ", cmds[0])
        ret = -1
    
    return ret

# ----- Main Entry Point ----- #
argc = len(sys.argv)
i = 1
while i < argc:
    cmds = [sys.argv[i]]

    while True:
        if i < argc - 1 and sys.argv[i+1][0] == "-":
            cmds.append(sys.argv[i+1][1:])
            i = i + 1
        else:
            break

    print("\n------------------------------")
    print("Executing: ", cmds[0])
    if len(cmds) > 1:
        print("With arguments: {}".format(", ".join(cmds[1:])))

    if RunCommand(cmds) != 0:
        break
    i = i + 1
