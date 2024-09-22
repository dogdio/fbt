Add-Type -AssemblyName Microsoft.VisualBasic
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

$Global:VALUE_WIDTH = 200
$Global:LABEL_WIDTH = 60
$Global:VALUES_JSON = 'values.json'

$Global:MENU_X0 = 30
$Global:MENU_Y0 = 30
$Global:MenuX = $MENU_X0
$Global:MenuY = $MENU_Y0

#$obj = [PSCustomObject]@{ v=$v; myobj=$button; label=$null; }
$Global:ObjList = @{}

function SaveConfig
{
    $ValueObj = @{}

    foreach ($key in $ObjList.Keys) {
        $obj = $ObjList[$Key]

        if ((IsValid $obj $false) -eq $false) { continue }

        if ($obj.v.type -eq "choice") {
            $ValueObj | Add-Member -MemberType NoteProperty -Name $key -Value $obj.myobj.SelectedItem
        }
        if ($obj.v.type -eq "number") {
            $ValueObj | Add-Member -MemberType NoteProperty -Name $key -Value $obj.myobj.Text
        }
        if ($obj.v.type -eq "string") {
            $ValueObj | Add-Member -MemberType NoteProperty -Name $key -Value $obj.myobj.Text
        }
        if ($obj.v.type -eq "date") {
            $date = $obj.myobj.Value.ToString("yyyy/MM/dd")
            $ValueObj | Add-Member -MemberType NoteProperty -Name $key -Value $date
        }
        if ($obj.v.type -eq "onoff") {
            $ValueObj | Add-Member -MemberType NoteProperty -Name $key -Value $obj.myobj.Checked
        }
    }

    $json = $ValueObj | ConvertTo-Json -Depth 4
    $json | Set-Content -Path $VALUES_JSON -Encoding UTF8
}

function InitObjList
{
    $ValueObj = Get-Content -Path $VALUES_JSON -Raw | ConvertFrom-Json
    Write-Host $ValueObj

    foreach ($key in $ObjList.Keys) {
        $obj = $ObjList[$Key]

        # JSONオブジェクトは obj[keyname]とは書けないのでPropertiesを使う
        # keynameがわかる場合は obj.keyname とする
        $prop = $ValueObj.PSObject.Properties[$key]
        if ($prop) {
            $value = $prop.Value

            if ($obj.v.type -eq "choice") {
                $obj.myobj.SelectedItem = $value
            }
            if ($obj.v.type -eq "number") {
                $obj.myobj.Text = $value
            }
            if ($obj.v.type -eq "string") {
                $obj.myobj.Text = $value
            }
            if ($obj.v.type -eq "date") {
                $obj.myobj.Value = [System.DateTime]$($value)
            }
            if ($obj.v.type -eq "onoff") {
                $obj.myobj.Checked = $value
            }
            continue
        }

        if ($obj.v.PSObject.Properties['init']) {
            if ($obj.v.type -eq "choice") {
                $obj.myobj.SelectedItem = $obj.v.init
            }
            if ($obj.v.type -eq "number") {
                $obj.myobj.Text = $obj.v.init
            }
            if ($obj.v.type -eq "string") {
                $obj.myobj.Text = $obj.v.init
            }
            if ($obj.v.type -eq "date") {
                $obj.myobj.Value = [System.DateTime]$($obj.v.init)
            }
            if ($obj.v.type -eq "onoff") {
                $obj.myobj.Checked = $obj.v.init
            }
        }
    }
}

function GetValueByName
{
    param ($id)

    $obj = $ObjList[$id]
    if ($obj.v.type -eq "choice") {
        return $obj.myobj.SelectedItem
    }
    if ($obj.v.type -eq "number") {
        return $obj.myobj.Text
    }
    if ($obj.v.type -eq "string") {
        return $obj.myobj.Text
    }
    if ($obj.v.type -eq "date") {
        return $obj.myobj.Value
    }
    if ($obj.v.type -eq "onoff") {
        return $obj.myobj.Checked
    }

    return $null
}

function GetObjectByName
{
    param ($id)
    return $ObjList[$id]
}

function GetObject
{
    param ($this)

    foreach ($key in $ObjList.Keys) {
        $obj = $ObjList[$Key]
        if ($this -eq $($obj.myobj)) {
            return $obj
        }
    }
    return $null
}

function SetObject
{
    param ($id, $newobj)

    $Global:ObjList[$id] = $newobj
}

function CreateComboBox
{
    param ($v, $tab, $x, $y)

    # ドロップダウンリストを作成
    $comboBox = New-Object System.Windows.Forms.ComboBox
    $comboBox.Width = $VALUE_WIDTH
    $comboBox.Location = New-Object System.Drawing.Point($x, $y)
    foreach ($item in $v.values) {
        $comboBox.Items.Add($item) | Out-Null
    }
    $comboBox.Text          = $v.values[0]
    $comboBox.SelectedItem  = $v.values[0]
    $comboBox.DropDownStyle = [System.Windows.Forms.ComboBoxStyle]::DropDownList  # ユーザーの入力を無効化.
#   Write-Host $comboBox.SelectedItem

    $tab.Controls.Add($comboBox)
    $newobj = [PSCustomObject]@{ v=$v; myobj=$comboBox; label=$null; }
    return $newobj
}

function CreateTextStr
{
    param ($v, $tab, $x, $y)

    # テキストボックスを作成
    $textBox = New-Object System.Windows.Forms.TextBox
    $textBox.Width = $VALUE_WIDTH
    $textBox.Location = New-Object System.Drawing.Point($x, $y)
    $textBox.Text = ""

    $y += 20
    $errorLabel = New-Object System.Windows.Forms.Label
    $errorLabel.Text = ""
    $errorLabel.ForeColor = [System.Drawing.Color]::Red
    $errorLabel.Location = New-Object System.Drawing.Point($x, $y)
    $errorLabel.Width = $VALUE_WIDTH

    $textBox.Add_TextChanged({ # 値が変わったときにコールされる
        $length = $this.Text.Length
        $obj = GetObject $this

        if ($obj -ne $null) {
            $errorLabel = $($obj.label)
            $min = -1
            $max = -1
            if ($obj.v.PSObject.Properties['min']) {
                $min = $obj.v.min
            }
            if ($obj.v.PSObject.Properties['max']) {
                $max = $obj.v.max
            }

            if (($min -ne -1) -and ($max -ne -1)) {
                #Write-Host "[obj] $this, $min, $max"
                if ($length -gt $max -or $length -lt $min) {
                    $errorLabel.Text = "長さは " + [string]$min + "～" + [string]$max + " としてください"
                }
                else {
                    $errorLabel.Text = ""
                }
            }
        }
    })
    $tab.Controls.Add($textBox)
    $tab.Controls.Add($errorLabel)
    $newobj = [PSCustomObject]@{ v=$v; myobj=$textBox; label=$errorLabel; }
    return $newobj
}

function CreateTextNum
{
    param ($v, $tab, $x, $y)

    # テキストボックスを作成
    $textBox = New-Object System.Windows.Forms.TextBox
    $textBox.Width = $VALUE_WIDTH
    $textBox.Location = New-Object System.Drawing.Point($x, $y)
    $textBox.Text = ""

    $y += 20
    $errorLabel = New-Object System.Windows.Forms.Label
    $errorLabel.Text = ""
    $errorLabel.ForeColor = [System.Drawing.Color]::Red
    $errorLabel.Location = New-Object System.Drawing.Point($x, $y)
    $errorLabel.Width = $VALUE_WIDTH

    $textBox.Add_KeyPress({ # 通常キーの検知
        param($sender, $e)
        if ($e.KeyChar -eq [char]8) {
            return
        }
        if ($e.KeyChar -lt '0' -or $e.KeyChar -gt '9') {
            $e.Handled = $true
        }
    })
    $textBox.Add_KeyDown({ # 特殊キーの検知
        param($sender, $e)
        
        try {
            $cv = [int]$this.Text
            switch ($e.KeyCode) {
                'Up'   { $this.Text = ($cv + 1).ToString() }
                'Down' { $this.Text = ($cv - 1).ToString() }
            }
        }
        catch {

        }
    })
    $textBox.Add_TextChanged({ # 値が変わったときにコールされる
        try {
            $inputValue = [int]$this.Text
        }
        catch {
            $inputValue = [int]::MaxValue
        }
        $obj = GetObject $this

        if ($obj -ne $null) {
            $errorLabel = $($obj.label)
            $min = $obj.v.min
            $max = $obj.v.max
            #Write-Host "[obj] $this, $min, $max"
            if ($inputValue -gt $max -or $inputValue -lt $min) {
                $errorLabel.Text = [string]$min + "～" + [string]$max + "の数値を入力してください"
            }
            else {
                $errorLabel.Text = ""
            }
        }
    })
    $tab.Controls.Add($textBox)
    $tab.Controls.Add($errorLabel)
    $newobj = [PSCustomObject]@{ v=$v; myobj=$textBox; label=$errorLabel; }
    return $newobj
}

function ActivateOthers
{
    foreach ($key in $ObjList.Keys) {
        $obj = $ObjList[$Key]
        if ($obj.v.type -eq "onoff") {
            ActivateOther $obj
        }
    }
}

function ActivateOther
{
    param ($obj)
    if ($obj.v.PSObject.Properties['activate_with_on']) {
        foreach ($name in $obj.v.activate_with_on) {
            $target = GetObjectByName $name
            $target.myobj.Enabled = $obj.myobj.Checked
        }
    }
    if ($obj.v.PSObject.Properties['activate_with_off']) {
        foreach ($name in $obj.v.activate_with_off) {
            $target = GetObjectByName $name
            $target.myobj.Enabled = -not $obj.myobj.Checked
        }
    }
}

function CreateDate
{
    param ($v, $tab, $x, $y)

    $datePicker = New-Object System.Windows.Forms.DateTimePicker
    $datePicker.Format = [System.Windows.Forms.DateTimePickerFormat]::Short
    $datePicker.Location = New-Object System.Drawing.Point($x, $y)

    $tab.Controls.Add($datePicker)

    $newobj = [PSCustomObject]@{ v=$v; myobj=$datePicker; label=$null; }
    return $newobj
}

function CreateCheckBox
{
    param ($v, $tab, $x, $y)

    # チェックボックスの作成
    $checkBox = New-Object System.Windows.Forms.CheckBox
    $checkBox.Text = " "
    $checkBox.Location = New-Object System.Drawing.Point($x, $y)
    $checkBox.AutoSize = $true
    $checkBox.Add_CheckedChanged({
        $obj = GetObject $this 
        if ($obj -eq $null) {
            return
        }

        ActivateOther $obj
    })
    $tab.Controls.Add($checkBox)

    $newobj = [PSCustomObject]@{ v=$v; myobj=$checkBox; label=$null; }
    return $newobj
}

function FileDialog
{
    param ($func)

    # OpenFileDialog オブジェクトを作成
    $dialog = New-Object System.Windows.Forms.OpenFileDialog
    $dialog.InitialDirectory = [Environment]::GetFolderPath('Desktop')
    $dialog.Filter = "すべてのファイル (*.*)|*.*|テキストファイル (*.txt)|*.txt"
    
    if ($dialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
        if (Get-Command $func -CommandType Function -ErrorAction SilentlyContinue) {
            & $func $dialog.FileName
        }
    }
}

function IsValid
{
    param ($obj, $withDialog)

    $min = $obj.v.min
    $max = $obj.v.max

    $text = $obj.myobj.Text
    $len = $text.Length
    $name = $id -replace "^.*\.", ""
    Write-Host "<< $name, $text, $min, $max"

    if ($obj.v.type -eq "number") {
        $cv = [int]$text
        if (($len -eq 0) -or ($cv -gt $max) -or ($cv -lt $min)) {
            $msg = $name + "の値がダメです"
            if($withDialog) {
                $ret = [System.Windows.Forms.MessageBox]::Show($msg, "Error", [System.Windows.Forms.MessageBoxButtons]::OK)
            }
            return $false
        }
    }
    if ($obj.v.type -eq "string") {
        if (($len -gt $max) -or ($len -lt $min)) {
            $msg = $name + "の値がダメです"
            if($withDialog) {
                $ret = [System.Windows.Forms.MessageBox]::Show($msg, "Error", [System.Windows.Forms.MessageBoxButtons]::OK)
            }
            return $false
        }
        Write-Host "<< $text, $($obj.v.pattern)"
        if ($obj.v.pattern.Length -gt 0) {
            if ($text -match $obj.v.pattern) {
                Write-Host "## OK"
            }
            else {
                $msg = $name + $obj.v.errmsg
                if($withDialog) {
                    $ret = [System.Windows.Forms.MessageBox]::Show($msg, "Error", [System.Windows.Forms.MessageBoxButtons]::OK)
                }
                return $false
            }
        }
    }
    return $true
}

function ValidationOnClick
{
    param ($obj)

    foreach ($id in $obj.v.validation) {
        $chkobj = GetObjectByName $id
        if ((IsValid $chkobj $true) -eq $false) {
            return $false
        }
    }
    return $true
}

function CreateButton
{
    param ($menuName, $v, $tab, $x, $y)

    # ボタン
    $button = New-Object System.Windows.Forms.Button
    $button.Text = $v.name
    $button.Location = New-Object System.Drawing.Point($x,$y)

    $button.Add_Click({
        $obj = GetObject $this
        if ($obj -eq $null) {
            return
        }
        if ((ValidationOnClick $obj) -eq $false) {
            Write-Host "<< INVALID"
            return
        }

        $funcName = "do_" + $obj.v.name
        if (Get-Command $funcName -CommandType Function -ErrorAction SilentlyContinue) {
            & $funcName
        }
    })
    $tab.Controls.Add($button)

    $newobj = [PSCustomObject]@{ v=$v; myobj=$button; label=$null; }
    return $newobj
}

function CreateMenu
{
    param ($menuName, $v, $tab)
    $id = $menuName + "." + $($v.name)
    $x = $MenuX
    $y = $MenuY

    if ($v.type -eq "column") {
        $Global:MenuY = $MENU_Y0
        $Global:MenuX += $VALUE_WIDTH + $LABEL_WIDTH + $v.margin
        return
    }

    if ($v.type -eq "button") {
        $x += $LABEL_WIDTH
        $newobj = CreateButton $menuName $v $tab $x $y
        SetObject $id $newobj
    }
    else {
        # ラベル表示
        $label = New-Object System.Windows.Forms.Label
        $label.Text = $v.name
        $label.Location = New-Object System.Drawing.Point($x, ($y + 3))
        $label.AutoSize = $true
        $tab.Controls.Add($label)
        $x += $LABEL_WIDTH

        $newobj = $null
        if ($v.type -eq "choice") {
            $newobj = CreateComboBox $v $tab $x $y
        }
        if ($v.type -eq "number") {
            $newobj = CreateTextNum $v $tab $x $y
        }
        if ($v.type -eq "string") {
            $newobj = CreateTextStr $v $tab $x $y
        }
        if ($v.type -eq "date") {
            $newobj = CreateDate $v $tab $x $y
        }
        if ($v.type -eq "onoff") {
            $newobj = CreateCheckBox $v $tab $x $y
        }
        if ($newobj -ne $null) {
            SetObject $id $newobj
            if ($v.PSObject.Properties['readonly']) {
                $newobj.myobj.ReadOnly = $true
            }
        }
    }

    $Global:MenuY = $y + 50
}

function CreateForm
{
    param (
        $appName,
        $width,
        $height,
        [Parameter(ValueFromRemainingArguments = $true)]
        [object[]]$Files
    )

    try {
        $form = New-Object System.Windows.Forms.Form
        $form.Text = $appName
        $form.Width = $width
        $form.Height = $height

        $width -= 35
        $height -= 60
        $tabCtrl = New-Object System.Windows.Forms.TabControl
        $tabCtrl.Size = New-Object System.Drawing.Size($width, $height)
        $tabCtrl.Location = New-Object System.Drawing.Point(10, 10)

        $configObj = New-Object 'PSObject[]' $Files.Length
        $i = 0
        foreach ($fileName in $Files) { # 1file => 1tab
            $Global:MenuX = $MENU_X0
            $Global:MenuY = $MENU_Y0
            $configObj[$i] = Get-Content -Path $fileName -Raw | ConvertFrom-Json

            foreach ($menu in $configObj[$i].PSObject.Properties) {
                $tab = New-Object System.Windows.Forms.TabPage
                $tab.Text = $menu.Name

                foreach ($v in $($menu.Value)) {
                    CreateMenu $($menu.Name) $v $tab
                }
            }
            $tabCtrl.TabPages.Add($tab)
            $i++
        }

        InitObjList
        ActivateOthers
        $form.Controls.Add($tabCtrl)
    }
    catch {
        Write-Host "Error: $_"
        return
    }

    $form.ShowDialog() # main loop

    SaveConfig
}

function ConfirmDialog
{
    param ($msg, $title, $func)

    # OK/Cancel ダイアログを表示
    $result = [System.Windows.Forms.MessageBox]::Show($msg, $title, [System.Windows.Forms.MessageBoxButtons]::OKCancel)

    if ($result -eq [System.Windows.Forms.DialogResult]::OK) {
        if (Get-Command $func -CommandType Function -ErrorAction SilentlyContinue) {
            & $func
        }
    }
}

function ProgressStart
{
    # フォームの作成
    $form = New-Object System.Windows.Forms.Form
    $form.Text = "進行状況"
    $form.Size = New-Object System.Drawing.Size(300, 150)
    $form.StartPosition = "CenterScreen"
    
    # プログレスバーの作成
    $progressBar = New-Object System.Windows.Forms.ProgressBar
    $progressBar.Location = New-Object System.Drawing.Point(10, 40)
    $progressBar.Size = New-Object System.Drawing.Size(260, 20)
    $progressBar.Minimum = 0
    $progressBar.Maximum = 100
    $progressBar.Value = 0
    $form.Controls.Add($progressBar)
    $form.Show()

    return $form, $progressBar
}

function ProgressEnd
{
    param ($prog)
    $form = $prog[0]
    $progressBar = $prog[1]

    if ($progressBar.Value -lt 70) {
        $progressBar.Value = 70
        Start-Sleep -Milliseconds 50
    }
    if ($progressBar.Value -lt 85) {
        $progressBar.Value = 85
        Start-Sleep -Milliseconds 50
    }
    $progressBar.Value = 100
    Start-Sleep -Milliseconds 50
    $form.Close()
}

function ProgressUpdate
{
    param ($process, $file, $prog)

    while(-not $process.HasExited) {
        Start-Sleep -Milliseconds 200

        Select-String -Path $file -Pattern "PROG" | Foreach-Object {
            if ($_ -match "\[PROG\].(\d+)") {
                $num = [int]$($matches[1])

                if ($num -gt $prog.Value) {
                    $prog.Value = $num
                    Write-Host "=== $num"
                }
            }
        }
    }
}

function GenerateNewFile
{
    param($file)
    if (Test-Path $file) { Remove-Item $file }
    New-Item -Path $file -ItemType File
}

##############################################################

# 'do_' + ボタン名
function do_Setting
{
    ConfirmDialog "本当に設定しますか？" "確認" {
        Write-Host "--- OK ---"
        Write-Host (GetValueByName "menu1.value1")
        Write-Host (GetValueByName "menu1.value2")
        Write-Host (GetValueByName "menu1.value3")
        Write-Host (GetValueByName "menu1.value4")
        Write-Host (GetValueByName "menu1.value5")

        $file = "transfer.log"
        GenerateNewFile $file

        $prog = ProgressStart

        $process = Start-Process powershell.exe -ArgumentList "-ExecutionPolicy Bypass -File ./sleep.ps1" -PassThru -NoNewWindow
        ProgressUpdate $process $file $prog[1]

        ProgressEnd $prog
    }
}

function do_OpenFile
{
    FileDialog {
        param ($fileName)
        Write-Host "選択されたファイル: $fileName"
        $obj = GetObjectByName "menu1.value6"
        $obj.myobj.Text = $fileName
    }
}

CreateForm 'SampleForm' 650 600 "config.json" "config2.json"

