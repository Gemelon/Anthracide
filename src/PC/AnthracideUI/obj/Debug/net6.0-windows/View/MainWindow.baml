   M S B A M L   `   `   `  ????G  CAnthracideUI, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null+clr-namespace:AnthracideUIAnthracideUI  5   	   ?$clr-namespace:AnthracideUI.ViewModelAnthracideUI.ViewModel  5   	   7 clr-namespace:AnthracideUI.ModelAnthracideUI.Model  5   	       AnthracideUI.MainWindow   5   	   W SPresentationCore, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   ,localclr-namespace:AnthracideUI     5   	   :	ViewModel$clr-namespace:AnthracideUI.ViewModel     5   	   2Model clr-namespace:AnthracideUI.Model     5   	   -   5        $	??800??5
      $	??790??6'     ? WindowStyle$
  None=?63   ???	#FF252525 ? SizeToContent$ WidthAndHeight=?6?    ? 	Resources 5       ?? MergedDictionaries ?? Source?? 5   
    5      ?? 5      $" /Resources/SlyderStyles.xaml??6%   ?? 5      $# /Resources/DiverseStyles.xaml??6%   ?? 5      $" /Resources/ButtonStyles.xaml??6%   %    5       ? DataContext 5      0  *AnthracideUI.ViewModel.FrontpanelViewModel  5   
   .??5      ? -   5         ?Name$ MainGrid??5      $??5,5,5,5q?6   ??5   
   ?? 5       ?? ImageSource$ /Images/Background.png??6    ? RowDefinitions 5   
   ?? 5       $??40 ?6   ?? 5!      $??* ?6   ?? 5"      $??20 ?6   ?? 5#      $??5 ?6   .+?5&   
   ? -   5        $ TopGrid??5&      .+?5'      ? $??5,10,5,0q?6   $??3??6%   $??30??65   	 ? ColumnDefinitions	 5(      ?? 5)      $??30 ?6'   ?? 5*      $	??350 ?6'   ?? 5+      $	??130 ?6'   ?? 5,      $	??100 ?6'   ?? 5-      $??* ?6'   ?? 5.      $??30 ?6'   ?? 5/      $??30 ?6'   ?? 50      $??30 ?6'   .+?52      ?? $??20??6   $??/Images/Bolt.png??6$   $
??Fill=?6>   $??20??6M   $	??Top=?6X   ?? 53      $&?? /Images/Anthracide Power Amp.png??6   $??29??6B   $??1??6N   $	??Top=?6^   $
??Left=?6v   $??30,0,0,0q?6?   ?? 54      -   5        
 ??Name$
 IPAddressTextBox??54      $??2??64   $??Arial)?6f   $??16(?6y   $??Center=?6?   
???????$??30??6?   ??6D   ?? *IPAddressTextBox+??6?   Z? 6?   ?? 55      -   5        $
 PortTextBox??55      $??3??6/   
???????$??50??6h   $??16(?6v   $??Arial)?6?   $??30??6?   $??Center=?6?   ??6R   Z? ??6?   ?? *PortTextBox+6?   ?? 56         ExitButtonStyle#???   6   $??5??6C   $??25??6s   $??25??6~   $	??Top=?6?   ??6S   ?? *ExitCommand+??57      ?? 58      $ /Images/Anthracide_X.png??6%   6N   5:      ?? 5;      $??20??6   $??/Images/Bolt.png??6$   $
??Fill=?6>   $??20??6M   $??7??6X   $??3??6h   $	??Top=?6|   5<      5=      ? 5?   
   -   5        $ ContentGrid??5?      $??1??6$   $??	10,5,0,18q?61    5@      ?? 5A      $	??440 ?6    ?? 5B      $	??100 ?6    ?? 5C      $??* ?6    .+?5F      ? $??0,5,0,20q?6   .+?5H      ?? $??7,7,7,7q?6    ?? CornerRadius$ 20,20,20,20??63   $??0??6N   $
??744 ??6[   $
??Left=?6h   $??9,0,0,0q?6?   ???	#FF6363636?   ?? 5I      $??5,5,5,5q?6   $ 20,20,20,20??63   $??0??6N   $
??742 ??6[   $
??Left=?6h    J? Opacity$	 0.5Q?6?   $??10,0,0,0q?6?   
???   ?$	??413??6?   ? 5K      	 5M      ?? 5N      $	??610 ?6+   ?? 5O      $	??110 ?6+   ?? 5P      $??* ?6+   .+?5S      ? -   5        $ EqualizerGrid??5S      $??20,0,0,0q?62   .+?5T      ??  ?? Orientation$ 
Horizontal=?6%   $??5,15,0,0q?6>   .+?5U      ?? .+?5V   "   ?? $
??10dB??6,   
???????$??Right=?6K   $??0,22,5,0q?6g   $??Arial Rounded MT Bold)?6y   ?? 5W   "   $??0??6,   
???????$??Right=?6H   $??	0,130,5,0q?6d   $??Arial Rounded MT Bold)?6w   ?? 5X   "   $??-10dB??6,   
???????$??Right=?6L   $??	0,132,5,0q?6h   $??Arial Rounded MT Bold)?6{   5Y      ?? 5Z      $??50??6)   .+?5[   "   ?? -   5         ??Name$ Equalizer_45Hz_Slider??5[   X     EqualizerSlyderStyle#???  6)   $	??350??6w    ? Minimum$	 -10Q?6?    ?? TickFrequency$ 2Q?6?    ?? TickPlacement$
 Both=?6?   $??Center=?6?    ?? Orientation$ Vertical=?6?    ? Value 6?   ?? *Equalizer_45Hz+?? 5\   "   $
??32Hz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5]      ?? 5^      $??50??6)   .+?5_   "   ?? -	   5        $ Equalizer_64Hz_Slider??5_   X   #???  6)   $ Vertical=?6w   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_64Hz+?? 5`   "   $
??64Hz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5a      ?? 5b      $??50??6)   .+?5c   "   ?? -
   5        $ Equalizer_125Hz_Slider??5c   X   #???  6)   $ Vertical=?6x   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_125Hz+?? 5d   "   $??125Hz??6,   
???????$??Center=?6L   $??0,10,0,0q?6i   $??Arial Rounded MT Bold)?6{   5e      ?? 5f      $??50??6)   .+?5g   "   ?? -   5        $ Equalizer_250Hz_Slider??5g   X   #???  6)   $ Vertical=?6x   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_250Hz+?? 5h   "   $??250Hz??6,   
???????$??Center=?6L   $??0,10,0,0q?6i   $??Arial Rounded MT Bold)?6{   5i      ?? 5j      $??50??6)   .+?5k   "   ?? -   5        $ Equalizer_500Hz_Slider??5k   X   #???  6)   $ Vertical=?6x   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_500Hz+?? 5l   "   $??500Hz??6,   
???????$??Center=?6L   $??0,10,0,0q?6i   $??Arial Rounded MT Bold)?6{   5m      ?? 5n      $??50??6)   .+?5o   "   ?? -   5        $ Equalizer_1KHz_Slider??5o   X   #???  6)   $ Vertical=?6w   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_1KHz+?? 5p   "   $
??1KHz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5q      ?? 5r      $??50??6)   .+?5s   "   ?? -   5        $ Equalizer_2KHz_Slider??5s   X   #???  6)   $ Vertical=?6w   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_2KHz+?? 5t   "   $
??2KHz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5u      ?? 5v      $??50??6)   .+?5w   "   ?? -   5        $ Equalizer_4KHz_Slider??5w   X   #???  6)   $ Vertical=?6w   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_4KHz+?? 5x   "   $
??4KHz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5y      ?? 5z      $??50??6)   .+?5{   "   ?? -   5        $ Equalizer_8KHz_Slider??5{   X   #???  6)   $ Vertical=?6w   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_8KHz+?? 5|   "   $
??8KHz??6,   
???????$??Center=?6K   $??0,10,0,0q?6h   $??Arial Rounded MT Bold)?6z   5}      ?? 5~      $??50??6)   .+?5   "   ?? -   5        $ Equalizer_16KHz_Slider??5   X   #???  6)   $ Vertical=?6x   $	??350??6?   $	 -10Q?6?   $ 2Q?6?   $
 Both=?6?   $??Center=?6?    6?   ?? *Equalizer_16KHz+?? 5?   "   $??16KHz??6,   
???????$??Center=?6L   $??0,10,0,0q?6i   $??Arial Rounded MT Bold)?6{   5?      ?? 5?      .+?5?   "   ?? $
??10dB??6,   
???????$
??Left=?6K   $??5,22,0,0q?6f   $??Arial Rounded MT Bold)?6x   ?? 5?   "   $??0??6,   
???????$
??Left=?6H   $??	5,130,0,0q?6c   $??Arial Rounded MT Bold)?6v   ?? 5?   "   $??-10dB??6,   
???????$
??Left=?6L   $??	5,132,0,0q?6g   $??Arial Rounded MT Bold)?6z   5?      5?      5?      ? 5?      -   5        $ 
VolumeGrid??5?      $??1??6/   $??0,15,0,0q?6?   $??Right=?6Q   	 5?      ?? 5?      $??40 ?6/   ?? 5?      $??* ?6/   .+?5?      ?? $??Right=?6%   .+?5?      ?? $	??0dB??6(   
???????$??Right=?6F   $??5,22,0,0q?6b   $??Arial Rounded MT Bold)?6t   ?? 5?      $??-10dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-20dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-30dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-40dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-50dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-60dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-70dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   ?? 5?      $??-80dB??6(   
???????$??Right=?6H   $??5,22,0,0q?6d   $??Arial Rounded MT Bold)?6v   5?      ?? 5?      $??Right=?6%   $??1??6A   $??3,0,0,0q?6Q   .+?5?      ?? -   5        $ 
MainVolume??5?   T   #???  6%    ? Maximum$ 0Q?6h   $	 -80Q?6t   $ Vertical=?6?   $	??350??6?   $??Center=?6?   $ 10Q?6?   $
 Both=?6?    6?   ?? *
MainVolume+6?   ?? 5?      $??Volume??6(   
???????$??Center=?6I   $??0,10,0,0q?6f   $??Arial Rounded MT Bold)?6x   5?      5?      5?      5?      ? 5?      $??1??6   .+?5?      ?? $??7,7,7,7q?6   $ 20,20,20,20??63   $??0??6N   $
??744 ??6[   $
??Left=?6h   $??9,0,0,0q?6?   ???	#FF6363636?   ?? 5?      $??5,5,5,5q?6   $ 20,20,20,20??63   $??0??6N   $
??742 ??6[   $
??Left=?6h   $	 0.5Q?6?   $??10,0,0,0q?6?   
???   ?$??99    ??6?   ? 5?      $??20,0,0,0q?6   	 5?      ?? 5?      $	??140 ?6+   ?? 5?      $	??150 ?6+   ?? 5?      $	??150 ?6+   ?? 5?      $	??150 ?6+   ?? 5?      $	??150 ?6+   .+?5?      ?? $
??Gain??6    
???????$??Center=?6?   $??0,5,0,0q?6\   $??Center=?6m   $??Arial Rounded MT Bold)?6?   $??16(?6?   ?? 5?      $??0,30,0,0q?6!   $??1??63   .+?5?      ?? #???  6!   $??30??6P   $	??140??6\   $??Center=?6h   $??Center=?6?   $	 -10Q?6?   $ TopLeft=?6?   $ 5Q?6?   $??Center=?6?   ?? 5?      $	??AUX??6$   
???????$??Center=?6B   $??0,5,0,0q?6_   $??Arial Rounded MT Bold)?6p   5?      ?? 5?      $??0,30,0,0q?6!   $??2??63   .+?5?      ?? #???  6!   $??30??6P   $	??140??6\   $??Center=?6h   $
??Left=?6?   $	 -10Q?6?   $ TopLeft=?6?   $ 5Q?6?   $??1??6?   ?? 5?      $??Digital??6$   
???????$??Center=?6F   $??0,5,0,0q?6c   $??Arial Rounded MT Bold)?6t   5?      ?? 5?      $??0,30,0,0q?6!   $??3??63   .+?5?      ?? #???  6!   $??30??6P   $	??140??6\   $??Center=?6h   $
??Left=?6?   $	 -10Q?6?   $ TopLeft=?6?   $ 5Q?6?   $??2??6?   ?? 5?      $??	Bluetooth??6$   
???????$??Center=?6H   $??0,5,0,0q?6e   $??Arial Rounded MT Bold)?6v   5?      5?      5?      5?      ? 5?   
   -   5        $ 
BottomGrid??5?      $??2??6#    J? IsManipulationEnabled$
 True??60   	 5?      ?? 5?      $??30 ?6#   ?? 5?      $??* ?6#   ?? 5?      $??30 ?6#   .+?5?      ?? $??20??6   $??/Images/Bolt.png??6    $
??Fill=?6:   $??20??6I   ?? 5?      -   5        $
 StatusTextBox??5?      $??1??6-   $??	10,0,10,0q?6=   ???	#FF272727
??????????	#FF272727??6P   ?? *StatusTextBox+6?   ?? 5?      $??20??6   $??/Images/Bolt.png??6    $
??Fill=?6:   $??20??6I   $??2??6T   $??3??6d   5?      5?      5?      