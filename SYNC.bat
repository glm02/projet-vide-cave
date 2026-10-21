@echo off
rem Double-clic : synchronise le projet KiCad avec GitHub (KiCad doit être fermé).
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0sync-kicad.ps1"
