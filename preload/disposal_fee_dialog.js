const ipcRenderer = require('electron').ipcRenderer;
const { getSettings, writeSettings } = require('../settings.js');

function isNumeric(str) {
	if (typeof str != "string") {
		return false
	}

	return !isNaN(str) && !isNaN(parseFloat(str))
}

window.addEventListener('DOMContentLoaded', _ => {
	const disposalFeeInput = document.getElementById('disposal-fee');
	let settings = getSettings();
	disposalFeeInput.value = settings.disposalFee.toFixed(2);


	document.getElementById('cancel').addEventListener('click', _ => ipcRenderer.send('close-dialog'));

	document.getElementById('save').addEventListener('click', _ => {
		function error(message) {
			document.getElementById('error-popup').style.visibility = 'visible';
			document.getElementById('error-popup-message').innerHTML = message;
		}

		if (!isNumeric(disposalFeeInput.value)) {
			error('Invalid Input');
			return;
		}

		settings.disposalFee = Number(Number(disposalFeeInput.value).toFixed(2));

		if (settings.disposalFee < 0) {
			error('Negative Disposal Fee')
			return;
		}
		writeSettings(settings);
		ipcRenderer.send('close-dialog');
		ipcRenderer.send('update-disposal-fee');
	});

	document.getElementById('disposal-fee').addEventListener('blur', _ => {
		if (isNumeric(disposalFeeInput.value)) {
			let fee = Number(disposalFeeInput.value);
			disposalFeeInput.value = fee.toFixed(2);
		}
	})
});