/////////////////////////////////////////////////////////////////////////////
// Name:        wx/html/winpars.h
// Purpose:     wxHtmlWinParser class (parser to be used with wxHtmlWindow)
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINPARS_H_
#define _WX_WINPARS_H_

#include "wx/defs.h"
#if wxUSE_HTML

#include "wx/module.h"
#include "wx/font.h"
#include "wx/html/htmlpars.h"
#include "wx/html/htmlcell.h"
#include "wx/encconv.h"

class WXDLLIMPEXP_FWD_HTML wxHtmlWindow;
class WXDLLIMPEXP_FWD_HTML wxHtmlWindowInterface;
class WXDLLIMPEXP_FWD_HTML wxHtmlWinParser;
class WXDLLIMPEXP_FWD_HTML wxHtmlWinTagHandler;
class WXDLLIMPEXP_FWD_HTML wxHtmlTagsModule;


//--------------------------------------------------------------------------------
// wxHtmlWinParser
//                  This class is derived from wxHtmlParser and its mail goal
//                  is to parse HTML input so that it can be displayed in
//                  wxHtmlWindow. It uses special wxHtmlWinTagHandler.
//--------------------------------------------------------------------------------

class WXDLLIMPEXP_HTML wxHtmlWinParser : public wxHtmlParser
{
    wxDECLARE_ABSTRACT_CLASS(wxHtmlWinParser);
    friend class wxHtmlWindow;

public:
    wxHtmlWinParser(wxHtmlWindowInterface *wndIface = nullptr);

    virtual ~wxHtmlWinParser();

    virtual void InitParser(const wxString& source) override;
    virtual void DoneParser() override;
    virtual wxObject* GetProduct() override;

    virtual wxFSFile *OpenURL(wxHtmlURLType type, const wxString& url) const override;

    // Set's the DC used for parsing. If SetDC() is not called,
    // parsing won't proceed
    virtual void SetDC(wxDC *dc, double pixel_scale = 1.0)
        { SetDC(dc, pixel_scale, pixel_scale); }
    void SetDC(wxDC *dc, double pixel_scale, double font_scale);

    wxDC *GetDC() {return m_DC;}
    double GetPixelScale() const { return m_PixelScale; }
    int GetCharHeight() const {return m_CharHeight;}
    int GetCharWidth() const {return m_CharWidth;}

    // NOTE : these functions do _not_ return _actual_
    // height/width. They return h/w of default font
    // for this DC. If you want actual values, call
    // GetDC()->GetChar...()

    // returns interface to the rendering window
    wxHtmlWindowInterface *GetWindowInterface() {return m_windowInterface;}

    // Sets fonts to be used when displaying HTML page. (if size null then default sizes used).
    void SetFonts(const wxString& normal_face, const wxString& fixed_face, const int *sizes = nullptr);

    // Sets font sizes to be relative to the given size or the system
    // default size; use either specified or default font
    void SetStandardFonts(int size = -1,
                          const wxString& normal_face = wxEmptyString,
                          const wxString& fixed_face = wxEmptyString);

    // Adds tags module. see wxHtmlTagsModule for details.
    static void AddModule(wxHtmlTagsModule *module);

    static void RemoveModule(wxHtmlTagsModule *module);

    // parsing-related methods. These methods are called by tag handlers:

    // Returns pointer to actual container. Common use in tag handler is :
    // m_WParser->GetContainer()->InsertCell(new ...);
    wxHtmlContainerCell *GetContainer() const {return m_Container;}

    // opens new container. This container is sub-container of opened
    // container. Sets GetContainer to newly created container
    // and returns it.
    wxHtmlContainerCell *OpenContainer();

    // works like OpenContainer except that new container is not created
    // but c is used. You can use this to directly set actual container
    wxHtmlContainerCell *SetContainer(wxHtmlContainerCell *c);

    // closes the container and sets actual Container to upper-level
    // container
    wxHtmlContainerCell *CloseContainer();

    int GetFontSize() const {return m_FontSize;}
    void SetFontSize(int s);
    // Try to map a font size in points to the HTML 1-7 font size range.
    void SetFontPointSize(int pt);
    int GetFontBold() const {return m_FontBold;}
    void SetFontBold(int x) {m_FontBold = x;}
    int GetFontItalic() const {return m_FontItalic;}
    void SetFontItalic(int x) {m_FontItalic = x;}
    int GetFontUnderlined() const {return m_FontUnderlined;}
    void SetFontUnderlined(int x) {m_FontUnderlined = x;}
    int GetFontFixed() const {return m_FontFixed;}
    void SetFontFixed(int x) {m_FontFixed = x;}
    wxString GetFontFace() const {return GetFontFixed() ? m_FontFaceFixed : m_FontFaceNormal;}
    void SetFontFace(const wxString& face);

    int GetAlign() const {return m_Align;}
    void SetAlign(int a) {m_Align = a;}

    wxHtmlScriptMode GetScriptMode() const { return m_ScriptMode; }
    void SetScriptMode(wxHtmlScriptMode mode) { m_ScriptMode = mode; }
    long GetScriptBaseline() const { return m_ScriptBaseline; }
    void SetScriptBaseline(long base) { m_ScriptBaseline = base; }

    const wxColour& GetLinkColor() const { return m_LinkColor; }
    void SetLinkColor(const wxColour& clr) { m_LinkColor = clr; }
    const wxColour& GetActualColor() const { return m_ActualColor; }
    void SetActualColor(const wxColour& clr) { m_ActualColor = clr ;}
    const wxColour& GetActualBackgroundColor() const { return m_ActualBackgroundColor; }
    void SetActualBackgroundColor(const wxColour& clr) { m_ActualBackgroundColor = clr;}
    int GetActualBackgroundMode() const { return m_ActualBackgroundMode; }
    void SetActualBackgroundMode(int mode) { m_ActualBackgroundMode = mode;}
    const wxHtmlLinkInfo& GetLink() const { return m_Link; }
    void SetLink(const wxHtmlLinkInfo& link);

    // applies current parser state (link, sub/supscript, ...) to given cell
    void ApplyStateToCell(wxHtmlCell *cell);

    // Needs to be called after inserting a cell that interrupts the flow of
    // the text like e.g. <img> and tells us to not consider any of the
    // following space as being part of the same space run as before.
    void StopCollapsingSpaces() { m_tmpLastWasSpace = false; }

    // creates font depending on m_Font* members.
    virtual wxFont* CreateCurrentFont();

    enum WhitespaceMode
    {
        Whitespace_Normal,  // normal mode, collapse whitespace
        Whitespace_Pre      // inside <pre>, keep whitespace as-is
    };

    // change the current whitespace handling mode
    void SetWhitespaceMode(WhitespaceMode mode) { m_whitespaceMode = mode; }
    WhitespaceMode GetWhitespaceMode() const { return m_whitespaceMode; }

protected:
    virtual void AddText(const wxString& txt) override;

private:
    void AddWord(wxHtmlWordCell *word);
    void AddWord(const wxString& word)
        { AddWord(new wxHtmlWordCell(word, *(GetDC()))); }
    void AddPreBlock(const wxString& text);

    bool m_tmpLastWasSpace;
    wxString m_tmpStrBuf;
        // temporary variables used by AddText
    wxHtmlWindowInterface *m_windowInterface;
            // window we're parsing for
    double m_PixelScale, m_FontScale;
    wxDC *m_DC;
            // Device Context we're parsing for
    static wxList m_Modules;
            // list of tags modules (see wxHtmlTagsModule for details)
            // This list is used to initialize m_Handlers member.

    wxHtmlContainerCell *m_Container;
            // current container. See Open/CloseContainer for details.

    int m_FontBold, m_FontItalic, m_FontUnderlined, m_FontFixed; // this is not true,false but 1,0, we need it for indexing
    int m_FontSize; // From 1 (smallest) to 7, default is 3.
    wxColour m_LinkColor;
    wxColour m_ActualColor;
    wxColour m_ActualBackgroundColor;
    int m_ActualBackgroundMode;
            // basic font parameters.
    wxHtmlLinkInfo m_Link;
            // actual hypertext link or empty string
    bool m_UseLink;
            // true if m_Link is not empty
    int m_CharHeight, m_CharWidth;
            // average height of normal-sized text
    int m_Align;
            // actual alignment
    wxHtmlScriptMode m_ScriptMode;
            // current script mode (sub/sup/normal)
    long m_ScriptBaseline;
            // current sub/supscript base

    wxFont* m_FontsTable[2][2][2][2][7];
    wxString m_FontsFacesTable[2][2][2][2][7];
            // table of loaded fonts. 1st four indexes are 0 or 1, depending on on/off
            // state of these flags (from left to right):
            // [bold][italic][underlined][fixed_size]
            // last index is font size : from 0 to 6 (remapped from html sizes 1 to 7)
            // Note : this table covers all possible combinations of fonts, but not
            // all of them are used, so many items in table are usually null.
    int m_FontsSizes[7];
    wxString m_FontFaceFixed, m_FontFaceNormal;
            // html font sizes and faces of fixed and proportional fonts

    // current whitespace handling mode
    WhitespaceMode m_whitespaceMode;

    wxHtmlWordCell *m_lastWordCell;

    // current position on line, in num. of characters; used to properly
    // expand TABs; only updated while inside <pre>
    int m_posColumn;

    wxDECLARE_NO_COPY_CLASS(wxHtmlWinParser);
};






//-----------------------------------------------------------------------------
// wxHtmlWinTagHandler
//                  This is basically wxHtmlTagHandler except
//                  it is extended with protected member m_Parser pointing to
//                  the wxHtmlWinParser object
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_HTML wxHtmlStyleParams;

class WXDLLIMPEXP_HTML wxHtmlWinTagHandler : public wxHtmlTagHandler
{
    wxDECLARE_ABSTRACT_CLASS(wxHtmlWinTagHandler);

public:
    wxHtmlWinTagHandler() : wxHtmlTagHandler() {}

    virtual void SetParser(wxHtmlParser *parser) override {wxHtmlTagHandler::SetParser(parser); m_WParser = (wxHtmlWinParser*) parser;}

protected:
    wxHtmlWinParser *m_WParser; // same as m_Parser, but overcasted

    void ApplyStyle(const wxHtmlStyleParams &styleParams);

    wxDECLARE_NO_COPY_CLASS(wxHtmlWinTagHandler);
};






//----------------------------------------------------------------------------
// wxHtmlTagsModule
//                  This is basic of dynamic tag handlers binding.
//                  The class provides methods for filling parser's handlers
//                  hash table.
//                  (See documentation for details)
//----------------------------------------------------------------------------

class WXDLLIMPEXP_HTML wxHtmlTagsModule : public wxModule
{
    wxDECLARE_DYNAMIC_CLASS(wxHtmlTagsModule);

public:
    wxHtmlTagsModule() : wxModule() {}

    virtual bool OnInit() override;
    virtual void OnExit() override;

    // This is called by wxHtmlWinParser.
    // The method must simply call parser->AddTagHandler(new
    // <handler_class_name>); for each handler
    virtual void FillHandlersTable(wxHtmlWinParser * WXUNUSED(parser)) { }
};


#endif

#endif // _WX_WINPARS_H_
