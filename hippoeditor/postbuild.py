import os, sys
import subprocess

TOOLS_DIR = "tools"
sys.path.append("{}/../{}".format(os.getcwd(), TOOLS_DIR))
import globals

WIN_SOURCE_PATHS = ["PostBuildCopy", "PostBuildCopy_windows"]
NIX_SOURCE_PATHS = ["PostBuildCopy"]

args = globals.ProcessArguments(sys.argv)
CONFIG = globals.GetArgumentValue(args, "config", "Debug")
PROJECT = globals.GetArgumentValue(args, "prj", globals.PROJECT_NAME)

dest = "{}/../bin/{}/{}".format(os.getcwd(), CONFIG, PROJECT)

if globals.IsWindows():
    for source in WIN_SOURCE_PATHS:
        subprocess.call(["cmd.exe", "/c", "robocopy", source, dest, "/E"])
else:
    import os
    import shutil

    # from https://stackoverflow.com/questions/814167/easiest-way-to-rm-rf-in-python
    def rm_r(path):
        if not os.path.exists(path):
            return
        if os.path.isfile(path) or os.path.islink(path):
            os.unlink(path)
        else:
            shutil.rmtree(path)
    
    # from https://stackoverflow.com/questions/1868714/how-do-i-copy-an-entire-directory-of-files-into-an-existing-directory-using-pyth
    def copytree(src, dst, symlinks=False, ignore=None):
        for item in os.listdir(src):
            s = os.path.join(src, item)
            d = os.path.join(dst, item)
            if os.path.exists(d):
                rm_r(d)
            if os.path.isdir(s):
                shutil.copytree(s, d, symlinks, ignore)
            else:
                shutil.copy2(s, d)
    
    for source in NIX_SOURCE_PATHS:
        copytree(source, dest)