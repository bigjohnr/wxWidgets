///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/richtooltipcmn.cpp
// Purpose:     wxRichToolTip implementation common to all platforms.
// Author:      Vadim Zeitlin
// Created:     2011-10-18
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#if wxUSE_RICHTOOLTIP

#ifndef WX_PRECOMP
    #include "wx/icon.h"
#endif // WX_PRECOMP

#include "wx/private/richtooltip.h"
#include "wx/generic/private/richtooltip.h"

// ============================================================================
// wxRichToolTipInfo implementation
// ============================================================================

wxRichToolTipInfo::wxRichToolTipInfo()
{
}

wxRichToolTipInfo::wxRichToolTipInfo( const wxString& title, 
                       const wxString& message,
                       int icon )
                       :m_title( title ),
                       m_message( message ),
                       m_iconId( icon ),
                       m_timeOut( 5000 ),
                       m_timeDelay( 0 ),
                       m_tipKind( wxTipKind_Auto )
 {
 }

void wxRichToolTipInfo::SetTitleAndMessage( const wxString& title, 
                                            const wxString& message )
{
    m_title = title;
    m_message = message;
}

void wxRichToolTipInfo::SetBackgroundColour( const wxColour &colour, 
                                             const wxColour &colourEnd )
{
    m_colourStart = colour;
    m_colourEnd = colourEnd;
}

void wxRichToolTipInfo::SetTimes( unsigned millisecondsTimeout, 
                                  unsigned millisecondsDelay )
{
    m_timeOut = millisecondsTimeout;
    m_timeDelay = millisecondsDelay;
}

// ============================================================================
// implementation
// ============================================================================

wxRichToolTip::wxRichToolTip(const wxString& title,
                             const wxString& message) :
    m_impl(wxRichToolTipImpl::Create(title, message))
{
}

wxRichToolTip::wxRichToolTip( const wxRichToolTipInfo& tipInfo )
             : m_impl( wxRichToolTipImpl::Create( tipInfo.GetTitle(), 
                                                  tipInfo.GetMessage() ) )
{
    // overwrite defaults set in wxRichToolTipImpl Create
    SetTipKind( tipInfo.GetTipKind() ); 
    SetTimeout( tipInfo.GetTimeout(), tipInfo.GetDelay() );

    if ( tipInfo.GetTitleFont().IsOk() )
        SetTitleFont( tipInfo.GetTitleFont() );

    SetIcon( tipInfo.GetStandardIcon() );

    if ( tipInfo.GetCustomIcon().IsOk() )
        SetIcon( tipInfo.GetCustomIcon() );

    if ( tipInfo.GetBackgroundColour().IsOk() )
    {
        if ( tipInfo.GetBackgroundEndColour().IsOk() )
            SetBackgroundColour( tipInfo.GetBackgroundColour(), tipInfo.GetBackgroundEndColour() );
        else
            SetBackgroundColour( tipInfo.GetBackgroundColour() );
    }
}

void
wxRichToolTip::SetBackgroundColour(const wxColour& col, const wxColour& colEnd)
{
    m_impl->SetBackgroundColour(col, colEnd);
}

void wxRichToolTip::SetIcon(int icon)
{
    m_impl->SetStandardIcon(icon);
}

void wxRichToolTip::SetIcon(const wxBitmapBundle& icon)
{
    m_impl->SetCustomIcon(icon);
}

void wxRichToolTip::SetTimeout(unsigned milliseconds,
                               unsigned millisecondsDelay)
{
    m_impl->SetTimeout(milliseconds, millisecondsDelay);
}

void wxRichToolTip::SetTipKind(wxTipKind tipKind)
{
    m_impl->SetTipKind(tipKind);
}

void wxRichToolTip::SetTitleFont(const wxFont& font)
{
    m_impl->SetTitleFont(font);
}

wxWindow* wxRichToolTip::ShowFor(wxWindow* win, const wxRect* rect)
{
    wxCHECK_MSG( win, NULL, wxS("Must have a valid window") );

    return m_impl->ShowFor(win, rect);
}

wxRichToolTip::~wxRichToolTip()
{
    delete m_impl;
}

#endif // wxUSE_RICHTOOLTIP
