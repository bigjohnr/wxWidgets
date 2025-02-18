///////////////////////////////////////////////////////////////////////////////
// Name:        wx/richtooltip.h
// Purpose:     Declaration of wxRichToolTip class.
// Author:      Vadim Zeitlin
// Created:     2011-10-07
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RICHTOOLTIP_H_
#define _WX_RICHTOOLTIP_H_

#include "wx/defs.h"

#if wxUSE_RICHTOOLTIP

#include "wx/colour.h"

class WXDLLIMPEXP_FWD_CORE wxBitmapBundle;
class WXDLLIMPEXP_FWD_CORE wxFont;
class WXDLLIMPEXP_FWD_CORE wxRect;
class WXDLLIMPEXP_FWD_CORE wxWindow;

class wxRichToolTipImpl;
class wxRichToolTipInfo;

// This enum describes the kind of the tip shown which combines both the tip
// position and appearance because the two are related (when the tip is
// positioned asymmetrically, a right handed triangle is used but an
// equilateral one when it's in the middle of a side).
//
// Automatic selects the tip appearance best suited for the current platform
// and the position best suited for the window the tooltip is shown for, i.e.
// chosen in such a way that the tooltip is always fully on screen.
//
// Other values describe the position of the tooltip itself, not the window it
// relates to. E.g. wxTipKind_Top places the tip on the top of the tooltip and
// so the tooltip itself is located beneath its associated window.
enum wxTipKind
{
    wxTipKind_None,
    wxTipKind_TopLeft,
    wxTipKind_Top,
    wxTipKind_TopRight,
    wxTipKind_BottomLeft,
    wxTipKind_Bottom,
    wxTipKind_BottomRight,
    wxTipKind_Auto
};

// ----------------------------------------------------------------------------
// wxRichToolTip: a customizable but not necessarily native tooltip.
// ----------------------------------------------------------------------------

// Notice that this class does not inherit from wxWindow.
class WXDLLIMPEXP_ADV wxRichToolTip
{
public:
    // Ctor must specify the tooltip title and main message, additional
    // attributes can be set later.
    wxRichToolTip(const wxString& title, const wxString& message);
    
    // Construct using attributes set in wxRichToolTipInfo tipInfo.
    // Additional attributes can be set or modified later.
    wxRichToolTip(const wxRichToolTipInfo& tipInfo);

    // Set the background colour: if two colours are specified, the background
    // is drawn using a gradient from top to bottom, otherwise a single solid
    // colour is used.
    void SetBackgroundColour(const wxColour& col,
                             const wxColour& colEnd = wxColour());

    // Set the small icon to show: either one of the standard information/
    // warning/error ones (the question icon doesn't make sense for a tooltip)
    // or a custom icon.
    void SetIcon(int icon = wxICON_INFORMATION);
    void SetIcon(const wxBitmapBundle& icon);

    // Set timeout after which the tooltip should disappear, in milliseconds.
    // By default the tooltip is hidden after system-dependent interval of time
    // elapses but this method can be used to change this or also disable
    // hiding the tooltip automatically entirely by passing 0 in this parameter
    // (but doing this can result in native version not being used).
    // Optionally specify a show delay.
    void SetTimeout(unsigned milliseconds, unsigned millisecondsShowdelay = 0);

    // Choose the tip kind, possibly none. By default the tip is positioned
    // automatically, as if wxTipKind_Auto was used.
    void SetTipKind(wxTipKind tipKind);

    // Set the title text font. By default it's emphasized using the font style
    // or colour appropriate for the current platform.
    void SetTitleFont(const wxFont& font);

    // Show the tooltip for the given window and optionally a specified area.
    wxWindow* ShowFor(wxWindow* win, const wxRect* rect = nullptr);

    // Non-virtual dtor as this class is not supposed to be derived from.
    ~wxRichToolTip();

private:
    wxRichToolTipImpl* const m_impl;

    wxDECLARE_NO_COPY_CLASS(wxRichToolTip);
};

// ----------------------------------------------------------------------------
// wxRichToolTipInfo: class to store wxRichToolTip settings
//                    to avoid setting for each tooltip instance
// ----------------------------------------------------------------------------

#include "wx/icon.h"

class WXDLLIMPEXP_ADV wxRichToolTipInfo
{
public:
    wxRichToolTipInfo();
    wxRichToolTipInfo( const wxString& title, 
                       const wxString& message,
                       int icon = wxICON_INFORMATION );

    void SetTitleAndMessage( const wxString& title, const wxString& message );
    void SetTitle( const wxString& title )          { m_title = title; }    
    void SetMessage( const wxString& message )      { m_message = message; }    
    void SetBackgroundColour( const wxColour &col, 
                              const wxColour &colEnd = wxColour() );
    
    void SetTimes( unsigned millisecondsTimeout, unsigned millisecondsDelay );
    void SetTipKind( wxTipKind tipKind )            { m_tipKind = tipKind; }
    
    void SetTitleFont( const wxFont &font )         { m_font = font; }
    void SetIcon( int icon = wxICON_INFORMATION )   { m_iconId = icon; }
    void SetIcon( const wxIcon &icon )              { m_icon = icon; }

    wxString GetTitle() const               { return m_title; }
    wxString GetMessage() const             { return m_message; }

    wxTipKind GetTipKind() const            { return m_tipKind; }

    unsigned GetTimeout() const             { return m_timeOut; }
    unsigned GetDelay() const               { return m_timeDelay; }

    wxColour GetBackgroundColour() const    { return m_colourStart; }
    wxColour GetBackgroundEndColour() const { return m_colourEnd; }

    wxFont GetTitleFont() const             { return m_font; }

    int GetStandardIcon() const             { return m_iconId; }
    wxIcon GetCustomIcon() const            { return m_icon; }

private:
    wxString    m_title,
                m_message;

    unsigned    m_iconId,
                m_timeOut,
                m_timeDelay;

    wxIcon      m_icon;
    wxColour    m_colourStart,
                m_colourEnd;

    wxFont      m_font;

    wxTipKind   m_tipKind;
};

#endif // wxUSE_RICHTOOLTIP

#endif // _WX_RICHTOOLTIP_H_
