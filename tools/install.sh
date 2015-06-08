#!/bin/bash
# Author: Alexandr theg4sh Kondratev
# rewrited original install.sh

set -e

CWD="$(dirname $BASH_SOURCE)"
ROOTDIR="$(dirname $CWD)/.."

cd $CWD

function install_sysoff() {
    [ -d "/etc/init.d" ] || return 1;
    cp sysoff /etc/init.d || return 1;
    chmod +x /etc/init.d/sysoff
    rm -f /etc/rc0.d/K01sysoff
    ln -s /etc/init.d/sysoff /etc/rc0.d/K01sysoff
    update-rc.d sysoff defaults 1 0
}

#TODO: add more package managers
function install_dependencies() {
    PKGS=(make pkgconfig gcc-c++ libusb-devel glibc-headers)
    if [ -n "$(which apt-get 2>/dev/null)" ]; then
        apt-get install -y g++ make pkgconfig libusb-1.0-0-dev
        return $?;
    fi
    if [ -n "$(which dnf 2>/dev/null)" ]; then
        dnf install -y ${PKGS[@]}
        return $?;
    fi
    if [ -n "$(which yum 2>/dev/null)" ]; then
        yum install -y ${PKGS[@]}
        return $?;
    fi
    echo "Unsupported system. Please install the following dependencies:";
    echo "  ${PKGS[@]}";
    return 1;
}

if [[ $UID -ne 0 ]]; then
    echo "Require root privileges.";
    #exit 1;
fi

install_sysoff || echo "Cannot install sysoff service" >&2;
install_dependencies || (echo "Cannot install dependencies" >&2; exit 1);

make -C "$ROOTDIR" clean
make -C "$ROOTDIR" all
