#!/bin/bash

hostname;

./rungame ${1};

MATCHES_COMPLETED=`wc -l games/results.* | tail -n 1 | awk '{ print $1 }'`;
TOTAL_MATCHES=`wc -l games/games.* | tail -n 1 | awk '{ print $1 }'`;

if [ ${MATCHES_COMPLETED} == ${TOTAL_MATCHES} ];
then
    touch GENERATION_DONE
fi
