--- gsmlib/gsm_me_ta.h.orig	Wed Dec 20 17:15:09 2006
+++ gsmlib/gsm_me_ta.h	Wed Dec 20 17:16:27 2006
@@ -291,8 +291,8 @@
     // 3 disable phone receive RF circuits only
     // 4 disable phone both transmit and receive RF circuits
     // 5...127 implementation-defined
-    int MeTa::getFunctionalityLevel() throw(GsmException);
-    void MeTa::setFunctionalityLevel(int level) throw(GsmException);
+    int getFunctionalityLevel() throw(GsmException);
+    void setFunctionalityLevel(int level) throw(GsmException);
 
     // return battery charge status (+CBC):
     // 0 ME is powered by the battery
@@ -386,13 +386,13 @@
     void setCallWaitingLockStatus(FacilityClass cl,
                                   bool lock)throw(GsmException);
 
-    void MeTa::setCLIRPresentation(bool enable) throw(GsmException);
+    void setCLIRPresentation(bool enable) throw(GsmException);
     //(+CLIR)
     
     // 0:according to the subscription of the CLIR service
     // 1:CLIR invocation
     // 2:CLIR suppression
-    int MeTa::getCLIRPresentation() throw(GsmException);
+    int getCLIRPresentation() throw(GsmException);
 
     friend class Phonebook;
     friend class SMSStore;
