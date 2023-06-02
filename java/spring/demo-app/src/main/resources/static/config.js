
document.getElementById("writeConfig").addEventListener("click", (event) => {
	let v0 = document.getElementById('lang').value;
	let v1 = document.getElementById('value1').value;
	let v2 = document.getElementById('value2').value;

	var url = 'writeConfig';
	var data = { lang: v0, value1: v1, value2: v2, reload: false };

	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/json' },
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

