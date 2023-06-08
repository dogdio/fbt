let deleteId;
function deleteConfirm()
{
	let m = document.getElementById("modal");
	m.style.display = "block";
	deleteId = itemId;
}

function deleteConfirmAction(answer)
{
	let m = document.getElementById("modal");
	m.style.display = "none";

	if (answer) {
		console.log("####### delete" + deleteId);
		location.href = "/delete/" + deleteId;
	}
}

let inputData = {
	title: "",
	priority: 0,
	status: 0,
	category: 0,
	worker: "",
	deadline: "",
};

let itemId = 0;

window.addEventListener("load", () => {
	inputData = {
		title: document.getElementById('title').value,
		priority: document.getElementById('priority').value,
		status: document.getElementById('status').value,
		category: document.getElementById('category').value,
		worker: document.getElementById('worker').value,
		deadline: document.getElementById('deadline').value,
	};
	console.log(inputData);

	let m = location.toString().match(/\/(\d+)$/);
	if(m.length == 2)
		itemId = m[1];
});

window.addEventListener("keydown", (event) => {
//	console.log(event);

	let m = document.getElementById("modal");
	if(m.style.display == 'block') {
		if(event.key == 'Escape')
			deleteConfirmAction(false);
	}
});

window.addEventListener('pageshow', function(event) {
	console.log("pageshow: " + location.toString());

	if (event.persisted) { // Cacheの場合はリロード
		window.location.reload();
	}
});

function writeItem()
{
	let url = '/writeItem';
	let req = {
		id: itemId,
		title: document.getElementById('title').value,
		priority: document.getElementById('priority').value,
		status: document.getElementById('status').value,
		category: document.getElementById('category').value,
		worker: document.getElementById('worker').value,
		deadline: document.getElementById('deadline').value,
	};

	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/json' },
		body: JSON.stringify(req)
	})
	.then((resp) => resp.json())
	.then((json) => {
		let data = json[0];
		console.log(data);

		if(inputData.title != data.title) {
			window.location.reload();
		}
		inputData = data;
	});
}

function BRtoLF(str)
{
	return str.replace(/<br>/gi, "\n");
}

function LFtoBR(str)
{
	return str.replace(/\n|\r\n/g, "<br>");
}

function writeProgress()
{
	let url = '/writeProgress';
	let req = {
		id: itemId,
		contents: LFtoBR(document.getElementById('progress').value),
	};

	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/json' },
		body: JSON.stringify(req)
	})
	.then((resp) => resp.json())
	.then((json) => {
		window.location.reload();
	});
}

function deleteProgress(event, id)
{
	let url = '/deleteProgress/' + itemId;
	let req = {
		id: id,
		contents: "Delete",
	};

	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/json' },
		body: JSON.stringify(req)
	})
	.then((resp) => resp.json())
	.then((json) => {
		window.location.reload();
	});

}

function updateProgress(event, id)
{
	console.log("Update/" + itemId + "/ " + id);
	console.log(document.getElementById('progressEditor').value);

	let url = '/updateProgress/' + itemId;
	let req = {
		id: id,
		contents: LFtoBR(document.getElementById('progressEditor').value),
	};

	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/json' },
		body: JSON.stringify(req)
	})
	.then((resp) => resp.json())
	.then((json) => {
		window.location.reload();
	});

}

let textSave = "";
let targetSave = null;
function toggleProgressEditor(event)
{
	let div1 = event.target.parentElement;
	let key = div1.nextElementSibling;
	let id = key.innerText.substring(1); // #[[${list.id}]]

	if(targetSave != null && (event.target != targetSave)) {
		let div = targetSave.parentElement.parentElement;
		let text = div.nextElementSibling;

		text.innerHTML = textSave;
		targetSave.checked = false;
	}

	let div2 = div1.parentElement; // <div style="display: flex;">
	let text = div2.nextElementSibling; // <p>[[${list.contents}]]</p>

	if(event.target.checked) {
		textSave = text.innerHTML;
		text.innerHTML = 
		'<div style="display: flex;" class="dynamicArea">' +
			'<div class="flexItem">' +
				'<textarea id="progressEditor" maxlength="800" rows="7" cols="80">' +
				BRtoLF(text.innerHTML) +
				'</textarea>' +
			'</div>' +
			'<div class="flexItem">' +
			'<input type="button" value="Update" onclick="updateProgress(event, ' + id + ')"><br><br>' +
			'<input type="button" value="Delete" onclick="deleteProgress(event, ' + id + ')">' +
			'</div>' +
		'</div>';
		let e = text.firstElementChild;
		e.classList.toggle("active");
	}
	else {
		text.innerHTML = textSave;
	}

	targetSave = event.target;
}

function toggleDynamicArea(event, id, setting)
{
	let checked = event.target.checked;

	let e = document.getElementById(id);
	e.classList.toggle("active");
	if(checked) {
		e.style.display = setting;
	}
	else {
		e.style.display = 'none';
	}
}

