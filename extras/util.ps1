$excludeDirs = ".git", "third_party", "build"
$excludeFiles = ,"*.sublime-workspace"

function Get-SourceFiles ($path = $pwd, $includes = (,"*"))
{
    foreach ($item in Get-ChildItem $path)
    {
        if ($item.PSIsContainer)
        {
            if ($excludeDirs -notcontains $item.Name)
            {
                Get-SourceFiles $item.FullName $includes
            }
        }
        else
        {
            $includeItem = $false

            foreach ($includePattern in $includes)
            {
                if ($item.Name -like $includePattern)
                {
                    $includeItem = $true
                    break
                }
            }

            foreach ($excludeFilePattern in $excludeFiles)
            {
                if ($item.Name -like $excludeFilePattern)
                {
                    $includeItem = $false
                    break
                }
            }

            if ($includeItem)
            {
                Write-Output $item
            }
        }
    }
}
