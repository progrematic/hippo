import globals
import os, sys, subprocess

args = globals.ProcessArguments(sys.argv)
config = globals.GetArgumentValue(args, "config", "Debug")
prj = globals.GetArgumentValue(args, "prj", globals.PROJECT_NAME)

exepath = "{}/bin/{}/{}/".format(os.getcwd(), config, prj)
ret = 0

if globals.IsWindows():
    ret = subprocess.call(["cmd.exe", "/c", "{}\\run.bat".format(globals.TOOLS_DIR), config, prj], cwd=os.getcwd())
else:
    ret = subprocess.call(["{}{}".format(exepath, prj)], cwd=exepath)

sys.exit(ret)