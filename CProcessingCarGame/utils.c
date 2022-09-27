//#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"


//CProcessing's origin 0,0 starts at the top left.
//Basically an over engineered utils.c file. 
//At least making buttons in the future might be easier (?) lol

//Button buffer? GUI Buffer?? Not sure what this should be called. 
//Feels like it should be an array of all GUI objects...
Button ButtonArray[100];
int buttonIndex = 0;		//index for the buttons. Used when creating and when updating
//Need to have some empty state for the buttons so that I can "remove" them
//Maybe an empty array, then inside a "remove buttons" function, I reset the index.



PhysicsObject  PhysicsObjectArray[100];
int physicsObjectIndex = 0;


CollisionData CollisionObjectsArray[100];
int collisionObjectIndex;
//TODO: What happens when buttons are destroyed? Or like need to remove from memory. hmm...
//Can I just save over? Or just --index..





//################################# || GUI BUTTON STUFF || ############################################################
void UpdateShapebutton(Button *_button)
{
	DisplayButton(_button);

	//Hacky way to change the color of the button when pressed imo. 
	CP_ColorHSL cachedColor = CP_ColorHSL_FromColor(_button->cachedColor);

	//Moving the button up when its hovered is a nice way to show it's being hovered without color.
	if (IsMouseInShapeArea(_button->shape))
	{
		//TODO: Fix the bug where the button spazzes out if you hover from below. 
		//I think you probably need to check from the cached position.
		//Edit: "fixed" by scaling the button up a little upon hover. DO NOT LOWER THE 1.12f!!!
		_button->color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, (int)(cachedColor.s * 1.1f), (int)(cachedColor.l / 1.1f), 255));
		_button->shape.transform.position.y =_button->cachedTransform.position.y - 3;		//if only you could grab the stroke weight.. anyway, 2px also to hide the bug
		_button->shape.transform.size = CP_Vector_Scale(_button->cachedTransform.size, 1.12f); //DO NOT LOWER THE 1.12f!!!
		CP_Settings_Stroke(GRAY);
		DisplayButton(_button);
	}
	else
	{
		_button->shape.transform = _button->cachedTransform;
		_button->color = CP_Color_FromColorHSL(cachedColor);
	}
	if (IsMouseClickedShapeArea(_button->shape))
	{
		_button->buttEvent();
		_button->color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, cachedColor.s / 2, cachedColor.l / 2, 255));
		CP_Settings_Stroke(BLACK);
		DisplayButton(_button);
	}
	else
	{
		_button->color = CP_Color_FromColorHSL(cachedColor);
	}
}


//Wrapper for some reason. I feel like this is correct? but at the same time it feels redundant
//The settings in the UI can also be split and refined more.

Button CreateButton(float x, float y, float sizeX, float sizeY, ShapeType shapeType, const char* text, CP_Color color,ButtonEvent buttevent)
{
	return CreateShapeButton(CreateShape(x, y, sizeX, sizeY, 0, shapeType), text, color, buttevent);
}

Button CreateShapeButton(Shape shape, const char* buttonText,CP_Color color, ButtonEvent buttEvent) 
{
	Button newButton;
	
	newButton.color = color;
	newButton.shape = shape;
	newButton.text = buttonText;
	newButton.buttEvent = buttEvent;
	newButton.cachedTransform = shape.transform;
	newButton.cachedColor = color;
	newButton.id = buttonIndex;
	ButtonArray[buttonIndex] = newButton;

	buttonIndex+=1;

	return newButton;
}

_Bool IsShapeButtonHovered(Button* _button)
{
	return IsMouseInShapeArea(_button->shape);
}

_Bool IsShapeButtonClicked(Button* _button)
{
	return(CP_Input_MouseDown(MOUSE_BUTTON_LEFT) && IsShapeButtonHovered(_button));
}

void UpdateGUI()
{
	//UpdateCameraShaker();
	if (buttonIndex == 0) return;
	for (short i = 0; i < buttonIndex; ++i)
	{
		UpdateShapebutton(&ButtonArray[i]);
	}
}

void DisplayTextInShape(Shape shape, const char* text)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	if(shape.shape == SHAPE_ELLIPSE || shape.shape == SHAPE_RECTANGLE)
	//Textbox pivots start from right to left
	CP_Font_DrawTextBox(text, shape.transform.position.x - (float)(shape.transform.size.x / 2.f), shape.transform.position.y, shape.transform.size.x);
	if(shape.shape == SHAPE_CIRCLE)
	CP_Font_DrawTextBox(text, shape.transform.position.x - (float)(shape.transform.size.x / 2.f), shape.transform.position.y- (float)(shape.transform.size.x / 2.f), shape.transform.size.x);

}
//Displays the button text, setting the color to the one specified.
//Note: Default color is BLACK. (If BLACK doesn't exist, colortable.h is missing)
void DisplayButtonText(Button *_button,CP_Color _color)
{
	CP_Settings_Fill(_color); //Color for the font, not button
	DisplayTextInShape(_button->shape, _button->text);
}
void DisplayButton(Button *button)
{
	CP_Settings_Fill(button->color);
	DisplayShape(button->shape);
	DisplayButtonText(button, BLACK);
	CP_Settings_NoStroke();
}



//which apparently you can't overload functions :c


_Bool IsMouseClickedShapeArea(Shape shape)
{
	return IsMouseInShapeArea(shape) && CP_Input_MouseDown(MOUSE_BUTTON_LEFT);
}

_Bool IsMouseInShapeArea(Shape shape)
{
	//Assuming the rect mode is set to center
	float disMouseCircle;

	_Bool IsWithinX_CENTERMODE;
	_Bool IsWithinY_CENTERMODE;
	switch (shape.shape)
	{
		case SHAPE_CIRCLE:

			disMouseCircle = Vector_Distance_Squared(shape.transform.position, GetMousePosition());

			return (disMouseCircle <= CP_Math_Square(shape.transform.size.x));

			break;
		case SHAPE_RECTANGLE:
		{	//since x is in the center of the button now, you need to add the size of X/2 to check right, and -size of X/2 to check left. 
	//same for y axis (just in case anyone actually reads this lmao)
			IsWithinX_CENTERMODE = (GetMousePosition().x >= shape.transform.position.x - (shape.transform.size.x / 2.f)) && ((GetMousePosition().x <= shape.transform.position.x + (shape.transform.size.x / 2.f)));
			IsWithinY_CENTERMODE = (GetMousePosition().y >= shape.transform.position.y - (shape.transform.size.y / 2.f)) && ((GetMousePosition().y <= shape.transform.position.y + (shape.transform.size.y / 2.f)));
			if (IsWithinX_CENTERMODE && IsWithinY_CENTERMODE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		case SHAPE_ELLIPSE:
			//Formula is ((x-h)^2/(rx^2)+(y-k)^2/(ry^2)) Where:
			//x,y = Mouse Position
			//h,k = Ellipse Position
			//rx,ry = ellipse Size
			disMouseCircle = (CP_Math_Square(GetMousePosition().x - shape.transform.position.x) / (CP_Math_Square(shape.transform.size.x/2))) + (CP_Math_Square(GetMousePosition().y - shape.transform.position.y)/(CP_Math_Square(shape.transform.size.y/2)));
			return (disMouseCircle < 1.f);	//if the dist < 1, true else > 1 false because out of the circle.
			break;
			
		default:
			return FALSE;

	}
}


Shape CreateShape(float x, float y, float sizeX, float sizeY,float rotation, ShapeType shapetype)
{
	Shape newShape;
	newShape.transform.position.x = x;
	newShape.transform.position.y = y;
	newShape.transform.size.x = sizeX;
	newShape.transform.size.y = sizeY;
	newShape.transform.rotation = rotation;
	newShape.shape = shapetype;
	return newShape;
}

void DisplayShape(Shape _shape)
{
	switch (_shape.shape)
	{
	case SHAPE_CIRCLE:
		CP_Graphics_DrawCircle(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x * 2);
		break;
	case SHAPE_RECTANGLE:
		CP_Graphics_DrawRectAdvanced(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x, _shape.transform.size.y, _shape.transform.rotation, 0);
		break;
	case SHAPE_ELLIPSE:
		CP_Graphics_DrawEllipseAdvanced(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x, _shape.transform.size.y, _shape.transform.rotation);
		break;
	default:
		break;
	}
}

void RemoveButton(Button button)
{
	//No clue how to do this for now. 
}


//##############################|| PHYSICS STUFF || #####################################################################

RigidBody2D CreateRigidBody(Collider collider, float mass, Constraints constraints)
{
	RigidBody2D rb;
	rb.constraints = constraints;
	rb.collider = collider;
	rb.mass = mass;
	rb.gravityScale = 0;
	rb.velocity = CP_Vector_Zero();
	rb.force = CP_Vector_Zero();
	//RigidBodyArray[rigidBodyIndex] = rb;
	//rigidBodyIndex += 1;
	//SetRigidbodyPointer(&rb);
	return rb;
}

Collider CreateCollider(Shape shape, _Bool isTrigger)
{
	Collider newCollider;
	newCollider.shape = shape;
	newCollider.colliderEvent = OnCollisionEnter;		
	newCollider.isTrigger = isTrigger;
	return newCollider;
}

PhysicsObject CreatePhysicsObject(float x, float y, float diameter, CP_Color color, float speed, float mass,Constraints constraints)
{
	PhysicsObject newObject;
	newObject.rigidBody = CreateRigidBody(CreateCollider(CreateShape(x, y, diameter, diameter, 0, SHAPE_CIRCLE), FALSE), mass,constraints);
	newObject.color = color;
	newObject.speed = speed;
	newObject.direction = VECTOR_UP;
	PhysicsObjectArray[physicsObjectIndex] = newObject;
	newObject.id = physicsObjectIndex;
	physicsObjectIndex += 1;
	return newObject;
}

void OnCollisionEnter()
{

}
void UpdatePhysics()
{
	if (physicsObjectIndex == 0) return;
	for (short i = 0; i < physicsObjectIndex; ++i)
	{
		UpdatePhysicsObjects(&PhysicsObjectArray[i]);
	}
	CheckCollisionsBruteForce();
	//if (collisionObjectIndex == 0) return;
	//for (short i = 0; i < collisionObjectIndex; ++i)
	//{
	//	float dist = DistanceBetweenPhysicsObject(*CollisionObjectsArray[i].ObjA, *CollisionObjectsArray[i].ObjB);
	//}
	//memset(CollisionObjectsArray, 0, sizeof(CollisionData) * 100);
	//collisionObjectIndex = 0;
}



//IMPLEMENT FIXED DELTA TIME!!!
void UpdatePhysicsObjects(PhysicsObject* obj)
{	
	//GRAVITY
	//obj->rigidBody.force = CP_VectorScale(CP_Vector_Add(obj->rigidBody.force, gravity),obj->rigidBody.gravityScale);
	//CheckCollisionsBruteForce();
	//NOTE: RK4 or Verlet can be used here
	//Velocity += Acceleration (force/mass); I think I'm using Semi Implicit Euler?
	obj->rigidBody.velocity = CP_Vector_Add(obj->rigidBody.velocity, CP_Vector_Scale(obj->rigidBody.force, 1.f / obj->rigidBody.mass));
	//Clamp the velocity to the max speed of the obj.. 
	if (CP_Vector_Length(obj->rigidBody.velocity) > obj->speed)
	{
		obj->rigidBody.velocity = CP_Vector_Scale(CP_Vector_Normalize(obj->rigidBody.velocity),obj->speed);
	}

	//"FRICTION" supposed to be -1 * u * N * mass. TODO: friction co-efficient
	CP_Vector friction = CP_Vector_Normalize(obj->rigidBody.velocity);
	friction = CP_Vector_Scale(friction, -0.02f * obj->rigidBody.mass);
	obj->rigidBody.velocity = CP_Vector_Add(obj->rigidBody.velocity, friction);

	//Stops the obj if it's too slow
	if (CP_Vector_Length(obj->rigidBody.velocity) <= 0.1f) obj->rigidBody.velocity = CP_Vector_Zero();

	obj->rigidBody.collider.shape.transform.position = CP_Vector_Add(obj->rigidBody.collider.shape.transform.position, obj->rigidBody.velocity);
	
	//CheckCollisionsBruteForce();	

	//CONSTRAINTS
	switch (obj->rigidBody.constraints)
	{
		_Bool CheckLeftBounds;
		_Bool CheckRightBounds;
		_Bool CheckTopBounds;
		_Bool CheckBottomBounds;

	case CONSTRAINTS_LEVEL:	//Assuming elasticity is MAX
		CheckLeftBounds = (obj->rigidBody.collider.shape.transform.position.x < obj->rigidBody.collider.shape.transform.size.x);
		CheckRightBounds = (obj->rigidBody.collider.shape.transform.position.x > ((float)CP_System_GetWindowWidth()-obj->rigidBody.collider.shape.transform.size.x));
		CheckTopBounds = (obj->rigidBody.collider.shape.transform.position.y < obj->rigidBody.collider.shape.transform.size.y);
		CheckBottomBounds = (obj->rigidBody.collider.shape.transform.position.y > ((float)CP_System_GetWindowHeight() - obj->rigidBody.collider.shape.transform.size.y));
		if (CheckLeftBounds)
		{
			obj->rigidBody.collider.shape.transform.position.x = obj->rigidBody.collider.shape.transform.size.x;
			obj->rigidBody.velocity = Reflect(obj->rigidBody.velocity, VECTOR_RIGHT);
			trauma += (CP_Vector_Length(obj->rigidBody.velocity)*obj->rigidBody.mass)/100.f;
		}
		if (CheckRightBounds)
		{
			obj->rigidBody.collider.shape.transform.position.x = (float)CP_System_GetWindowWidth() - obj->rigidBody.collider.shape.transform.size.x;
			obj->rigidBody.velocity = Reflect(obj->rigidBody.velocity, VECTOR_LEFT);
			trauma += (CP_Vector_Length(obj->rigidBody.velocity) * obj->rigidBody.mass) / 100.f;

		}
		if (CheckTopBounds)
		{
			obj->rigidBody.collider.shape.transform.position.y = obj->rigidBody.collider.shape.transform.size.y;
			obj->rigidBody.velocity = Reflect(obj->rigidBody.velocity, VECTOR_DOWN);
			trauma += (CP_Vector_Length(obj->rigidBody.velocity) * obj->rigidBody.mass) / 100.f;
		}
		if (CheckBottomBounds)
		{
			obj->rigidBody.collider.shape.transform.position.y = (float)CP_System_GetWindowHeight() - obj->rigidBody.collider.shape.transform.size.y;
			obj->rigidBody.velocity = Reflect(obj->rigidBody.velocity, VECTOR_UP);
			trauma += (CP_Vector_Length(obj->rigidBody.velocity) * obj->rigidBody.mass) / 100.f;
		}
		break;
	case CONSTRAINTS_NONE:
		//do nothing
		break;
	default:
		break;
	}







	//Reset force
	obj->rigidBody.force = CP_Vector_Zero();
	
	CP_Settings_Fill(obj->color);
	DisplayShape(obj->rigidBody.collider.shape);
}

float DistanceBetweenPhysicsObject(PhysicsObject ObjA, PhysicsObject ObjB)
{
	return CP_Vector_Distance(ObjA.rigidBody.collider.shape.transform.position, ObjB.rigidBody.collider.shape.transform.position);
}

_Bool IsCircleIntersecting(Shape shapeA, Shape shapeB)
{	//remove the squareroot for efficiency
	
	return (CP_Math_Square(shapeA.transform.size.x + shapeB.transform.size.x)) > Vector_Distance_Squared(shapeA.transform.position, shapeB.transform.position);
	//return ((shapeA.transform.size.x + shapeB.transform.size.x) > CP_Vector_Distance(shapeA.transform.position, shapeB.transform.position));
}


void AddForce(PhysicsObject obj, CP_Vector force, ForceMode forcemode)
{
	switch (forcemode)
	{
	case FORCEMODE_FORCE:
		//velocity += force/mass;
		PhysicsObjectArray[obj.id].rigidBody.force = CP_Vector_Add(PhysicsObjectArray[obj.id].rigidBody.force, force);
		//obj.rigidBody.force = CP_Vector_Add(PhysicsObjectArray[obj.id].rigidBody.force, force);
		break;
	case FORCEMODE_IMPULSE:
		PhysicsObjectArray[obj.id].rigidBody.force = CP_Vector_Add(PhysicsObjectArray[obj.id].rigidBody.force, CP_Vector_Scale(force, PhysicsObjectArray[obj.id].rigidBody.mass));
		break;
	default:
		break;
	}
}

void CheckCollisionsBruteForce()	//NAIVE IMPLEMENTATION. DO SWEEP AND PRUNE NEXT TIME
{
	for (int i = 0; i < physicsObjectIndex - 1; i++)
	{
		PhysicsObject* objA = &PhysicsObjectArray[i];
		for (int j = i + 1; j < physicsObjectIndex; j++)
		{
			PhysicsObject* objB = &PhysicsObjectArray[j];

			if (IsCircleIntersecting(objA->rigidBody.collider.shape, objB->rigidBody.collider.shape))
			{
				//First we do static collisions

				CP_Vector normalA = CP_Vector_Normalize(CP_Vector_Subtract(objB->rigidBody.collider.shape.transform.position, objA->rigidBody.collider.shape.transform.position));
				CP_Vector normalB = CP_Vector_Normalize(CP_Vector_Subtract(objA->rigidBody.collider.shape.transform.position, objB->rigidBody.collider.shape.transform.position));
				float overlap = fAbs(0.5f * (DistanceBetweenPhysicsObject(*objA, *objB) - (objA->rigidBody.collider.shape.transform.size.x + objB->rigidBody.collider.shape.transform.size.x)));
				objA->rigidBody.collider.shape.transform.position = CP_Vector_Add(objA->rigidBody.collider.shape.transform.position, CP_Vector_Scale(normalB, overlap));
				objB->rigidBody.collider.shape.transform.position = CP_Vector_Add(objB->rigidBody.collider.shape.transform.position, CP_Vector_Scale(normalA, overlap));

				//Add them to the collision array
				//CreateCollisionData(objA, objB);
				CP_Vector Tangent = CP_Vector_Set(-normalA.y, normalA.x);
				float dotProductTangentA = CP_Vector_DotProduct(objA->rigidBody.velocity, Tangent);
				float dotProductTangentB = CP_Vector_DotProduct(objB->rigidBody.velocity, Tangent);
				//float dotProductB = CP_Vector_DotProduct(objB->rigidBody.velocity, Tangent);
				float dotProductNormalA = CP_Vector_DotProduct(objA->rigidBody.velocity, normalA);
				float dotProductNormalB = CP_Vector_DotProduct(objB->rigidBody.velocity, normalA);
				//momentum calculations (see wikipedia : https://en.wikipedia.org/wiki/Elastic_collision)
				float m1 = (dotProductNormalA * (objA->rigidBody.mass - objB->rigidBody.mass) + 2.0f * objB->rigidBody.mass * dotProductNormalB) / (objA->rigidBody.mass + objB->rigidBody.mass);
				float m2 = (dotProductNormalB * (objB->rigidBody.mass - objA->rigidBody.mass) + 2.0f * objA->rigidBody.mass * dotProductNormalA) / (objA->rigidBody.mass + objB->rigidBody.mass);

				objA->rigidBody.velocity = CP_Vector_Add(CP_Vector_Scale(Tangent, dotProductTangentA), CP_Vector_Scale(normalA, m1));
				objB->rigidBody.velocity = CP_Vector_Add(CP_Vector_Scale(Tangent, dotProductTangentB), CP_Vector_Scale(normalA, m2));
	
				trauma += overlap*0.3f;
			}

		}
	}
}

void CreateCollisionData(PhysicsObject* ObjA, PhysicsObject* ObjB)
{
	CollisionData newCollisionData;
	newCollisionData.ObjA = ObjA;
	newCollisionData.ObjB = ObjB;
	newCollisionData.id = collisionObjectIndex;
	CollisionObjectsArray[collisionObjectIndex] = newCollisionData;
	collisionObjectIndex++;
}
//##############################|| MATHS AND HELPER FUNCTIONS ||##########################################################

//Gets the mouse input X and Y from CProcessing and puts them into a vector
CP_Vector GetMousePosition()
{
	return CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
}

//Returns the distance squared between 2 vectors (value is abs)
float Vector_Distance_Squared(CP_Vector a, CP_Vector b)
{
	float distSquared = (CP_Math_Square((b.x - a.x)) + CP_Math_Square((b.y - a.y)));
	return fAbs(distSquared);
}

CP_Vector AngleToVector(float radian_angle)
{
    // TODO 
    CP_Vector ret = CP_Vector_Zero();
    return ret;
}

//Function that returns absolute float values
float fAbs(float f)
{
	if (f < 0)
	{
		return -f;
	}
	else return f;
}

//Returns a point in a circle area at angleInDeg away from VECTOR_RIGHT of the circle.
//Note: this function is not used anymore, it's outdated
CP_Vector PointInCircle(Shape shape, float angleInDeg)
{	//Assuming (1,0) in the circle is 0deg, rotates clockwise (note, origin is top left so it is clockwise)
	CP_Vector startV = VECTOR_RIGHT;
	
	//Thank you khan academy for this LMAOOOO
	float angleInRadians = CP_Math_Radians(angleInDeg);
	float x2 = (startV.x * cosf(angleInRadians)) - ((startV.y) * (sinf(angleInRadians)));
	float y2 = (startV.x * sinf(angleInRadians)) + ((startV.y) * (cosf(angleInRadians)));
	CP_Vector resultingVector = CP_Vector_Set((x2 * (shape.transform.size.x)+shape.transform.position.x), (y2 * (shape.transform.size.x)+shape.transform.position.y));
	return resultingVector;
}

//Gets a point in the circle that is angleInDeg away from VECTOR_RIGHT
//Note: Increasing the angle returns a point in the clockwise direction from VECTOR_RIGHT
CP_Vector GetPointInCircle(Shape shape, float angleInDeg)
{
	CP_Vector circlePos = CP_Vector_Set(shape.transform.position.x, shape.transform.position.y);
	CP_Vector startV = CP_Vector_Set(shape.transform.size.x, 0);
	CP_Vector result = RotateVectorByAngle(startV, angleInDeg);
	return CP_Vector_Add(circlePos,result);
}

//Rotates a vector by angleInDeg.
//Note: Increasing the angle returns a vector in the clockwise direction from VECTOR_RIGHT
CP_Vector RotateVectorByAngle(CP_Vector vector, float angleInDeg)
{
	float vectorMagnitude = CP_Vector_Length(vector);
	CP_Vector normalisedV = CP_Vector_Normalize(vector);
	float angleInRadians = CP_Math_Radians(angleInDeg);
	//Thank you khan academy for this LMAOOOO
	float x2 = (normalisedV.x * cosf(angleInRadians) - normalisedV.y * sinf(angleInRadians));
	float y2 = (normalisedV.x * sinf(angleInRadians) + normalisedV.y * (cosf(angleInRadians)));
	CP_Vector resultVector = CP_Vector_Set(x2 * vectorMagnitude, y2 * vectorMagnitude);
	return resultVector;
}

//Draws a triangle with the 3 points and rotation in degrees for the triangle
void DrawTriangleAdvanced(CP_Vector a, CP_Vector b, CP_Vector c, float deg)
{
	CP_Settings_Fill(FULL_WHITE);
	CP_Graphics_DrawTriangleAdvanced(a.x, a.y, b.x, b.y, c.x, c.y, deg);
}

CP_Vector Reflect(CP_Vector direction, CP_Vector normal)
{
	//r = d-2(d*n)n where n is normalised, d*n is dot product
	CP_Vector reflectedVector;
	normal = CP_Vector_Normalize(normal);
	float dotProduct = CP_Vector_DotProduct(direction, normal);
	reflectedVector = CP_Vector_Subtract(direction, (CP_Vector_Scale(normal, 2.f * (dotProduct))));
	return reflectedVector;
}

//Returns float perlin noise (might not be correct, use CProcessing Perlin instead)
float PerlinNoise(int x, int y)
{
	int n;
	n = x + y + 57;
	n = (n << 13) ^ n;
	return (float)(1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}


float fMax(float a, float b)
{
	return (a >= b) ? a : b;
}
float fMin(float a, float b)
{
	return(a <= b) ? a : b;
}

//========================================= CAMERA SHAKER STUFF======================================================

_Bool cameraShakeActive = TRUE;		//Set to false to disable camera shake completely
float trauma;						//Clamped to 0 to 1, will be used to control the shaking
float traumaMult = 16;				//the POWERRR of the shake
float traumaMagnitude = 7.f;		//Range of movement when translated. 
float traumaRotationMagnitude = 17.f;		//Range of rotation (not used)
float traumaDecay = 1.3f;					//the speed at which the shake falls off

float timeCounter = 0;						//counter for time

//By right, supposed to seed it but laze
float GetPerlinFloat(float seed)
{
	return(CP_Random_Noise(seed, timeCounter,0) - .5f) * 2.f;
}

//Gets a random vector based off of perlin floats.
//Note: if you want it to be seeded, you need to create a random seed in this function for the getfloat.
CP_Vector GetRandomVector(void)
{
	//CP_Random_NoiseSeed //probably? then I can just delete the need for the seed since this sets it. Just random a number in.
	return CP_Vector_Set(GetPerlinFloat(1), GetPerlinFloat(10));
}

//Function that updates the camera shaker. Handles the translation and the decay calculations
void UpdateCameraShaker(void)
{
	trauma = CP_Math_ClampFloat(trauma, 0.f, 1.f);
	if (cameraShakeActive && trauma > 0)
	{
		timeCounter += CP_System_GetDt() * (float)pow(trauma, 0.3f) * traumaMult;
		CP_Vector newPos = CP_Vector_Scale(GetRandomVector(), traumaMagnitude * trauma);
		//float newRot = newPos.x * traumaRotationMagnitude;	//no rotation shake because camera rotates from upper left
		//CP_Settings_Rotate(newRot);
		CP_Settings_Translate(newPos.x*traumaMagnitude*trauma, newPos.y * traumaMagnitude * trauma);
		trauma -= CP_System_GetDt() * traumaDecay * (trauma + 0.3f);
	}
	else
	{
		//This is where you would normally reset the positions for the camera.
		// I do not know why CProcessing somehow doesn't need to do that?
		//CP_Settings_Rotate(0);
		//CP_Settings_Translate(0, 0);
	}
}

//Example Shake Function. Just add trauma to shake the camera
void Shake(void)
{
	trauma += 0.3f;
}
