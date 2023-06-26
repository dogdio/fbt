function deleteItemConfirm(event)
{
	let item = document.getElementById("title").value;
	let msg = document.getElementById("modalDialogMsg").value;

	createConfirmDialog(event, item, msg, function(answer) {
		console.log(">>>>>" + answer + ", " + itemId);

		if(answer) {
			let url = '/delete/' + itemId;
			let req = '';
            
			postJson(url, req).then(json => {
				for (let info of json) {
					if(info.key == 'deleted' && info.value == itemId && info.attr == 'EXT')
						window.location.replace("/summary");
				}
			});
		}
	});
}

function deleteProgressConfirm(event, keyId, msgId)
{
	let msg = document.getElementById("modalDialogMsg").value;
	console.log(event);
	console.log("### Y " + event.clientY);
	console.log("### X " + event.clientX);

	createConfirmDialog(event, msgId, msg, function(answer) {
		console.log("### confirm: " + answer);

		if(answer) {
			let url = '/deleteProgress/' + keyId;
			let req = '';

			postJson(url, req).then(json => {
				for (let info of json) {
					if(info.key == 'deleted' && info.value == keyId && info.attr == 'EXT')
						window.location.reload();
				}
			});
		}
	});
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

	let url = location.toString();
	let regex = /.*\//;

	url = url.replace(regex, '/');
	let m = url.match(/^\/(\d+)/);
	if(m.length == 2) {
		itemId = m[1];
		console.log("itemId: " + itemId);
	}
});

document.addEventListener('DOMContentLoaded', function() {
	let e = document.getElementById("titleTr");
	e.style.display = 'none';
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

	postJson(url, req).then(json => {
		if(inputData.title != req.title) {
			setTimeout(function() {
				window.location.reload();
			}, 600);
		}
		updateInputTable('editTable', json);
		inputData = req;
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

	postJson(url, req).then(json => {
		updateInputArea("progress", "progressMsg", "contents", json);

		for (let info of json) {
			if(info.key == 'write' && info.value == itemId && info.attr == "EXT") {
				window.location.reload();
				break;
			}
		}
	});
}

function updateProgress(event, id)
{
	console.log("Update/" + id);
	console.log(document.getElementById('progressEditor').value);

	let url = '/updateProgress';
	let req = {
		id: id,
		contents: LFtoBR(document.getElementById('progressEditor').value),
	};

	postJson(url, req).then(json => {
		updateInputArea("progressEditor", "progressEditorMsg", "contents", json);

		for (let info of json) {
			if(info.key == 'update' && info.value == id && info.attr == "EXT") {
				window.location.reload();
				break;
			}
		}
	});
}

let textSave = "";
let targetSave = null;
function toggleProgressEditor(event)
{
	let div1 = event.target.parentElement;
	let key = div1.nextElementSibling;
	let id = key.innerText.substring(1); // #[[${list.id}]]
	let msgId = "'" + div1.previousElementSibling.previousElementSibling.innerText + "'";
	console.log(msgId);

	// 既に開いている方をすぐに閉じる（アニメなし）
	if(targetSave != null && (event.target != targetSave)) {
		let div = targetSave.parentElement.parentElement.parentElement;
		let text = div.nextElementSibling;

		text.innerHTML = textSave;
		targetSave.checked = false;
	}

	let div2 = div1.parentElement.parentElement; // <div style="display: flex;"> FIXME: id
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
			'<input class="customButton1" type="button" value="Update" onclick="updateProgress(event, ' + id + ')">' +
			'<div style="margin-bottom: 6px;"></div>' +
			'<input class="customButton1" type="button" value="Delete" onclick="deleteProgressConfirm(event, ' + id + ', ' + msgId + ')">' +
			'<p id="progressEditorMsg"></p>' +
			'</div>' +
		'</div>';
		let e = text.firstElementChild;
		e.classList.add("active");
		e.classList.remove("inactive");
	}
	else {
		let e = text.firstElementChild;
		e.addEventListener("animationend", (event) => {
			if(e.classList.contains('inactive'))
				text.innerHTML = textSave;
		});
		e.classList.remove("active");
		e.classList.add("inactive");
	}

	targetSave = event.target;
}

function toggleDynamicArea(event, id, setting)
{
	let checked = event.target.checked;

	let e = document.getElementById(id);
	if(checked) {
		e.classList.add("active");
		e.classList.remove("inactive");
		e.style.display = setting;
	}
	else {
		e.addEventListener("animationend", (event) => {
			if(e.classList.contains('inactive'))
				e.style.display = 'none';
		});
		e.classList.remove("active");
		e.classList.add("inactive");
	}
}

