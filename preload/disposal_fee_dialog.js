
const ipcRenderer = require('electron').ipcRenderer;
const { getSettings, writeSettings } = require('../settings.js');

window.addEventListener('DOMContentLoaded', _ => {
	const disposalFeeInput = document.getElementById('disposal-fee');
	let settings = getSettings();
	disposalFeeInput.value = settings.disposalFee.toFixed(2);


	document.getElementById('cancel').addEventListener('click', _ => ipcRenderer.send('close-dialog'));

	document.getElementById('save').addEventListener('click', _ => {
		settings.disposalFee = Number(Number(disposalFeeInput.value).toFixed(2));
		writeSettings(settings);
		ipcRenderer.send('close-dialog')
	});

	document.getElementById('disposal-fee').addEventListener('blur', _ => {
		let fee = Number(disposalFeeInput.value);
		if (fee < 0) {
			fee = 0;
		}
		disposalFeeInput.value = fee.toFixed(2);
	})
});