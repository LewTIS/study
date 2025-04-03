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

# 创建主框架
main_frame = ttk.Frame(root, padding="20")
main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

# 标题标签
title_label = tk.Label(main_frame, text="C801 EVSE USB FW packing tool", font=("Arial", 12))
title_label.grid(row=0, column=0, columnspan=2, pady=(0, 20))

# 统一标签宽度和下拉框宽度
LABEL_WIDTH = 12
COMBOBOX_WIDTH = 30

# CBS FW 下拉框
cbs_fw_label = tk.Label(main_frame, text="CBS FW:", width=LABEL_WIDTH, anchor='w')
cbs_fw_label.grid(row=1, column=0, sticky=tk.W, pady=5)
cbs_fw_var = tk.StringVar()
cbs_fw_combobox = ttk.Combobox(main_frame, textvariable=cbs_fw_var, 
                              values=["Option 1", "Option 2", "Option 3"],
                              width=COMBOBOX_WIDTH)
cbs_fw_combobox.grid(row=1, column=1, sticky=tk.W, pady=5)

# HMI FW 下拉框
hmi_fw_label = tk.Label(main_frame, text="HMI FW:", width=LABEL_WIDTH, anchor='w')
hmi_fw_label.grid(row=2, column=0, sticky=tk.W, pady=5)
hmi_fw_var = tk.StringVar()
hmi_fw_combobox = ttk.Combobox(main_frame, textvariable=hmi_fw_var, 
                              values=["Option A", "Option B", "Option C"],
                              width=COMBOBOX_WIDTH)
hmi_fw_combobox.grid(row=2, column=1, sticky=tk.W, pady=5)

# CB FW 下拉框
cb_fw_label = tk.Label(main_frame, text="CB FW:", width=LABEL_WIDTH, anchor='w')
cb_fw_label.grid(row=3, column=0, sticky=tk.W, pady=5)
cb_fw_var = tk.StringVar()
cb_fw_combobox = ttk.Combobox(main_frame, textvariable=cb_fw_var, 
                             values=["Option X", "Option Y", "Option Z"],
                             width=COMBOBOX_WIDTH)
cb_fw_combobox.grid(row=3, column=1, sticky=tk.W, pady=5)

# EVSE version 输入框
evse_version_label = tk.Label(main_frame, text="EVSE version:", width=LABEL_WIDTH, anchor='w')
evse_version_label.grid(row=4, column=0, sticky=tk.W, pady=5)
evse_version_entry = tk.Entry(main_frame, width=COMBOBOX_WIDTH)
evse_version_entry.grid(row=4, column=1, sticky=tk.W, pady=5)

# 定义生成配置文件的函数
def generate_config():
    # 获取输入值
    cbs_fw_value = cbs_fw_var.get()
    hmi_fw_value = hmi_fw_var.get()
    cb_fw_value = cb_fw_var.get()
    evse_version_value = evse_version_entry.get()

    # 检查输入是否为空
    if not all([cbs_fw_value, hmi_fw_value, cb_fw_value, evse_version_value]):
        messagebox.showwarning("Warning", "Please fill in all fields.")
        return

    # 构建配置文件内容
    config_content = f"BMAP_GZ={cbs_fw_value}\n"
    config_content += f"CBFW={cb_fw_value}\n"
    config_content += f"HMIFW={hmi_fw_value}\n"
    config_content += f"EVSEVER={evse_version_value}\n"

    # 写入配置文件
    with open("msiusbp.conf", "w") as file:
        file.write(config_content)

    messagebox.showinfo("Success", "Configuration file generated successfully.")

# Generate 按钮
generate_button = tk.Button(main_frame, text="Generate", command=generate_config,
                          bg="#3498db", fg="white",
                          font=("Arial", 10, "bold"),
                          padx=20, pady=5)
generate_button.grid(row=5, column=0, columnspan=2, pady=20)

# 配置列的权重，使其可以扩展
main_frame.columnconfigure(1, weight=1)

# 运行主循环
root.mainloop()