PORTNAME=	kio
DISTVERSION=	${KDE_FRAMEWORKS_VERSION}
CATEGORIES=	devel kde kde-frameworks

MAINTAINER=	kde@FreeBSD.org
COMMENT=	KF5 resource and network access abstraction

USES=		cmake compiler:c++11-lib desktop-file-utils \
		gettext gnome kde:5 qt:5 ssl tar:xz
USE_GNOME=	libxml2 libxslt
USE_KDE=	archive auth bookmarks codecs completion config \
		configwidgets coreaddons crash dbusaddons doctools ecm \
		i18n iconthemes itemviews jobwidgets kded \
		notifications service solid sonnet textwidgets wallet \
		widgetsaddons windowsystem xmlgui
USE_QT=		concurrent core dbus declarative gui network \
		script widgets x11extras xml \
		buildtools_build qmake_build

.include <bsd.port.mk>
