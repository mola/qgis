# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_files/DlgAddRelation.ui'
#
# Created: Tue Jul 14 14:44:27 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_DlgAddRelation(object):
    def setupUi(self, DlgAddRelation):
        DlgAddRelation.setObjectName("DlgAddRelation")
        DlgAddRelation.resize(620, 461)
        DlgAddRelation.setModal(False)
        self.vboxlayout = QtGui.QVBoxLayout(DlgAddRelation)
        self.vboxlayout.setObjectName("vboxlayout")
        self.hboxlayout = QtGui.QHBoxLayout()
        self.hboxlayout.setObjectName("hboxlayout")
        self.label = QtGui.QLabel(DlgAddRelation)
        self.label.setObjectName("label")
        self.hboxlayout.addWidget(self.label)
        spacerItem = QtGui.QSpacerItem(24, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)
        self.typeCombo = QtGui.QComboBox(DlgAddRelation)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.typeCombo.sizePolicy().hasHeightForWidth())
        self.typeCombo.setSizePolicy(sizePolicy)
        self.typeCombo.setMinimumSize(QtCore.QSize(164, 24))
        self.typeCombo.setMaximumSize(QtCore.QSize(164, 16777215))
        self.typeCombo.setEditable(True)
        self.typeCombo.setObjectName("typeCombo")
        self.hboxlayout.addWidget(self.typeCombo)
        self.typeInfoButton = QtGui.QToolButton(DlgAddRelation)
        self.typeInfoButton.setObjectName("typeInfoButton")
        self.hboxlayout.addWidget(self.typeInfoButton)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem1)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.hboxlayout1 = QtGui.QHBoxLayout()
        self.hboxlayout1.setObjectName("hboxlayout1")
        self.vboxlayout1 = QtGui.QVBoxLayout()
        self.vboxlayout1.setObjectName("vboxlayout1")
        self.hboxlayout2 = QtGui.QHBoxLayout()
        self.hboxlayout2.setObjectName("hboxlayout2")
        self.label_2 = QtGui.QLabel(DlgAddRelation)
        self.label_2.setObjectName("label_2")
        self.hboxlayout2.addWidget(self.label_2)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout2.addItem(spacerItem2)
        self.loadStandardTagsButton = QtGui.QToolButton(DlgAddRelation)
        self.loadStandardTagsButton.setObjectName("loadStandardTagsButton")
        self.hboxlayout2.addWidget(self.loadStandardTagsButton)
        self.removeTagButton = QtGui.QToolButton(DlgAddRelation)
        self.removeTagButton.setObjectName("removeTagButton")
        self.hboxlayout2.addWidget(self.removeTagButton)
        self.vboxlayout1.addLayout(self.hboxlayout2)
        self.relTagsTable = QtGui.QTableWidget(DlgAddRelation)
        self.relTagsTable.setMinimumSize(QtCore.QSize(240, 0))
        self.relTagsTable.setObjectName("relTagsTable")
        self.relTagsTable.setColumnCount(0)
        self.relTagsTable.setRowCount(0)
        self.vboxlayout1.addWidget(self.relTagsTable)
        self.hboxlayout1.addLayout(self.vboxlayout1)
        self.vboxlayout2 = QtGui.QVBoxLayout()
        self.vboxlayout2.setObjectName("vboxlayout2")
        self.hboxlayout3 = QtGui.QHBoxLayout()
        self.hboxlayout3.setObjectName("hboxlayout3")
        self.label_5 = QtGui.QLabel(DlgAddRelation)
        self.label_5.setObjectName("label_5")
        self.hboxlayout3.addWidget(self.label_5)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout3.addItem(spacerItem3)
        self.chooseMemberButton = QtGui.QToolButton(DlgAddRelation)
        self.chooseMemberButton.setCheckable(True)
        self.chooseMemberButton.setObjectName("chooseMemberButton")
        self.hboxlayout3.addWidget(self.chooseMemberButton)
        self.removeMemberButton = QtGui.QToolButton(DlgAddRelation)
        self.removeMemberButton.setObjectName("removeMemberButton")
        self.hboxlayout3.addWidget(self.removeMemberButton)
        self.vboxlayout2.addLayout(self.hboxlayout3)
        self.relMembersTable = QtGui.QTableWidget(DlgAddRelation)
        self.relMembersTable.setMinimumSize(QtCore.QSize(346, 0))
        self.relMembersTable.setObjectName("relMembersTable")
        self.relMembersTable.setColumnCount(0)
        self.relMembersTable.setRowCount(0)
        self.vboxlayout2.addWidget(self.relMembersTable)
        self.tagInfoTextEdit = QtGui.QTextEdit(DlgAddRelation)
        self.tagInfoTextEdit.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.tagInfoTextEdit.sizePolicy().hasHeightForWidth())
        self.tagInfoTextEdit.setSizePolicy(sizePolicy)
        self.tagInfoTextEdit.setMaximumSize(QtCore.QSize(16777215, 140))
        self.tagInfoTextEdit.setReadOnly(True)
        self.tagInfoTextEdit.setObjectName("tagInfoTextEdit")
        self.vboxlayout2.addWidget(self.tagInfoTextEdit)
        self.hboxlayout1.addLayout(self.vboxlayout2)
        self.vboxlayout.addLayout(self.hboxlayout1)
        self.hboxlayout4 = QtGui.QHBoxLayout()
        self.hboxlayout4.setObjectName("hboxlayout4")
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout4.addItem(spacerItem4)
        self.createRelButton = QtGui.QPushButton(DlgAddRelation)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.createRelButton.sizePolicy().hasHeightForWidth())
        self.createRelButton.setSizePolicy(sizePolicy)
        self.createRelButton.setMinimumSize(QtCore.QSize(0, 30))
        self.createRelButton.setAutoDefault(False)
        self.createRelButton.setObjectName("createRelButton")
        self.hboxlayout4.addWidget(self.createRelButton)
        self.stornoButton = QtGui.QPushButton(DlgAddRelation)
        self.stornoButton.setMinimumSize(QtCore.QSize(0, 30))
        self.stornoButton.setAutoDefault(False)
        self.stornoButton.setObjectName("stornoButton")
        self.hboxlayout4.addWidget(self.stornoButton)
        self.vboxlayout.addLayout(self.hboxlayout4)

        self.retranslateUi(DlgAddRelation)
        QtCore.QMetaObject.connectSlotsByName(DlgAddRelation)

    def retranslateUi(self, DlgAddRelation):
        DlgAddRelation.setWindowTitle(QtGui.QApplication.translate("DlgAddRelation", "Create OSM relation", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("DlgAddRelation", "Relation type:", None, QtGui.QApplication.UnicodeUTF8))
        self.typeInfoButton.setText(QtGui.QApplication.translate("DlgAddRelation", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("DlgAddRelation", "Properties", None, QtGui.QApplication.UnicodeUTF8))
        self.loadStandardTagsButton.setText(QtGui.QApplication.translate("DlgAddRelation", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.removeTagButton.setText(QtGui.QApplication.translate("DlgAddRelation", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("DlgAddRelation", "Members", None, QtGui.QApplication.UnicodeUTF8))
        self.chooseMemberButton.setText(QtGui.QApplication.translate("DlgAddRelation", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.removeMemberButton.setText(QtGui.QApplication.translate("DlgAddRelation", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.createRelButton.setText(QtGui.QApplication.translate("DlgAddRelation", "Create", None, QtGui.QApplication.UnicodeUTF8))
        self.stornoButton.setText(QtGui.QApplication.translate("DlgAddRelation", "Storno", None, QtGui.QApplication.UnicodeUTF8))
