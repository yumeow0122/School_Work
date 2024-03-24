# NLP-LAB-HW

這個專案為NCHU NLP LAB面試之作業。


## 使用方法(Docker)
1. 建立image
    ```
    sudo bash init.sh
    ```

2. 執行
    ```
    sudo bash run.sh
    ```

    
## 使用方法(一般環境)

1. 安裝必要的套件：
    ```
    pip install -r requirements.txt
    ```

2. 設定環境變數（如果有必要）：
    ```
    # 可以在 .env 檔案中設定環境變數
    touch .env
    ```

3. 執行程式：
    ```
    python main.py
    ```

## 注意事項

- 請確保已安裝 Python 和 pip。
- 請確保已安裝 Docker，如果使用 Docker 執行的話。
- 請確保網路連接正常，以便能夠正確爬取數據。