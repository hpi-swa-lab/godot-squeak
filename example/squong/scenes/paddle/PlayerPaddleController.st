Class {
	#name : #GDSPlayerPaddleController,
	#superclass : #GodotReference,
	#instVars : [
		'paddle'
	],
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSPlayerPaddleController >> computeVerticalDirectionFor: ball [

	(input isActionPressedAction: 'up') ifTrue: [^ -1].
	(input isActionPressedAction: 'down') ifTrue: [^ 1].
	^ 0
]

{ #category : #behavior }
GDSPlayerPaddleController >> paddle: paddleObject [

	paddle := paddleObject
]

{ #category : #behavior }
GDSPlayerPaddleController >> stabilizedPositionDelta: delta ball: ball [

	^ paddle position
]
