import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
from tkinter import filedialog
import hashlib,os
import zipfile

#create main window
root = tk.Tk()
root.title("C801 EVSE USB FW packing tool")
root.configure(bg="#f0f2f5")

# set window size and position
window_width = 500
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
                       fg="black",)
#Place the label in the main window
title_label.grid(row=0, column=0, columnspan=3, pady=15)

# set label and entry width
LABEL_WIDTH = 15
ENTRY_WIDTH = 25

#add "CBS FW:" label and "CBS FW" entry
cbs_fw_label = tk.Label(main_frame, 
                        text='CBS FW :', 
                        font=("Arial", 13, 'bold'), 
                        bg="#f0f2f5",
                        width=LABEL_WIDTH, 
                        anchor='w')

cbs_fw_label.grid(row=1, column=0, sticky=tk.W, pady=10, padx=(30,0))
cbs_fw_var = tk.StringVar()
cbs_entry = ttk.Entry(main_frame, textvariable=cbs_fw_var, font=('Arial', 11), width=ENTRY_WIDTH)
cbs_entry.grid(row=1, column=1, sticky=tk.W, padx=(0,2))

def select_cbs_file():
    file_path = filedialog.askopenfilename(
        title="Select CBS FW file",
        filetypes=(("BMAP files", "*.bmap.gz"), ("All files", "*.*"))
    )
    if file_path:
        cbs_fw_var.set(file_path)
ttk.Button(main_frame, text="Browse...", command=select_cbs_file, width=8).grid(row=1, column=2)

#add "HMI FW:" label and "HMI FW" entry

hmi_fw_label = tk.Label(main_frame, 
                        text='HMI FW  :', 
                        font=("Arial", 13, 'bold'), 
                        bg="#f0f2f5",
                        width=LABEL_WIDTH,
                        anchor='w')
hmi_fw_label.grid(row=2, column=0, sticky=tk.W, pady=10, padx=(30,0))
hmi_fw_var = tk.StringVar()
hmi_entry = ttk.Entry(main_frame, textvariable=hmi_fw_var, font=('Arial', 11), width=ENTRY_WIDTH)
hmi_entry.grid(row=2, column=1, sticky=tk.W, padx=(0,2))

def select_hmi_file():
    file_path = filedialog.askopenfilename(
        title="Select HMI FW file",
        filetypes=(("BIN files", "*.bin"), ("All files", "*.*"))
    )
    if file_path:
        hmi_fw_var.set(file_path)

ttk.Button(main_frame, text="Browse...", command=select_hmi_file, width=8).grid(row=2, column=2)

#add "CB FW:" label and "CB FW" entry
cb_fw_label = tk.Label(main_frame, 
                       text='CB FW   :', 
                       font=("Arial", 13, 'bold'), 
                       bg="#f0f2f5",
                       width=LABEL_WIDTH,
                       anchor='w')
cb_fw_label.grid(row=3, column=0, sticky=tk.W, pady=10, padx=(30,0))
cb_fw_var = tk.StringVar()
cb_entry = ttk.Entry(main_frame, textvariable=cb_fw_var, font=('Arial', 11), width=ENTRY_WIDTH)
cb_entry.grid(row=3,column=1,sticky=tk.W,padx=(0,2))
def select_cb_file():
    file_path = filedialog.askopenfilename(
        title="Select CB FW file",
        filetypes=(("BIN files", "*.bin"), ("All files", "*.*"))
    )
    if file_path:
        cb_fw_var.set(file_path) 

ttk.Button(main_frame, text="Browse...", command=select_cb_file, width=8).grid(row=3, column=2)

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
                              width=ENTRY_WIDTH,
                              )
evse_version_entry.grid(row=4, column=1, pady=10, padx=(0,2))


def generate_config_file():
    cbs_fw_value = cbs_fw_var.get()
    hmi_fw_value = hmi_fw_var.get()
    cb_fw_value = cb_fw_var.get()
    evse_version_value = evse_version_var.get() 
    
    # check if the input item is empty
    if not all([cbs_fw_value,hmi_fw_value,cb_fw_value,evse_version_value]):
        tk.messagebox.showinfo("Warning", "Please fill all fields!")
        return
    
    #check if the file exists
    selected_files = [cbs_fw_value, hmi_fw_value, cb_fw_value]
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

    #content of the config file
    config_content = f'BMAP_GZ={os.path.basename(cbs_fw_value)}\n'
    config_content += f'BMAP_GZ.md5={cbs_fw_md5}\n'
    config_content += f'HMIFW={os.path.basename(hmi_fw_value)}\n'
    config_content += f'HMIFW.md5={hmi_fw_md5}\n'
    config_content += f'CBFW={os.path.basename(cb_fw_value)}\n'
    config_content += f'CBFW.md5={cb_fw_md5}\n'
    config_content += f'EVSEVER={evse_version_value}\n'
    
    #packing file    
    zip_path = filedialog.asksaveasfilename(
        title="Save Zip File",
        initialfile=f"{evse_version_value}.zip",
        filetypes=(("Zip files", "*.zip"), ("All files", "*.*"))
    )
    if not zip_path:
        return

    try:
        with zipfile.ZipFile(zip_path,'w') as zipf:
            for file in [cbs_fw_value,hmi_fw_value,cb_fw_value]:
                    zipf.write(file,os.path.basename(file)) 
            
            zipf.writestr('msiusbp.conf',config_content)
                    
        #notify success message        
        tk.messagebox.showinfo("Success", f"The file has been saved to:\n{zip_path}")
    except Exception as e:
        tk.messagebox.showerror("Error", f"An error occurred: {e}")

generate_button = tk.Button(root,text="Generate",
                            command=generate_config_file,
                            fg='white',
                            bg='steelblue',
                            font=('Arial', 14, "bold"),
                            )
generate_button.grid(row=5, column=0, columnspan=2, pady=5)

root.mainloop()