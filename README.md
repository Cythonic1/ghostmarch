# ghostty omarchy theme switcher or "ghostmarch".

## Description.
Since i love omarchy and i love ghostty its hard to make ghostty work due to some limitation from ghostty itself
such it does not allow import theming not singal to update the config or reload it. 

So i decided to make a simpe program that will work every time you change theme the program shall be added into the 
`~/.local/share/omarchy/bin/omarchy-theme-set` after the theme is applied.


## Usage and compiling.
### clone the repo.


```bash

git clone git@github.com:Cythonic1/ghostmarch.git
cd ghostmarch
```

### install
Compiling is very simple just run `sudo make install` to install it system wide. Or `make` to just install
in the current working directory.



## Limitation. 
Unformtionaly its very hard to make this fully automated as i said ghostty does not support that. So after switching theme
all you have to do it to press .

`Press Ctrl+Shift+, (comma).`    

And the theme shall be applied.

## Customization.
in the header file it include the path for the config file. Also if your ghostty config is not in the default place you can 
easly modified it in the code and i will make that better by adding it into the header for easier and experince.

## Contributions. 
Eveybody is welcome to change and help imporve this code even better and thank you before hand.♥️

