
////////////////////////////////////////////////////////
//
// Update, Clear
//
////////////////////////////////////////////////////////
document.getElementById("clear").addEventListener("click", (event) => {
	// <tbody>のみをクリア。<thead>はキープ
	const table = document.getElementById("syslogTable");
	const tbody = table.getElementsByTagName("tbody")[0];
	tbody.innerHTML = "";

	// <form>のnameに対して.でアクセス出来る
	document.syslogFormName.num.value = "";
	document.syslogFormName.str.value = "";
	document.syslogFormName.seq.value = "new";
});


document.getElementById("update").addEventListener("click", (event) => {
	let num = document.getElementById('num').value;
	let str = document.getElementById('str').value;
	let seq = document.getElementById('seq').value;
//	alert("num=" + num + "str=" + str + "seq=" + seq);

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
	if(!document.getElementById('numCheckFlag').checked)
		return;

	let input = document.getElementById(event.target.id);
	input.value = input.value.replace(/\D/g, "");
});

// 入力中：キーを離したとき
document.getElementById("num").addEventListener("keyup", (event) => {
	console.log("U:" + event.key + ", " + event.code);
	if(!document.getElementById('numCheckFlag').checked)
		return;

	let input = document.getElementById(event.target.id);
	input.value = input.value.replace(/\D/g, "");
});

// 入力中：キーを押したとき
document.getElementById("num").addEventListener("keydown", (event) => {
	console.log("D:" + event.key + ", " + event.code);
	if(!document.getElementById('numCheckFlag').checked)
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
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "半角正数3桁を入力<br>します";
});

// 入力中：フォーカスが外れたとき
document.getElementById("num").addEventListener("focusout", (event) => {
	console.log("Focus: Out");
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// 入力中：フォーカスが合ったとき
document.getElementById("str").addEventListener("focusin", (event) => {
	console.log("Focus: In");
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "英字,数字,半角SP<br>を入力します";
});

// 入力中：フォーカスが外れたとき
document.getElementById("str").addEventListener("focusout", (event) => {
	console.log("Focus: Out");
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// Update button
document.getElementById("update").addEventListener("mousemove", (event) => {
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "テーブルを再検索<br>します";
//	console.log(event.clientX + ", " + event.clientY);
});

document.getElementById("update").addEventListener("mouseout", (event) => {
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

// Clear button
document.getElementById("clear").addEventListener("mousemove", (event) => {
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'block';
	input_help.innerHTML = "テーブルと入力値を<br>消去をします";
});

document.getElementById("clear").addEventListener("mouseout", (event) => {
	if(!document.getElementById('helpFlag').checked)
		return;

	let input_help = document.getElementById('input_help');
	input_help.style.display = 'none';
});

////////////////////////////////////////////////////////
//
// Right Click
//
////////////////////////////////////////////////////////
var SyslogMenuIndex = -1;
var SyslogRows = ['sort_date', 'sort_host', 'sort_proc'];

function setRowIndex(id) {
	for(let i = 0; i < SyslogRows.length; i++) {
		if(id == SyslogRows[i]) {
			SyslogMenuIndex = i;
			break;
		}
	}
}

function unsetStyle() {
	if(0 <= SyslogMenuIndex && SyslogMenuIndex <= 2) {
		let e = document.getElementById(SyslogRows[SyslogMenuIndex]);
		e.style.color = '#000000';
		e.style.backgroundColor = '#eeeeee';
	}
}

// 背景色だけがtransition対象となり、文字色は即座に反映される
function setStyle(id) {
	let e = document.getElementById(id);
	e.style.color = '#f0f8ff';
	e.style.backgroundColor = '#4682b4';
	e.style.transition = 'background-color 0.6s ease-in-out 0s';
}

function doSort(idx) {
	sortTable(idx);
	disableSyslogMenu();
}

function getColumnValue(tr, col) {
	let find = 0;
	let ret = "";

	for(let i = 0; i < tr.childNodes.length; i++) {
//		console.log(tr.childNodes[i]);
		if(tr.childNodes[i].nodeType != Node.TEXT_NODE)
			continue;

		let td = tr.childNodes[i].nextSibling;
		if(td == null)
			continue;

		if(col == find) {
			ret = td.innerText;
			break;
		}
		find++;
	}
	return ret;
}

function enableSyslogMenu(event) {
	event.preventDefault();

//	console.log("X=" + event.clientX + ", Y=" + event.clientY);
	// クリックした位置を起点にメニューを表示
	let e = document.getElementById('syslogMenuId');
	if(e.classList.contains('show')) // 右クリック → 右クリックは無効
		return;

	e.classList.toggle('show');
	e.style.top  = event.clientY + 'px'; // 'px' つけないとダメ!
	e.style.left = event.clientX + 'px';
	e.style.visibility = 'visible';

	let cell = event.target;
	let tr = cell.parentNode;

	console.log(tr.rowIndex);    // clicked Row   : 1 base
	console.log(cell.cellIndex); // clicked Column: 0 base
	console.log(cell.innerText); // clicked text

	console.log(getColumnValue(tr, 0));
}

function disableSyslogMenu() {

	unsetStyle();

	let e = document.getElementById('syslogMenuId');
	// transitionが完了するまではhiddenにしない
	if(e.classList.contains('show'))
		e.classList.toggle('show');

	SyslogMenuIndex = -1;
}

// 上下キーを押したとき
function keyDownSyslogMenu(index) {

	unsetStyle();

	SyslogMenuIndex += index;
	if(SyslogMenuIndex < 0)
		SyslogMenuIndex = 2;
	else if(SyslogMenuIndex >= SyslogRows.length)
		SyslogMenuIndex = 0;

	setStyle(SyslogRows[SyslogMenuIndex]);

	console.log("idx=" + SyslogMenuIndex);
}

// フォーカスが合ったとき
function mouseMoveSyslogMenu(event) {

	if(event.target.id != SyslogRows[SyslogMenuIndex])
		unsetStyle();

	setStyle(event.target.id);
	setRowIndex(event.target.id);
}

// フォーカスが外れたとき
function mouseOutSyslogMenu(event) {
	unsetStyle();
}


// 特定のクラス名を持つすべての要素を取得し、イベントリスナーを設定
// classの場合は.をつける(CSSと同じ)
document.querySelectorAll(".syslogTr").forEach(function(element) {
	element.addEventListener("contextmenu", enableSyslogMenu);
});

document.querySelectorAll(".syslogMenuTr").forEach(function(element) {
	element.addEventListener("mousemove", mouseMoveSyslogMenu);
});

document.querySelectorAll(".syslogMenuTr").forEach(function(element) {
	element.addEventListener("mouseout", mouseOutSyslogMenu);
});

// transition 完了ハンドラ
document.getElementById('syslogMenuId').addEventListener("transitionend", (event) => {
//	console.log(event);

	// 子要素の<tr>で遷移した後もこのイベントが発生する
	if(event.target.id == 'syslogMenuId') {
		let e = event.target;
		if(!e.classList.contains('show'))
			e.style.visibility = 'hidden';
		// hiddenにしないと、画面には映らないがクリック可能な領域が残る
	}

});

window.addEventListener("keydown", (event) => {
//	console.log(event);

	let e = document.getElementById('syslogMenuId');
	if(e.classList.contains('show')) {
		if(event.key == 'd')
			doSort(0);
		else if(event.key == 'h')
			doSort(1);
		else if(event.key == 'p')
			doSort(2);
		else if(event.key == 'ArrowDown')
			keyDownSyslogMenu(1);
		else if(event.key == 'ArrowUp')
			keyDownSyslogMenu(-1);
		else if(event.key == 'Enter')
			doSort(SyslogMenuIndex);
		else if(event.key == 'Escape')
			disableSyslogMenu();
	}
});

window.addEventListener("click", (event) => {
//	console.log(event);

	// 別のエリアをクリックしたとき、メニューの選択肢を選んだときに元に戻す
	let e = document.getElementById('syslogMenuId');
	if(e.classList.contains('show'))
		disableSyslogMenu();

});


