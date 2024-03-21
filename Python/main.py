import webbrowser
from PyQt6.QtWidgets import *
from PyQt6 import uic
from PyQt6.QtGui import *
import sys
import requests

if __name__=="__main__":
        app = QApplication(sys.argv)
        w = uic.loadUi("main.ui")
        login = uic.loadUi("login.ui")
        register = uic.loadUi("register.ui")
        p1=QPixmap('OIP.jpg')      
        w.label_2.setPixmap(p1.scaled(100,100))
        uri_base="http://localhost"

        def resg():
                uri=uri_base+"/register"
                if(register.lineEdit.text()=="" or register.lineEdit_2.text()==""):
                        res=QMessageBox.information(register,"info","please enter username and password",QMessageBox.StandardButton.Ok)
                        return
                res=requests.get(uri,params={'username':register.lineEdit.text(),'password':register.lineEdit_2.text()})
                if(res.text.find("success")):
                        register.lineEdit_2.setText("");
                        res=QMessageBox.information(login,"info",res.text,QMessageBox.StandardButton.Ok)
                        if(res==QMessageBox.StandardButton.Ok):
                                login.show()
                                register.close()
        def loginf():
                uri=uri_base+"/login"
                if(login.lineEdit.text()=="" or login.lineEdit_2.text()==""):
                        res=QMessageBox.information(login,"info","please enter username and password",QMessageBox.StandardButton.Ok)
                        return
                resp=requests.get(uri,params={'username':login.lineEdit.text(),'password':login.lineEdit_2.text()})     
                QMessageBox.information(login,"info",resp.text,QMessageBox.StandardButton.Ok)      
                if(resp.text.find("success")!=-1):  
                        login.lineEdit_2.setText("")     
                        login.close()
                        w.label.setText(login.lineEdit.text())
                        w.pushButton.setText("退出")
        def main_login():
                if w.pushButton.text() == "退出":
                        w.label.setText("")
                        w.pushButton.setText("登录")
                else:
                        login.show()


                        
                
        w.pushButton.clicked.connect(main_login)
        w.pushButton_2.clicked.connect(lambda: webbrowser.open('https://bing.com/search?q='+w.lineEdit.text()))


        login.register_btn.clicked.connect(lambda:(register.show(),login.close()))
        login.login_btn.clicked.connect(loginf)


        register.cancel.clicked.connect(lambda:(login.show(),register.close(),register.lineEdit_2.setText("")))     
        register.register_btn.clicked.connect(resg)
        w.show()
        sys.exit(app.exec())