/* Header: world
 * Description: Header file for rendering background and/or environment objects.
 */

#ifndef _WORLD_H_
#define _WORLD_H_

extern double tallY;
extern double shortY;
extern int Animate;

void worldInit();
void renderAxis();
void setBoundingBox(struct boundBox * box);
void renderBoundingBox();

#endif
