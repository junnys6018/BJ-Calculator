#include <wx/wx.h>
#include <wx/valnum.h>
#include <functional>
class MyApp : public wxApp
{
public:
	virtual bool OnInit() override;
};

class PercentageDiscountCalculator
{
public:
	PercentageDiscountCalculator()
		:m_DisposalFee(3.18f), m_SystemCost(0), m_Discount(0)
	{

	}
	const wxString Calculate()
	{
		float val = (m_SystemCost - m_DisposalFee) * (100.0f - m_Discount) / 100 + m_DisposalFee;
		if (m_SystemCost == 0.0f || m_Discount == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("%.2f", val);
	}

	float m_DisposalFee;
	float m_SystemCost;
	float m_Discount;
};

class CashbackDiscountCalculator
{
public:
	CashbackDiscountCalculator()
		:m_SystemCost(0), m_Cashback(0)
	{

	}
	const wxString Calculate()
	{
		float val = m_SystemCost - m_Cashback / 1.1f;
		if (m_SystemCost == 0.0f || m_Cashback == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("%.2f", val);
	}

	float m_SystemCost;
	float m_Cashback;
};

struct LabelTextCtrl
{
	wxBoxSizer* sizer;
	wxTextCtrl* ctrl;
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
private:
	LabelTextCtrl LabeledTextInput(const wxString& text, float* val, long style = 0L, float max = 9999.0f);
	void OnCalculate(wxCommandEvent& event);

private:
	PercentageDiscountCalculator m_pdc;
	CashbackDiscountCalculator m_cbc;
	wxTextCtrl* m_DiscPriceRel;
	wxTextCtrl* m_DiscPriceAbs;
	wxPanel* m_Panel;
};

enum
{
	ID_Input = 1
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame();
	frame->Show(true);
	frame->SetIcon(wxICON(PROG));
	return true;
}

MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, "Discount Calculator"), m_pdc(), m_cbc()
{
	// Set font
	wxFont myFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	SetFont(myFont);

	wxValidator::SuppressBellOnError(true);

	// Panel
	m_Panel = new wxPanel(this, wxID_ANY);
	m_Panel->SetBackgroundColour(*wxLIGHT_GREY);

	wxStaticText* text = new wxStaticText(m_Panel, wxID_ANY, "Disposal Fee ($)");

	wxFloatingPointValidator<float> validator(2, &m_pdc.m_DisposalFee, wxNUM_VAL_NO_TRAILING_ZEROES | wxNUM_VAL_ZERO_AS_BLANK);
	validator.SetMin(0.0f);
	validator.SetMax(10.0f);
	wxTextCtrl* textCtrl = new wxTextCtrl(m_Panel, ID_Input, wxString::Format("%.2f", m_pdc.m_DisposalFee), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	textCtrl->SetValidator(validator);

	wxBoxSizer* line_zero = new wxBoxSizer(wxHORIZONTAL);
	line_zero->AddStretchSpacer(1);
	line_zero->Add(text, 0, wxALL, 10);
	line_zero->Add(textCtrl, 1, wxALL, 10);
	line_zero->AddStretchSpacer(1);

	wxBoxSizer* line_one = new wxBoxSizer(wxHORIZONTAL);
	line_one->Add(LabeledTextInput("System Cost ($)", &m_pdc.m_SystemCost).sizer, 1, wxALL, 10);
	line_one->Add(new wxStaticText(m_Panel, wxID_ANY, "-"), 0, wxALIGN_CENTRE);
	line_one->Add(LabeledTextInput("Discount (%)", &m_pdc.m_Discount, 0, 100.0f).sizer, 1, wxALL, 10);
	line_one->Add(new wxStaticText(m_Panel, wxID_ANY, "="), 0, wxALIGN_CENTRE);
	auto [sizer1, ctrl1] = LabeledTextInput("Discounted Price ($)", NULL, wxTE_READONLY);
	m_DiscPriceRel = ctrl1;
	line_one->Add(sizer1, 1, wxALL, 10);

	wxBoxSizer* line_two = new wxBoxSizer(wxHORIZONTAL);
	line_two->Add(LabeledTextInput("System Cost ($)", &m_cbc.m_SystemCost).sizer, 1, wxALL, 10);
	line_two->Add(new wxStaticText(m_Panel, wxID_ANY, "-"), 0, wxALIGN_CENTRE);
	line_two->Add(LabeledTextInput("Cash Back ($)", &m_cbc.m_Cashback).sizer, 1, wxALL, 10);
	line_two->Add(new wxStaticText(m_Panel, wxID_ANY, "="), 0, wxALIGN_CENTRE);
	auto [sizer2, ctrl2] = LabeledTextInput("Discounted Price ($)", NULL, wxTE_READONLY);
	m_DiscPriceAbs = ctrl2;
	line_two->Add(sizer2, 1, wxALL, 10);


	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(line_zero, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 10);
	mainSizer->Add(line_one, 0, wxEXPAND | wxALL, 10);
	mainSizer->Add(line_two, 0, wxEXPAND | wxALL, 10);

	Bind(wxEVT_TEXT, &MyFrame::OnCalculate, this, ID_Input);
	
	m_Panel->SetSizerAndFit(mainSizer);
	SetMinSize(GetEffectiveMinSize());
}

LabelTextCtrl MyFrame::LabeledTextInput(const wxString& text, float* val, long style, float max)
{
	wxStaticText* staticText = new wxStaticText(m_Panel, wxID_ANY, text);
	wxFloatingPointValidator<float> validator(2, val, wxNUM_VAL_NO_TRAILING_ZEROES | wxNUM_VAL_ZERO_AS_BLANK);
	validator.SetMin(0.0f);
	validator.SetMax(max);
	wxTextCtrl* ctrl = new wxTextCtrl(m_Panel, style & wxTE_READONLY ? wxID_ANY : ID_Input, wxEmptyString, wxDefaultPosition, wxDefaultSize, style | wxTE_CENTRE | wxTE_PROCESS_ENTER, validator);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, 0, wxALIGN_CENTER);
	sizer->Add(ctrl, 0, wxEXPAND);

	return { sizer,ctrl };
}

void MyFrame::OnCalculate(wxCommandEvent& event)
{
	wxTextCtrl* tc = wxDynamicCast(event.GetEventObject(), wxTextCtrl);
	if (tc)
	{
		tc->GetValidator()->TransferFromWindow();
		m_DiscPriceRel->SetValue(m_pdc.Calculate());
		m_DiscPriceAbs->SetValue(m_cbc.Calculate());
	}
}