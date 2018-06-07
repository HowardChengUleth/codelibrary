##################################################
# Sets up a contest environment for you          #
#                                                #
# Author: Darcy Best                             #
#                                                #
# Type this out exactly the same as you see here #
# with exception to the bottom, where you must   #
# say what the name of the problem is.           #
#                                                #
#                                                #
# Run this script in an empty folder!            #
# It may delete certain files/folders!           #
#                                                #
# ie. In the contest, if problem A must be       #
#     called range, write "f A range".           #
#                                                #
# This creates 5 files: (ie. problem name: range)#
#      range.cc, data, ans, Makefile, run        #
#                                                #
# range.cc : The layout for a basic C++ program  #
# Makefile : Just type "make", and it will       #
#            compile with warning flags          #
# range.in : You must put your data into this    #
#            file in order for run to work       #
# range.out: You must put the solution to your   #
#            data in the file in order for run   #
#            to work                             #
# run      : This will run the program on the    #
#            input in "data", then compare with  #
#            the solution in "ans". It will      #
#            return one of 3 results:            #
#                 - AC (Accepted)                #
#                 - WA (Wrong Answer)            #
#                 - PE (Presentation Error)      #
#            It does not check for Runtime Error.#
#            If it is WA or PE, look in the      #
#            "Diff" section to see what is       #
#            different between your output and   #
#            "ans".                              #
##################################################


#!/bin/bash

function f {
    rm -Rf $1
    mkdir $1
    touch $1/$2.in $1/$2.out
    cat > $1/$2.cc <<EOF
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

#define ll long long
void do_case() {

}

int main() {
    return 0;
}
EOF
#
# If using emacs, type Ctrl-Q Ctrl-I to insert a TAB below before "g++"
    cat > $1/Makefile <<EOF
all:
	g++ $2.cc -Wall -o $2   
EOF
    cat > $1/run <<EOF
#!/bin/bash

./$2 < $2.in > myOut
echo Output:
cat myOut | cat -vet
echo
echo Diff:
diff $2.out myOut
echo
echo Result:
diff $2.out myOut > /dev/null
if [ \$? -ne 0 ] ; then
    diff -iBw $2.out myOut > /dev/null
    if [ \$? -ne 0 ] ; then
	echo WA
    else
	echo PE
    fi
else
    echo AC
fi
rm -f myOut
EOF
    chmod +x $1/run
}

f A name_of_A
f B name_of_B
f C name_of_C
f D name_of_D
f E name_of_E
f F name_of_F
f G name_of_G
f H name_of_H
f I name_of_I

cat >> ~/.emacs <<EOF
   (add-hook 'c-mode-common-hook 'turn-on-font-lock)
EOF
