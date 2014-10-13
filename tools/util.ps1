$excludeDirs = ".git", "third_party", "build"

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
            if ($includeItem)
            {
                Write-Output $item
            }
        }
    }
}
