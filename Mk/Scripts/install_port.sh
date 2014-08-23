#! /bin/sh

[ $# -eq 3 ] || { echo "Usage: $0 ORIGIN PKGNAME REAL_INSTALL_TARGET" >&2
    exit 1;}
[ -n "${PORTSDIR}" ] || { echo "PORTSDIR not set" >&2; exit 1;}
[ -n "${SCRIPTSDIR}" ] || { echo "SCRIPTSDIR not set" >&2; exit 1;}
[ -n "${MAKE}" ] || { echo "MAKE not set" >&2; exit 1;}
[ -n "${PKG_SUFX}" ] || { echo "PKG_SUFX not set" >&2; exit 1;}
[ -n "${PACKAGES}" ] || { echo "PACKAGES not set" >&2; exit 1;}
ORIGIN="$1"
PKGNAME="$2"
REAL_INSTALL_TARGET="$3"

: ${PKG_BIN:=pkg-static}

# Determine main package

pkg_available="$(sh ${SCRIPTSDIR}/want_pkg.sh ${ORIGIN})"

case ${pkg_available} in
	remote|local)
		echo "====> Installing ${ORIGIN} from ${pkg_available} package"
		;;
	none)
		echo "====> Building ${ORIGIN} from ports"
		;;
esac
case ${pkg_available} in
	remote)
		# Fetch remote package
		: ${PACKAGES:=$(mktemp -d ports)}
		${PKG_BIN} fetch -o ${PACKAGES} -Uy ${ORIGIN} || {
			echo "Failed to fetch ${ORIGIN}" >&2
			exit 1
		}
		;;
	local)
		# Install local PKGFILE
		;;
	none)
		# Must build it.
		${MAKE} -C ${PORTSDIR}/${ORIGIN} build-depends
		exec ${MAKE} -C ${PORTSDIR}/${ORIGIN} ${REAL_INSTALL_TARGET}
		;;
esac

export PACKAGES
exec ${MAKE} -C ${PORTSDIR}/${ORIGIN} install-package
