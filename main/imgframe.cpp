#include "imgframe.h"

ImgFrame::ImgFrame()
{
	IgnoreClip=IgnoreLight=true;
	rowVec.SetColor(255,0,0);
	colVec.SetColor(0,255,0);
	sliceVec.SetColor(0,0,255);
	origin.SetColor(255,255,255);
}

void ImgFrame::generate(){
	rowVec.DrawVektor(origin);
	colVec.DrawVektor(origin);
	sliceVec.DrawVektor(origin);

	const SGLVektor row_col=dvector(rowVec+colVec-origin);
	const SGLVektor slice_col=dvector(sliceVec+colVec-origin);
	const SGLVektor row_slice=dvector(sliceVec+rowVec-origin);
	const SGLVektor row_slice_col=dvector(row_slice+colVec-origin);

	glColor3f(0,0,0);

	glBegin(GL_LINE_STRIP);
	rowVec.DrawPureVertex();
	row_col.DrawPureVertex();
	colVec.DrawPureVertex();
	glEnd();

	glBegin(GL_LINE_STRIP);
	rowVec.DrawPureVertex();
	row_slice.DrawPureVertex();
	sliceVec.DrawPureVertex();
	glEnd();

	glBegin(GL_LINE_STRIP);
	colVec.DrawPureVertex();
	slice_col.DrawPureVertex();
	sliceVec.DrawPureVertex();
	glEnd();

	glBegin(GL_LINE_STRIP);
	row_slice.DrawPureVertex();
	row_slice_col.DrawPureVertex();
	slice_col.DrawPureVertex();
	glEnd();

	glBegin(GL_LINES);
	row_slice_col.DrawPureVertex();
	row_col.DrawPureVertex();
	glEnd();

}

SGLVektor ImgFrame::getCenter()const{
	#warning implement me
	return SGLVektor(0,0,0);
}
