#! /bin/sh

#
# Exports and gives export lines that can be evalled by the caller.
# See export_ports_env() in functions.sh for more information.
#

# MAINTAINER: portmgr@FreeBSD.org

set -o pipefail

# Backwards compat for Poudriere
if [ "${SCRIPTSDIR-__null}" != "__null" ]; then
	dp_SCRIPTSDIR="${SCRIPTSDIR}"
fi
if [ "${MAKE-__null}" != "__null" ]; then
	dp_MAKE="${MAKE}"
fi

if [ -z "${dp_SCRIPTSDIR}" ]; then
	echo "Must set dp_SCRIPTSDIR" >&2
	exit 1
fi

. ${dp_SCRIPTSDIR}/functions.sh

export_ports_env
