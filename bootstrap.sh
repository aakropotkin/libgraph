#! /usr/bin/env sh
BACKUP_FILES=( ./config.guess~ ./config.h.in~ ./config.sub~ ./configure~ );
aclocal && autoreconf -if && \rm -f ${BACKUP_FILES[*]};
