if !&cp && !exists(":TabClose") && has("user_commands")

command -nargs=* TabClose call TABCLOSE(<f-args>)
function TABCLOSE(del_file)
	let first = expand("%") " current file name

	execute ":tabnext"
	let fname = expand("%")

	while fname != first " foreach all tabs
"		echo fname . "||" . a:del_file

		if fname == a:del_file
"			echo "FIND"
			execute ':tabclose'
		endif

		execute ":tabnext"
		let fname = expand("%")
	endwhile

	unlet first
	unlet fname
endfunction
endif
