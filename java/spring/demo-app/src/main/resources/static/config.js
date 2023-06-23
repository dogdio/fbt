window.addEventListener('pageshow', function(event) {
	console.log("pageshow: " + location.toString());

	if (event.persisted) { // Cacheの場合はリロード
		window.location.reload();
	}
});

function selectTab(tab, menu)
{
	tab.classList.add("selected");

	menu.style.display = 'block';
	menu.classList.add("active");
	menu.classList.remove("inactive");
}

function unselectTab(tab, menu)
{
	tab.classList.remove("selected");

	menu.style.display = 'none';
	menu.classList.remove("active");
	menu.classList.add("inactive");
}

let tabToMenu = new Map([
	['tabSystem',   'configSystem'],
	['tabPassword', 'configPassword'],
	['tabAdmin',    'configAdmin'],
]);

function getMenu(id)
{
	return document.getElementById(tabToMenu.get(id));
}

document.querySelectorAll(".tabMenu").forEach(function(tab) {
	tab.addEventListener("click", tabSwitch);
});

function tabSwitch(event)
{
	let selected = event.target;

	document.querySelectorAll(".tabMenu").forEach(function(tab) {
		let menu = getMenu(tab.id);

		if(selected == tab) {
			selectTab(tab, menu);
		}
		else {
			unselectTab(tab, menu);
		}
	});
}

window.addEventListener("load", () => {
	let id = "tabSystem";
	let tab = document.getElementById(id);
	let menu = getMenu(tab.id);
	selectTab(tab, menu);

});

function deleteConfirm()
{
	let m = document.getElementById("modal2");
	let name = document.getElementById("username").value;
	let msg = document.getElementById("deleteUserName");

	msg.innerHTML = "[" + name + "]";
	m.style.display = "block";
}

function deleteConfirmAction(answer) // called by modal2
{
	let m = document.getElementById("modal2");
	m.style.display = "none";

	if (answer)
		configAdminButton('Delete');
}

function configSystemButton()
{
	let url = '/configSystem';
	let data = {
		lang:              document.getElementById('lang').value,
		itemSortOrder:     document.getElementById('itemSortOrder').value,
		progressSortOrder: document.getElementById('progressSortOrder').value,
	};

	postJson(url, data).then(json => {
		for (let info of json)
			console.log(info);

		updateInputTable('configSystem', json);

		for (let info of json) {
			if(info.key == 'lang' && info.value == 'updated') {
				setTimeout(function() {
					window.location.reload();
				}, 600);
				break;
			}
		}
	});
}

function configPasswordButton()
{
	let url = '/configPassword';
	let data = {
		newPassword1: document.getElementById('newPassword1').value,
		newPassword2: document.getElementById('newPassword2').value,
	};

	postJson(url, data).then(json => {
		for (let info of json)
			console.log(info);

		updateInputTable('configPassword', json);
	});
}

function configAdminButton(action)
{
	let url = '/admin/userAdmin';
	let data = {
		username: document.getElementById('username').value,
		passwordReset: document.getElementById('passwordReset').value,
		enabled: document.getElementById('enabled').value,
		action: action
	};

	postJson(url, data).then(json => {
		for (let info of json)
			console.log(info);

		updateInputTable('configAdmin', json);
	});
}

