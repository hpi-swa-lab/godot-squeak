Class {
	#name : #GDSCpuPaddleController,
	#superclass : #GodotNode,
	#instVars : [
		'paddle'
	],
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSCpuPaddleController >> computeVerticalDirectionFor: ball [

	^ (ball position y - paddle position y) sign
]

{ #category : #behavior }
GDSCpuPaddleController >> paddle: paddleObject [

	paddle := paddleObject
]

{ #category : #behavior }
GDSCpuPaddleController >> stabilizedPositionDelta: delta ball: ball [

	| stabilizedPosition |
	stabilizedPosition := paddle position.
	((paddle position y - ball position y) abs) <= (delta * paddle speed)
		ifTrue: [stabilizedPosition y: ball position y].
	^ stabilizedPosition
]
