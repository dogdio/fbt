if !&cp && has("user_commands")
  command -range=% Diff :call DIFF()
  func DIFF()
    let line = getline(".")
	let mx='Files\s\(\f\+\)\sand\s*\(\f\+\)'
	let l = matchstr(line, mx)
	let file1 = substitute(l, mx, '\1', '')
	let file2 = substitute(l, mx, '\2', '')
	let exe1 = "tabnew " . file2
	let exe2 = ":vert diffsplit " . file1

	execute ":set scrolloff=3"
	execute exe1
	execute ":set syntax=off"
	execute exe2
	execute ":set syntax=off"

	unlet line
	unlet mx
	unlet l
	unlet file1
	unlet file2
	unlet exe1
	unlet exe2
  endfunc
endif
