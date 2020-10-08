#pragma once
#include <wx/wx.h>

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
		if (m_DisposalFee == 0.0f || m_SystemCost == 0.0f || m_Discount == 0.0f)
		{
			return wxString("");
		}
		else if (m_DisposalFee > m_SystemCost)
		{
			return wxString("Disp Fee > Cost");
		}
		return wxString::Format("$%.2f", val);
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
		return wxString::Format("$%.2f", val);
	}

	float m_SystemCost;
	float m_Cashback;
};

class ProfitCalculator
{
public:
	// Inputs
	float m_CostPriceexGST;
	float m_SellPriceincGST;

	void Calculate()
	{
		m_SellPriceexGST = m_SellPriceincGST / 1.1f;
		m_GrossProfitDollar = m_SellPriceexGST - m_CostPriceexGST;
		m_GrossProfitPercent = 100 * m_GrossProfitDollar / m_SellPriceexGST;
		m_GrossAfterLevy = m_GrossProfitDollar - (0.055 * m_SellPriceexGST);
	}

	const wxString GetSellPrice()
	{
		if (m_CostPriceexGST == 0.0f || m_SellPriceincGST == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_SellPriceexGST);
	}

	const wxString GetGPPercent()
	{
		if (m_CostPriceexGST == 0.0f || m_SellPriceincGST == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("%.2f%%", m_GrossProfitPercent);
	}

	const wxString GetGPDollar()
	{
		if (m_CostPriceexGST == 0.0f || m_SellPriceincGST == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_GrossProfitDollar);
	}

	const wxString GetGrossAfterLevy()
	{
		if (m_CostPriceexGST == 0.0f || m_SellPriceincGST == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_GrossAfterLevy);
	}
private:
	// Outputs
	float m_SellPriceexGST;
	float m_GrossProfitDollar;
	float m_GrossProfitPercent;
	float m_GrossAfterLevy;
};