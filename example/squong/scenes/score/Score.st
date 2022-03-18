Class {
	#name : #GDSScore,
	#superclass : #GodotLabel,
	#instVars : [
		'playerScore',
		'cpuScore'
	],
	#category : #'Godot-Scripts'
}

{ #category : #behavior }
GDSScore >> incrementCpuScore [

	cpuScore := cpuScore + 1.
	self updateText
]

{ #category : #behavior }
GDSScore >> incrementPlayerScore [

	playerScore := playerScore + 1.
	self updateText
]

{ #category : #behavior }
GDSScore >> physicsProcess: delta [
	^ nil
]

{ #category : #behavior }
GDSScore >> process: delta [
	^ nil
]

{ #category : #behavior }
GDSScore >> ready [
	
	playerScore := 0.
	cpuScore := 0.
]

{ #category : #behavior }
GDSScore >> updateText [

	self text: ('{1} - {2}' format: {playerScore. cpuScore})
]








