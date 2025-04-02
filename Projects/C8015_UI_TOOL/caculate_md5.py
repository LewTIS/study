import hashlib
import os

def caculate_md5(file_path):
    if not os.path.exists(file_path):
        return ""
    with open(file_path, "rb") as f:
        file_hash = hashlib.md5()
        while chunk := f.read(4096):
            file_hash.update(chunk)
    return file_hash.hexdigest()

md5_hash = caculate_md5("RC8017IMSI_ D0020.bin")
print("md5_hash: %s" %md5_hash)