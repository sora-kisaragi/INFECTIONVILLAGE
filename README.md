# INFECTIONVILLAGE

学生時代に制作した C++ の 3D ゾンビシューティングゲームです。OpenGL で描画し、OpenAL でサウンドを再生します。

## 目的

- ソースコード公開・レビュー用の保管
- ビルド/実行に必要な情報の共有

## リポジトリ構成

- `SourceProgram/`
  - Visual Studio ソリューションとソースコード
  - `SourceProgram/Src/` にゲーム本体の C++ 実装
- `Res/`（`SourceProgram/Res/`）
  - モデル/テクスチャ/シェーダーなどのゲーム素材
- `IV_ver2.0/`
  - 当時の配布物や関連データ（必要に応じて参照）

## 必要環境

- Windows 10 以上
- Visual Studio（ソリューション: `SourceProgram/INFECTIONVILLAGE.sln`）
- OpenAL SDK

> メモ: 依存ライブラリは `SourceProgram/packages/` と `packages.config` に含まれています。

## ビルド手順（Windows）

1. `SourceProgram/INFECTIONVILLAGE.sln` を Visual Studio で開きます。
2. ソリューション構成を `Release` もしくは `Debug` に設定します。
3. 必要に応じて OpenAL SDK をインストールします。
4. ビルドを実行します。

## 実行方法

- ビルド後に生成される `INFECTIONVILLAGE.exe` を実行します。
- 実行時に `SourceProgram/Res/` が参照できる配置で起動してください。

## 主な操作

- 移動: WASD
- 走る: Left Shift
- 攻撃: 左クリック / Space
- リロード: R
- 武器切替: Left Ctrl
- 視点切替: M

詳細は `README.old.md` に記載しています。

## 開発ドキュメント

- 貢献ガイド: `CONTRIBUTING.md`
- 行動規範: `CODE_OF_CONDUCT.md`
- セキュリティ報告: `SECURITY.md`
- 変更履歴: `CHANGELOG.md`

## ライセンス

商用利用不可・改変可能・再配布不可です。詳細は `LICENSE` を参照してください。

## 作者

- 如月優空

## 旧ドキュメント

- 旧 README は `README.old.md` に保管しています。
