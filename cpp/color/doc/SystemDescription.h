/** @file SystemDescription.h

    ## 概要
    標準入力から得たデータに対して色付けなどの装飾を行う\n
    装飾パターンはコマンドラインで複数指定できる

    @ref Color::ItemIF "ItemIF 階層図"
    - 通常の文字列　(@ref Color::Normal "Normal")
    - 取り消し線　　(@ref Color::StrikeThrough "StrikeThrough")  
    - 下線　　　　　(@ref Color::Underline "Underline")
    - 装飾IF　　　　(@ref Color::DecorateItem "DecorateItem")
      - 太字　　　　　　　　　(@ref Color::Bold "Bold")
      - 文字色変更　　　　　　(@ref Color::ForegroundColor "ForegroundColor")
      - 背景色変更　　　　　　(@ref Color::BackgroundColor "BackgroundColor")
      - 背景と文字を入れ替え　(@ref Color::SwapColor "SwapColor")
      - 点滅　　　　　　　　　(@ref Color::Blink "Blink")

    @note
      基本パターンとして、通常文字列 or 取り消し線 or 下線がある\n
      これらに対して装飾を追加していく\n
      - (下線　　　　 +　{文字色変更 + 太字})
      - (通常の文字列 +　{文字色変更 + 背景入れ替え + 点滅})
    
    <br>
    ## デコレーター 生成方法
    -# ItemIFを継承した基本パターンをnew()する
    -# 上記のObjectをDecoratedItemのnew()に渡す
      - これを何度も繰り返す
    -# 一番大外のオブジェクトを使って処理を行う

    @code {.cpp}
    ItemIF *obj = new BackgroundColor("green",      // DecorateItem
                      new ForegroundColor("white",  // DecorateItem
                          new Normal()              // ItemIF
                      )
                  );

    // これで各オブジェクトのPrepare()が芋づる式に呼ばれる
    obj->Prepare("Hoge");
    @endcode

    <br>
    ## 特徴
    - 各デコレーターで、Prepare()を呼ばないとインスタンスの連鎖が途切れてしまう
      - 必ず呼ぶのが慣例となる
    - 「どの装飾を実施するか」を都度判定しないかわりに、インスタンス生成時に構造を確定させる
      - if(fg_color) { ... }
      - if(bold)  { ... }
      - if(blink) { ... } のようなコードにならない
    - オブジェクトは連結順序に依存しないことが求められる

    @attention
      ただし、インスタンス生成コードをベタ書きすると逆に柔軟性がなくなってしまう\n
      多様なパターンに対応できるように、なるべくループで回せるようにしたい\n
      スクリプト言語であれば実行時コード評価を使うのもあり\n
      c++の場合は、生成したインスタンスの解放についてもケアが必要
*/

