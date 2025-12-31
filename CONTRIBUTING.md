# CONTRIBUTING

INFECTIONVILLAGE への貢献ありがとうございます。以下のルールを守ってください。

## 受け入れる変更

- バグ修正
- バグ報告
- 機能追加

> いずれも Pull Request（PR）での提案を前提とします。

## 連絡手段

- Issue もしくは Pull Request を使用してください。

## 開発環境・コーディング規約

- **Microsoft C++ Guidelines** を採用します。
- Visual Studio の C++ 標準設定に合わせます。
- フォーマッタによる自動整形を **必須** とします。

### ルール概要（環境で自動整形ができない場合のために明文化）

- **インデント**: スペース 4
- **波括弧**: 1TBS（制御文の `{` は同じ行、`}` は次の行）
- **行幅**: 100〜120 文字を目安
- **命名規則**:
  - クラス/構造体/列挙型: `PascalCase`
  - 関数/メソッド: `camelCase`
  - 変数/メンバ: `lower_snake_case` または `camelCase`（ファイル内で統一）
  - 定数/マクロ: `UPPER_SNAKE_CASE`
- **ヘッダー**: include guard もしくは `#pragma once` を使用
- **using namespace**: ヘッダー内は禁止、CPP 内で限定使用
- **nullptr**: `NULL` ではなく `nullptr` を使用
- **new/delete**: 可能な範囲で `std::unique_ptr` / `std::shared_ptr` を優先

### 自動フォーマッタ / リンター（必須導入）

自動整形ができる環境の導入を **必須** とします。

**Visual Studio 2019 以降**
- **clang-format** を導入し、保存時 or Format Document で整形
- **MSVC Code Analysis (/analyze)** を推奨
- **clang-tidy** を使用できる場合は推奨

**Visual Studio Code**
- `xaver.clang-format`（clang-format: 自動整形）
- `llvm-vs-code-extensions.vscode-clangd`（clangd: 診断/補完）
- `ms-vscode.cpptools`（C/C++ 拡張）

> フォーマット設定は `clang-format` の標準スタイル（LLVM など）を基本とし、
> プロジェクト側に設定ファイルがある場合はそれに従ってください。

## テスト・エビデンス（必須）

PR には **必ず** 以下のエビデンスを添付してください。

- 実行や動作確認の結果
- スクリーンショット、動画、またはビルド結果のログ

例:
- 実行結果のスクリーンショット
- ビルド成功ログ
- 動画リンク

## PR テンプレート（推奨）

- 変更内容
- テスト/確認結果（コマンドや結果）
- エビデンス（画像/動画/ログへのリンク）

以上、よろしくお願いします。
