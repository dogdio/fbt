" NOTE: this is not the original file! converted for use with xterm-256

"------------ for ALL -------------
hi Folded term=standout ctermbg=Black ctermfg=DarkCyan
hi FoldColumn term=standout ctermbg=Black ctermfg=DarkCyan


hi Search  term=standout ctermbg=Yellow ctermfg=Black
set hlsearch
set background=light
"hi Normal          ctermfg=Black ctermbg=252
"hi clear          

hi Normal          ctermfg=Black ctermbg=247
hi LineNr          ctermfg=238 ctermbg=245
hi Cursor          ctermfg=White ctermbg=Black
hi CursorLine      cterm=none ctermbg=246
"hi CursorLine      cterm=underline
hi CursorLineNr    ctermfg=White ctermbg=26

hi TabLineSel      ctermfg=White ctermbg=Blue


"------------ for *.diff -------------
hi diffOnly    ctermfg=Black
"hi diffFile   ctermbg=191 ctermfg=26 cterm=bold
hi diffAdded   ctermfg=28
hi diffRemoved ctermfg=165

"hi Search  cterm=bold ctermbg=190 ctermfg=232
hi DiffChange term=standout ctermbg=248 ctermfg=21
hi DiffText   term=standout ctermbg=117 ctermfg=21
hi DiffDelete term=standout ctermbg=232 ctermfg=159
hi DiffAdd    term=standout ctermbg=159 ctermfg=232

"------------ for Makefile -------------
hi makeIdent  ctermfg=20
hi makeTarget ctermfg=93 cterm=bold

"------------ for javaScript -------------
"syn keyword javaScriptIdentifier	arguments this var
hi javaScriptIdentifier	ctermfg=20 cterm=bold
hi javaScriptFunction   ctermfg=20 cterm=bold
hi javaScriptBraces	     ctermfg=20
hi javaScriptParens	     ctermfg=20

"------------ for C, etc,, -------------
"hi Normal          ctermfg=Black ctermbg=146
hi Type            ctermfg=26 cterm=bold
hi Comment         ctermfg=28

hi Number          ctermfg=201
hi Operator        ctermfg=20 cterm=bold
hi Label           ctermfg=20 cterm=bold
hi Conditional     ctermfg=20 cterm=bold
hi Statement       ctermfg=20 cterm=bold
"hi Keyword         ctermfg=33 cterm=bold
hi String          ctermfg=160
hi PreProc         ctermfg=93

hi cHoge           ctermfg=32
hi Todo            ctermfg=Yellow ctermbg=Black cterm=bold

hi Visual          ctermfg=117 ctermbg=Black
hi StatusLine      ctermbg=152 ctermfg=26 cterm=bold
"hi StatusLineNC    ctermfg=236 ctermbg=252

hi cssClassName    ctermfg=Black
hi cssBraces       ctermfg=Black
hi cssIdentifier   ctermfg=Black
hi cssFunctoinName ctermfg=Black
hi htmlTag         ctermfg=Black
hi htmlEndTag      ctermfg=Black
hi xmlTag          ctermfg=Black
hi xmlTagName      ctermfg=Black
hi xmlEndTag       ctermfg=Black

hi Special         ctermfg=Black

hi helpHyperTextJump	ctermfg=20 cterm=bold
hi helpHyperTextEntry	ctermfg=20 cterm=bold 
hi helpVim           	ctermfg=20 cterm=bold 

hi vimFuncName       	ctermfg=26 cterm=bold 
hi vimVar        	 	ctermfg=Black
"hi Identifier        	ctermfg=26 cterm=bold 

"
"hi Visual          ctermfg=236 ctermbg=210 cterm=reverse
"hi Title           ctermfg=15 ctermbg=236 cterm=bold
"hi Boolean         ctermfg=181 cterm=bold
"hi Character       ctermfg=181 cterm=bold
"hi Comment         ctermfg=8
"hi Constant        ctermfg=181 cterm=bold
"hi Debug           ctermfg=181 cterm=bold
"hi Define          ctermfg=223 cterm=bold
"hi Delimiter       ctermfg=245
"hi DiffAdd         ctermbg=239
"hi DiffChange      ctermbg=236
"hi DiffDelete      ctermfg=236 ctermbg=238 cterm=none
"hi DiffText        ctermfg=15 ctermbg=234 cterm=bold
"hi Directory       ctermfg=15 cterm=bold
"hi Error           ctermfg=0 ctermbg=14
"hi ErrorMsg        ctermfg=0 ctermbg=6
"hi Exception       ctermfg=123 cterm=underline
"hi Float           ctermfg=139
"hi FoldColumn      ctermfg=181 ctermbg=238
"hi Folded          ctermfg=181 ctermbg=236
"hi Function        ctermfg=228
"hi Identifier      ctermfg=15
"hi Include         ctermfg=223 cterm=bold
"hi IncSearch       ctermfg=0 ctermbg=131
"hi Keyword         ctermfg=15 cterm=bold
"hi Macro           ctermfg=223 cterm=bold
"hi ModeMsg         ctermfg=181 cterm=bold
"hi MoreMsg         ctermfg=15 cterm=bold
"hi NonText         ctermfg=234
"hi PreCondit       ctermfg=180 cterm=bold
"hi Question        ctermfg=15 cterm=bold
"hi Repeat          ctermfg=123 cterm=underline
"hi Search          ctermfg=0 ctermbg=131
"hi SpecialChar     ctermfg=181 cterm=bold
"hi SpecialComment  ctermfg=181 cterm=bold
"hi SpecialKey      ctermfg=8
"hi StorageClass    ctermfg=15 cterm=bold
"hi Structure       ctermfg=15 cterm=bold,underline
"hi Tag             ctermfg=181 cterm=bold
"hi Title           ctermfg=15 ctermbg=236 cterm=bold
"hi Typedef         ctermfg=15 cterm=bold,underline
"hi VertSplit       ctermfg=236 ctermbg=252
"hi Visual          ctermfg=236 ctermbg=210 cterm=reverse
"hi VisualNOS       ctermfg=236 ctermbg=210 cterm=bold,underline
"hi WarningMsg      ctermfg=15 ctermbg=236 cterm=bold
"hi WildMenu        ctermfg=0 ctermbg=181
