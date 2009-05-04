/***************************************************************************
    qgsconfigureshortcutsdialog.cpp
    ---------------------
    begin                : May 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsconfigureshortcutsdialog.h"

#include "qgsshortcutsmanager.h"

#include "qgslogger.h"

#include <QKeyEvent>
#include <QKeySequence>
#include <QMessageBox>

QgsConfigureShortcutsDialog::QgsConfigureShortcutsDialog(QWidget* parent)
  : QDialog(parent), mGettingShortcut(false)
{
  setupUi(this);

  connect(btnChangeShortcut, SIGNAL(clicked()), this, SLOT(changeShortcut()));
  connect(btnResetShortcut, SIGNAL(clicked()), this, SLOT(resetShortcut()));
  connect(btnSetNoShortcut, SIGNAL(clicked()), this, SLOT(setNoShortcut()));

  connect(treeActions, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
          this, SLOT(actionChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

  populateActions();
}

void QgsConfigureShortcutsDialog::populateActions()
{
  QList<QAction*> actions = QgsShortcutsManager::instance()->listActions();

  QList<QTreeWidgetItem *> items;
  for (int i = 0; i < actions.count(); ++i)
  {
    QString actionText = actions[i]->text();
    actionText.remove('&'); // remove the accelerator

    QStringList lst; lst << actionText << actions[i]->shortcut().toString();
    QTreeWidgetItem* item = new QTreeWidgetItem(lst);
    item->setIcon(0, actions[i]->icon());
    item->setData(0, Qt::UserRole, qVariantFromValue((QObject*)actions[i]) );
    items.append(item);
  }

  treeActions->addTopLevelItems(items);

  // make sure everything's visible and sorted
  treeActions->resizeColumnToContents(0);
  treeActions->sortItems(0, Qt::AscendingOrder);

  actionChanged(treeActions->currentItem(), NULL);
}


void QgsConfigureShortcutsDialog::changeShortcut()
{
  setFocus(); // make sure we have focus
  setGettingShortcut(true);
}

void QgsConfigureShortcutsDialog::resetShortcut()
{
  QAction* action = currentAction();
  if ( !action ) return;

  // set default shortcut
  QString shortcut = QgsShortcutsManager::instance()->actionDefaultShortcut( action );
  setCurrentActionShortcut( shortcut );
}

void QgsConfigureShortcutsDialog::setNoShortcut()
{
  setCurrentActionShortcut( QKeySequence() );
}

QAction* QgsConfigureShortcutsDialog::currentAction()
{
  if (treeActions->currentItem() == NULL)
    return NULL;

  QObject* action = treeActions->currentItem()->data(0, Qt::UserRole).value<QObject*>();
  return qobject_cast<QAction*>(action);
}

void QgsConfigureShortcutsDialog::actionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
  // cancel previous shortcut setting (if any)
  setGettingShortcut(false);

  QAction* action = currentAction();
  if ( !action )
    return;

  // show which one is the default action
  QString shortcut = QgsShortcutsManager::instance()->actionDefaultShortcut( action );
  if (shortcut.isEmpty())
    shortcut = tr("None");
  btnResetShortcut->setText( tr("Set default (%1)").arg(shortcut) );

  // if there's no shortcut, disable set none
  btnSetNoShortcut->setEnabled( !action->shortcut().isEmpty() );
  // if the shortcut is default, disable set default
  btnResetShortcut->setEnabled( action->shortcut() != QKeySequence(shortcut) );
}

void QgsConfigureShortcutsDialog::keyPressEvent ( QKeyEvent * event )
{
  if (!mGettingShortcut)
  {
    QDialog::keyPressEvent(event);
    return;
  }

  int key = event->key();
  switch (key)
  {
    // modifiers
    case Qt::Key_Meta:
      mModifiers |= Qt::META;
      updateShortcutText();
      break;
    case Qt::Key_Alt:
      mModifiers |= Qt::ALT;
      updateShortcutText();
      break;
    case Qt::Key_Control:
      mModifiers |= Qt::CTRL;
      updateShortcutText();
      break;
    case Qt::Key_Shift:
      mModifiers |= Qt::SHIFT;
      updateShortcutText();
      break;

    // escape aborts the acquisition of shortcut
    case Qt::Key_Escape:
      setGettingShortcut(false);
      break;

    default:
      mKey = key;
      updateShortcutText();
  }
}

void QgsConfigureShortcutsDialog::keyReleaseEvent ( QKeyEvent * event )
{
  if (!mGettingShortcut)
  {
    QDialog::keyPressEvent(event);
    return;
  }

  int key = event->key();
  switch (key)
  {
    // modifiers
    case Qt::Key_Meta:
      mModifiers &= ~Qt::META;
      updateShortcutText();
      break;
    case Qt::Key_Alt:
      mModifiers &= ~Qt::ALT;
      updateShortcutText();
      break;
    case Qt::Key_Control:
      mModifiers &= ~Qt::CTRL;
      updateShortcutText();
      break;
    case Qt::Key_Shift:
      mModifiers &= ~Qt::SHIFT;
      updateShortcutText();
      break;

    case Qt::Key_Escape:
      break;

    default:
      {
        // an ordinary key - set it with modifiers as a shortcut
        setCurrentActionShortcut( QKeySequence(mModifiers + mKey) );
        setGettingShortcut(false);
      }
  }
}


void QgsConfigureShortcutsDialog::updateShortcutText()
{
  // update text of the button so that user can see what has typed already
  QKeySequence s(mModifiers + mKey);
  btnChangeShortcut->setText( tr("Input: ") + s.toString() );
}

void QgsConfigureShortcutsDialog::setGettingShortcut(bool getting)
{
  mModifiers = 0;
  mKey = 0;
  mGettingShortcut = getting;
  if (!getting)
  {
    btnChangeShortcut->setChecked(false);
    btnChangeShortcut->setText(tr("Change"));
  }
  else
  {
    updateShortcutText();
  }
}

void QgsConfigureShortcutsDialog::setCurrentActionShortcut(QKeySequence s)
{
  QAction* action = currentAction();
  if ( !action ) return;

  // first check whether this action is not taken already
  QAction* otherAction = QgsShortcutsManager::instance()->actionForShortcut(s);
  if (otherAction != NULL)
  {
    QString otherActionText = otherAction->text();
    otherActionText.remove('&'); // remove the accelerator

    int res = QMessageBox::question( this, tr("Shortcut conflict"),
        tr("This shortcut is already assigned to action %1. Reassign?").arg(otherActionText),
        QMessageBox::Yes | QMessageBox::No);

    if (res != QMessageBox::Yes)
      return;

    // reset action of the conflicting other action!
    QgsShortcutsManager::instance()->setActionShortcut( otherAction, QString() );
    QList<QTreeWidgetItem*> items = treeActions->findItems(otherActionText, Qt::MatchExactly);
    if (items.count() > 0) // there should be exactly one
      items[0]->setText(1, QString() );
  }

  // update manager
  QgsShortcutsManager::instance()->setActionShortcut( action, s.toString() );

  // update gui
  treeActions->currentItem()->setText(1, s.toString() );

  actionChanged( treeActions->currentItem(), NULL );
}