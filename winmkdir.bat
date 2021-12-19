@echo off

for %%x in (%*) do (
    if not exist "%%~x" (
        ::echo Create dir: %%~x
        mkdir "%%~x"
    )
)
