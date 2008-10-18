#ifndef KR_OPENGL_TEXTURE_INCLUDED
#define KR_OPENGL_TEXTURE_INCLUDED

#ifdef KYRA_SUPPORT_OPENGL
	#include "SDL_opengl.h"
	#define KYRA_GLuint GLuint
#else
	#define KYRA_GLuint int
#endif
#include "krmath.h"
#include "color.h"
#include "../util/gldynarray.h"
#include "../util/glmap.h" //maks

#define OGL_OFFSET 1 //maks: Use a border and OGL_OFFSET to solve artifacts in nvidia cards

/*	A wrapper to the actual OpenGL texture.
*/
class KrOglTexture
{
	friend class KrTexture;

  public:
	KrOglTexture(	const KrRGBA*, int width, int height,
					KrTRect<float>* textCoords,
					KrRect* pixelCoords, bool bRemoveBorder = false ); //maks

	~KrOglTexture();

	void AddRef()		{ ++refCount; }
	void RemoveRef()	{	--refCount;
							if ( refCount == 0 )
								delete this;
						}

	KYRA_GLuint	Id()					{ return textureId; }
	bool	Scale()					{ return scaled; }


  private:
	void SetTexture(	const KrRGBA* pixels,
						const KrRect& pixelCoords );

	void SetScaledTexture(	const KrRGBA* pixels, int w, int h );


	void	FillRow( U8* target, const KrRGBA* source, int width );
	void	CreateTexture(	U8* target, int targetW, int targetH, int targetScan,
							const KrRGBA* source, int sourceW, int sourceH );

	void	CreateScaledTexture(	U8* target, int targetW, int targetH, int targetScan,
									const KrRGBA* source, int sourceW, int sourceH );

	int		refCount;
	KYRA_GLuint  textureId;
	bool	canSetTexture;
	bool	scaled;
};


/*	A texture is presented to the Kyra engine.
	It has (opelGL) bounds, an id number, and may or may not be scaled.
	Scaled textures occur when a texture that is requested is too large,
	either because a given OpenGL system doesn't support it,
	or because KrEngine::MaxOglTextureSize() has restricted memory usage.
*/
class KrTexture
{
  public:
	KrTexture(	KrOglTexture* oglTexture,
				const KrTRect<float>& bounds,
				const KrRect& pixelBounds );

	~KrTexture();

	const KrTRect<float>& Bounds()	{ return bounds; }
	KYRA_GLuint Id()						{ return oglTexture->Id(); }
	bool   Scale()					{ return oglTexture->Scale(); }

	void SetTexture( const KrRGBA* image, int width, int height );

  private:
	KrOglTexture* oglTexture;
	KrTRect<float>	bounds;
	KrRect			pixelBounds;
};

typedef GlMap< KrTexture **, char, GlPointerHash<KrTexture **> >		MapOwnerTexture; //maks
typedef GlMapIterator< KrTexture **, char, GlPointerHash<KrTexture **> >		MapOwnerTextureIterator; //maks


/*	Singleton class to manage textures.
	Conceptually, the TextureManager returns a pointer to newly created
	Texture. The Texture is a wrapper to an OglTexture, which describes
	the actually memory held by openGL. Multiple Textures could wrap the
	same OglTexture.

	At this point, there is a one to one relationship from the Texture
	to the OglTexture, so some functionality is unused.
*/
class KrTextureManager
{
  public:
	~KrTextureManager();
	static KrTextureManager* Instance();

	// Call this to allocate and return a new texture, set up with 'data'
	KrTexture* CreateTexture( const KrRGBA* data, int width, int height, bool _bRemoveBorder = false ); //maks

	static int TextureIndex()		{ return ( instance ) ? instance->oglTextureCount : 0; }

	// When the ogl texture deletes, it warns the manager. Called by destructor.
	void OglTextureDeleting( KrOglTexture* goingAway );

	static void AddTextureOwner( KrTexture **texture ) {mapOwnerTexture.Add(texture, 1);} //maks
	static void RemoveTextureOwner( KrTexture **texture ) {mapOwnerTexture.Remove(texture);} //maks
	static void RemoveTextures(); //maks

  private:
	KrTextureManager() : oglTextureCount( 0 )		{}

	static KrTextureManager* instance;
	int oglTextureCount;

//	GlDynArray< KrOglTexture* > texArray;

	static MapOwnerTexture mapOwnerTexture; //maks
};

#endif


