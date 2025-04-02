import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import os

# 创建主窗口
root = tk.Tk()
root.title("C801 EVSE USB FW packing tool")

# 设置窗口大小和位置
window_width = 400
window_height = 300
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
x = (screen_width - window_width) // 2
y = (screen_height - window_height) // 2
root.geometry(f"{window_width}x{window_height}+{x}+{y}")

# 标题标签
title_label = tk.Label(root, text="C801 EVSE USB FW packing tool", font=("Arial", 12))
title_label.pack(pady=10)

# CBS FW 下拉框
cbs_fw_label = tk.Label(root, text="CBS FW:")
cbs_fw_label.pack(anchor=tk.W, padx=20)
cbs_fw_var = tk.StringVar()
cbs_fw_combobox = ttk.Combobox(root, textvariable=cbs_fw_var, values=["Option 1", "Option 2", "Option 3"])
cbs_fw_combobox.pack(anchor=tk.W, padx=20)

# HMI FW 下拉框
hmi_fw_label = tk.Label(root, text="HMI FW:")
hmi_fw_label.pack(anchor=tk.W, padx=20)
hmi_fw_var = tk.StringVar()
hmi_fw_combobox = ttk.Combobox(root, textvariable=hmi_fw_var, values=["Option A", "Option B", "Option C"])
hmi_fw_combobox.pack(anchor=tk.W, padx=20)

# CB FW 下拉框
cb_fw_label = tk.Label(root, text="CB FW:")
cb_fw_label.pack(anchor=tk.W, padx=20)
cb_fw_var = tk.StringVar()
cb_fw_combobox = ttk.Combobox(root, textvariable=cb_fw_var, values=["Option X", "Option Y", "Option Z"])
cb_fw_combobox.pack(anchor=tk.W, padx=20)

# EVSE version 输入框
evse_version_label = tk.Label(root, text="EVSE version:")
evse_version_label.pack(anchor=tk.W, padx=20)
evse_version_entry = tk.Entry(root)
evse_version_entry.pack(anchor=tk.W, padx=20)

# 定义生成配置文件的函数
def generate_config():
    # 获取输入值
    cbs_fw_value = cbs_fw_var.get()
    hmi_fw_value = hmi_fw_var.get()
    cb_fw_value = cb_fw_var.get()
    evse_version_value = evse_version_entry.get()

    # 检查输入是否为空
    if not all([cbs_fw_value, hmi_fw_value, cb_fw_value, evse_version_value]):
        tk.messagebox.showwarning("Warning", "Please fill in all fields.")
        return

    # 构建配置文件内容
    config_content = f"BMAP_GZ={cbs_fw_value}\n"
    config_content += f"CBFW={cb_fw_value}\n"
    config_content += f"HMIFW={hmi_fw_value}\n"
    config_content += f"EVSEVER={evse_version_value}\n"

    # 写入配置文件
    with open("msiusbp.conf", "w") as file:
        file.write(config_content)

    tk.messagebox.showinfo("Success", "Configuration file generated successfully.")

# Generate 按钮
generate_button = tk.Button(root, text="generate", command=generate_config)
generate_button.pack(pady=20)

# 运行主循环
root.mainloop()