const { ipcRenderer } = require('electron');
const { getSettings } = require('../settings.js');

function updateDisposalFeebadge() {
	const settings = getSettings();
	let disposalFeeBadge = document.getElementById('disposal-fee-badge');
	disposalFeeBadge.innerText = '$' + settings.disposalFee.toFixed(2);

}

window.addEventListener('DOMContentLoaded', _ => {
	updateDisposalFeebadge();

	ipcRenderer.on('update-disposal-fee', updateDisposalFeebadge);
});