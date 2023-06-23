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

function updateInputTable(tableName, json)
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

