import sys
from PyQt6.QtWidgets import *
from PyQt6 import uic
import requests
import fileinput
from threading import *

if __name__=='__main__':
        app = QApplication(sys.argv)
        tf =uic.loadUi("transfrom.ui")
        tf.checkBox_2.setChecked(True)
        tf.checkBox.setChecked(True)
        def choose():
            path= QFileDialog.getOpenFileName(tf,"选择文件","./","All Files (*)")[0]
            tf.lineEdit.setText(path)
        def transfrom():
                uri='http://192.168.100.88:8080/upload'
                files = {'file':(tf.lineEdit.text().split('/')[-1],open(tf.lineEdit.text(),'rb'))}
                res =requests.post(uri,files=files,params={'targetIp':'127.0.0.1','isSaveFile':tf.checkBox.isChecked(),'isExecShell':tf.checkBox_2.isChecked()})
                QMessageBox.information(tf,"info",res.text,QMessageBox.StandardButton.Ok)
        tf.choose.clicked.connect(choose)
        tf.pushButton.clicked.connect(transfrom)
        tf.show()



        sys.exit(app.exec())