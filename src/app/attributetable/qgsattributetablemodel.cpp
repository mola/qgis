/***************************************************************************
     QgsAttributeTableModel.cpp
     --------------------------------------
    Date                 : Feb 2009
    Copyright            : (C) 2009 Vita Cizek
    Email                : weetya (at) gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsattributetablemodel.h"
//#include "qgsattributetableview.h"

#include "qgsvectordataprovider.h"
#include "qgsfield.h"
#include "qgsvectorlayer.h"

#include <QtGui>
#include <QVariant>
#include <QtAlgorithms>
#include "qgslogger.h"

//could be faster when type guessed before sorting
bool idColumnPair::operator<( const idColumnPair &b ) const
{
  //QVariat thinks gid is a string!
  QVariant::Type columnType = columnItem.type();

  if ( columnType == QVariant::Int || columnType == QVariant::UInt || columnType == QVariant::LongLong || columnType == QVariant::ULongLong )
    return columnItem.toLongLong() < b.columnItem.toLongLong();

  if ( columnType == QVariant::Double )
    return columnItem.toDouble() < b.columnItem.toDouble();

  return columnItem.toString() < b.columnItem.toString();
}

//////////////////
// Filter Model //
//////////////////

void QgsAttributeTableFilterModel::sort( int column, Qt::SortOrder order )
{
  (( QgsAttributeTableModel * )sourceModel() )->sort( column, order );
}

QgsAttributeTableFilterModel::QgsAttributeTableFilterModel( QgsVectorLayer* theLayer )
{
  mLayer = theLayer;
  mHideUnselected = false;
  setDynamicSortFilter( true );
}

bool QgsAttributeTableFilterModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  if ( mHideUnselected )
    // unreadable? yes, i agree :-)
    return mLayer->selectedFeaturesIds().contains((( QgsAttributeTableModel * )sourceModel() )->rowToId( sourceRow ) );

  return true;
}

/*
QModelIndex QgsAttributeTableFilterModel::mapFromSource ( const QModelIndex& sourceIndex ) const
{
  return sourceIndex;
}

QModelIndex QgsAttributeTableFilterModel::mapToSource ( const QModelIndex& filterIndex ) const
{
  return filterIndex;
}
*/

////////////////////////////
// QgsAttributeTableModel //
////////////////////////////

QgsAttributeTableModel::QgsAttributeTableModel( QgsVectorLayer *theLayer, QObject *parent )
    : QAbstractTableModel( parent )
{
  mLastRowId = -1;
  mLastRow = NULL;
  mLayer = theLayer;
  mFeatureCount = mLayer->pendingFeatureCount();
  mFieldCount = mLayer->pendingFields().size();
  mAttributes = mLayer->pendingAllAttributesList();

  connect( mLayer, SIGNAL( layerModified( bool ) ), this, SLOT( layerModified( bool ) ) );
  //connect(mLayer, SIGNAL(attributeAdded(int)), this, SLOT( attributeAdded(int)));
  //connect(mLayer, SIGNAL(attributeDeleted(int)), this, SLOT( attributeDeleted(int)));
  //connect(mLayer, SIGNAL(attributeValueChanged(int, int, const QVariant&)), this, SLOT( attributeValueChanged(int, int, const QVariant&)));
  //connect(mLayer, SIGNAL(featureDeleted(int)), this, SLOT( featureDeleted(int)));
  //connect(mLayer, SIGNAL(featureAdded(int)), this, SLOT( featureAdded(int)));

  loadLayer();
}

void QgsAttributeTableModel::featureDeleted( int fid )
{
  QgsDebugMsg( "entered." );

#ifdef QGISDEBUG
  int idx = mIdRowMap[fid];
  QgsDebugMsg( idx );
  QgsDebugMsg( fid );
#endif

#if 0
  --mFeatureCount;
  mIdRowMap.remove( fid );
  mRowIdMap.remove( idx );

  // fill the hole in the view
  if ( idx != mFeatureCount )
  {
    QgsDebugMsg( "jo" );
    //mRowIdMap[idx] = mRowIdMap[mFeatureCount];
    //mIdRowMap[mRowIdMap[idx]] = idx;
    int movedId = mRowIdMap[mFeatureCount];
    mRowIdMap.remove( mFeatureCount );
    mRowIdMap.insert( idx, movedId );
    mIdRowMap[movedId] = idx;
    //mIdRowMap.remove(mRowIdMap[idx]);
    //mIdRowMap.insert(mRowIdMap[idx], idx);
  }

  QgsDebugMsg( QString( "map sizes:%1, %2" ).arg( mRowIdMap.size() ).arg( mIdRowMap.size() ) );
  emit layoutChanged();
  //reload(index(0,0), index(rowCount(), columnCount()));
#endif

  QgsDebugMsg( "id->row" );
  QMap<int, int>::iterator it;
  for ( it = mIdRowMap.begin(); it != mIdRowMap.end(); ++it )
    QgsDebugMsg( QString( "%1->%2" ).arg( it.key() ).arg( *it ) );

  QgsDebugMsg( "row->id" );

  for ( it = mRowIdMap.begin(); it != mRowIdMap.end(); ++it )
    QgsDebugMsg( QString( "%1->%2" ).arg( it.key() ).arg( *it ) );

}

void QgsAttributeTableModel::featureAdded( int fid )
{
  QgsDebugMsg( "BM feature added" );
  ++mFeatureCount;
  mIdRowMap.insert( fid, mFeatureCount - 1 );
  mRowIdMap.insert( mFeatureCount - 1, fid );
  QgsDebugMsg( QString( "map sizes:%1, %2" ).arg( mRowIdMap.size() ).arg( mIdRowMap.size() ) );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

void QgsAttributeTableModel::attributeAdded( int idx )
{
  QgsDebugMsg( "BM attribute added" );
  loadLayer();
  QgsDebugMsg( QString( "map sizes:%1, %2" ).arg( mRowIdMap.size() ).arg( mIdRowMap.size() ) );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
  emit modelChanged();
}

void QgsAttributeTableModel::attributeDeleted( int idx )
{
  QgsDebugMsg( "BM attribute deleted" );
  loadLayer();
  QgsDebugMsg( QString( "map sizes:%1, %2" ).arg( mRowIdMap.size() ).arg( mIdRowMap.size() ) );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
  emit modelChanged();
}

void QgsAttributeTableModel::layerDeleted()
{
  QgsDebugMsg( "entered." );
  mIdRowMap.clear();
  mRowIdMap.clear();
  QgsDebugMsg( QString( "map sizes:%1, %2" ).arg( mRowIdMap.size() ).arg( mIdRowMap.size() ) );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

//TODO: check whether caching in data()/setData() doesn't cache old value
void QgsAttributeTableModel::attributeValueChanged( int fid, int idx, const QVariant &value )
{
  QgsDebugMsg( "entered." );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

void QgsAttributeTableModel::layerModified( bool onlyGeometry )
{
  if ( onlyGeometry )
    return;

  loadLayer();
  emit modelChanged();
}

void QgsAttributeTableModel::loadLayer()
{
  QgsDebugMsg( "entered." );

  QgsFeature f;
  bool ins = false, rm = false;

  mRowIdMap.clear();
  mIdRowMap.clear();

  if ( mFeatureCount < mLayer->pendingFeatureCount() )
  {
    QgsDebugMsg( "ins" );
    ins = true;
    beginInsertRows( QModelIndex(), mFeatureCount, mLayer->pendingFeatureCount() - 1 );
// QgsDebugMsg(QString("%1, %2").arg(mFeatureCount).arg(mLayer->pendingFeatureCount() - 1));
  }
  else if ( mFeatureCount > mLayer->pendingFeatureCount() )
  {
    QgsDebugMsg( "rm" );
    rm = true;
    beginRemoveRows( QModelIndex(), mLayer->pendingFeatureCount(), mFeatureCount - 1 );
// QgsDebugMsg(QString("%1, %2").arg(mFeatureCount).arg(mLayer->pendingFeatureCount() -1));
  }

  mLayer->select( QgsAttributeList(), QgsRectangle(), false );

  for ( int i = 0; mLayer->nextFeature( f ); ++i )
  {
    mRowIdMap.insert( i, f.id() );
    mIdRowMap.insert( f.id(), i );
  }

  // not needed when we have featureAdded signal
  mFeatureCount = mLayer->pendingFeatureCount();
  mFieldCount = mLayer->pendingFields().size();

  if ( ins )
  {
    endInsertRows();
    QgsDebugMsg( "end ins" );
  }
  else if ( rm )
  {
    endRemoveRows();
    QgsDebugMsg( "end rm" );
  }

#if 0
  QgsDebugMsg( "id->row" );
  QMap<int, int>::iterator it;
  for ( it = mIdRowMap.begin(); it != mIdRowMap.end(); ++it )
    QgsDebugMsg( QString( "%1->%2" ).arg( it.key() ).arg( *it ) );

  QgsDebugMsg( "row->id" );

  for ( it = mRowIdMap.begin(); it != mRowIdMap.end(); ++it )
    QgsDebugMsg( QString( "%1->%2" ).arg( it.key() ).arg( *it ) );
#endif
}

void QgsAttributeTableModel::swapRows( int a, int b )
{
  if ( a == b )
    return;

  int rowA = idToRow( a );
  int rowB = idToRow( b );

  //emit layoutAboutToBeChanged();

  mRowIdMap.remove( rowA );
  mRowIdMap.remove( rowB );
  mRowIdMap.insert( rowA, b );
  mRowIdMap.insert( rowB, a );

  mIdRowMap.remove( a );
  mIdRowMap.remove( b );
  mIdRowMap.insert( a, rowB );
  mIdRowMap.insert( b, rowA );

  //emit layoutChanged();
}

int QgsAttributeTableModel::idToRow( const int id ) const
{
  if ( !mIdRowMap.contains( id ) )
  {
    QgsDebugMsg( QString( "idToRow: id %1 not in map" ).arg( id ) );
    return -1;
  }

  return mIdRowMap[id];
}

int QgsAttributeTableModel::rowToId( const int id ) const
{
  if ( !mRowIdMap.contains( id ) )
  {
    QgsDebugMsg( QString( "rowToId: row %1 not in map" ).arg( id ) );
    return -1;
  }

  return mRowIdMap[id];
}

int QgsAttributeTableModel::rowCount( const QModelIndex &parent ) const
{
  return mFeatureCount;
}

int QgsAttributeTableModel::columnCount( const QModelIndex &parent ) const
{
  return mFieldCount;
}

QVariant QgsAttributeTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role == Qt::DisplayRole )
  {
    if ( orientation == Qt::Vertical ) //row
    {
      return QVariant( section );
    }
    else
    {
      QgsField field = mLayer->pendingFields()[ mAttributes[section] ]; //column
      return QVariant( field.name() );
    }
  }
  else return QVariant();
}

void QgsAttributeTableModel::sort( int column, Qt::SortOrder order )
{
  QgsAttributeMap row;
  idColumnPair pair;
  QgsAttributeList attrs;
  QgsFeature f;

  attrs.append( mAttributes[column] );

  emit layoutAboutToBeChanged();
// QgsDebugMsg("SORTing");

  mSortList.clear();
  mLayer->select( attrs, QgsRectangle(), false );
  while ( mLayer->nextFeature( f ) )
  {
    row = f.attributeMap();

    pair.id = f.id();
    pair.columnItem = row[ mAttributes[column] ];

    mSortList.append( pair );
  }

  if ( order == Qt::AscendingOrder )
    qStableSort( mSortList.begin(), mSortList.end() );
  else
    qStableSort( mSortList.begin(), mSortList.end(), qGreater<idColumnPair>() );

  // recalculate id<->row maps
  mRowIdMap.clear();
  mIdRowMap.clear();

  int i = 0;
  QList<idColumnPair>::Iterator it;
  for ( it = mSortList.begin(); it != mSortList.end(); ++it, ++i )
  {
    mRowIdMap.insert( i, it->id );
    mIdRowMap.insert( it->id, i );
  }

  // restore selection
  emit layoutChanged();
  //reset();
  emit modelChanged();
}

QVariant QgsAttributeTableModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || ( role != Qt::TextAlignmentRole && role != Qt::DisplayRole && role != Qt::EditRole ) )
    return QVariant();

  QVariant::Type fldType = mLayer->pendingFields()[ mAttributes[index.column()] ].type();
  bool fldNumeric = ( fldType == QVariant::Int || fldType == QVariant::Double );

  if ( role == Qt::TextAlignmentRole )
  {
    if ( fldNumeric )
      return QVariant( Qt::AlignRight );
    else
      return QVariant( Qt::AlignLeft );
  }

  // if we don't have the row in current cache, load it from layer first
  if ( mLastRowId != rowToId( index.row() ) )
  {
    bool res = mLayer->featureAtId( rowToId( index.row() ), mFeat, false, true );

    if ( !res )
      return QVariant( "ERROR" );

    mLastRowId = rowToId( index.row() );
    mLastRow = ( QgsAttributeMap * ) & mFeat.attributeMap();
  }

  if ( !mLastRow )
    return QVariant( "ERROR" );

  QVariant& val = ( *mLastRow )[ mAttributes[index.column()] ];

  if ( val.isNull() )
  {
    // if the value is NULL, show that in table, but don't show "NULL" text in editor
    if ( role == Qt::EditRole )
      return QVariant();
    else
      return QVariant( "NULL" );
  }

  // force also numeric data for EditRole to be strings
  // otherwise it creates spinboxes instead of line edits
  // (probably not what we do want)
  if ( fldNumeric && role == Qt::EditRole )
    return val.toString();

  // convert to QString from some other representation
  // this prevents displaying greater numbers in exponential format
  return val.toString();
}

bool QgsAttributeTableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !index.isValid() || role != Qt::EditRole )
    return false;

  if ( !mLayer->isEditable() )
    return false;

  bool res = mLayer->featureAtId( rowToId( index.row() ), mFeat, false, true );

  if ( res )
  {
    mLastRowId = rowToId( index.row() );
    mLastRow = ( QgsAttributeMap * )( &( mFeat.attributeMap() ) );

    mLayer->changeAttributeValue( rowToId( index.row() ), index.column(), value, true );
  }

  if ( !mLayer->isModified() )
    return false;

  emit dataChanged( index, index );
  return true;
}

Qt::ItemFlags QgsAttributeTableModel::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags = QAbstractItemModel::flags( index );

  if ( mLayer->isEditable() )
    flags |= Qt::ItemIsEditable;

  return flags;
}

void QgsAttributeTableModel::reload( const QModelIndex &index1, const QModelIndex &index2 )
{
  emit dataChanged( index1, index2 );
}

void QgsAttributeTableModel::resetModel()
{
  reset();
}

void QgsAttributeTableModel::changeLayout()
{
  emit layoutChanged();
}

void QgsAttributeTableModel::incomingChangeLayout()
{
  emit layoutAboutToBeChanged();
}

/////////////////////
// In-Memory model //
/////////////////////

void QgsAttributeTableMemModel::loadLayer()
{
  QgsAttributeTableModel::loadLayer();
  mLayer->select( mLayer->pendingAllAttributesList(), QgsRectangle(), false );

  QgsFeature f;
  while ( mLayer->nextFeature( f ) )
    mFeatureMap.insert( f.id(), f );
}

QgsAttributeTableMemModel::QgsAttributeTableMemModel
( QgsVectorLayer *theLayer )
    : QgsAttributeTableModel( theLayer )
{
  loadLayer();
}

QVariant QgsAttributeTableMemModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || ( role != Qt::TextAlignmentRole && role != Qt::DisplayRole && role != Qt::EditRole ) )
    return QVariant();

  QVariant::Type fldType = mLayer->pendingFields()[ mAttributes[index.column()] ].type();
  bool fldNumeric = ( fldType == QVariant::Int || fldType == QVariant::Double );

  if ( role == Qt::TextAlignmentRole )
  {
    if ( fldNumeric )
      return QVariant( Qt::AlignRight );
    else
      return QVariant( Qt::AlignLeft );
  }

  // if we don't have the row in current cache, load it from layer first
  if ( mLastRowId != rowToId( index.row() ) )
  {
    //bool res = mLayer->featureAtId(rowToId(index.row()), mFeat, false, true);
    bool res = mFeatureMap.contains( rowToId( index.row() ) );

    if ( !res )
      return QVariant( "ERROR" );

    mLastRowId = rowToId( index.row() );
    mFeat = mFeatureMap[rowToId( index.row() )];
    mLastRow = ( QgsAttributeMap * ) & mFeat.attributeMap();
  }

  if ( !mLastRow )
    return QVariant( "ERROR" );

  QVariant &val = ( *mLastRow )[ mAttributes[index.column()] ];

  if ( val.isNull() )
  {
    // if the value is NULL, show that in table, but don't show "NULL" text in editor
    if ( role == Qt::EditRole )
      return QVariant();
    else
      return QVariant( "NULL" );
  }

  // force also numeric data for EditRole to be strings
  // otherwise it creates spinboxes instead of line edits
  // (probably not what we do want)
  if ( fldNumeric && role == Qt::EditRole )
    return val.toString();

  // convert to QString from some other representation
  // this prevents displaying greater numbers in exponential format
  return val.toString();
}

bool QgsAttributeTableMemModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !index.isValid() || role != Qt::EditRole )
    return false;

  if ( !mLayer->isEditable() )
    return false;

  //bool res = mLayer->featureAtId(rowToId(index.row()), mFeat, false, true);
  bool res = mFeatureMap.contains( rowToId( index.row() ) );

  if ( res )
  {
    mLastRowId = rowToId( index.row() );
    mFeat = mFeatureMap[rowToId( index.row() )];
    mLastRow = ( QgsAttributeMap * ) & mFeat.attributeMap();


// QgsDebugMsg(mFeatureMap[rowToId(index.row())].id());
    mFeatureMap[rowToId( index.row() )].changeAttribute( index.column(), value );
    // propagate back to the layer
    mLayer->changeAttributeValue( rowToId( index.row() ), index.column(), value, true );
  }

  if ( !mLayer->isModified() )
    return false;

  emit dataChanged( index, index );
  return true;
}

void QgsAttributeTableMemModel::featureDeleted( int fid )
{
  QgsDebugMsg( "entered." );
  mFeatureMap.remove( fid );
  QgsAttributeTableModel::featureDeleted( fid );
}

void QgsAttributeTableMemModel::featureAdded( int fid )
{
  QgsDebugMsg( "entered." );
  QgsFeature f;
  mLayer->featureAtId( fid, f, false, true );
  mFeatureMap.insert( fid, f );
  QgsAttributeTableModel::featureAdded( fid );
}

#if 0
void QgsAttributeTableMemModel::attributeAdded( int idx )
{
  QgsDebugMsg( "entered." );
  loadLayer();
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

void QgsAttributeTableMemModel::attributeDeleted( int idx )
{
  QgsDebugMsg( "entered." );
  loadLayer();
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}
#endif

void QgsAttributeTableMemModel::layerDeleted()
{
  QgsDebugMsg( "entered." );
  mFeatureMap.clear();
  QgsAttributeTableModel::layerDeleted();
}

void QgsAttributeTableMemModel::attributeValueChanged( int fid, int idx, const QVariant &value )
{
  QgsDebugMsg( "entered." );
  mFeatureMap[fid].changeAttribute( idx, value );
  reload( index( 0, 0 ), index( rowCount(), columnCount() ) );
}