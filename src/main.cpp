#include <wx/wx.h>
#include <wx/valnum.h>
#include <functional>
#include "calculator.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit() override;
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
	LabelTextCtrl LabeledTextInput(const wxString& text, float* val, long style = 0L, float max = 99999.0f, bool validate = true);
	void OnCalculate(wxCommandEvent& event);

	wxStaticBoxSizer* GenerateDiscountGUI();
	wxStaticBoxSizer* GenerateGrossProfitGUI();	

private:
	PercentageDiscountCalculator m_PercentCalc;
	CashbackDiscountCalculator m_CashbackCalc;
	ProfitCalculator m_ProfitCalc;

	wxTextCtrl* m_DiscPriceRel;
	wxTextCtrl* m_DiscPriceAbs;

	wxTextCtrl* m_SellPriceex;
	wxTextCtrl* m_GPPercent;
	wxTextCtrl* m_GPDollar;
	wxTextCtrl* m_GrossAfterLevy;

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
	: wxFrame(NULL, wxID_ANY, "BobJane Calculator"), m_PercentCalc(), m_CashbackCalc(), m_ProfitCalc()
{
	// Set font
	wxFont myFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	SetFont(myFont);

	wxValidator::SuppressBellOnError(true);

	// Panel
	m_Panel = new wxPanel(this, wxID_ANY);
	m_Panel->SetBackgroundColour(*wxLIGHT_GREY);

	auto discountSizer = GenerateDiscountGUI();
	auto grossProfitSizer = GenerateGrossProfitGUI();

	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	main->Add(discountSizer, 0, wxEXPAND | wxALL, 10);
	main->Add(grossProfitSizer, 0, wxEXPAND | wxALL, 10);

	Bind(wxEVT_TEXT, &MyFrame::OnCalculate, this, ID_Input);
	
	m_Panel->SetSizerAndFit(main);
	SetMinSize(GetEffectiveMinSize());
}

LabelTextCtrl MyFrame::LabeledTextInput(const wxString& text, float* val, long style, float max, bool validate)
{
	wxStaticText* staticText = new wxStaticText(m_Panel, wxID_ANY, text);
	wxFloatingPointValidator<float> validator(2, val, wxNUM_VAL_NO_TRAILING_ZEROES | wxNUM_VAL_ZERO_AS_BLANK);
	validator.SetMin(0.0f);
	validator.SetMax(max);
	wxTextCtrl* ctrl = new wxTextCtrl(m_Panel, style & wxTE_READONLY ? wxID_ANY : ID_Input, wxEmptyString, wxDefaultPosition, wxDefaultSize, style | wxTE_CENTRE | wxTE_PROCESS_ENTER, validate ? validator : wxDefaultValidator);

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
		m_DiscPriceRel->SetValue(m_PercentCalc.Calculate());
		m_DiscPriceAbs->SetValue(m_CashbackCalc.Calculate());

		m_ProfitCalc.Calculate();

		m_SellPriceex->SetValue(m_ProfitCalc.GetSellPrice());
		m_GPPercent->SetValue(m_ProfitCalc.GetGPPercent());
		m_GPDollar->SetValue(m_ProfitCalc.GetGPDollar());
		m_GrossAfterLevy->SetValue(m_ProfitCalc.GetGrossAfterLevy());
	}
}

wxStaticBoxSizer* MyFrame::GenerateDiscountGUI()
{
	wxStaticText* text = new wxStaticText(m_Panel, wxID_ANY, "Disposal Fee ($)");

	wxFloatingPointValidator<float> validator(2, &m_PercentCalc.m_DisposalFee, wxNUM_VAL_NO_TRAILING_ZEROES | wxNUM_VAL_ZERO_AS_BLANK);
	validator.SetMin(0.0f);
	validator.SetMax(999.0f);
	wxTextCtrl* textCtrl = new wxTextCtrl(m_Panel, ID_Input, wxString::Format("%.2f", m_PercentCalc.m_DisposalFee), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	textCtrl->SetValidator(validator);

	wxBoxSizer* line_zero = new wxBoxSizer(wxHORIZONTAL);
	line_zero->AddStretchSpacer(1);
	line_zero->Add(text, 0, wxALIGN_CENTER | wxALL, 10);
	line_zero->Add(textCtrl, 1, wxALL, 10);
	line_zero->AddStretchSpacer(1);

	wxBoxSizer* line_one = new wxBoxSizer(wxHORIZONTAL);
	line_one->Add(LabeledTextInput("System Cost ($)", &m_PercentCalc.m_SystemCost).sizer, 1, wxALL, 10);
	line_one->Add(new wxStaticText(m_Panel, wxID_ANY, "-"), 0, wxALIGN_CENTRE);
	line_one->Add(LabeledTextInput("Discount (%)", &m_PercentCalc.m_Discount, 0, 100.0f).sizer, 1, wxALL, 10);
	line_one->Add(new wxStaticText(m_Panel, wxID_ANY, "="), 0, wxALIGN_CENTRE);
	auto [sizer1, ctrl1] = LabeledTextInput("Discounted Price ($)", NULL, wxTE_READONLY, 99999.0f, false);
	m_DiscPriceRel = ctrl1;
	line_one->Add(sizer1, 1, wxALL, 10);

	wxBoxSizer* line_two = new wxBoxSizer(wxHORIZONTAL);
	line_two->Add(LabeledTextInput("System Cost ($)", &m_CashbackCalc.m_SystemCost).sizer, 1, wxALL, 10);
	line_two->Add(new wxStaticText(m_Panel, wxID_ANY, "-"), 0, wxALIGN_CENTRE);
	line_two->Add(LabeledTextInput("Cash Back ($)", &m_CashbackCalc.m_Cashback).sizer, 1, wxALL, 10);
	line_two->Add(new wxStaticText(m_Panel, wxID_ANY, "="), 0, wxALIGN_CENTRE);
	auto [sizer2, ctrl2] = LabeledTextInput("Discounted Price ($)", NULL, wxTE_READONLY, 99999.0f, false);
	m_DiscPriceAbs = ctrl2;
	line_two->Add(sizer2, 1, wxALL, 10);


	wxStaticBoxSizer* discountSizer = new wxStaticBoxSizer(wxVERTICAL, m_Panel, "Discount");
	discountSizer->Add(line_zero, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 10);
	discountSizer->Add(line_one, 0, wxEXPAND | wxALL, 10);
	discountSizer->Add(line_two, 0, wxEXPAND | wxALL, 10);

	return discountSizer;
}

wxStaticBoxSizer* MyFrame::GenerateGrossProfitGUI()
{
	wxBoxSizer* left = new wxBoxSizer(wxVERTICAL);
	left->Add(LabeledTextInput("Cost Price [ex GST] ($)", &m_ProfitCalc.m_CostPriceexGST).sizer, 1, wxEXPAND | wxALL, 10);
	left->Add(LabeledTextInput("Sell Price [inc GST] ($)", &m_ProfitCalc.m_SellPriceincGST).sizer, 1, wxEXPAND | wxALL, 10);

	wxBoxSizer* right = new wxBoxSizer(wxVERTICAL);

	auto [sizer1, ctrl1] = LabeledTextInput("Sell Price [ex GST] ($)", NULL, wxTE_READONLY, 99999.0f, false);
	m_SellPriceex = ctrl1;
	right->Add(sizer1, 1, wxEXPAND | wxALL, 10);

	auto [sizer2, ctrl2] = LabeledTextInput("Gross Profit (%)", NULL, wxTE_READONLY, 99999.0f, false);
	m_GPPercent = ctrl2;
	right->Add(sizer2, 1, wxEXPAND | wxALL, 10);

	auto [sizer3, ctrl3] = LabeledTextInput("Gross Profit [ex GST] ($)", NULL, wxTE_READONLY, 99999.0f, false);
	m_GPDollar = ctrl3;
	right->Add(sizer3, 1, wxEXPAND | wxALL, 10);

	auto [sizer4, ctrl4] = LabeledTextInput("Gross After Levy ($)", NULL, wxTE_READONLY, 99999.0f, false);
	m_GrossAfterLevy = ctrl4;
	right->Add(sizer4, 1, wxEXPAND | wxALL, 10);

	wxStaticBoxSizer* grossProfitSizer = new wxStaticBoxSizer(wxHORIZONTAL, m_Panel, "Gross Profit");
	grossProfitSizer->Add(left, 1);
	grossProfitSizer->AddSpacer(40);
	grossProfitSizer->Add(right, 1);
	return grossProfitSizer;
}
