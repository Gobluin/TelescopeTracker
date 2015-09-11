/*
 * Viewer.h
 *
 *  Created on: 25 авг. 2015 г.
 *      Author: shrike
 */

#ifndef VIEWER_H_
#define VIEWER_H_

class Viewer
{
	public:
		Viewer();
		virtual ~Viewer();


		void Init();

		void SetX(float&);
		void SetY(float&);
		void SetZ(float&);
		void SetTemp(float&);
		void SetData(float,float,float,float);
	protected:
		char floatBuf[11];
};

#endif /* VIEWER_H_ */
