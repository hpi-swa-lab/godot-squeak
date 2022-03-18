Class {
	#name : #GDSFpsCounter,
	#superclass : #GodotLabel,
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSFpsCounter >> physicsProcess: delta [
	^ nil
]

{ #category : #behavior }
GDSFpsCounter >> process: delta [
	
	self text: 'FPS: ', engine getFramesPerSecond
]

{ #category : #behavior }
GDSFpsCounter >> ready [

	^ nil
]
