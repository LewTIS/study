import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import hashlib,os
import zipfile

#create main window
root = tk.Tk()
root.title("C801 EVSE USB FW packing tool")
root.configure(bg="#f0f2f5")

# set window size and position
window_width = 450
window_height = 300
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
x_offset = (screen_width - window_width) // 2
y_offset = (screen_height - window_height) // 2

root.geometry(f'{window_width}x{window_height}+{x_offset}+{y_offset}')

# create main frame
main_frame = tk.Frame(root, bg="#f0f2f5")
main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

title_label = tk.Label(main_frame,text='C801 EVSE USB FW packing tool',
                       font=("Calibri", 14, "bold"),
                       bg="#f0f2f5",
                       fg="black")
#Place the label in the main window
title_label.grid(row=0, column=0, columnspan=2, pady=15)

# set label and combobox width
LABEL_WIDTH = 12
COMBOBOX_WIDTH = 25

#add "CBS FW:" label and "CBS FW" combobox

cbs_fw_label = tk.Label(main_frame, 
                        text='CBS FW :', 
                        font=("Arial", 13, 'bold'), 
                        bg="#f0f2f5",
                        width=LABEL_WIDTH, 
                        anchor='w')
cbs_fw_label.grid(row=1, column=0, sticky=tk.W, pady=10, padx=(30,0))
cbs_fw_var = tk.StringVar()
cbs_fw_combobox = ttk.Combobox(main_frame, 
                               textvariable=cbs_fw_var, 
                               values=["RC8015RMSI_C0030.bmap.gz","option 2","option 3"], 
                               font=('Arial', 12),
                               width=COMBOBOX_WIDTH)
cbs_fw_combobox.grid(row=1, column=1, sticky=tk.W, pady=10, padx=(0,30))

#add "HMI FW:" label and "HMI FW" combobox

hmi_fw_label = tk.Label(main_frame, 
                        text='HMI FW  :', 
                        font=("Arial", 13, 'bold'), 
                        bg="#f0f2f5",
                        width=LABEL_WIDTH,
                        anchor='w')
hmi_fw_label.grid(row=2, column=0, sticky=tk.W, pady=10, padx=(30,0))
hmi_fw_var = tk.StringVar()
hmi_fw_combobox = ttk.Combobox(main_frame, 
                               textvariable=hmi_fw_var, 
                               values=["RC8017IMSI_D0020.bin","option 2","option 3"], 
                               font=('Arial', 12),
                               width=COMBOBOX_WIDTH)
hmi_fw_combobox.grid(row=2, column=1, sticky=tk.W, pady=10, padx=(0,30))

#add "CB FW:" label and "CB FW" combobox
cb_fw_label = tk.Label(main_frame, 
                       text='CB FW   :', 
                       font=("Arial", 13, 'bold'), 
                       bg="#f0f2f5",
                       width=LABEL_WIDTH,
                       anchor='w')
cb_fw_label.grid(row=3, column=0, sticky=tk.W, pady=10, padx=(30,0))

cb_fw_var = tk.StringVar()
cb_fw_combobox = ttk.Combobox(main_frame, 
                              textvariable=cb_fw_var, 
                              values=["RC8011EMSI_E0040.bin","option 2","option 3"], 
                              font=('Arial', 12),
                              width=COMBOBOX_WIDTH)
cb_fw_combobox.grid(row=3, column=1, sticky=tk.W, pady=10, padx=(0,30))

#add "EVSE version" label and "EVSE version" combobox

evse_version_label = tk.Label(main_frame, 
                              text='EVSE version:', 
                              font=("Arial", 13, 'bold'), 
                              bg="#f0f2f5",
                              width=LABEL_WIDTH,
                              anchor='w')
evse_version_label.grid(row=4, column=0, sticky=tk.W, pady=10, padx=(30,10))

evse_version_var = tk.StringVar()
evse_version_entry = tk.Entry(main_frame, 
                              textvariable=evse_version_var, 
                              font=('Arial', 12),
                              width=COMBOBOX_WIDTH,
                              )
evse_version_entry.grid(row=4, column=1, pady=10, padx=(5,30))


def generate_config_file():
    cbs_fw_value = cbs_fw_var.get()
    hmi_fw_value = hmi_fw_var.get()
    cb_fw_value = cb_fw_var.get()
    evse_version_value = evse_version_var.get() 
    
    # check if the input item is empty
    if not all([cbs_fw_value,hmi_fw_value,cb_fw_value,evse_version_value]):
        tk.messagebox.showinfo("Warning", "Please fill all fields!")
        return
    selected_files = [cbs_fw_value, hmi_fw_value, cb_fw_value]
    
    #check if the file exists 
    for file in selected_files:
        if not os.path.exists(file):
            tk.messagebox.showinfo("Warning", f"{file} does not exist!")
            return
    
    #caculate the selected file checksum
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

    #write to msiusbp.conf
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
                            fg='white',
                            bg='steelblue',
                            font=('Arial', 14, "bold"),
                            )
generate_button.grid(row=5, column=0, columnspan=2, pady=5)

root.after(800, lambda: messagebox.showinfo("Tip", "Please put the current program and the selected file in the same directory!"))
root.mainloop()