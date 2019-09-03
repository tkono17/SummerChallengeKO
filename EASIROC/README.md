# SummerChallenge2019 で実際に作成、使用したDAQ コード等
- moving stage の自動化に伴って、実行ファイル"ConnectSiTCP.cc"のなかでmoving stage を動かすようにしたため、stage.py までの絶対パスが書かれている。
また、dat ファイルをroot ファイルに変換するのも一括でできるようにしてある。
今後このコードを使うようであれば、stage.py までのパスが正しいかとか、変換後のroot ファイルが置かれる場所が望んでいる位置にあるかとか確認してね。

## コンテンツ
### Data
測定データがroot ファイルとして放り投げられる場所。中身の容量が重くてGit には上げてないけどここにあった。中身が欲しい人は谷口まで。
ディレクトリ"easiroc_autoOP" の中にあるfullcheck.cc を実行すると、Data ディレクトリの中に新しく測定データ群用のディレクトリが作成され、そこに一連のroot ファイルが作成される。
### easiroc
easiroc自動化に伴ってConnectSiTCP.cc を直接いじる方針になったので、バックアップ用のもとのコードを残させた。それがこれ。
### easiroc_autoOP
この中に実際に自動化させたコードが入ってる。
#### easiroc/
自動化させた方での実行の仕方は、例えばtest_i.dat, step 幅(stage が測定毎に動く幅) 3mm, move 幅(全体で動く幅) 70mm のとき : 
- "6.StartDAQ" で測定開始させた後、"Input output file name : "と聞かれるのでtest_ と入力 
- 次に "Input step width[mm] : " と聞かれるので3 を入力
- んで"Input move width[mm] : " と聞かれるので70 を入力
- 次に"Input directory name : " .dat ファイルが入るディレクトリの名前を入力
- 最後にevent 数を入力
もうお気づきだろうが、何個の.dat ファイルを作りたいとかは幅とstep 間隔で決まる。step 幅をやたら細かくするととんでもない量のファイルができるので注意。
測定開始後のeasiroc, movingstage の挙動は、
1. movingstage 固有の原点に戻る(stage 可動域の端から数mm 程度中心よりの点っぽい)
2. スリットの中心から左右に等しい領域で測定するために、スリット中心から今回の場合35mm 離れたところまでデータを取らずに移動
3. 測定開始~ 終了
4. 指定されたステップ幅で(今回の場合3mm)動く
3~ 4 を繰り返す。って感じ。
2 の工程は大したもんで、ConnectSiTCP.cc の中に"int start_point = 30909 - 250* move_width" という行があって、この数字をちゃんと考えて設定してた。素晴らしい。
### /easiroc/fullcheck.cc
.dat から.root へ一括変換してくれる素晴らしいやつ。 使う方法は
1. root -l
2. .L fullcheck6.C
3. .L fullcheck.cc
4. fullcheck("directory名", "ファイル名(今回の場合 test_ )", "ファイルの数");

### /./easiroc_TA
この辺は谷口がDAQ 班がコードを書くために試しに自分でコードを書いてみたものが入っているだけ
特に大したものではないので見なくていいです。


