#include <unistd.h>
#include <string.h>
#include "iostream"
#include "stdio.h"
#include "stdint.h"
#include "fillsize.h"

using namespace std;

void usage(void)
{
    cout << "Usage: fillsize <filename> [address]" << endl;
}

int main(int argc, const char *argv[])
{
    /* check argc */
    if((2 != argc) && (3 != argc)) {
        usage();
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

