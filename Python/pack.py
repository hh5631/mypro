import sys
from PyQt6.QtWidgets import *

if __name__=='__main__':
        app = QApplication(sys.argv)
        w = QWidget()
        w.resize(280, 160)
        w.move(300, 300)
        btn = QPushButton('Quit', w)
        btn.move(200,120)
        btn.clicked.connect(w.close)
        w.setWindowTitle('Simple')
        w.show()
        sys.exit(app.exec())