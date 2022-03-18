Class {
	#name : #GDSGoal,
	#superclass : #GodotArea2D,
	#category : #'Godot-Scripts'
}

{ #category : #'as yet unclassified' }
GDSGoal class >> signals [

	^ #(score)
]

{ #category : #behavior }
GDSGoal >> onBodyEntered: body [

	(body isInGroup: 'ball')
		ifTrue: [self emitSignal: 'score']
		ifFalse: [self error]
]

{ #category : #behavior }
GDSGoal >> physicsProcess: delta [
	^ nil
]

{ #category : #behavior }
GDSGoal >> process: delta [
	^ nil
]

{ #category : #behavior }
GDSGoal >> ready [
	^ nil
]


