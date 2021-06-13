const ipcRenderer = require('electron').ipcRenderer;
const { getSettings, writeSettings } = require('../settings.js');

window.addEventListener('DOMContentLoaded', _ => {
	const disposalFeeInput = document.getElementById('disposal-fee');
	let settings = getSettings();
	disposalFeeInput.value = settings.disposalFee.toFixed(2);


	document.getElementById('cancel').addEventListener('click', _ => ipcRenderer.send('close-dialog'));

	document.getElementById('save').addEventListener('click', _ => {
		settings.disposalFee = Number(Number(disposalFeeInput.value).toFixed(2));
		if (settings.disposalFee >= 0) {
			writeSettings(settings);
			ipcRenderer.send('close-dialog');
			ipcRenderer.send('update-disposal-fee');
		}
		else {
			document.getElementById('error-popup').style.visibility = 'visible';
		}
	});

	document.getElementById('disposal-fee').addEventListener('blur', _ => {
		let fee = Number(disposalFeeInput.value);
		disposalFeeInput.value = fee.toFixed(2);
	})
});