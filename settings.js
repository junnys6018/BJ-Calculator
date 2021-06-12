const fs = require('fs');
const path = require('path');

const defaultSettings = {
	disposalFee: 3.5
};

module.exports = {
	getSettings: _ => {
		try {
			const file = fs.readFileSync(path.resolve(__dirname, 'settings.json'));
			const settings = JSON.parse(file);
			if (typeof settings.disposalFee !== 'number') {
				return defaultSettings;
			}

			if (settings.disposalFee === NaN || settings.disposalFee === Infinity || settings.disposalFee === -Infinity) {
				return defaultSettings;
			}

			return settings;
		}
		catch (err) {
			return defaultSettings;
		}
	},
	writeSettings: (settings) => {
		const settingsPath = path.resolve(__dirname, 'settings.json');
		fs.writeFileSync(settingsPath, JSON.stringify(settings, null, 4));
	}
}

