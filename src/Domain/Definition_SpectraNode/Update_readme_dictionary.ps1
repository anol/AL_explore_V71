<#
.SYNOPSIS
    Splices the generated SpectraNode command/keyword/structure Markdown tables into the
    top-level README.md, between matching <!-- BEGIN:xxx --> / <!-- END:xxx --> markers.

.DESCRIPTION
    Run automatically by SpectraNode_generator.bat after it regenerates
    src/Domain/Generated_code/SpectraNode_{command,keyword,structure}.md, so the copies
    embedded in README.md's "SpectraNode command & keyword dictionary" section never go
    stale. Never edit the text between the markers in README.md directly -- it is
    overwritten the next time this script runs.
#>

$ErrorActionPreference = "Stop"

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..\..")
$ReadmePath = Join-Path $RepoRoot "README.md"
$GeneratedDir = Join-Path $RepoRoot "src\Domain\Generated_code"

# Strips the leading HTML-comment banner (copyright + "generated, do not edit" notice),
# the top-level "# ..." title line and, when present, the "Date: ..." line, leaving just
# the body (headings/tables) that belongs inside the README's <details> block.
function Get-DictionaryBody {
    param(
        [Parameter(Mandatory)] [string] $Path,
        [switch] $HasDateLine
    )

    $lines = Get-Content -Path $Path -Encoding UTF8
    $i = 0

    if ($i -lt $lines.Count -and $lines[$i].Trim() -eq "<!--") {
        while ($i -lt $lines.Count -and $lines[$i].Trim() -ne "-->") { $i++ }
        $i++ # skip the closing "-->" line itself
    }
    while ($i -lt $lines.Count -and $lines[$i].Trim() -eq "") { $i++ }
    if ($i -lt $lines.Count -and $lines[$i] -match '^# ') { $i++ }
    while ($i -lt $lines.Count -and $lines[$i].Trim() -eq "") { $i++ }
    if ($HasDateLine -and $i -lt $lines.Count -and $lines[$i] -match '^Date: ') { $i++ }
    while ($i -lt $lines.Count -and $lines[$i].Trim() -eq "") { $i++ }

    # Drop trailing blank lines.
    $end = $lines.Count - 1
    while ($end -ge $i -and $lines[$end].Trim() -eq "") { $end-- }

    if ($end -lt $i) { return @() }
    return $lines[$i..$end]
}

# Nests "## Provider: X" one level deeper, since inside the README this content sits
# under the <details><summary>Commands ...</summary> pseudo-heading rather than at the
# top of its own document.
function Set-NestedHeadings {
    param([string[]] $Lines)
    return $Lines | ForEach-Object {
        if ($_ -match '^## ') { "##$_" } else { $_ }
    }
}

$commandBody = Set-NestedHeadings (Get-DictionaryBody -Path (Join-Path $GeneratedDir "SpectraNode_command.md") -HasDateLine)
$keywordBody = Get-DictionaryBody -Path (Join-Path $GeneratedDir "SpectraNode_keyword.md") -HasDateLine
$structureBody = Get-DictionaryBody -Path (Join-Path $GeneratedDir "SpectraNode_structure.md")

# The structure table currently has no data rows (its source XML has the definition
# commented out) -- note that explicitly rather than leaving a bare, unexplained header.
# (The header row and its "|---|---|" separator row both match '^\|' too, hence -gt 2.)
$structureHasRows = ($structureBody | Where-Object { $_ -match '^\|' }).Count -gt 2
if (-not $structureHasRows) {
    $structureBody = $structureBody + @(
        "",
        "_(Empty $([char]0x2014) the structure definition is currently commented out in its source XML and isn't",
        "part of the build.)_"
    )
}

function Set-Section {
    param(
        [string] $Text,
        [string] $Marker,
        [string[]] $Body
    )
    $beginTag = "<!-- BEGIN:$Marker -->"
    $endTag = "<!-- END:$Marker -->"
    $beginIdx = $Text.IndexOf($beginTag)
    $endIdx = $Text.IndexOf($endTag)
    if ($beginIdx -lt 0 -or $endIdx -lt 0) {
        throw "Could not find $beginTag / $endTag markers in README.md"
    }
    $before = $Text.Substring(0, $beginIdx + $beginTag.Length)
    $after = $Text.Substring($endIdx)
    $bodyText = ($Body -join "`n")
    return "$before`n$bodyText`n$after"
}

$readme = Get-Content -Path $ReadmePath -Raw -Encoding UTF8
$readme = Set-Section -Text $readme -Marker "SpectraNode_command" -Body $commandBody
$readme = Set-Section -Text $readme -Marker "SpectraNode_keyword" -Body $keywordBody
$readme = Set-Section -Text $readme -Marker "SpectraNode_structure" -Body $structureBody

Set-Content -Path $ReadmePath -Value $readme -Encoding UTF8 -NoNewline
Write-Host "Updated the SpectraNode command/keyword/structure tables in README.md"
