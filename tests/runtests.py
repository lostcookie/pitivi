import glob
import os
import sys
import unittest

from pitivi.system import getSystem, GnomeSystem


SKIP_FILES = ['common', 'runtests', 'test_integration', 'test_system_gnome']

system = getSystem()
if isinstance(system, GnomeSystem):
    SKIP_FILES.remove('test_system_gnome')

def gettestnames(which):
    if not which:
        dir = os.path.split(os.path.abspath(__file__))[0]
        which = [os.path.basename(p) for p in glob.glob('%s/test_*.py' % dir)]

    names = map(lambda x: x[:-3], which)
    for f in SKIP_FILES:
        if f in names:
            names.remove(f)
    return names

suite = unittest.TestSuite()
loader = unittest.TestLoader()

for name in gettestnames(sys.argv[1:]):
    suite.addTest(loader.loadTestsFromName(name))

descriptions = 1
verbosity = 1
if os.environ.has_key('VERBOSE'):
    descriptions = 2
    verbosity = 2
from pitivi.log import log
log.init('PITIVI_DEBUG', 1)

testRunner = unittest.TextTestRunner(descriptions=descriptions,
    verbosity=verbosity)
result = testRunner.run(suite)
if result.failures or result.errors:
    sys.exit(1)
