/***************************************************************************
                          qgsmaptoolmovelabel.cpp
                          -----------------------
    begin                : 2010-11-03
    copyright            : (C) 2010 by Marco Hugentobler
    email                : marco dot hugentobler at sourcepole dot ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsmaptoolmovelabel.h"
#include "qgsmapcanvas.h"
#include "qgsmaplayerregistry.h"
#include "qgsrubberband.h"
#include "qgsvectorlayer.h"
#include <QMouseEvent>

QgsMapToolMoveLabel::QgsMapToolMoveLabel( QgsMapCanvas* canvas ): QgsMapToolLabel( canvas )
{
}

QgsMapToolMoveLabel::~QgsMapToolMoveLabel()
{
}

void QgsMapToolMoveLabel::canvasPressEvent( QMouseEvent * e )
{
  deleteRubberBands();

  if ( !labelAtPosition( e, mCurrentLabelPos ) )
  {
    return;
  }

  QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( mCurrentLabelPos.layerID );
  if ( !layer )
  {
    return;
  }

  int xCol, yCol;
  if ( layerIsMoveable( layer, xCol, yCol ) )
  {
    mStartPointMapCoords = toMapCoordinates( e->pos() );
    mClickOffsetX = mStartPointMapCoords.x() - mCurrentLabelPos.labelRect.xMinimum();
    mClickOffsetY = mStartPointMapCoords.y() - mCurrentLabelPos.labelRect.yMinimum();
    createRubberBands();
  }
}

void QgsMapToolMoveLabel::canvasMoveEvent( QMouseEvent * e )
{
  if ( mLabelRubberBand )
  {
    QgsPoint pointCanvasCoords = toMapCoordinates( e->pos() );
    double offsetX = pointCanvasCoords.x() - mStartPointMapCoords.x();
    double offsetY = pointCanvasCoords.y() - mStartPointMapCoords.y();
    mLabelRubberBand->setTranslationOffset( offsetX, offsetY );
    mLabelRubberBand->updatePosition();
    mLabelRubberBand->update();
    mFixPointRubberBand->setTranslationOffset( offsetX, offsetY );
    mFixPointRubberBand->updatePosition();
    mFixPointRubberBand->update();
  }
}

void QgsMapToolMoveLabel::canvasReleaseEvent( QMouseEvent * e )
{
  if ( !mLabelRubberBand )
  {
    return;
  }

  deleteRubberBands();

  QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( mCurrentLabelPos.layerID );
  if ( !layer )
  {
    return;
  }

  QgsVectorLayer* vlayer = dynamic_cast<QgsVectorLayer*>( layer );
  if ( !vlayer )
  {
    return;
  }

  if ( !vlayer->isEditable() )
  {
    return;
  }

  QgsPoint releaseCoords = toMapCoordinates( e->pos() );
  double xdiff = releaseCoords.x() - mStartPointMapCoords.x();
  double ydiff = releaseCoords.y() - mStartPointMapCoords.y();

  int xCol, yCol;
  double xPosOrig, yPosOrig;
  bool xSuccess, ySuccess;

  if ( !dataDefinedPosition( vlayer, mCurrentLabelPos.featureId, xPosOrig, xSuccess, yPosOrig, ySuccess, xCol, yCol ) )
  {
    return;
  }

  double xPosNew, yPosNew;

  if ( !xSuccess || !ySuccess )
  {
    xPosNew = releaseCoords.x() - mClickOffsetX;
    yPosNew = releaseCoords.y() - mClickOffsetY;
  }
  else
  {
    xPosNew = xPosOrig + xdiff;
    yPosNew = yPosOrig + ydiff;
  }

  vlayer->beginEditCommand( tr( "Label moved" ) );
  vlayer->changeAttributeValue( mCurrentLabelPos.featureId, xCol, xPosNew, false );
  vlayer->changeAttributeValue( mCurrentLabelPos.featureId, yCol, yPosNew, false );
  vlayer->endEditCommand();

  mCanvas->refresh();
}



