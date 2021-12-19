@echo off

for %%x in (%*) do (
    if exist "%%~x" (
        ::echo Delete dir: %%~x
        rmdir /s /q "%%~x"
    )
)
