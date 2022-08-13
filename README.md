# Reflexes-Game

これはc言語で書かれた反応速度測定ゲームとなっています
ncursesライブラリを用いてウィンドウを表示したり、マウス操作を可能にしています。

動作環境は
OS: windows10 <br>
Terminal: MSYS2
です。

実際にゲーム開始をするとまだ不具合が出ることがあるためまだ未完成...

プログラム内には
``` c
#include <locale.h>
setlocale(LC_ALL, "");
```
をインクルードしているが、現在日本語表記している部分は一つもない

メニュー選択画面ではマウスのカーソルを選択しに合わせて、マウスの左ボタンを押すことにより遷移可能。

メニュー選択画面は、"PLAY GAME","SHOW RANKING","OPTION","DESCRIPTION","QUIT"の計５つの選択肢から構成されている。

![](./img/2022-08-13-13-17-50.png)

各オプション内はqキーを押すことによりメニュー選択画面へ戻れるようになっている。