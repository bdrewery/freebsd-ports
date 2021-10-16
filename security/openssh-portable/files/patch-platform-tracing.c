--- platform-tracing.c.orig	2021-10-15 11:00:04.976766000 -0700
+++ platform-tracing.c	2021-10-15 11:01:36.757144000 -0700
@@ -16,6 +16,10 @@
 
 #include "includes.h"
 
+#if defined(HAVE_PROCCTL)
+#include <string.h>
+#include <unistd.h>
+#endif
 #include <sys/types.h>
 #ifdef HAVE_SYS_PROCCTL_H
 #include <sys/procctl.h>
@@ -40,8 +44,9 @@ platform_disable_tracing(int strict)
 {	/* On FreeBSD, we should make this process untraceable */
 	int disable_trace = PROC_TRACE_CTL_DISABLE;
 
-	if (procctl(P_PID, 0, PROC_TRACE_CTL, &disable_trace) && strict)
-		fatal("unable to make the process untraceable");
+	if (procctl(P_PID, getpid(), PROC_TRACE_CTL, &disable_trace) && strict)
+		fatal("unable to make the process untraceable: %s for pid %d",
+		    strerror(errno), (int)getpid());
 }
 #endif
 #if defined(HAVE_PRCTL) && defined(PR_SET_DUMPABLE)
