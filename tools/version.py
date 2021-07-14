import globals
import sys

print("hippo tools - v{}.{}".format(globals.V_MAJOR, globals.V_MINOR))
if len(sys.argv) >= 2:
    print(", ".join(sys.argv))

sys.exit(0)