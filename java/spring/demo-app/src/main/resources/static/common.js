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
