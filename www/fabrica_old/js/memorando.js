var Memorando   = {
    valorTotal  : null,
    
    calcularGlosaMemorando : function( valorPorcentagemGrau, valorTotal ){   
        this.valorTotal = valorTotal !== undefined ? valorTotal : this.valorTotal;          
        var valorGlosa  = valorPorcentagemGrau * ( this.valorTotal / 100 );        
        return valorGlosa;
    }
};