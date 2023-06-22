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

function updateConfigMenu(tableName, json)
{
	let table = document.getElementById(tableName);

	for (let r = 0; r < table.rows.length; r++) {
		let td0 = table.rows[r].cells[0];
		let td1 = table.rows[r].cells[1];
		let td2 = table.rows[r].cells[2];
		let input = td1.firstElementChild;
		let find = null;

		if(td0.innerHTML.length == 0)
			continue;

		for (let info of json) {
			if(input.id == info.key) {
				find = info;
				break;
			}
		}
		if(find == null) {
			td2.innerHTML = "";
			input.style.background = "#ffffff";
		}
		else {
			td2.innerHTML = find.value;
			if(find.attr == "ERR") {
				input.style.background = "#ffe0e0";
				td2.style.color = 'red';
			}
			else if(find.attr == "WARN") {
				input.style.background = "#ffdab9";
				td2.style.color = 'orange';
			}
			else if(find.attr == "INFO") {
				input.style.background = "#e0ffe0";
				td2.style.color = 'green';
			}
		}
	}
}

function postJson(url, data)
{
	return fetch(url, {
		method: 'POST',
		headers: {
			'Content-Type': 'application/json',
			'X-CSRF-Token': document.getElementById('csrfToken').value
		},
		body: JSON.stringify(data)
	})
	.then((resp) => resp.json())
	.catch(error => {
		console.log(error);
	});
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

		updateConfigMenu('configSystem', json);

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

		updateConfigMenu('configPassword', json);
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

		updateConfigMenu('configAdmin', json);
	});
}

