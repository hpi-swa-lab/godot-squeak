Class {
	#name : #GDSPaddle,
	#superclass : #GodotKinematicBody2D,
	#instVars : [
		'speed',
		'isPlayer',
		'controller',
		'red',
		'blue',
		'paused'
	],
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSPaddle >> ball [

	^ (self getTree getNodesInGroup: 'ball') getIdx: 0
]

{ #category : #behavior }
GDSPaddle >> collidesVerticallyWith: ball [

	^ (self extents y + ball extents y) < ((self position y - ball position y) abs + 1)
]

{ #category : #behavior }
GDSPaddle >> extents [

	^ (self getNodePath: 'Area2D/CollisionShape2D') getShape extents
]

{ #category : #behavior }
GDSPaddle >> isPlayer [
	<export: 'true'>
	
	^ isPlayer
]

{ #category : #behavior }
GDSPaddle >> isPlayer: aBoolean [

	isPlayer := aBoolean
]

{ #category : #behavior }
GDSPaddle >> maxReflectionAngle [

	^ Float pi * 0.3
]

{ #category : #behavior }
GDSPaddle >> paused [
	<export: 'true'>
	
	^ paused
]

{ #category : #behavior }
GDSPaddle >> paused: aBoolean [

	paused := aBoolean
]

{ #category : #behavior }
GDSPaddle >> physicsProcess: delta [
	| dir |
	self paused ifTrue: [^ nil].
	self halt.
	
	dir := GDVector2 newX: 0 y: self verticalDirection.
	self moveAndSlideLinearVelocity: dir * self speed.
	self stabilizePosition: delta
]

{ #category : #behavior }
GDSPaddle >> ready [

	| controllerPath |
	
	controllerPath := self isPlayer
		ifTrue: ['res://scenes/paddle/PlayerPaddleController.st']
		ifFalse: ['res://scenes/paddle/CpuPaddleController.st'].
	controller := (resourceLoader loadPath: controllerPath) new.
	
	controller paddle: self.
]

{ #category : #behavior }
GDSPaddle >> reflectDirection: ball [

	^ (self collidesVerticallyWith: ball)
		ifTrue: [self reflectDirectionVertical: ball]
		ifFalse: [self reflectDirectionHorizontal: ball]
]

{ #category : #behavior }
GDSPaddle >> reflectDirectionHorizontal: ball [

	| dir angleStrength verticalDirection |
	angleStrength := (self position y - ball position y) abs / self extents y
		clampLow: 0
		high: 1.
	dir := (GDVector2 newX: 1 y: 0) rotatedPhi: self maxReflectionAngle * angleStrength.
	
	dir x: dir x * ball direction x sign * -1.
	
	verticalDirection := (ball position y - self position y) sign.
	verticalDirection = 0 ifTrue: [verticalDirection := 1.0].
	dir y: dir y * verticalDirection.
	
	^ dir
]

{ #category : #behavior }
GDSPaddle >> reflectDirectionVertical: ball [

	| dir |
	dir := ball direction.
	dir y: dir y * -1.
	^ dir
]

{ #category : #behavior }
GDSPaddle >> speed [
	<export: '200.0'>

	^ speed
]

{ #category : #behavior }
GDSPaddle >> speed: aNumber [

	speed := aNumber
]

{ #category : #behavior }
GDSPaddle >> stabilizePosition: delta [
	
	self position: (controller stabilizedPositionDelta: delta ball: self ball)
]

{ #category : #behavior }
GDSPaddle >> verticalDirection [
	
	^ controller computeVerticalDirectionFor: self ball
]
