" Vim syntax file
" Language:	Txt
" Maintainer:	Hoge
" Last Change:	2007/02/28

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

syn match	tComment0  "\<[0-9]\>"
syn match	tComment0  "\<[0-9][0-9.]\+\>"
syn match	tComment1  "^\s*#.*"
syn match	tComment2  "^===.*"
syn match	tComment3  "^---.*"

syn match	tComment4  "^[0-9]\..*"
"syn match	tComment4  "^\s[0-9]\s"
"syn match	tComment4  "^\s[0-9][0-9.]\+"

syn match	helpNote  "[nN][oO][tT][eE][sS]*:"
syn match	helpNote  "[tT][oO][dD][oO]:"
syn match	helpNote  "[fF][iI][xX][mM][eE]:"
syn match	helpNote  "[xX][xX][xX]:"

"syn match	tComment5 "[mM][uU][sS][tT]"
"syn match	tComment5 "[sS][hH][oO][uU][lL][dD]"
syn keyword     tComment5 NOT MUST SHOULD REQUIRED SHALL RECOMMENDED MAY OPTIONAL
"syn match	tComment5 "[mM][aA][nN][dD][aA][tT][oO][rR][yY]"
syn match	tComment0 "[sS][eE][cC][tT][iI][oO][nN]"

"syn keyword     tComment6  HTTP UDP TCP HTTPS IP ICMP IGMP RIP OSPF BGP CRC CRC32 CRLF CR LF FTP TFTP DNS DHCP SNMP 
"syn match	tComment7  "^\s*[a-zA-Z0-9_\/\.][a-zA-Z0-9_\/\.\s]*:"
"syn match	tComment7  "^\s*[\sa-zA-Z0-9_\/\.]\+:"
syn match	tComment7  "^.\+:"
syn match	tComment8  "[0-9]\+:"

syn match	tComment10  "RFC.[0-9]\+"
syn match	tComment9  "^RFC.*"


"syn keyword helpNote		note Note NOTE note: Note: NOTE: Notes Notes:

"if exists("c_comment_strings")
"  " A comment can contain cString, cCharacter and cNumber.
"  " But a "*/" inside a cString in a cComment DOES end the comment!  So we
"  " need to use a special type of cString: cCommentString, which also ends on
"  " "*/", and sees a "*" at the start of the line as comment again.
"  " Unfortunately this doesn't very well work for // type of comments :-(
"  syntax match	cCommentSkip	contained "^\s*\*\($\|\s\+\)"
"  syntax region cCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=cSpecial,cCommentSkip
"  syntax region cComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=cSpecial
"  syntax region  cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cComment2String,cCharacter,cNumbersCom,cSpaceError,@Spell
"  if exists("c_no_comment_fold")
"    syntax region cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell
"  else
"    syntax region cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell fold
"  endif
"else
"  syn region	cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cSpaceError,@Spell
"  if exists("c_no_comment_fold")
"    syn region	cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell
"  else
"    syn region	cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell fold
"  endif
"endif

"hi def link tComment0		Type
hi def link tComment1		Comment
"hi def link tComment7		Type
"hi def link tComment2		String
"hi def link tComment3		Keyword
"hi def link tComment4		Type

hi def link helpNote		Todo

hi tComment0 term=standout  ctermfg=Magenta
"hi tComment1 term=standout  ctermfg=Green
hi tComment2 term=standout  ctermfg=Magenta
hi tComment3 term=standout  ctermfg=Yellow
hi tComment4 term=standout  ctermfg=Blue

hi tComment5 term=standout  ctermfg=DarkGreen
hi tComment6 term=standout  ctermfg=Yellow
hi tComment7 term=standout  ctermfg=Blue
hi tComment8 term=standout  ctermfg=Brown
hi tComment9 term=standout  ctermfg=Black

hi tComment10 term=standout ctermbg=Black ctermfg=Yellow

"hi def link cComment		Comment
"hi def link cCommentError	cError
"hi def link cCommentStartError	cError
"hi def link cCommentL		cComment
"hi def link cCommentStart	cComment
"hi def link cCommentString	cString
"hi def link cComment2String	cString
"hi def link cCommentSkip	cComment
let b:current_syntax = "txt"

" vim: ts=8
