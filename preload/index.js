const { ipcRenderer } = require('electron');
const { getSettings } = require('../settings.js');

function updateDisposalFeebadge(value) {
	let disposalFeeBadge = document.getElementById('disposal-fee-badge');
	disposalFeeBadge.innerText = '$' + value.toFixed(2);
}

let cachedDisposalFee = getSettings().disposalFee;

window.addEventListener('DOMContentLoaded', _ => {
	updateDisposalFeebadge(cachedDisposalFee);

	// Setup forms
	function isNumeric(str) {
		if (typeof str !== 'string') {
			return false
		}

		return !isNaN(str) && !isNaN(parseFloat(str))
	}

	class Calculator {
		constructor(inputElements, outputElements, handler) {
			this.inputElements = inputElements;
			this.outputElements = outputElements;
			this.handler = handler;
			
			function setupInputHandlers(inputElement) {
				inputElement.addEventListener('blur', _ => {
					if (isNumeric(inputElement.value)) {
						let value = Number(inputElement.value);
						inputElement.value = value.toFixed(2);
					}
				})
				
				inputElement.addEventListener('input', this.updateForm.bind(this));
			}
			
			for (let inputElement of inputElements) {
				setupInputHandlers.bind(this)(inputElement)
			}
		}

		updateForm() {
			const allInputsValid = this.inputElements.every(e => isNumeric(e.value));

			if (allInputsValid) {
				const inputs = this.inputElements.map(e => Number(e.value));
				this.handler(...inputs);
			}
			else {
				for (let outputElement of this.outputElements) {
					outputElement.value = '';
				}
			}
		}
	}

	function writeOutput(element, value) {
		element.value = value.toFixed(2);
	}

	// ----------------------------- Discount ----------------------------- //

	let systemCost1 = document.getElementById('i-system-cost-1');
	let discount = document.getElementById('i-discount');

	let discountEx1 = document.getElementById('o-discount-ex-1');
	let discountInc1 = document.getElementById('o-discount-inc-1');

	let discountCalculator = new Calculator([systemCost1, discount], [discountEx1, discountInc1], (systemCostVal, discountVal) => {
		const discountExVal = (systemCostVal - cachedDisposalFee) * (100 - discountVal) / 100 + cachedDisposalFee;
		const discountIncVal = discountExVal * 1.1;

		writeOutput(discountEx1, discountExVal);
		writeOutput(discountInc1, discountIncVal);
	});

	// ----------------------------- Cashback ----------------------------- //

	let systemCost2 = document.getElementById('i-system-cost-2');
	let cashBack = document.getElementById('i-cash-back');

	let discountEx2 = document.getElementById('o-discount-ex-2');
	let discountInc2 = document.getElementById('o-discount-inc-2');

	let cashBackCalculator = new Calculator([systemCost2, cashBack], [discountEx2, discountInc2], (systemCostVal, cashBackVal) => {
		const discountExVal = systemCostVal - cashBackVal / 1.1;
		const discountIncVal = discountExVal * 1.1;

		writeOutput(discountEx2, discountExVal);
		writeOutput(discountInc2, discountIncVal);
	});

	// ----------------------------- Gross Profit ----------------------------- //

	let costPrice = document.getElementById('i-cost-price');
	let sellPriceInc = document.getElementById('i-sell-price-inc');

	let sellPriceEx = document.getElementById('o-sell-price-ex');
	let grossProfitEx = document.getElementById('o-gross-profit-ex');
	let grossProfitP = document.getElementById('o-gross-profit-p');
	let grossLevy = document.getElementById('o-gross-levy');

	let grossProfitCalculator = new Calculator([costPrice, sellPriceInc], [sellPriceEx, grossProfitP, grossProfitEx, grossLevy], (costPriceVal, sellPriceIncVal) => {
		const sellPriceExVal = sellPriceIncVal / 1.1;
		const grossProfitExVal = sellPriceExVal - costPriceVal;
		const grossProfitPVal = 100 * grossProfitExVal / sellPriceExVal;
		const grossLevyVal = grossProfitExVal - (0.055 * sellPriceExVal);

		writeOutput(sellPriceEx, sellPriceExVal);
		writeOutput(grossProfitEx, grossProfitExVal);
		writeOutput(grossProfitP, grossProfitPVal);
		writeOutput(grossLevy, grossLevyVal);
	});

	ipcRenderer.on('update-disposal-fee', _ => {
		const settings = getSettings();
		updateDisposalFeebadge(settings.disposalFee);
		cachedDisposalFee = settings.disposalFee;

		// Update discount form based on new value
		discountCalculator.updateForm();
	});
});