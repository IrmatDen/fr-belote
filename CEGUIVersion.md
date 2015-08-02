## CEGUI changeset in use ##
2e6f3d66b765

## Shelve file ##

Reasons:
  * Lua bindings are a bit lagging behind their current codebase because they mainly use Python
  * Soft validation added to CEGUI::EditBox (more info available [here](http://www.cegui.org.uk/phpBB2/viewtopic.php?f=10&t=5447&start=15))

Shelve content:

```
diff --git a/cegui/include/CEGUIPCRERegexMatcher.h b/cegui/include/CEGUIPCRERegexMatcher.h
--- a/cegui/include/CEGUIPCRERegexMatcher.h
+++ b/cegui/include/CEGUIPCRERegexMatcher.h
@@ -45,10 +45,11 @@
     ~PCRERegexMatcher();
 
     // implement required interface
-    void setRegexString(const String& regex);
-    const String& getRegexString() const;
-    bool matchRegex(const String& str) const;
+    void			setRegexString(const String& regex);
+    const String&	getRegexString() const;
+    MatchState		matchRegexSoft(const String& str) const;
+    bool			matchRegex(const String& str) const;
 
 private:
     //! free the compiled PCRE regex, if any.
     void release();
@@ -51,7 +52,9 @@
 
 private:
     //! free the compiled PCRE regex, if any.
     void release();
+	//! implementation of the actual regex matching.
+	MatchState doMatch(const String& str, bool partial_match_allowed) const;
 
     //! Copy of the regex string assigned.
     String d_string;
diff --git a/cegui/include/CEGUIRegexMatcher.h b/cegui/include/CEGUIRegexMatcher.h
--- a/cegui/include/CEGUIRegexMatcher.h
+++ b/cegui/include/CEGUIRegexMatcher.h
@@ -40,4 +40,15 @@
     public AllocatedObject<RegexMatcher>
 {
 public:
+    //! enumerates possible match modes
+    enum MatchState
+    {
+        //! Input string clearly can't be a match, even in the future
+        MS_Invalid,
+        //! Input string is valid, but incomplete
+        MS_Intermediate,
+        //! Input string is valid as a final result
+        MS_Acceptable
+    };
+
     //! Destructor.
@@ -43,11 +54,21 @@
     //! Destructor.
-    virtual ~RegexMatcher() {}
-    //! Set the regex string that will be matched against.
-    virtual void setRegexString(const String& regex) = 0;
-    //! Return reference to current regex string set.
-    virtual const String& getRegexString() const = 0;
-    //! Return whether a given string matches the set regex.
-    virtual bool matchRegex(const String& str) const = 0;
+    virtual					~RegexMatcher() {}
+    
+	//! Set the regex string that will be matched against.
+    virtual void			setRegexString(const String& regex) = 0;
+    
+	//! Return reference to current regex string set.
+    virtual const String&	getRegexString() const = 0;
+    
+	//! Return compliancy of the input string with the validator.
+    virtual MatchState		matchRegexSoft(const String& str) const = 0;
+    
+	//! Return whether a given string matches the set regex.
+	// This method is still present to allow full checks to use eventual regex engine optimization when a partial match is not desired.
+    virtual bool			matchRegex(const String& str) const
+	{
+		return matchRegexSoft(str) == MS_Acceptable;
+	}
 };
 
 } // End of  CEGUI namespace section
diff --git a/cegui/include/elements/CEGUIEditbox.h b/cegui/include/elements/CEGUIEditbox.h
--- a/cegui/include/elements/CEGUIEditbox.h
+++ b/cegui/include/elements/CEGUIEditbox.h
@@ -33,6 +33,7 @@
 #include "../CEGUIBase.h"
 #include "../CEGUIWindow.h"
 #include "CEGUIEditboxProperties.h"
+#include "CEGUIRegexMatcher.h"
 
 #if defined(_MSC_VER)
 #   pragma warning(push)
@@ -111,6 +112,12 @@
      * WindowEventArgs::window set to the Editbox whose text has become invalid.
      */
     static const String EventTextInvalidated;
+    /** Event fired when the current text has become a possible match as regards
+     * to the validation string.
+     * Handlers are passed a const WindowEventArgs reference with
+     * WindowEventArgs::window set to the Editbox whose text has become invalid.
+     */
+    static const String EventValidEntry;
     /** Event fired when the user attempts to chage the text in a way that would
      * make it invalid as regards to the validation string.
      * Handlers are passed a const WindowEventArgs reference with
@@ -431,7 +438,7 @@
         return true if the given string matches the validation regular
         expression.
     */
-    bool isStringValid(const String& str) const;
+    bool isStringValid(const String& str, bool check_hard_match = true) const;
 
     //! Processing for backspace key
     void handleBackspace(void);
@@ -518,10 +525,12 @@
         Handler called when something has caused the current text to now fail
         validation.
 
-        This can be caused by changing the validation string or setting a
-        maximum length that causes the current text to be truncated.
+        This can be caused by changing the validation string, setting a
+        maximum length that causes the current text to be truncated or simply
+		while the user modifies the text in a way that doesn't make the input
+		fully valid.
     */
     virtual void onTextInvalidatedEvent(WindowEventArgs& e);
 
     /*!
     \brief
@@ -523,8 +532,15 @@
     */
     virtual void onTextInvalidatedEvent(WindowEventArgs& e);
 
     /*!
     \brief
+        Handler called when something has caused the current text to now succeed
+        validation.
+    */
+    virtual void onValidEntryEvent(WindowEventArgs& e);
+
+    /*!
+    \brief
         Handler called when the user attempted to make a change to the edit box
         that would have caused it to fail validation.
     */
@@ -566,6 +582,7 @@
     void onCharacter(KeyEventArgs& e);
     void onKeyDown(KeyEventArgs& e);
     void onTextChanged(WindowEventArgs& e);
+	void onDeactivated(ActivationEventArgs &e);
 
     //! True if the editbox is in read-only mode
     bool d_readOnly;
@@ -589,6 +606,8 @@
     bool d_dragging;
     //! Selection index for drag selection anchor point.
     size_t d_dragAnchorIdx;
+	//! Remembers how the current text matches the validation string.
+	mutable RegexMatcher::MatchState d_lastMatchState;
 
 private:
     static EditboxProperties::ReadOnly         d_readOnlyProperty;
diff --git a/cegui/src/CEGUIPCRERegexMatcher.cpp b/cegui/src/CEGUIPCRERegexMatcher.cpp
--- a/cegui/src/CEGUIPCRERegexMatcher.cpp
+++ b/cegui/src/CEGUIPCRERegexMatcher.cpp
@@ -52,7 +52,7 @@
     // try to compile this new regex string
     const char* prce_error;
     int pcre_erroff;
-    d_regex = pcre_compile(regex.c_str(), PCRE_UTF8,
+    d_regex = pcre_compile(regex.c_str(), PCRE_UTF8 | PCRE_ANCHORED,
                            &prce_error, &pcre_erroff, 0);
 
     // handle failure
@@ -72,5 +72,11 @@
 }
 
 //----------------------------------------------------------------------------//
+RegexMatcher::MatchState PCRERegexMatcher::matchRegexSoft(const String& str) const
+{
+	return doMatch(str, true);
+}
+
+//----------------------------------------------------------------------------//
 bool PCRERegexMatcher::matchRegex(const String& str) const
 {
@@ -75,26 +81,6 @@
 bool PCRERegexMatcher::matchRegex(const String& str) const
 {
-    // if the regex is not valid, then an exception is thrown
-    if (!d_regex)
-        CEGUI_THROW(InvalidRequestException("PCRERegexMatcher::matchRegex: "
-            "Attempt to use invalid RegEx '" + d_string + "'."));
-
-    int match[3];
-    const char* utf8_str = str.c_str();
-    const int len = static_cast<int>(strlen(utf8_str));
-    const int result = pcre_exec(d_regex, 0, utf8_str, len, 0, 0, match, 3);
-
-    // a match must be for the entire string
-    if (result >= 0)
-        return (match[1] - match[0] == len);
-    // no match found or if test string or regex is 0
-    else if ((result == PCRE_ERROR_NOMATCH) || (result == PCRE_ERROR_NULL))
-        return false;
-    // anything else is an error
-    else
-        CEGUI_THROW(InvalidRequestException("PCRERegexMatcher::matchRegex: "
-            "An internal error occurred while attempting to match the RegEx '" +
-            d_string + "'."));
+	return doMatch(str, false) == MS_Acceptable;
 }
 
 //----------------------------------------------------------------------------//
@@ -107,4 +93,45 @@
     }
 }
 
+//----------------------------------------------------------------------------//
+RegexMatcher::MatchState PCRERegexMatcher::doMatch(const String& str, bool partial_match_allowed) const
+{
+    // if the regex is not valid, then an exception is thrown
+    if (!d_regex)
+        CEGUI_THROW(InvalidRequestException("PCRERegexMatcher::matchRegex: "
+            "Attempt to use invalid RegEx '" + d_string + "'."));
+
+    int match[3];
+    const char* utf8_str = str.c_str();
+    const int len = static_cast<int>(strlen(utf8_str));
+
+	// nothing to check if the string is empty
+	if (len == 0)
+		return MS_Intermediate;
+
+	const int pcre_options = (partial_match_allowed ? (PCRE_PARTIAL_SOFT | PCRE_ANCHORED) : PCRE_ANCHORED);
+    const int result = pcre_exec(d_regex, 0, utf8_str, len, 0, pcre_options, match, 3);
+
+	MatchState matchState = MS_Invalid;
+
+	if (match[1] - match[0] == len)
+	{
+		if (result >= 0)
+			matchState = MS_Acceptable;
+		else if (result == PCRE_ERROR_PARTIAL) // this return code will never appear if PCRE_PARTIAL_SOFT is *not* set, no need for double checking.
+			matchState = MS_Intermediate;
+	}
+
+    // a match must be for the entire string
+    if ((result < 0) && (result != PCRE_ERROR_PARTIAL) &&
+		(result != PCRE_ERROR_NOMATCH) && (result != PCRE_ERROR_NULL))
+	{
+        CEGUI_THROW(InvalidRequestException("PCRERegexMatcher::matchRegex: "
+            "An internal error occurred while attempting to match the RegEx '" +
+            d_string + "'."));
+	}
+	
+	return matchState;
+}
+
 } // End of  CEGUI namespace section
diff --git a/cegui/src/ScriptingModules/LuaScriptModule/package/FontManager.pkg b/cegui/src/ScriptingModules/LuaScriptModule/package/FontManager.pkg
--- a/cegui/src/ScriptingModules/LuaScriptModule/package/FontManager.pkg
+++ b/cegui/src/ScriptingModules/LuaScriptModule/package/FontManager.pkg
@@ -5,7 +5,7 @@
 {
 	static FontManager& getSingleton();
 
-    tolua_throws|CEGUI::AlreadyExistsException,error| Font& create(utf8string xml_filename, utf8string resourcegroup="");
+    tolua_throws|CEGUI::AlreadyExistsException,error| Font& createFromFile(utf8string xml_filename, utf8string resourcegroup="");
     tolua_throws|CEGUI::AlreadyExistsException,error|CEGUI::GenericException,error| Font& createFreeTypeFont(utf8string font_name, float point_size, bool anti_aliased,
                                                                 string font_filename, utf8string resource_group = "", bool auto_scaled = false,
                                                                 float native_horz_res = 640.0f, float native_vert_res = 480.0f,
diff --git a/cegui/src/ScriptingModules/LuaScriptModule/package/SchemeManager.pkg b/cegui/src/ScriptingModules/LuaScriptModule/package/SchemeManager.pkg
--- a/cegui/src/ScriptingModules/LuaScriptModule/package/SchemeManager.pkg
+++ b/cegui/src/ScriptingModules/LuaScriptModule/package/SchemeManager.pkg
@@ -5,7 +5,7 @@
 {
 	static SchemeManager& getSingleton();
 
-	Scheme& create(utf8string filename, utf8string resourcegroup="");
+	Scheme& createFromFile(utf8string filename, utf8string resourcegroup="");
 	void destroy(utf8string name);
 	bool isDefined(utf8string name) const;
 	Scheme& get(utf8string name) const;
diff --git a/cegui/src/ScriptingModules/LuaScriptModule/package/Window.pkg b/cegui/src/ScriptingModules/LuaScriptModule/package/Window.pkg
--- a/cegui/src/ScriptingModules/LuaScriptModule/package/Window.pkg
+++ b/cegui/src/ScriptingModules/LuaScriptModule/package/Window.pkg
@@ -282,7 +282,7 @@
     void setMouseInputPropagationEnabled(bool enabled);
     bool isMouseInputPropagationEnabled() const;
 
-    Window* clone(utf8string newName, bool deepCopy = true) const;
+    Window* clone(bool deepCopy = true) const;
 
     unsigned int getZIndex() const;
     bool isInFront(const Window& wnd) const;
diff --git a/cegui/src/ScriptingModules/LuaScriptModule/package/WindowManager.pkg b/cegui/src/ScriptingModules/LuaScriptModule/package/WindowManager.pkg
--- a/cegui/src/ScriptingModules/LuaScriptModule/package/WindowManager.pkg
+++ b/cegui/src/ScriptingModules/LuaScriptModule/package/WindowManager.pkg
@@ -6,5 +6,5 @@
 	static WindowManager& getSingleton();
 
 	tolua_throws|CEGUI::AlreadyExistsException,nil|CEGUI::InvalidRequestException,nil| Window* createWindow(utf8string type, utf8string name="");
-	tolua_throws|CEGUI::Exception,error|std::exception,error|any,error| Window* loadWindowLayout(utf8string filename, utf8string resourcegroup="");
+	tolua_throws|CEGUI::Exception,error|std::exception,error|any,error| Window* loadLayoutFromFile(utf8string filename, utf8string resourcegroup="");
 
@@ -10,6 +10,6 @@
 
-    void writeWindowLayoutToStream(const Window& window, OutStream& out, bool writeParent=false) const;
-    void saveWindowLayout(const Window& window, utf8string filename, bool writeParent = false) const;
+    void writeLayoutToStream(const Window& window, OutStream& out, bool writeParent=false) const;
+    void saveLayoutToFile(const Window& window, utf8string filename, bool writeParent = false) const;
 
 	void destroyWindow(Window* window);
 	void destroyAllWindows();
diff --git a/cegui/src/elements/CEGUIEditbox.cpp b/cegui/src/elements/CEGUIEditbox.cpp
--- a/cegui/src/elements/CEGUIEditbox.cpp
+++ b/cegui/src/elements/CEGUIEditbox.cpp
@@ -71,6 +71,7 @@
 const String Editbox::EventValidationStringChanged( "ValidationStringChanged" );
 const String Editbox::EventMaximumTextLengthChanged( "MaximumTextLengthChanged" );
 const String Editbox::EventTextInvalidated("TextInvalidated");
+const String Editbox::EventValidEntry("ValidEntry");
 const String Editbox::EventInvalidEntryAttempted( "InvalidEntryAttempted" );
 const String Editbox::EventCaretMoved( "CaretMoved" );
 const String Editbox::EventTextSelectionChanged( "TextSelectionChanged" );
@@ -276,4 +277,8 @@
         {
             String newText = getText();
             newText.resize(d_maxTextLen);
+
+			// Ensure the valid/invalid entry event isn't fire here.
+			const bool eventsMuted = isMuted();
+			setMutedState(true);
             setText(newText);
@@ -279,4 +284,6 @@
             setText(newText);
+			setMutedState(false);
+
             onTextChanged(args);
 
             // see if new text is valid
@@ -314,4 +321,8 @@
         {
             String newText = getText();
             newText.erase(getSelectionStartIndex(), getSelectionLength());
+
+			// Ensure the valid/invalid entry event isn't fire here.
+			const bool eventsMuted = isMuted();
+			setMutedState(true);
             setText(newText);
@@ -317,4 +328,5 @@
             setText(newText);
+			setMutedState(false);
 
             // trigger notification that text has changed.
             WindowEventArgs args(this);
@@ -326,5 +338,5 @@
 }
 
 //----------------------------------------------------------------------------//
-bool Editbox::isStringValid(const String& str) const
+bool Editbox::isStringValid(const String& str, bool check_hard_match) const
 {
@@ -330,5 +342,16 @@
 {
-    return d_validator ? d_validator->matchRegex(str) : true;
+	if (!d_validator)
+	{
+		d_lastMatchState = RegexMatcher::MS_Acceptable;
+		return true;
+	}
+	
+	d_lastMatchState = d_validator->matchRegexSoft(str);
+
+	if (check_hard_match)
+		return d_lastMatchState == RegexMatcher::MS_Acceptable;
+
+	return d_lastMatchState != RegexMatcher::MS_Invalid;
 }
 
 //----------------------------------------------------------------------------//
@@ -463,6 +486,8 @@
     // fire event.
     fireEvent(EventCharacterKey, e, Window::EventNamespace);
 
+	RegexMatcher::MatchState oldMatchState = d_lastMatchState;
+
     // only need to take notice if we have focus
     if (e.handled == 0 && hasInputFocus() && !isReadOnly() &&
         getFont()->isCodepointAvailable(e.codepoint))
@@ -476,7 +501,7 @@
         {
             tmp.insert(getSelectionStartIndex(), 1, e.codepoint);
 
-            if (isStringValid(tmp))
+            if (isStringValid(tmp, false))
             {
                 // erase selection using mode that does not modify getText()
                 // (we just want to update state)
@@ -486,8 +511,12 @@
                 // handlers!)
                 d_caretPos++;
 
-                // set text to the newly modified string
-                setText(tmp);
+                // set text to the newly modified string while
+				// ensuring the valid/invalid entry event isn't fire here.
+				const bool eventsMuted = isMuted();
+				setMutedState(true);
+				setText(tmp);
+				setMutedState(false);
 
                 // char was accepted into the Editbox - mark event as handled.
                 ++e.handled;
@@ -491,6 +520,18 @@
 
                 // char was accepted into the Editbox - mark event as handled.
                 ++e.handled;
+
+				// Check if the string is a possible full match, and react accordingly.
+				if (d_lastMatchState == RegexMatcher::MS_Acceptable)
+				{
+					WindowEventArgs args(this);
+					onValidEntryEvent(args);
+				}
+				else // otherwise, just signify to event listeners that text isn't yet valid.
+				{
+					WindowEventArgs args(this);
+					onInvalidEntryAttempted(args);
+				}
             }
             else
             {
@@ -494,9 +535,8 @@
             }
             else
             {
-                // Trigger invalid modification attempted event.
-                WindowEventArgs args(this);
-                onInvalidEntryAttempted(args);
+				// *Adding* an invalid character doesn't make the entry invalidated since it is not actually added to the editbox's text.
+				d_lastMatchState = oldMatchState;
             }
 
         }
@@ -582,6 +622,21 @@
 }
 
 //----------------------------------------------------------------------------//
+void Editbox::onDeactivated(ActivationEventArgs &e)
+{
+	if (!isStringValid(getText()))
+	{
+		// Trigger invalid modification attempted event.
+		WindowEventArgs args(this);
+		onInvalidEntryAttempted(args);
+	}
+
+	Window::onDeactivated(e);
+
+	++e.handled;
+}
+
+//----------------------------------------------------------------------------//
 void Editbox::handleBackspace(void)
 {
     if (!isReadOnly())
@@ -592,9 +647,9 @@
         {
             tmp.erase(getSelectionStartIndex(), getSelectionLength());
 
-            if (isStringValid(tmp))
+            if (isStringValid(tmp, false))
             {
                 // erase selection using mode that does not modify getText()
                 // (we just want to update state)
                 eraseSelectedText(false);
 
@@ -596,10 +651,26 @@
             {
                 // erase selection using mode that does not modify getText()
                 // (we just want to update state)
                 eraseSelectedText(false);
 
-                // set text to the newly modified string
-                setText(tmp);
+                // set text to the newly modified string while
+				// ensure the valid/invalid entry event isn't fire here.
+				const bool eventsMuted = isMuted();
+				setMutedState(true);
+				setText(tmp);
+				setMutedState(false);
+
+				// Check if the string is a possible full match, and react accordingly.
+				if (d_lastMatchState == RegexMatcher::MS_Acceptable)
+				{
+					WindowEventArgs args(this);
+					onValidEntryEvent(args);
+				}
+				else // otherwise, just signify to event listeners that text isn't yet valid.
+				{
+					WindowEventArgs args(this);
+					onInvalidEntryAttempted(args);
+				}
             }
             else
             {
@@ -613,7 +684,7 @@
         {
             tmp.erase(d_caretPos - 1, 1);
 
-            if (isStringValid(tmp))
+            if (isStringValid(tmp, false))
             {
                 setCaretIndex(d_caretPos - 1);
 
@@ -617,8 +688,24 @@
             {
                 setCaretIndex(d_caretPos - 1);
 
-                // set text to the newly modified string
-                setText(tmp);
+                // set text to the newly modified string while
+				// ensuring the valid/invalid entry event isn't fire here.
+				const bool eventsMuted = isMuted();
+				setMutedState(true);
+				setText(tmp);
+				setMutedState(false);
+
+				// Check if the string is a possible full match, and react accordingly.
+				if (d_lastMatchState == RegexMatcher::MS_Acceptable)
+				{
+					WindowEventArgs args(this);
+					onValidEntryEvent(args);
+				}
+				else // otherwise, just signify to event listeners that text isn't yet valid.
+				{
+					WindowEventArgs args(this);
+					onInvalidEntryAttempted(args);
+				}
             }
             else
             {
@@ -644,9 +731,9 @@
         {
             tmp.erase(getSelectionStartIndex(), getSelectionLength());
 
-            if (isStringValid(tmp))
+            if (isStringValid(tmp, false))
             {
                 // erase selection using mode that does not modify getText()
                 // (we just want to update state)
                 eraseSelectedText(false);
 
@@ -648,10 +735,26 @@
             {
                 // erase selection using mode that does not modify getText()
                 // (we just want to update state)
                 eraseSelectedText(false);
 
-                // set text to the newly modified string
-                setText(tmp);
+                // set text to the newly modified string while
+				// ensuring the valid/invalid entry event isn't fire here.
+				const bool eventsMuted = isMuted();
+				setMutedState(true);
+				setText(tmp);
+				setMutedState(false);
+
+				// Check if the string is a possible full match, and react accordingly.
+				if (d_lastMatchState == RegexMatcher::MS_Acceptable)
+				{
+					WindowEventArgs args(this);
+					onValidEntryEvent(args);
+				}
+				else // otherwise, just signify to event listeners that text isn't yet valid.
+				{
+					WindowEventArgs args(this);
+					onInvalidEntryAttempted(args);
+				}
             }
             else
             {
@@ -665,5 +768,5 @@
         {
             tmp.erase(d_caretPos, 1);
 
-            if (isStringValid(tmp))
+            if (isStringValid(tmp, false))
             {
@@ -669,6 +772,22 @@
             {
-                // set text to the newly modified string
-                setText(tmp);
+                // set text to the newly modified string while
+				// ensuring the valid/invalid entry event isn't fire here.
+				const bool eventsMuted = isMuted();
+				setMutedState(true);
+				setText(tmp);
+				setMutedState(false);
+
+				// Check if the string is a possible full match, and react accordingly.
+				if (d_lastMatchState == RegexMatcher::MS_Acceptable)
+				{
+					WindowEventArgs args(this);
+					onValidEntryEvent(args);
+				}
+				else // otherwise, just signify to event listeners that text isn't yet valid.
+				{
+					WindowEventArgs args(this);
+					onInvalidEntryAttempted(args);
+				}
             }
             else
             {
@@ -798,6 +917,12 @@
 }
 
 //----------------------------------------------------------------------------//
+void Editbox::onValidEntryEvent(WindowEventArgs& e)
+{
+    fireEvent(EventValidEntry, e, EventNamespace);
+}
+
+//----------------------------------------------------------------------------//
 void Editbox::onInvalidEntryAttempted(WindowEventArgs& e)
 {
     fireEvent(EventInvalidEntryAttempted , e, EventNamespace);
@@ -826,7 +951,16 @@
 //----------------------------------------------------------------------------//
 void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
 {
-    fireEvent(EventTextAccepted, e, EventNamespace);
+	if (isStringValid(getText()))
+	{
+		fireEvent(EventTextAccepted, e, EventNamespace);
+	}
+	else
+	{
+		// Trigger invalid modification attempted event.
+		WindowEventArgs args(this);
+		onInvalidEntryAttempted(args);
+	}
 }
 
 //----------------------------------------------------------------------------//
@@ -842,6 +976,18 @@
     if (d_caretPos > getText().length())
         setCaretIndex(getText().length());
 
+	// allows validation events to be fired
+	if (isStringValid(getText(), true))
+	{
+		WindowEventArgs args(this);
+		onValidEntryEvent(args);
+	}
+	else
+	{
+		WindowEventArgs args(this);
+		onInvalidEntryAttempted(args);
+	}
+
     ++e.handled;
 }
 
```