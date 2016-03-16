
describe("Mapas", function() {

	var chamada = {
		estado: ['AL'],
		municipio: false
	};

	Mapas.eTeste = true;
	Mapas.getEstadoPoligono( chamada );

	it("Busca de Estado em GeoJSON - valida json", function(){
		expect( IsJson(Mapas.requisicao) ).toEqual( true );
    });

	// console.log(Mapas.requisicao);
	// console.log(Mapas.poligonos.poligono);
    it("Busca de Estado e registra nos poligonos - valida registro", function(){
		expect( Mapas.requisicao == Mapas.poligonos.poligono ).toEqual( true );
    });

    // it("Busca de Estado e atualiza no mapa - valida no mapa", function(){
		// expect( IsJson(Mapas.requisicao) ).toEqual( true ); // TODO: como?
    // });

	// it("Busca de Estado e atualiza no mapa - valida requisicao pelo form", function(){
	// 	expect( IsJson(Mapas.requisicao) ).toEqual( true ); // TODO: como?
 //    });

});


// verifica se string json é valido
function IsJson(str) {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}