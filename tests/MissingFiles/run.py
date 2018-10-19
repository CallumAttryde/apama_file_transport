from pysys.constants import *
from apama.correlator import CorrelatorHelper
from pysys.basetest import BaseTest

class PySysTest(BaseTest):
	def execute(self):
		self.correlator = CorrelatorHelper(self)
		self.correlator.start(environ={'APAMA_FILEPLUGIN_ROOT_DIR':self.output})
		self.correlator.injectEPL(filenames=['FilePlugin.mon'], filedir=PROJECT.APAMA_WORK + '/monitors')
		self.correlator.injectMonitorscript(filenames=['test.mon'])
		self.waitForSignal('correlator.out', expr='Test finished', errorExpr=[' (ERROR|FATAL) '])

	def validate(self):
		self.assertGrep('correlator.out', expr=' (ERROR|FATAL) ', contains=False)
		# non standard exception messages means we can't succinctly check for specifics
		self.assertGrep('correlator.out', expr='in plugin method read')
		self.assertGrep('correlator.out', expr='in plugin method get_file_size_KB')
		self.assertGrep('correlator.out', expr='in plugin method copy') 
		self.assertGrep('correlator.out', expr='in plugin method remove')
