function changeErrItemColor()
{
	let table = document.getElementById('registTable');

	for (let r = 0; r < table.rows.length; r++) {
		console.log(table.rows[r].cells[2]);
		let td0 = table.rows[r].cells[0];
		let td1 = table.rows[r].cells[1];
		let td2 = table.rows[r].cells[2];

		if(td0.innerHTML.length != 0) {
			if(td2.innerHTML.length != 0) {
				td1.firstElementChild.style.background = "#ffe0e0";
			}
			else {
				td1.firstElementChild.style.background = "#ffffff";
			}
		}
	}
}

window.addEventListener("load", () => {
	changeErrItemColor();
});

