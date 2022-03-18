Class {
	#name : #GDSWall,
	#superclass : #GodotStaticBody2D,
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSWall >> physicsProcess: delta [
	^ nil
]

{ #category : #behavior }
GDSWall >> process: delta [
	^ nil
]

{ #category : #behavior }
GDSWall >> ready [
	^ nil
]

{ #category : #behavior }
GDSWall >> reflectDirection: ball [

	| dir |
	dir := ball direction.
	dir y: dir y * -1.
	^ dir
]
