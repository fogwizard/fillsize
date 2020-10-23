#include <unistd.h>
#include <string.h>
#include "iostream"
#include "stdio.h"
#include "stdint.h"
#include "fillsize.h"
#include "mergebin.h"
#include "alignbin.h"
#include "ddbin.h"

using namespace std;

int usage(void)
{
    cout << "Usage: fillsize <filename> [address]" << endl;
    return 0;
}

int usage_merge(void)
{
    cout << "Usage: fillsize -merge <loader> <offset> <app>" << endl;
    return 0;
}

int usage_align(void)
{
    cout << "Usage: fillsize -align <inFile> <alignValue>" << endl;
    return 0;
}

int usage_dd(void)
{
    cout << "Usage: fillsize -dd <inFile> <outFile> [skip] [out_length]" << endl;
    return 0;
}

int main(int argc, const char *argv[])
{
    if((argc >= 2) && (0 == strcmp("-merge", argv[1]))) {
        cout << "merge function active" << endl;
        mergeBin *mf = NULL;
        if(argc != 5) {
            return usage_merge();
        }
        /* check input file */
        if (-1 == access(argv[2], 0)) {
            cout << "file:" << argv[2]  << " not exist" << endl;
            return usage_merge();
        }
        if (-1 == access(argv[4], 0)) {
            cout << "file:" << argv[4]  << " not exist" << endl;
            return usage_merge();
        }

        mf = new mergeBin(argv[2], argv[3], argv[4]);
        mf->fill_loader();
        mf->fill_app();
        mf->fill_magic();
        cout << "merge function end..." << endl;
        return 0;
    }

    /* align tool */
    if((argc >= 2) && (0 == strcmp("-align", argv[1]))) {
        if((argc != 4) &&(argc != 5)) {
            return usage_align();
        }
        /* check input file */
        if (-1 == access(argv[2], 0)) {
            cout << "file:" << argv[2]  << " not exist" << endl;
            return usage_align();
        }
        alignBin *ab;
        if(4 == argc) {
            ab = new alignBin(argv[2], argv[3]);
        } else {
            ab = new alignBin(argv[2], argv[3], argv[4]);
        }
        ab->do_align();
        return 0;
    }

    /* dd tool */
    if((argc >= 2) && (0 == strcmp("-dd", argv[1]))) {
        if((4 != argc) && (5 != argc) &&(6 != argc)) {
            return usage_dd();
        }
        /* check input file */
        if (-1 == access(argv[2], 0)) {
            cout << "file:" << argv[2]  << " not exist" << endl;
            return usage_dd();
        }
        ddBin *dd;
        switch(argc) {
        case 4:
            dd = new ddBin(argv[2], argv[3]);
            break;
        case 5:
            dd = new ddBin(argv[2], argv[3], argv[4]);
            break;
        case 6:
            dd = new ddBin(argv[2], argv[3], argv[4], argv[5]);
            break;

        }
        dd->dd();
        return 0;
    }

    /* check argc */
    if((2 != argc) && (3 != argc)) {
        return usage();
    }

    /* check input file */
    if (-1 == access(argv[1], 0)) {
        cout << "file:" << argv[1]  << " not exist" << endl;
        return 0;
    }

    fillVal * pf = NULL;
    if(argc == 3) {
        pf = new fillVal(argv[1], argv[2]);
    } else {
        pf = new fillVal(argv[1]);
    }

    pf->fill_size();
    pf->fill_version();

    return 0;
}

