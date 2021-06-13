const { getSettings } = require('../settings.js');

window.addEventListener('DOMContentLoaded', _ => {
	const settings = getSettings();
	let disposalFeeBadge = document.getElementById('disposal-fee-badge');
	disposalFeeBadge.innerText = '$' + settings.disposalFee.toFixed(2);
});