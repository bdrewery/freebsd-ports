#! /bin/sh

#
# Wrap the given command with ports_env.sh exported vars
#

# MAINTAINER: portmgr@FreeBSD.org

set -o pipefail

if [ -z "${dp_SCRIPTSDIR}" ]; then
	echo "Must set dp_SCRIPTSDIR" >&2
	exit 1
fi

. ${dp_SCRIPTSDIR}/ports_env.sh > /dev/null

"$@"
