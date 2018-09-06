# Gravity Desktop  

### 動作画面  
[https://www.youtube.com/watch?v=ogNGlpY-RSw](https://www.youtube.com/watch?v=ogNGlpY-RSw)  

### コンパイル済みファイルのダウンロード  
[Gravity Desktop v1.0](https://github.com/wakewakame/GravityDesktop/releases)  

### 動作条件
OS:Windows 10 64bit  

### 使用方法
premake5を[https://premake.github.io](https://premake.github.io)からダウンロード  
ダウンロードしたpremake5.exeをGravityDesktop\Box2D\Box2Dにコピー  
コマンドプロンプトでGravityDesktop\Box2D\Box2Dで"premake5.exe vs2017"と入力し実行  
GravityDesktop.slnを開く  
構成をRelease x64に変更  
mainプロジェクトを右クリックし、"スタートアッププロジェクトに設定"をクリック  
F5キーで実行  
Escで終了  

### 未実装機能  
アイコンが重複していた際、起動時中のみ自動的にアイコン整列  
エラー発生時の原因表示  

## 改善点  
フレームレートを1フレームの経過時間で取得するのではなく100フレーム程度の経過時間から算出する  
(フレームが急に変化したときにBox2Dの挙動が少しおかしくなる為)  

### 使用ライブラリ  
Box2D:[https://github.com/erincatto/Box2D](https://github.com/erincatto/Box2D)  
DirectX Tool Kit:[https://github.com/Microsoft/DirectXTK](https://github.com/Microsoft/DirectXTK)  