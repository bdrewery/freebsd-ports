
$FreeBSD$

--- sip/b2b/AuthAgent.cxx	2003/01/19 16:15:54	1.1
+++ sip/b2b/AuthAgent.cxx	2003/01/19 18:03:43
@@ -367,6 +367,31 @@
 AuthAgent::doCancel()
 {
     cpLog(LOG_DEBUG, "AuthAgent::doCancel()");
+    Sptr<MultiLegCallData> mData =
+		CallDB::instance().getMultiLegCallData(myInvokee->getCallLeg());
+    assert(mData != 0);
+
+    Sptr<AccountingData> accData = mData->getAccountingData();
+    time_t t;
+    t = time(&t);
+    accData->setEndTime(t);
+
+    if(B2bConfig::instance().getStr(B2bConfig::PP_OPTIONS)
+		   != B2bConfig::PP_FREE)
+    {
+	///Send Stop records to billing
+	int duration = t - accData->getStartTime();
+	Sptr<BillingData> bData;
+	bData.dynamicCast(myAAAData);
+	assert(bData != 0);
+	bData->setDuration(duration);
+
+	Sptr<AAAEvent> aEvent = new AAAEvent(this);
+	cpLog(LOG_DEBUG, "AuthAgent::Sending Acct Stop");
+	B2bUa::instance().getAAATransceiver()->sendAcctStop(aEvent);
+    }
+
+    ///Transit the controller state
     myState->cancel(*this);
 }
 
@@ -424,6 +449,31 @@
 AuthAgent::callFailed()
 {
     cpLog(LOG_DEBUG, "AuthAgent::callFailed()");
+    Sptr<MultiLegCallData> mData =
+		CallDB::instance().getMultiLegCallData(myInvokee->getCallLeg());
+    assert(mData != 0);
+
+    Sptr<AccountingData> accData = mData->getAccountingData();
+    time_t t;
+    t = time(&t);
+    accData->setEndTime(t);
+
+    if(B2bConfig::instance().getStr(B2bConfig::PP_OPTIONS)
+		   != B2bConfig::PP_FREE)
+    {
+	///Send Stop records to billing
+	int duration = t - accData->getStartTime();
+	Sptr<BillingData> bData;
+	bData.dynamicCast(myAAAData);
+	assert(bData != 0);
+	bData->setDuration(duration);
+
+	Sptr<AAAEvent> aEvent = new AAAEvent(this);
+	cpLog(LOG_DEBUG, "AuthAgent::Sending Acct Stop");
+	B2bUa::instance().getAAATransceiver()->sendAcctStop(aEvent);
+    }
+
+    ///Transit the controller state
     myState->fail(*this);
 }
 
@@ -488,5 +538,37 @@
 void 
 AuthAgent::receivedStatus(UaBase& agent, const Sptr<SipMsg>& msg)
 {
+    Sptr<StatusMsg> statusMsg;
+    statusMsg.dynamicCast(msg);
+    assert(statusMsg != 0);
+    int statusCode = statusMsg->getStatusLine().getStatusCode();
+    if(statusCode > 200)
+    {
+	Sptr<MultiLegCallData> mData =
+		CallDB::instance().getMultiLegCallData(myInvokee->getCallLeg());
+	assert(mData != 0);
+
+	Sptr<AccountingData> accData = mData->getAccountingData();
+	time_t t;
+	t = time(&t);
+	accData->setEndTime(t);
+
+	if(B2bConfig::instance().getStr(B2bConfig::PP_OPTIONS)
+		   != B2bConfig::PP_FREE)
+	{
+	    ///Send Stop records to billing
+	    int duration = t - accData->getStartTime();
+	    Sptr<BillingData> bData;
+	    bData.dynamicCast(myAAAData);
+	    assert(bData != 0);
+	    bData->setDuration(duration);
+	    bData->setTermCause(BT_SERVICE_UNAVAIL);
+
+	    Sptr<AAAEvent> aEvent = new AAAEvent(this);
+	    cpLog(LOG_DEBUG, "AuthAgent::Sending Acct Stop");
+	    B2bUa::instance().getAAATransceiver()->sendAcctStop(aEvent);
+	}
+    }
+
     MultiLegCallControl::instance().receivedStatus(agent,msg);
 }
