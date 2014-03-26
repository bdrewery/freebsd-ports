#! /bin/sh

[ $# -eq 1 ] || { echo "Usage: $0 ORIGIN" >&2; exit 1;}
[ -n "${PORTSDIR}" ] || { echo "PORTSDIR not set" >&2; exit 1;}
[ -n "${MAKE}" ] || { echo "MAKE not set" >&2; exit 1;}
ORIGIN="$1"

: ${PKG_BIN:=pkg-static}
: ${PKG_ABI:=$(${PKG_BIN} config ABI)}

WANTED_OPTIONS=$(${MAKE} -C ${PORTSDIR}/${ORIGIN} pretty-print-config |
    tr ' ' '\n' | sed -n 's/^\+\(.*\)/\1/p' | sort | paste -s -d ' ' -)

pkg_query() {
	local pattern="$1"
	local target="$2"
	local remote="${3}"

	if [ ${remote} -eq 1 ]; then
		${PKG_BIN} rquery -U "${pattern}" ${target}
	else
		${PKG_BIN} query -F ${target} "${pattern}"
	fi
}

pkg_get_options() {
	local target="$1"
	local remote="$2"

	pkg_query '%Ov%Ok' "${target}" ${remote} | sed '/^off/d;s/^on//' |
	    sort | paste -s -d ' ' -
}

pkg_get_abi() {
	local target="$1"
	local remote="$2"

	pkg_query '%q' "${target}" ${remote}
}

package_eligible() {
	local pkg_options="$1"
	local wanted_options="$2"
	local pkg_abi="$3"
	local wanted_abi="$4"

	[ "${pkg_options}" = "${wanted_options}" ] || return 1
	[ "${pkg_abi}" = "${wanted_abi}" ] || return 1
	# XXX: Compare version

	return 0
}

check_pkg() {
	local remote="$1"
	local target="$2"
	local wanted_options="$3"
	local wanted_abi="$4"
	local pkg_options pkg_abi

	pkg_options=$(pkg_get_options ${target} ${remote})
	pkg_abi=$(pkg_get_abi ${target} ${remote})
	if package_eligible "${pkg_options}" "${wanted_options}" "${pkg_abi}" \
	    "${wanted_abi}"; then
		if [ ${remote} -eq 0 ]; then
			echo "local"
		else
			echo "remote"
		fi
		return 0
	fi

	return 1
}


find_package() {
	local origin="$1"
	local wanted_options="$2"
	local wanted_abi="$3"
	local pkgfile

	# Is there a local package for this?
	pkgfile=$(${MAKE} -C ${PORTSDIR}/${origin} -V PKGFILE)
	[ -r "${pkgfile}" ] && check_pkg 0 "${pkgfile}" "${wanted_options}" \
	    "${wanted_abi}" && return 0

	# Is there a remote package?
	check_pkg 1 "${origin}" "${wanted_options}" "${wanted_abi}" &&
	    return 0

	return 1
}

if ! find_package "${ORIGIN}" "${WANTED_OPTIONS}" "${PKG_ABI}"; then
	echo "none"
	exit 1
fi

exit 0
