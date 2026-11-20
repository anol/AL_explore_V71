@echo off
:: Regenerate the graphs in doc\ from the sources (see README.md in this directory).
python "%~dp0update_graphs.py" %*
