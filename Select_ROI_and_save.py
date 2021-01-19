## this script is used to calculate the ROI of "GRGB Bayer" image, you can use to check ROI value and save it.
## this templete is able to choose 25.1/100.3 MB size of raw file
## author : chengyuw
## input image must be GRBG bayer raw file
import numpy as np
import os
from matplotlib import pyplot as plt
import cv2
import matplotlib
import tkinter as tk
from tkinter.filedialog import askdirectory
from PIL import Image,ImageTk
count = 0
def img_save_window(cut_img,width,height): #call ROI Window
	def save_image():
		global count
		cv2.imwrite('ROI_image'+str(count)+'.jpg',cut_img)
		print('save the ROI image as "ROI_image'+str(count)+'.jpg"')
		count += 1
		return count
	def close_window():
		win.destroy()
	win = tk.Tk()
	screen_width = win.winfo_screenwidth()
	screen_height = win.winfo_screenheight()	#print("screen_width={0},screen_height={1}".format(screen_width,screen_height))
	win.title("ROI_IMG")
	lb = tk.Label(bg="black",fg="white",text="do you want to save ROI image?").pack()
	btn = tk.Button(text="yes",bg="white",command=save_image).pack() 
	btn2 = tk.Button(text="close",bg="white",command=close_window).pack() 
	cut_img_rgb = cv2.cvtColor(cut_img,cv2.COLOR_BGR2RGB) #cv2 default is BGR, need to convert it to RGB
	Image.fromarray(cut_img_rgb).save('temp.png') #convert Array to image
	img = Image.open('temp.png')
	w,h = img.size
	print("ROI_size={0}x{1}".format(w,h))
	for i in range(4,0,-1):
		if w>(i*screen_width) or h > (i*screen_height):
			resized = img.resize((int(w/(i*2)),int(h/(i*2))),Image.ANTIALIAS)
			print('resized/'+str(i*2))
			break
		else:
			resized = img.resize((int(w/(2)),int(h/(2))),Image.ANTIALIAS)
	resized_img = ImageTk.PhotoImage(resized)
	lb = tk.Label(master=win,bg="red",image=resized_img).pack()
	win.mainloop()
def on_mouse(event, x, y, flags, param): #drag the ROI
    global img,point1,point2,Bayer_Matrix
    copy_img = img.copy()    
    if event == cv2.EVENT_LBUTTONDOWN:     #press   
        point1 = (x,y)
        cv2.circle(copy_img, point1, 10, (0,255,0), 5)
        cv2.imshow('*press space to quick', copy_img)
    elif event == cv2.EVENT_MOUSEMOVE and (flags & cv2.EVENT_FLAG_LBUTTON): #drag
        cv2.rectangle(copy_img, point1, (x,y), (255,0,0), 5)
        cv2.imshow('*press space to quick', copy_img)
    elif event == cv2.EVENT_LBUTTONUP:         #release
        point2 = (x,y)
        cv2.rectangle(copy_img, point1, point2, (0,0,255), 5) 
        cv2.imshow('*press space to quick', copy_img)
        min_x = min(point1[0],point2[0])     
        min_y = min(point1[1],point2[1])
        width = abs(point1[0] - point2[0])
        height = abs(point1[1] -point2[1])
        cut_img = img[min_y:min_y+height, min_x:min_x+width]
        (y1,x1) = point1
        (y2,x2) = point2
        RawConvert.meanvalue(Bayer_Matrix,x1,y1,x2,y2)
        img_save_window(cut_img,width,height)
        key = cv2.waitKey(0)
class RawConvert:
	def toArray(files):
		files = np.fromfile(files, dtype=np.int16)
		try: 
			change_files = np.reshape(files,(3024,4032)) 
		except:
			change_files = np.reshape(files,(6048,8164))
		return change_files
	def meanvalue(A,x1,y1,x2,y2):
		global Bayer_Matrix
		Gr_sum = R_sum = Gb_sum = B_sum = Gr_counter = R_counter = Gb_counter = B_counter = 0
		for i in range(x1,x2):
			for j in range(y1,y2):
				if(i%2) == 0 and (j%2)==0:
					Gr_sum = Gr_sum + A[i,j]
					Gr_counter+=1
				if(i%2)==0 and (j%2) == 1:
					R_sum = R_sum + A[i,j]
					R_counter+=1
				if (i%2) == 1 and (j%2)==0:
					B_sum = B_sum + A[i,j]
					B_counter+=1
				if (i%2) == 1 and (j%2)==1:
					Gb_sum = Gb_sum + A[i,j]
					Gb_counter+=1
		mean_Gr = Gr_sum/(Gr_counter);mean_R = R_sum/(R_counter);mean_Gb = Gb_sum/(Gb_counter);mean_B = B_sum/(B_counter)
		print("mean_Gr:",mean_Gr)
		print("mean_R:",mean_R)
		print("mean_B:",mean_B)
		print("mean_Gb:",mean_Gb)
def selectPath():
	root = tk.Tk()
	file_path = tk.filedialog.askopenfilename(title='Select .raw file',filetypes=[('RAW','*.raw')])
	root.destroy()
	return file_path
def main():
	global img,Bayer_Matrix
	file_path = selectPath()
	if (type(file_path)):
		print("!!please select .raw file")
		return -1
	path = os.path.dirname(file_path)
	print("Selected path:",path)
	print("(Image load.....)")	
	Bayer_Matrix = RawConvert.toArray(file_path)
	matplotlib.image.imsave(path+'/temp_rgb.png',Bayer_Matrix)
	img = cv2.imread(path+'/temp_rgb.png')
	cv2.namedWindow('*press space to quick',0)
	cv2.resizeWindow('*press space to quick',540,960)
	print("please drag ROI of bayer image")	
	cv2.imshow('*press space to quick', img)
	cv2.setMouseCallback('*press space to quick', on_mouse)
	os.remove(path+'/temp_rgb.png')
	key = cv2.waitKey(0)
	while True:
		if key == 32:
			print("done!!")
			try:
				os.remove(os.getcwd()+'/temp.png')
			except:
				return False	
		else:
			print("quit the script")
			return False
if __name__ == '__main__':
	main()

