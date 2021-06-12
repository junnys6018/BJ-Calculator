const { app, Menu, ipcMain, BrowserWindow } = require('electron');
const path = require('path')

function createWindow () {
	const win = new BrowserWindow({
		width: 800,
		height: 600,
		show: false,
		icon: 'assets/bobjane.ico'
	});

	let template = [{
		label: 'Settings',
		submenu: [{
			label: 'Set Disposal Fee',
			click: function (item, focusedWindow) {
				if (focusedWindow) {
					const dialog = new BrowserWindow({
						width: 400,
						height: 200,
						show: false,
						parent: win,
						modal: true,
						icon: 'assets/bobjane.ico',
						webPreferences: {
							preload: path.join(__dirname, 'preload/disposal_fee_dialog.js')
						}
					});
					dialog.loadFile('disposal_fee_dialog.html');
					dialog.removeMenu();
					dialog.once('ready-to-show', _ => dialog.show());
					ipcMain.once('close-dialog', _ => dialog.close());
					dialog.on('closed', _ => ipcMain.removeAllListeners('close-dialog'));
					dialog.webContents.openDevTools()
				}
			}
		}]
	}]

	win.loadFile('index.html');
	const menu = Menu.buildFromTemplate(template);
	win.setMenu(menu);

	win.once('ready-to-show', _ => win.show());
	win.webContents.openDevTools()
}

app.on('window-all-closed', _ => app.quit());
app.on('ready', createWindow);

