
document.getElementById("writeConfig").addEventListener("click", (event) => {
	let v0 = document.getElementById('lang').value;
	let v1 = document.getElementById('itemSortOrder').value;
	let v2 = document.getElementById('value2').value;
	let token = document.getElementById('csrfToken').value;
	console.log(token);

	var url = 'writeConfig';
	var data = { lang: v0, itemSortOrder: v1, value2: v2, reload: false };

	fetch(url, {
		method: 'POST',
		headers: {
			'Content-Type': 'application/json',
			'X-CSRF-Token': token
		},
		body: JSON.stringify(data)
	})
	.then((resp) => resp.json())
	.then((json) => {
		let config = json[0];
		console.log(config.lang + ", " + config.reload);

		if(config.reload)
			window.location.reload();
	});
});

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
	let id = document.getElementById('selectedTab').value;
	let tab = document.getElementById(id);
	let menu = getMenu(tab.id);
	selectTab(tab, menu);
});

