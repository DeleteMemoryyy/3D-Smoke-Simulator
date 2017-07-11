#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include "render.h"

#define PI			(3.1415926)

void draw_box(float box_l, float box_r, float box_d,
	float box_u, float box_f, float box_b,
	float surf_r, float surf_g, float surf_b, float surf_a,
	GLuint *NoForAll, int *t_idx)
{
	glColor4f(surf_r, surf_g, surf_b, surf_a);
	float surfaceAmbient[4] = { 0., 0., 0., 0. };
	float surfaceDiffuse[4] = { surf_r, surf_g, surf_b, surf_a };
	float surfaceSpecular[4] = { 0., 0., 0., 0. };
	float surfaceEmission[4] = { 0., 0., 0., 0. };
	float surfaceShiness = 0.;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[0]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_l, box_d, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_l, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_u, box_f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[1]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // right
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_r, box_u, box_f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[2]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // up
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_u, box_b);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[3]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // down
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_b);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[4]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // front
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[5]]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);
	glBegin(GL_POLYGON); // back
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_b);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_b);
	glEnd();
}

void draw_sphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat aa, GLfloat bb, GLfloat cc, GLfloat M, GLfloat N)
{
	float step_z = PI / M;
	float step_xy = 2 * PI / N;
	float x[4], y[4], z[4];
	float angle_z = 0.0;
	float angle_xy = 0.0;
	int i = 0, j = 0;


	for (i = 0; i < M; i++)
	{
		angle_z = i * step_z;

		for (j = 0; j < N; j++)
		{
			angle_xy = j * step_xy;
			x[0] = aa * sin(angle_z) * cos(angle_xy);
			y[0] = bb * sin(angle_z) * sin(angle_xy);
			z[0] = cc * cos(angle_z);
			x[1] = aa * sin(angle_z + step_z) * cos(angle_xy);
			y[1] = bb * sin(angle_z + step_z) * sin(angle_xy);
			z[1] = cc * cos(angle_z + step_z);
			x[2] = aa * sin(angle_z + step_z) * cos(angle_xy + step_xy);
			y[2] = bb * sin(angle_z + step_z) * sin(angle_xy + step_xy);
			z[2] = cc * cos(angle_z + step_z);
			x[3] = aa * sin(angle_z) * cos(angle_xy + step_xy);
			y[3] = bb * sin(angle_z) * sin(angle_xy + step_xy);
			z[3] = cc * cos(angle_z);
			glBegin(GL_POLYGON);
			glNormal3f(sin(angle_z) * cos(angle_xy) / aa, sin(angle_z) * sin(angle_xy) / bb, cos(angle_z) / cc);

			for (int k = 0; k < 4; k++)
			{
				glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
			}
			glEnd();
		}
	}
	glEnd();
}

void Draw_Box(float box_l, float box_r, float box_d,
	float box_u, float box_f, float box_b,
	float surf_r, float surf_g, float surf_b, float surf_a,
	float line_r, float line_g, float line_b, float line_a,
	GLuint* NoForAll, int *t_idx)
{
	glDisable(GL_TEXTURE_2D);

	float surfaceAmbient[4] = { 0., 0., 0., 0. };
	float surfaceDiffuse[4] = { 0.1, 0.1, 0.1, 0.1 };
	float surfaceSpecular[4] = { 0.1, 0.1, 0.1, 0.1 };
	float surfaceEmission[4] = { 0., 0., 0., 0. };
	float surfaceShiness = 1.;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surfaceEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, surfaceShiness);

	glColor4f(line_r, line_g, line_b, line_a);
	glBegin(GL_LINES);
	glVertex3f(box_l, box_u, box_b); glVertex3f(box_l, box_u, box_f);
	glVertex3f(box_l, box_d, box_b); glVertex3f(box_l, box_d, box_f);
	glVertex3f(box_l, box_u, box_b); glVertex3f(box_l, box_d, box_b);
	glVertex3f(box_l, box_u, box_f); glVertex3f(box_l, box_d, box_f);
	glVertex3f(box_r, box_u, box_b); glVertex3f(box_r, box_u, box_f);
	glVertex3f(box_r, box_d, box_b); glVertex3f(box_r, box_d, box_f);
	glVertex3f(box_r, box_u, box_b); glVertex3f(box_r, box_d, box_b);
	glVertex3f(box_r, box_u, box_f); glVertex3f(box_r, box_d, box_f);
	glVertex3f(box_l, box_u, box_b); glVertex3f(box_r, box_u, box_b);
	glVertex3f(box_l, box_d, box_b); glVertex3f(box_r, box_d, box_b);
	glVertex3f(box_l, box_u, box_f); glVertex3f(box_r, box_u, box_f);
	glVertex3f(box_l, box_d, box_f); glVertex3f(box_r, box_d, box_f);
	glEnd();

	glColor4f(0, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[0]]);
	glBegin(GL_POLYGON);				// left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_l, box_d, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_l, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_u, box_f);
	glEnd();
	glColor4f(surf_r, surf_g, surf_b, surf_a);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[1]]);
	glBegin(GL_POLYGON);				// right
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_r, box_u, box_f);
	glEnd();
	glColor4f(surf_r, surf_g, surf_b, surf_a);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[2]]);
	glBegin(GL_POLYGON);				// up
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_u, box_b);
	glEnd();
	glColor4f(0, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[3]]);
	glBegin(GL_POLYGON);				// down
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_d, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_b);
	glEnd();
	glColor4f(surf_r, surf_g, surf_b, surf_a);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[4]]);
	glBegin(GL_POLYGON);				// front
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_f);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_f);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_f);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_f);
	glEnd();
	glColor4f(0, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, NoForAll[t_idx[5]]);
	glBegin(GL_POLYGON);				// back
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(box_l, box_u, box_b);
	glTexCoord2f(0, 1);
	glVertex3f(box_r, box_u, box_b);
	glTexCoord2f(1, 1);
	glVertex3f(box_r, box_d, box_b);
	glTexCoord2f(1, 0);
	glVertex3f(box_l, box_d, box_b);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void grab(int Width, int Height)
{
	FILE*    pDummyFile;  //ָ����һbmp�ļ������ڸ��������ļ�ͷ����Ϣͷ����  
	FILE*    pWritingFile;  //ָ��Ҫ�����ͼ��bmp�ļ�  
	GLubyte* pPixelData;    //ָ���µĿյ��ڴ棬���ڱ����ͼbmp�ļ�����  
	GLubyte  BMP_Header[54];
	GLint    i, j;
	GLint    PixelDataLength;   //BMP�ļ������ܳ���  

								// �����������ݵ�ʵ�ʳ���  
	i = Width * 3;   // �õ�ÿһ�е��������ݳ���  
	while (i % 4 != 0)      // �������ݣ�ֱ��i�ǵı���  
		++i;
	PixelDataLength = i * Height;  //��������λ��  

									 // �����ڴ�ʹ��ļ�  
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pDummyFile = fopen("dummy.bmp", "rb");//ֻ����ʽ��  
	if (pDummyFile == 0)
		exit(0);

	pWritingFile = fopen("grab.bmp", "wb"); //ֻд��ʽ��  
	if (pWritingFile == 0)
		exit(0);

	//�Ѷ����bmp�ļ����ļ�ͷ����Ϣͷ���ݸ��ƣ����޸Ŀ������  
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //��ȡ�ļ�ͷ����Ϣͷ��ռ��54�ֽ�  
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //�ƶ���0X0012����ָ��ͼ���������ڴ�  
	i = Width;
	j = Height;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// ��ȡ��ǰ������ͼ�����������  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //����4λ���뷽ʽ  
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, Width, Height,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// д����������  
	fseek(pWritingFile, 0, SEEK_END);
	//��������BMP�ļ�����д��pWritingFile  
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// �ͷ��ڴ�͹ر��ļ�  
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}
