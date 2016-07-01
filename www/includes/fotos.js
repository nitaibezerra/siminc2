//
// $Id$
//



/**
 * 
 */
var Foto = Class.create(
{
    /**
     * constructor
     */
    initialize: function (url, entidade, camposObrigatorios)
    {
        this._fotos = [];
        this._index = null;
        this._total = 0;
    },


    /**
     * 
     */
    next: function()
    {
        if (this._index < this._fotos.length) {
            this._index += 1;
        }

        return this._index;
    },


    /**
     * 
     */
    previous: function()
    {
        if (this._index > 0) {
            this._index -= 1;
        }

        return this._index;
    },


    /**
     * 
     */
    show: function()
    {
        alert(this._index);
    }
}


var Fotos = new Foto();





