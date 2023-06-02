////////////////////////////////////////////////////////
//
// Table Sort
//
////////////////////////////////////////////////////////
var SortOrder = [ 0, 0, 0, 0, 0, 0, 0 ];

function sortTable(col)
{
	let table = document.getElementById("itemTable");
	let tbody = table.getElementsByTagName("tbody")[0];

	let isNumber = true;
	let sortArray = new Array;
	for (let r = 1; r < table.rows.length; r++) {
		let sortData = new Object;
		sortData.originalRow = table.rows[r];
		sortData.cmpValue = table.rows[r].cells[col].textContent;
		sortArray.push(sortData);
		if(isNaN(sortData.cmpValue)) // Not a Number
			isNumber = false;
	}

	console.log("order=%d, isNum=%s", SortOrder[col], isNumber);
	if(SortOrder[col] == 0) {
		if(isNumber)
			sortArray.sort(compareNum);
		else
			sortArray.sort(compareStr);
	}
	else {
		if(isNumber)
			sortArray.sort(compareNumRev);
		else
			sortArray.sort(compareStrRev);
	}
	SortOrder[col] ^= 1;
	if(SortOrder[col] == 0)
		document.getElementById("sortIcon" + col).innerHTML = "&#x25b2";
	else
		document.getElementById("sortIcon" + col).innerHTML = "&#x25bc";

	// appendChild: 追加ノードが元々存在する場合は新しい位置に置き換えられる
	for (let r = 0; r < sortArray.length; r++)
		tbody.appendChild(sortArray[r].originalRow);
}

function compareStr(a, b)
{
	if(a.cmpValue < b.cmpValue)
		return -1;
	else
		return 1;
}

function compareStrRev(a, b)
{
	if(a.cmpValue > b.cmpValue)
		return -1;
	else
		return 1;
}

function compareNum(a, b)
{
	return a.cmpValue - b.cmpValue;
}

function compareNumRev(a, b)
{
	return b.cmpValue - a.cmpValue;
}

window.addEventListener('pageshow', function(event) {
	console.log("pageshow: " + location.toString());

	if (event.persisted) { // Cacheの場合はリロード
		window.location.reload();
	}
});

