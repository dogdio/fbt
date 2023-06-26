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

function getDialogPosition(event)
{
	let dialogWidth = (300+5+5)/2;
	let buttonWidth = (170)/2;
	let rect = event.target.getBoundingClientRect();
	let x = rect.left - dialogWidth + buttonWidth;
	let y;
	console.log(event.target);

	if(rect.top > 170)
		y = rect.top - 160;
	else
		y = rect.top + 40;

	return [x, y];
}

let modalCallback;
function createConfirmDialog(event, msg1, msg2, func)
{
	let d = document.getElementById("modalDialogId");
	d.innerHTML =
		'<div id="modalConfirm" class="modal">' +
		  '<div class="modalDialog" id="modalDialogBody">' +
		    '<p><b id="modalMessage1" style="margin-right: 10px;"></b></p>' +
		    '<p><b id="modalMessage2"></b></p>' +
		    '<button id="modalYes" onclick="confirmAction(true)">Yes</button>' +
		    '<button id="modalNo" onclick="confirmAction(false)">No</button>' +
		  '</div>' +
		'</div>';

	modalCallback = func;

	if(msg1.length > 13) {
		msg1 = msg1.substring(0, 13);
		msg1 += "...";
	}
	document.getElementById("modalMessage1").innerHTML = '[' + msg1 + ']';
	document.getElementById("modalMessage2").innerHTML = msg2;

	let [x, y] = getDialogPosition(event);
	document.getElementById("modalDialogBody").style.top = y + 'px';
	document.getElementById("modalDialogBody").style.left = x + 'px';

	let m = document.getElementById("modalConfirm");
	m.addEventListener("click", (event) => {
		console.log(event.target.id);
		if(event.target == m)
			confirmAction(false); // 背景をクリックしたらダイアログを消す
	});
	document.getElementById("modalConfirm").style.display = 'block';
}

function confirmAction(answer)
{
	let d = document.getElementById("modalDialogId");
	d.innerHTML = "";

	modalCallback(answer);
}

window.addEventListener("keydown", (event) => {

	let m = document.getElementById("modalConfirm");
	if(m != null) {
		if(m.style.display == 'block') {
			if(event.key == 'Escape')
				confirmAction(false);
		}
	}
});

