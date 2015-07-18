//
// Created by Lumaio on 7/18/2015.
//

#ifndef SLIDE_ENEMIE_H
#define SLIDE_ENEMIE_H


class Enemie {
public:
	float x,y;
	int flag;
	Enemie(float x, float y, int flag) {
		this->x=x;
		this->y=y;
		this->flag=flag;
	}

	void dispose(float &diff) {
		//diff-=0.005;
	}
};


#endif //SLIDE_ENEMIE_H
