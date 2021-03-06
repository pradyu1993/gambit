//
// This file is part of Gambit
// Copyright (c) 1994-2010, The Gambit Project (http://www.gambit-project.org)
//
// FILE: src/gui/dleditmove.cc
// Dialog for viewing and editing properties of a move
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif  // WX_PRECOMP

#include "libgambit/libgambit.h"
#include "dleditmove.h"
#include "renratio.h"

class gbtActionSheet : public wxSheet {
private:
  Gambit::GameInfoset m_infoset;

  // Overriding wxSheet members
  wxSheetCellAttr GetAttr(const wxSheetCoords &p_coords, wxSheetAttr_Type) const;

public:
  gbtActionSheet(wxWindow *p_parent, Gambit::GameInfoset p_infoset)
    : wxSheet(p_parent, 1), m_infoset(p_infoset) { }
};

wxSheetCellAttr
gbtActionSheet::GetAttr(const wxSheetCoords &p_coords, wxSheetAttr_Type) const
{
  if (IsRowLabelCell(p_coords)) {
    wxSheetCellAttr attr(GetSheetRefData()->m_defaultRowLabelAttr);
    attr.SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD));
    attr.SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    attr.SetOrientation(wxHORIZONTAL);
    attr.SetReadOnly(true);
    return attr;
  }
  else if (IsColLabelCell(p_coords)) {
    wxSheetCellAttr attr(GetSheetRefData()->m_defaultColLabelAttr);
    attr.SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD));
    attr.SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    attr.SetOrientation(wxHORIZONTAL);
    attr.SetReadOnly(true);
    return attr;
  }
  else if (IsCornerLabelCell(p_coords)) {
    wxSheetCellAttr attr(GetSheetRefData()->m_defaultCornerLabelAttr);
    return attr;
  }

  wxSheetCellAttr attr(GetSheetRefData()->m_defaultGridCellAttr);
  attr.SetFont(wxFont(10, wxSWISS, wxNORMAL, wxNORMAL));
  attr.SetAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
  attr.SetOrientation(wxHORIZONTAL);
  attr.SetReadOnly(false);
  if (p_coords.GetCol() == 1) {
    attr.SetRenderer(wxSheetCellRenderer(new gbtRationalRendererRefData()));
    attr.SetEditor(wxSheetCellEditor(new gbtRationalEditorRefData()));
  }
  else {
    attr.SetEditor(wxSheetCellEditor(new wxSheetCellTextEditorRefData()));
  }
  return attr;
}

//======================================================================
//                      class gbtEditMoveDialog
//======================================================================

gbtEditMoveDialog::gbtEditMoveDialog(wxWindow *p_parent,
				     Gambit::GameInfoset p_infoset)
  : wxDialog(p_parent, -1, _("Move properties"), wxDefaultPosition), 
    m_infoset(p_infoset)
{
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *labelSizer = new wxBoxSizer(wxHORIZONTAL);
  labelSizer->Add(new wxStaticText(this, wxID_STATIC, 
				   _("Information set label")),
		  0, wxALL | wxALIGN_CENTER, 5);
  m_infosetName = new wxTextCtrl(this, -1,
				 wxString(p_infoset->GetLabel().c_str(), *wxConvCurrent));
  labelSizer->Add(m_infosetName, 1, wxALL | wxCENTER | wxEXPAND, 5);
  topSizer->Add(labelSizer, 0, wxALL | wxEXPAND, 0);

  topSizer->Add(new wxStaticText(this, wxID_STATIC,
				 wxString::Format(_("Number of members: %d"),
						  p_infoset->NumMembers())),
		0, wxALL | wxALIGN_CENTER, 5);

  wxBoxSizer *playerSizer = new wxBoxSizer(wxHORIZONTAL);
  playerSizer->Add(new wxStaticText(this, wxID_STATIC, _("Belongs to player")),
		   0, wxALL | wxALIGN_CENTER, 5);
  m_player = new wxChoice(this, -1);
  if (p_infoset->IsChanceInfoset()) {
    m_player->Append(_("Chance"));
    m_player->SetSelection(0);
  }
  else {
    for (int pl = 1; pl <= p_infoset->GetGame()->NumPlayers(); pl++) {
      m_player->Append(wxString::Format(_T("%d: "), pl) +
		       wxString(p_infoset->GetGame()->GetPlayer(pl)->GetLabel().c_str(), *wxConvCurrent));
    } 
    m_player->SetSelection(p_infoset->GetPlayer()->GetNumber() - 1);
  }
  playerSizer->Add(m_player, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(playerSizer, 0, wxALL | wxEXPAND, 0);

  wxStaticBoxSizer *actionBoxSizer =
    new wxStaticBoxSizer(new wxStaticBox(this, -1, _("Actions")), wxHORIZONTAL);
 
  m_actionSheet = new gbtActionSheet(this, p_infoset);
  m_actionSheet->CreateGrid(p_infoset->NumActions(), 
			    (p_infoset->IsChanceInfoset()) ? 2 : 1);
  m_actionSheet->SetRowLabelWidth(40);
  m_actionSheet->SetColLabelHeight(25);
  m_actionSheet->SetColLabelValue(0, wxT("Label"));
  if (p_infoset->IsChanceInfoset()) {
    m_actionSheet->SetColLabelValue(1, wxT("Probability"));
  }

  for (int act = 1; act <= p_infoset->NumActions(); act++) {
    m_actionSheet->SetCellValue(wxSheetCoords(act-1, 0),
				wxString(p_infoset->GetAction(act)->GetLabel().c_str(),
					 *wxConvCurrent));
    if (p_infoset->IsChanceInfoset()) {
      m_actionSheet->SetCellValue(wxSheetCoords(act-1, 1),
				  wxString(p_infoset->GetActionProb(act, "").c_str(),
					   *wxConvCurrent));
    }
  }
  m_actionSheet->SetDefaultColWidth(150);
  m_actionSheet->AutoSizeRows();

  actionBoxSizer->Add(m_actionSheet, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(actionBoxSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
  wxButton *okButton = new wxButton(this, wxID_OK, _("OK"));
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  topSizer->Add(buttonSizer, 0, wxALL | wxALIGN_RIGHT, 5);

  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);
  Layout();
  CenterOnParent();
}

wxString gbtEditMoveDialog::GetActionName(int p_act) const 
{ 
  if (m_actionSheet->IsCellEditControlCreated()) {
    m_actionSheet->SaveEditControlValue();
    m_actionSheet->HideCellEditControl();
  }
  return m_actionSheet->GetCellValue(wxSheetCoords(p_act-1, 0));
}

wxString gbtEditMoveDialog::GetActionProb(int p_act) const 
{ 
  if (m_actionSheet->IsCellEditControlCreated()) {
    m_actionSheet->SaveEditControlValue();
    m_actionSheet->HideCellEditControl();
  }
  return m_actionSheet->GetCellValue(wxSheetCoords(p_act-1, 1));
}





