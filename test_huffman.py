import subprocess
import glob
import os
import re
import random
import time

TEST_CASES_DIR = "test_cases"
SCRIPT = "bin/main"


def generate_binary_file():
    with open(os.path.join(TEST_CASES_DIR, "binary_input.bin"), "w") as f:
        byte_array = bytearray([random.randint(0, 255) for _ in xrange(5*(10**6))])
        f.write(byte_array)


def create_empty_file():
    with open(os.path.join(TEST_CASES_DIR, "empty_input"), "w") as f:
        pass


def create_one_symbol_file():
    with open(os.path.join(TEST_CASES_DIR, "one_symbol_input"), "w") as f:
        f.write('a')


def run_test(test_case, result, output):
    t = time.time()
    subprocess.call([SCRIPT, "-c", "-f", test_case, "-o", result])
    time_code = time.time() - t
    t = time.time()
    subprocess.call([SCRIPT, "-u", "-f", result, "-o", output])
    time_decode = time.time() - t
    with open(test_case, 'r') as inp, open(output, 'r') as out, open(result, 'r') as res:
        i, o, r = inp.read(), out.read(), res.read()
    print "===================="
    print "Length of {} is {}".format(test_case, len(i))
    print "Encode time is {}".format(time_code)
    print "Length of {} is {}".format(result, len(r))
    print "Decode time is {}".format(time_decode)
    print "Input equals output: {}".format(i == o)
    print "===================="


def test_huffman():
    run_test(*[os.path.join(TEST_CASES_DIR, i) for i in ("empty_input", "empty_result", "empty_output")])
    run_test(*[os.path.join(TEST_CASES_DIR, i) for i in ("one_symbol_input", "one_symbol_result", "one_symbol_output")])
    for postfix in ["txt", "pdf", "bin"]:
        for test_case in glob.glob(os.path.join(TEST_CASES_DIR, "*_input.{}".format(postfix))):
            result = test_case.replace("_input.{}".format(postfix), "_result.bin")
            open(result, 'a').close()
            output = test_case.replace("_input.{}".format(postfix), "_output.{}".format(postfix))
            open(output, 'a').close()
            run_test(test_case, result, output)


if __name__ == "__main__":
    create_empty_file()
    create_one_symbol_file()
    generate_binary_file()
    test_huffman()
