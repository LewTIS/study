import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import hashlib,os
import zipfile

#create main window
root = tk.Tk()
root.title("C801 EVSE USB FW packing tool")
root.configure(bg="Gainsboro")

# set window size and position
window_width = 450
window_height = 300
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
x_offset = (screen_width - window_width) // 2
y_offset = (screen_height - window_height) // 2

root.geometry(f'{window_width}x{window_height}+{x_offset}+{y_offset}')

title_label = tk.Label(root,text='C801 EVSE USB FW packing tool',
                       font=("Calibri", 14, "bold"),
                       bg="SteelBlue",
                       fg="black")
#Place the label in the main window
title_label.pack(pady=10) 

#add "CBS FW:" label and "CBS FW" combobox
cbs_fw_frame = tk.Frame(root, bg="Gainsboro")
cbs_fw_frame.pack(anchor=tk.W, padx=80, pady=20)

cbs_fw_label = tk.Label(cbs_fw_frame, text='CBS FW :', font=("Arial", 13), bg="Gainsboro")
cbs_fw_label.pack(side=tk.LEFT, anchor=tk.W)

cbs_fw_var = tk.StringVar()
cbs_fw_combobox = ttk.Combobox(cbs_fw_frame, textvariable=cbs_fw_var, values=["RC8015RMSI_C0030.bmap.gz","option 2","option 3"])
cbs_fw_combobox.pack(side=tk.LEFT, anchor=tk.W, padx=5)

#add "HMI FW:" label and "HMI FW" combobox
hmi_fw_frame = tk.Frame(root, bg="Gainsboro")
hmi_fw_frame.pack(anchor=tk.W, padx=80, pady=10)

hmi_fw_label = tk.Label(hmi_fw_frame, text='HMI FW :', font=("Arial", 13), bg="Gainsboro")
hmi_fw_label.pack(side=tk.LEFT, anchor=tk.W)

hmi_fw_var = tk.StringVar()
hmi_fw_combobox = ttk.Combobox(hmi_fw_frame, textvariable=hmi_fw_var, values=["RC8017IMSI_ D0020.bin","option 2","option 3"])
hmi_fw_combobox.pack(side=tk.LEFT, anchor=tk.W, padx=10)

#add "CB FW:" label and "CB FW" combobox
cb_fw_frame = tk.Frame(root, bg="Gainsboro")
cb_fw_frame.pack(anchor=tk.W, padx=80, pady=20)

cb_fw_label = tk.Label(cb_fw_frame, text='CB FW :', font=("Arial", 13), bg="Gainsboro")
cb_fw_label.pack(side=tk.LEFT, anchor=tk.W)

cb_fw_var = tk.StringVar()
cb_fw_combobox = ttk.Combobox(cb_fw_frame, textvariable=cb_fw_var, values=["RC8011EMSI_E0040.bin","option 2","option 3"])
cb_fw_combobox.pack(side=tk.LEFT, anchor=tk.W, padx=18)

#add "EVSE version" label and "EVSE version" combobox
evse_version_frame = tk.Frame(root, bg="Gainsboro")
evse_version_frame.pack(anchor=tk.W, padx=50, pady=5)

evse_version_label = tk.Label(evse_version_frame, text='EVSE VERSION :', font=("Arial", 13), bg="Gainsboro")
evse_version_label.pack(side=tk.LEFT, anchor=tk.W)

evse_version_var = tk.StringVar()
evse_version_entry = tk.Entry(evse_version_frame,textvariable=evse_version_var)
evse_version_entry.pack(side=tk.LEFT,anchor=tk.W, padx=10)


def generate_config_file():
    cbs_fw_value = cbs_fw_var.get()
    hmi_fw_value = hmi_fw_var.get()
    cb_fw_value = cb_fw_var.get()
    evse_version_value = evse_version_var.get() 
    
    if not all([cbs_fw_value,hmi_fw_value,cb_fw_value,evse_version_value]):
        tk.messagebox.showinfo("Warning", "Please fill all fields!")
        return
    
    def caculate_md5(file_path):
        if not os.path.exists(file_path):
            return ""
        with open(file_path, "rb") as f:
            file_hash = hashlib.md5()
            while chunk := f.read(4096):
                file_hash.update(chunk)
        return file_hash.hexdigest()
    
    cbs_fw_md5 = caculate_md5(cbs_fw_value)
    hmi_fw_md5 = caculate_md5(hmi_fw_value)
    cb_fw_md5 = caculate_md5(cb_fw_value)

    config_content = f'BMAP_GZ={cbs_fw_value}\n'
    config_content += f'BMAP_GZ.md5={cbs_fw_md5}\n'
    config_content += f'HMIFW={hmi_fw_value}\n'
    config_content += f'HMIFW.md5={hmi_fw_md5}\n'
    config_content += f'CBFW={cb_fw_value}\n'
    config_content += f'CBFW.md5={cb_fw_md5}\n'
    config_content += f'EVSEVER={evse_version_value}\n'
    
    with open('msiusbp.conf','w') as f:
        f.write(config_content)
    
    #packing file    
    zip_file_name = f'{evse_version_value}.zip'
    files_to_zip = [cbs_fw_value,hmi_fw_value,cb_fw_value,'msiusbp.conf'] 
    with zipfile.ZipFile(zip_file_name,'w') as zipf:
        for file in files_to_zip:
            if os.path.exists(file):
                zipf.write(file,os.path.basename(file)) 
            else:
                print(f'file {file} not exist!')
            
    tk.messagebox.showinfo("Success", "Configuration file generated successfully.")
    

generate_button = tk.Button(root,text="Generate",
                            command=generate_config_file,
                            width=8,
                            height=3,
                            fg='white',
                            bg='steelblue',
                            font=('Arial', 14, "bold"),
                            )
generate_button.pack(pady=5)

root.mainloop()