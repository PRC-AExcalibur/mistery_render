from PIL import Image
 

def replace_last(s, to_replace, replacement):
    start_index = s.rfind(to_replace)
    end_index = start_index + len(to_replace)
 
    if start_index != -1:
        return s[:start_index] + replacement + s[end_index:]
    else:
        return s



if __name__ == "__main__":
    file_path = "model/keqing/面.png"

    png_image = Image.open(file_path)
    
    save_path = replace_last(file_path, ".png", ".tga")

    png_image.save(save_path)