
#ifndef _UIGLFLOWCOVER_H_
#define _UIGLFLOWCOVER_H_

#include <math.h>
#define VISABLE_ITEM_NUM	(3)	//visible on screen 
#define SPREAD_IMAGE		(0.1)	//spread between images (screen measured from -1 to 1)
#define FLANK_SPREAD		(0.4)	//flank spread out; this is how much an image moves way from center

const GLfloat GFlowCoverVertices[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,	1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

const GLshort GFlowCoverTextures[] = {
	0, 1,
	1, 1,
	0, 0,
	1, 0,
};


class UIGLFlowCover

{

public:
	//image
	GLuint  _textureImage[6];
	GLint   _textureImage_Width[6];
	GLint   _textureImage_Height[6];

	UIGLFlowCover(double width, double height)
	{
		_offset = 0;
		_aspect = width/height; 
	}

	void Draw()
	{
		glPushMatrix();
		//////////////////////////////////////////////////////////////////////////
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glScalef(1, _aspect, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		_offset += 0.1f;
		if (_offset >=30)
		{
			_offset = 0;
		}
		
		int itemMax = 6*5;

		int mid = (int)floor(_offset + 0.5f);

		int iBeginPos = mid - VISABLE_ITEM_NUM;
		if (iBeginPos<0) iBeginPos = 0;

		for (int i=iBeginPos; i<mid; i++)
		{
			DrawItem(i, i-_offset);
		}


		int iEndPos = mid + VISABLE_ITEM_NUM;
		if (iEndPos >= itemMax) iEndPos = itemMax - 1;

		for (int i=iEndPos; i>=mid; --i)
		{
			DrawItem(i, i-_offset);
		}
		//////////////////////////////////////////////////////////////////////////
		glPopMatrix();
	}
protected:

private:
	//pos
	float	_offset; // FlowCover param for mouse hoz scroll 
	double	_aspect;

	void DrawItem(int index, double atOffset)
	{
		int imgIndex = index % 6;

		/*	
		0 1 2 3
		4 5 6 7
		8 9 A B
		C D E F
		*/
		GLfloat matrix[16];
		memset(matrix, 0, sizeof(matrix));
		matrix[0] = 1;
		matrix[5] = 1;
		matrix[10] = 1;
		matrix[15] = 1;
		
		double trans = atOffset * SPREAD_IMAGE;
		double f = atOffset * FLANK_SPREAD;

		if (f < -FLANK_SPREAD) {
			f = -FLANK_SPREAD;
		}else if (f > FLANK_SPREAD) {
			f = FLANK_SPREAD;
		}

		matrix[3] = -f;
		matrix[0] = 1-fabs(f);

		double scale = 0.5 * matrix[0];
		trans+= f * 1;

		glPushMatrix();
		glTranslatef(trans, 0, 0);
		glScalef(scale, scale, 1.0f);
		glMultMatrixf(matrix);		
		glColor4f(1.0,1.0,1.0,1.0);


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureImage[imgIndex]);

		glEnableClientState( GL_VERTEX_ARRAY ); 
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glVertexPointer(3, GL_FLOAT, 0, GFlowCoverVertices);		
		glTexCoordPointer(2, GL_SHORT, 0, GFlowCoverTextures);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


		//Reflect		
		glTranslatef(0, -2, 0);
		glScalef(1, -1, 1);
		glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);		
		glDisableClientState( GL_VERTEX_ARRAY ); 
		glDisableClientState( GL_TEXTURE_COORD_ARRAY ); 
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
};

#endif