# TycheFrontend
A frontend to the Tyche Package

This Qt application is designed to be deployed to a website (as a WASM app) to interface to a running tyche endpoint. 

## To Use

1. Ensure you have a working install of the tyche JSON RPC endpoint
    1. Head over to `https://github.com/NREL/tyche`
    1. Download and install from the GUI_dev branch
    1. Run the `tyche/src/tyche-gui
    /server.py` script to activate the endpoint
1. Run the frontend
    1. Download a release package from `https://github.com/nicholasbl/TycheFrontend`. 
    1. Unpack, and run the `serve_wasm.py` script
    1. Open a browser to the address the script mentions
    

## Notes and Bugs

Currently the serve script for the frontend is NOT OPTIMAL and is super super slow for some reason! Just wait a bit and it will work.

For this dev release we are disabling all CORS! This is not secure for production!
