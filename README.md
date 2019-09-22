# HL2GravityGunPrototype
A 48h project made in Unreal Engine 4.18, where I created a prototype for Half Life 2's gravity gun.

Let’s start with a “How to use” first:

-	The main map that I used for testing is /Maps/FirstPersonExampleMap.
-	They’re will be a FirstPersonCharacter in the map that you’ll be using.
-	You start off with a Gravity Gun in your hand. Here’s how to use it: 
*	Left click is the Primary fire. This would knock away objects where you aim. If you aim at nothing (like the sky), 
nothing will happen.
*	Right click is the Secondary fire. If a pickable object is in front of you (aim at it), and is in range, you’ll grab it. 
That object will hover in front of you. Use either left click to launch it forward, or right click to release it. 
You can’t pick up static object or very heavy objects (like most of the boxes in the level). If you try to grab an object 
that is out of range, but still close enough, and it is visible, the object(s) will be pulled until you grab it, 
if you keep the right mouse button pressed. If the object you’re aiming at is too far away, nothing will happen.
o	Use “F” to pick up a weapon if you don’t have one. You can drop the weapon you’re holding by pressing “F” too. 
If you drop the weapon while you have an object grabbed, that object will be detached as a consequence of dropping the weapon. 
-	Play around with the environment, and try different cases like a grabbable object behind a wall, and try to pull it etc.
-	There are a lot of customization options in /Core/Weapons/BP_GravityGun, and in /Core/Characters/FirstPersonCharacter, 
that you can play around with.
