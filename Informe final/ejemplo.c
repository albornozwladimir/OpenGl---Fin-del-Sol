void mercurio (float x, float y, float z) {
	glPushMatrix();
	//Texturas
	//Transformaciones
	glRotate(theta, 0, 1, 0);
	glTranslate(x, y, z);
	//...
	glPopMatrix();
}