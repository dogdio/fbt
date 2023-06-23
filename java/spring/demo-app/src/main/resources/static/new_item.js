function registItem()
{
	let url = '/regist';
	let req = {
		title: document.getElementById('title').value,
		priority: document.getElementById('priority').value,
		status: document.getElementById('status').value,
		category: document.getElementById('category').value,
		worker: document.getElementById('worker').value,
		deadline: document.getElementById('deadline').value,
	};

	postJson(url, req).then(json => {
		updateInputTable('registTable', json);

		for (let info of json) {
			if(info.key == 'newItemId' && info.attr == 'EXT') {
				console.log("####### new item: " + info.value);
				window.location.replace("/show/" + info.value);
			}
		}
	});
}

