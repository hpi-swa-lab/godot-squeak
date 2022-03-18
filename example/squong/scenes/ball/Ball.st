Class {
	#name : #GDSBall,
	#superclass : #GodotKinematicBody2D,
	#instVars : [
		'dir',
		'speed',
		'direction',
		'paused',
		'red',
		'blue'
	],
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSBall >> direction [

	^ direction
]

{ #category : #behavior }
GDSBall >> direction: aGDVector2 [

	direction := aGDVector2
]

{ #category : #behavior }
GDSBall >> extents [

	^ (self getNodePath: 'CollisionShape2D') getShape extents
]

{ #category : #behavior }
GDSBall >> paused [
	<export: 'true'>
	
	^ paused
]

{ #category : #behavior }
GDSBall >> paused: aBoolean [

	paused := aBoolean
]

{ #category : #behavior }
GDSBall >> physicsProcess: delta [
	
	self paused ifTrue: [^ self].
	
	(self moveAndCollideRelVec: self direction * self speed * delta)
		ifNotNil: [:collision |
			self direction: (collision collider reflectDirection: self)]
]

{ #category : #behavior }
GDSBall >> ready [
	
	self reset
]

{ #category : #behavior }
GDSBall >> reset [
	
	self position: self getViewport size / 2.0.
	self direction: (GDVector2 newX: #(-1 1) atRandom y: #(-1 1) atRandom) normalized.
]

{ #category : #behavior }
GDSBall >> speed [
	<export: '200.0'>
	
	^ speed
]

{ #category : #behavior }
GDSBall >> speed: aNumber [

	speed := aNumber
]
