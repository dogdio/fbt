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

let toggleTitle = 0;
function toggleTitleEdit()
{
	toggleTitle ^= 1;

	let e = document.getElementById("titleTr");
	let b = document.getElementById("toggleTitleButton");

	// visibilityだと枠が確保されて空白が丸見えになる
	if(toggleTitle) {
		e.style.display = 'table-row';
		b.innerHTML = "-";
	}
	else {
		e.style.display = 'none';
		b.innerHTML = "+";
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

function writeProgress()
{
	let url = '/writeProgress';
	let req = {
		id: itemId,
		contents: document.getElementById('progress').value,
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

function deleteProgress(event, progressId)
{
	let url = '/deleteProgress/' + itemId;
	let req = {
		id: progressId,
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

function updateProgress(event, progressId)
{
	console.log("Update: " + itemId + " " + progressId);
	console.log(document.getElementById('progressEditor').value);

	let url = '/updateProgress/' + itemId;
	let req = {
		id: progressId,
		contents: document.getElementById('progressEditor').value,
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

let progressEditorOn = 0;
let textSave = "";
let targetSave = "";
function toggleProgressEditor(event)
{
	let div1 = event.target.parentElement;
	let progressId = div1.previousElementSibling.previousElementSibling;
	let id = progressId.innerText.substring(1); // #[[${list.id}]]

	if(progressEditorOn && (event.target != targetSave)) {
		let div = targetSave.parentElement.parentElement;
		let text = div.nextElementSibling;

		text.innerHTML = textSave;
		targetSave.innerHTML = "+";
		progressEditorOn = 0;
	}

	let div2 = div1.parentElement; // <div style="display: flex;">
	let text = div2.nextElementSibling; // <p>[[${list.contents}]]</p>

	progressEditorOn ^= 1;
	if(progressEditorOn) {
		textSave = text.innerHTML;
		text.innerHTML = 
		'<div style="display: flex;">' +
			'<div class="flexItem">' +
				'<textarea id="progressEditor" maxlength="800" rows="7" cols="80">' +
				text.innerHTML + 
				'</textarea>' +
			'</div>' +
			'<div class="flexItem">' +
			'<input type="button" value="Update" onclick="updateProgress(event, ' + id + ')"><br><br>' +
			'<input type="button" value="Delete" onclick="deleteProgress(event, ' + id + ')">' +
			'</div>' +
		'</div>';
		event.target.innerHTML = "-";
	}
	else {
		text.innerHTML = textSave;
		event.target.innerHTML = "+";
	}

	targetSave = event.target;
}
