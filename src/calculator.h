#pragma once
#include <wx/wx.h>

class PercentageDiscountCalculator
{
public:
	PercentageDiscountCalculator()
		:m_DisposalFee(3.18f), m_SystemCost(0), m_Discount(0)
	{

	}
	const void Calculate()
	{
		m_DiscExGST = (m_SystemCost - m_DisposalFee) * (100.0f - m_Discount) / 100 + m_DisposalFee;
		m_DiscIncGST = m_DiscExGST * 1.1f;
	}

	const wxString GetDiscEx()
	{
		if (m_DisposalFee == 0.0f || m_SystemCost == 0.0f || m_Discount == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_DiscExGST);
	}

	const wxString GetDiscInc()
	{
		if (m_DisposalFee == 0.0f || m_SystemCost == 0.0f || m_Discount == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_DiscIncGST);
	}

	float m_DisposalFee;
	float m_SystemCost;
	float m_Discount;

private:
	float m_DiscExGST;
	float m_DiscIncGST;
};

class CashbackDiscountCalculator
{
public:
	CashbackDiscountCalculator()
		:m_SystemCost(0), m_Cashback(0)
	{

	}

	const void Calculate()
	{
		m_DiscEx = m_SystemCost - m_Cashback / 1.1f;
		m_DiscInc = m_DiscEx * 1.1f;
	}

	const wxString GetDiscEx()
	{
		if (m_SystemCost == 0.0f || m_Cashback == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_DiscEx);
	}

	const wxString GetDiscInc()
	{
		if (m_SystemCost == 0.0f || m_Cashback == 0.0f)
		{
			return wxString("");
		}
		return wxString::Format("$%.2f", m_DiscInc);
	}

	float m_SystemCost;
	float m_Cashback;

private:
	float m_DiscEx;
	float m_DiscInc;
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