
////////////////////////////////////////////////////////
//
// Update, Clear
//
////////////////////////////////////////////////////////

// このスクリプトを含むHTMLがロードされた後、最後に呼ばれる
window.addEventListener("load", () => {
	fetchSyslog("sample.json");
});

// JSONの取得
// {
//  "date": "May 13 22:35:09",
//  "host": "debian11",
//  "proc": "systemd[1]:",
//  "info": "anacron.service: Succeeded."
// }
function fetchSyslog(path)
{
	console.log(path);

	// path: スペースなどの特殊文字はエンコードされる
	fetch(path)
	.then((resp) => resp.json())
	.then((json) => {
		const table = document.getElementById("syslogTable");
		const tbody = table.getElementsByTagName("tbody")[0];
		tbody.innerHTML = "";

		// <tbody>のみを更新。<thead>はキープ
		// JSONの要素には .KEY でアクセスする
		for (const info of json) {
		tbody.innerHTML +=
			"<tr class=\"syslogTr\">" +
			`<td>${info.date}</td>` +
			`<td>${info.host}</td>` +
			`<td>${info.proc}</td>` +
			`<td>${info.info}</td>` +
			"</tr>";
		}
	});
}

document.getElementById("clear").addEventListener("click", (event) => {
	// <tbody>のみをクリア。<thead>はキープ
	const table = document.getElementById("syslogTable");
	const tbody = table.getElementsByTagName("tbody")[0];
	tbody.innerHTML = "";

	// <form>のnameに対して.でアクセス出来る
	document.syslogFormName.num.value = "";
	document.syslogFormName.str.value = "";
	document.syslogFormName.rev.value = "on";
});

document.getElementById("update").addEventListener("click", (event) => {
	let num = document.getElementById('num').value;
	let str = document.getElementById('str').value;
	let rev = document.getElementById('rev').value;
//	alert("num=" + num + "str=" + str + "rev=" + rev);

	// pattern= で指定した文字列チェック
	// 'submit'ボタンじゃない場合は手動でReportする
	let n = document.getElementById("num");
	if(n.validity.patternMismatch) {
		n.setCustomValidity("正数のみ入力可能です");
		n.reportValidity();
		console.log("n:Error");
		return;
	}

	let s = document.getElementById("str");
	if(s.validity.patternMismatch) {
		s.setCustomValidity("正数,英字,半角スペースのみ入力可能です");
		s.reportValidity();
		console.log("s:Error");
		return;
	}

	fetchSyslog("sample.json?num=" + num + "&str=" + str + "&rev=" + rev);
});

////////////////////////////////////////////////////////
//
// Table Sort
//
////////////////////////////////////////////////////////
var SortOrder = [ 0, 0, 0 ];

function printRows(rows)
{
	let d = rows.getElementsByTagName("td")[0];
	let h = rows.getElementsByTagName("td")[1];
	let p = rows.getElementsByTagName("td")[2];
	let i = rows.getElementsByTagName("td")[3];
	console.log("%s, %s, %s, %s", 
		d.innerHTML.toString(), h.innerHTML.toString(),
		p.innerHTML.toString(), i.innerHTML.toString());
}

function sortTable(col)
{
	let table = document.getElementById("syslogTable");
	let tbody = table.getElementsByTagName("tbody")[0];
//	for (let i = 1; i < (table.rows.length); i++) printRows(table.rows[i]);

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

////////////////////////////////////////////////////////
//
// Input check
//
////////////////////////////////////////////////////////

// フォーカスが外れたとき(半角/全角キーで残るときの対策)
document.getElementById("num").addEventListener("change", (event) => {
	console.log("C:" + event.key + ", " + event.code);
	if(!document.getElementById('num_check_flag').checked)
		return;

	let input = document.getElementById(event.target.id);
	input.value = input.value.replace(/\D/g, "");
});

// 入力中：キーを離したとき
document.getElementById("num").addEventListener("keyup", (event) => {
	console.log("U:" + event.key + ", " + event.code);
	if(!document.getElementById('num_check_flag').checked)
		return;

	let input = document.getElementById(event.target.id);
	input.value = input.value.replace(/\D/g, "");
});

// 入力中：キーを押したとき
document.getElementById("num").addEventListener("keydown", (event) => {
	console.log("D:" + event.key + ", " + event.code);
	if(!document.getElementById('num_check_flag').checked)
		return;

	if(0 <= event.key && event.key <= 9)
		return;
	if(event.key == "ArrowRight" || event.key == "ArrowLeft")
		return;
	if(event.key == "Backspace" || event.key == "Delete" || event.key == "Tab")
		return;

	// イベントをキャンセルし、不正な値をそもそも入力させない
	event.preventDefault();
});

document.getElementById("syslogForm").addEventListener("input", (event) => {
	// setCustomValidity使用後は、空文字をいれないとエラー状態が解除されない

	let n = document.getElementById("num");
	if(!n.validity.patternMismatch)
		n.setCustomValidity('');

	let s = document.getElementById("str");
	if(!s.validity.patternMismatch)
		s.setCustomValidity('');
});

////////////////////////////////////////////////////////
//
// Help message
//
////////////////////////////////////////////////////////

// 入力中：フォーカスが合ったとき
document.getElementById("num").addEventListener("focusin", (event) => {
	console.log("Focus: In");
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "半角正数3桁を入力<br>します";
});

// 入力中：フォーカスが外れたとき
document.getElementById("num").addEventListener("focusout", (event) => {
	console.log("Focus: Out");
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// 入力中：フォーカスが合ったとき
document.getElementById("str").addEventListener("focusin", (event) => {
	console.log("Focus: In");
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "英字,数字,半角SP<br>を入力します";
});

// 入力中：フォーカスが外れたとき
document.getElementById("str").addEventListener("focusout", (event) => {
	console.log("Focus: Out");
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// Update button
document.getElementById("update").addEventListener("mousemove", (event) => {
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "テーブルを再検索<br>します";
//	console.log(event.clientX + ", " + event.clientY);
});

document.getElementById("update").addEventListener("mouseout", (event) => {
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// Clear button
document.getElementById("clear").addEventListener("mousemove", (event) => {
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "テーブルと入力値を<br>消去をします";
});

document.getElementById("clear").addEventListener("mouseout", (event) => {
	if(!document.getElementById('help_flag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});
