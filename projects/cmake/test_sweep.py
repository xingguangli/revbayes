#!/usr/bin/env python
import subprocess
import sys
import os
import re
filter = re.compile(r'^RevBayes LnL:\s*(\S+)')
vals = ['READ_MORPHO_AS_DNA', 'USE_GTR_RATE_MAT', 'USE_TIME_TREE', 'USE_NUCLEOTIDE', 'USE_3_STATES', 'USE_RATE_HET']
short = ['DNA', 'GTR', 'TIME', 'NUC', '3', 'RATEHET']
use3f = 1 << vals.index('USE_3_STATES')
incompatf = (1 << vals.index('USE_GTR_RATE_MAT')) + (1 << vals.index('USE_NUCLEOTIDE'))
MAX = 1 << len(vals)
def run_serial_overwrite(cmd, out_tag, cwd=None):
    with open(out_tag + 'stdout', 'w') as fo:
        with open(out_tag + 'stderr', 'w') as fe:
            proc = subprocess.Popen(cmd, stderr=fe, stdout=fo, cwd=cwd, shell=True)
            return proc.wait()
abs_cur = os.path.abspath('.')
par = os.path.split(abs_cur)[0]
grand_par = os.path.split(par)[0]
exe_path = os.path.join(abs_cur, 'testrb')
test_exe_dir = os.path.join(grand_par, 'examples')
if len(sys.argv) > 1:
    flag = int(sys.argv[1])
    user_spec = True
else:
    flag = 0
    user_spec = False
RUN_IN_GDB = '-d' in sys.argv

while flag < MAX:
    if (flag & use3f) and (flag & incompatf):
        if user_spec:
            sys.exit('flag {b} is not buildable\n'.format(b=flag))
        flag += 1
        continue
    tag = ''
    with open('../../src/test/TestingDefs.h', 'w') as fo:
        fo.write('''#ifndef TESTING_DEFS_H
#define TESTING_DEFS_H
''')
        check_flag = 1
        for n, v in enumerate(vals):
            s = short[n]
            if check_flag & flag:
                tag += '-' + s
                fo.write('#define {f}\n'.format(f=v))
            else:
                tag += '-NO' + s
                fo.write('//#define {f}\n'.format(f=v))
            check_flag <<= 1
        fo.write('''#endif
''')
    print 'trying', tag[1:], 'which is flag =', flag
    if run_serial_overwrite('make', 'build' + tag) != 0:
        sys.exit('Could not build' + tag[1:] + '\n')
    else:
        frag = 'testrun' + tag
        if RUN_IN_GDB:
            proc = subprocess.Popen(['gdb', exe_path], cwd=test_exe_dir, shell=True)
            sys.exit(proc.wait())
        else:
            if run_serial_overwrite(exe_path, frag, cwd=test_exe_dir) != 0:
                sys.exit('Test run failed for' + tag[1:] + '\n')
        test_out = frag + 'stdout'
        with open(test_out, 'rU') as fo:
            for line in fo:
                m = filter.match(line)
                if m:
                    print m.group(1)
    if user_spec:
        sys.exit(0)
    flag += 1
    
