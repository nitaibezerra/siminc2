
/**
 * Objeto para lidar com mapas com Gmaps e Google Maps
 * 
 * @author Sávio Resende <savio@savioresende.com.br>
 * @link http://hpneo.github.io/gmaps Documentation of Gmaps
 */
var Mapas = {

	versao: 'beta',
	poligonos: [],
	fundoBrasil: '',
	idTag: '',
	eTeste: false, // encapsulamento da requisicao ajax para teste jasmine
	poligonosDesenhados: [],
	posLegenda: '', // acao a ser executada apos a aplicacao da legenda
	posDesenhoPoligono: '', // acao a ser executada apos o desenho do poligono
	estilo: 'default',
	orgid: '',
	popup: null,
	poligonosCentro: [],
	marcadores: [],
	casa: 0,

	// centros dos estados para as siglas (não é o centro real)
	centroEstados: [
		[ "AC", -11.615191, -70.726764 ],
		[ "AL", -11.458018, -36.478033 ],
		[ "AP", 1.311648, -53.146021 ],
		[ "AM", -4.070592, -65.601665 ],
		[ "BA", -13.725678, -42.310651 ],
		[ "CE", -5.967653, -40.387462 ],
		[ "DF", -16.342755, -49.362535 ],
		[ "ES", -21.396513, -40.909884 ],
		[ "GO", -17.710158, -51.289962 ],
		[ "MA", -5.618566, -46.027267 ],
		[ "MT", -12.256661, -57.013594 ],
		[ "MS", -21.893160, -55.651290 ],
		[ "MG", -19.939395, -46.203048 ],
		[ "PR", -26.209254, -52.443282 ],
		[ "PB", -8.353385, -37.564680 ],
		[ "PA", -5.924623, -52.772872 ],
		[ "PE", -9.670559, -37.039962 ],
		[ "PI", -7.582897, -43.401534 ],
		[ "RJ", -24.496119, -43.461959 ],
		[ "RN", -6.619917, -36.794655 ],
		[ "RS", -30.470374, -54.874377 ],
		[ "RO", -11.665184, -65.119690 ],
		[ "RR", 1.838791, -63.260181 ],
		[ "SC", -28.590937, -51.576237 ],
		[ "SE", -12.687295, -37.875421 ],
		[ "SP", -23.927472, -49.663507 ],
		[ "TO", -11.755162, -50.060382 ]
	],
	buscaMunicipioSuporte: [],

	/**
	 * Método que tem o objetivo de mostrar o caminho 
	 * sequencial da execução do JS, para que se possa
	 * debugar melhor. O resultado é apresentado no console.
	 */
	mostraPosicao: function( parte ){
		// console.log(parte);
	},

	/**
	 * Inicializa mapa
	 *
	 * @param idTag - id do tag html
	 */
	inicializar: function( idTag ){
		this.mostraPosicao( 'inicializar' );

		var that = this;
		this.idTag = idTag;
		// console.log('inicio');

		switch( this.estilo ) {
			case 'externo_blank':

				var maxZoomValue = that.trataZoom();
				var zoomInicio = that.trataZoomInicial();

			    this.map = new GMaps({
					div: idTag,
					mapTypeId: 'blank',
					lat: -14.689881, 
					lng: -52.373047,
					zoom: zoomInicio,
					zoomControl: true
				});
			    break;
			case 'externo_blank_pais':

				this.map = new GMaps({
					div: idTag,
					mapTypeId: 'blank',
					lat: -15.389881, 
					lng: -54.373047,
					zoom: 4,
					zoomControl: true,
					scrollwheel: false,
					draggable: false,
					disableDoubleClickZoom: true,
					disableDefaultUI: true
				});
				setTimeout(function(){
					that.aplicaTituloEstados();
				},500);
				break;
			default:
				this.map = new GMaps({
					div: idTag,
					lat: -15.689881, 
					lng: -52.373047,
					zoom: 4,
					mapTypeId: google.maps.MapTypeId.ROADMAP
				});
		}

		this.poligonosDesenhados = [];
	},

	/**
	 * Tratamento de Zoom de acordo com o estado
	 */
	trataZoom: function(){
		this.mostraPosicao( 'trataZoom' );

		switch( jQuery('#uf').val() ){
			case 'SE':
			case 'AL':
			case 'PE':
			case 'PB':
			case 'RN':
			case 'SP':
				return 9;
				break;
			case 'BA':
			case 'MA':
			case 'MG':
			case 'MS':
			case 'PA':
			case 'PI':
				return 8;
				break;
			case 'AM':
			case 'MT':
			case 'TO':
				return 7;
				break;
			default:
				return 8
				break;
		}
	},

	/**
	 * Tratamento de Zoom de acordo com o estado
	 */
	trataZoomInicial: function(){
		this.mostraPosicao( 'trataZoomInicial' );

		switch( jQuery('#uf').val() ){
			case 'SE':
			case 'AL':
			case 'PE':
			case 'PB':
			case 'RN':
			case 'SP':
				return 9;
				break;
			case 'BA':
			case 'MA':
			case 'MG':
			case 'MS':
			case 'PA':
			case 'PI':
				return 8;
				break;
			case 'AM':
			case 'MT':
			case 'TO':
				return 7;
				break;
			case 'RR':
				return 6;
				break;
			default:
				return 8
				break;
		}
	},


	/**
	 * Busca gerada por um form de multiselect para mostrar no mapa multiplos estados e multiplos tipos de municipíos
	 *
	 * @param estuf - id do tag html
	 */
	buscaEstadoForm: function( estuf, tpmid, muncod, origemRequisicao ){
		this.mostraPosicao( 'buscaEstadoForm' );

		var selecionados = jQuery(estuf).val();
        var selTpmid = jQuery(tpmid).val();
        var selMuncod = jQuery(muncod).val();
		this.origemRequisicao = origemRequisicao;

		if( selecionados == null && tpmid == null ){
			this.poligonos.poligono = '';
			this.atualizaMapa();
		}else{
			var requisicao = {
				estado: selecionados,
                tpmid: selTpmid,
                muncod: selMuncod,
				municipio: true
			};

			if( origemRequisicao == 'organizacoesterritoriais' ){
				requisicao.orgao = jQuery('#orgao').val();
			}
			this.getEstadoPoligono( requisicao );
		}
	},

    /**
     * Busca gerada por um form de multiselect para mostrar no mapa municípios por tipo de municípios
     *
     * @param tpmid - id do tag html
     */
    buscaMunicipioPorTipo: function(tpmid, origemRequisicao){
        this.mostraPosicao( 'buscaMunicipioPorTipo' );

        var selecionados = jQuery(tpmid).val();
        //console.log(selecionados+": Teste");
        this.origemRequisicao = origemRequisicao;

        if( selecionados == null ){
            this.poligonos.poligono = '';
            this.atualizaMapa();
        }else{
            var requisicao = {
                tpmid: selecionados
            };

            if( origemRequisicao == 'organizacoesterritoriais' ){
                requisicao.orgao = jQuery('#orgao').val();
            }
            this.getEstadoPoligono( requisicao );
        }
    },

	/**
	 * Resgata poligono de estados passados
	 *
	 * Formato aceito para @param chamada:
	 *     var chamada = { estado: [], municipio: [] };
	 *	ou
	 *	   var chamada = { false, municipio: [] };
	 * obs.: lembrando que "[]" é array js, e "{}" é objeto.
	 */
	getEstadoPoligono: function( chamada ){
		this.mostraPosicao( 'getEstadoPoligono' );

		var that = this;
		this.chamada = chamada;

		var datad = new Date();
		var nomeTemporario = datad.getDay() + '' + datad.getMonth() + '' + datad.getYear() + '' + datad.getHours() + '' + datad.getMinutes() + '' + datad.getSeconds() + '' + datad.getMilliseconds();
		jQuery( this.idTag+'txt' ).append( "<div style='position:absolute;line-height:34px;z-index:8;' id='"+nomeTemporario+"'><img style='width:20px;margin-right:5px;' src='../imagens/carregando.gif'/>Carregando...</div>" );

		// encapsulamento da requisicao ajax para teste jasmine
		if( this.eTeste == true ){ 
			this.requisicao = '[{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.36,-10.25],[-36.27,-10.28],[-36.3,-10.34],[-36.39,-10.31],[-36.4,-10.25],[-36.36,-10.25]]]]},"muncod":"2702702","mundescricao":"Feliz Deserto","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.54,-9.88],[-36.69,-9.8],[-36.76,-9.71],[-36.69,-9.63],[-36.64,-9.64],[-36.57,-9.71],[-36.54,-9.88]]]]},"muncod":"2700300","mundescricao":"Arapiraca","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.03,-9.4],[-37.1,-9.46],[-37.15,-9.44],[-37.05,-9.31],[-36.98,-9.36],[-37.03,-9.4]]]]},"muncod":"2702504","mundescricao":"Dois Riachos","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36,-9.58],[-36.18,-9.61],[-36.23,-9.51],[-36.21,-9.46],[-36.15,-9.47],[-35.93,-9.37],[-35.93,-9.48],[-36,-9.58]]]]},"muncod":"2700409","mundescricao":"Atalaia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.5,-9.42],[-35.52,-9.44],[-35.55,-9.4],[-35.66,-9.4],[-35.69,-9.37],[-35.52,-9.35],[-35.51,-9.31],[-35.46,-9.35],[-35.5,-9.42]]]]},"muncod":"2700508","mundescricao":"Barra de Santo Antonio","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.05,-9.42],[-36.16,-9.47],[-36.11,-9.31],[-36.18,-9.31],[-36.18,-9.21],[-36.07,-9.28],[-36.05,-9.33],[-36,-9.33],[-36.01,-9.4],[-36.05,-9.42]]]]},"muncod":"2701704","mundescricao":"Capela","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.58,-9.7],[-36.63,-9.63],[-36.62,-9.58],[-36.57,-9.56],[-36.54,-9.66],[-36.58,-9.7]]]]},"muncod":"2702009","mundescricao":"Coite do Noia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.66,-8.92],[-35.71,-8.98],[-35.68,-9.03],[-35.83,-8.98],[-35.82,-8.86],[-35.78,-8.86],[-35.75,-8.92],[-35.66,-8.88],[-35.66,-8.92]]]]},"muncod":"2702108","mundescricao":"Colonia Leopoldina","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.77,-9.64],[-35.8,-9.68],[-35.87,-9.63],[-35.83,-9.64],[-35.79,-9.6],[-35.77,-9.64]]]]},"muncod":"2702207","mundescricao":"Coqueiro Seco","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.18,-10.2],[-36.27,-10.28],[-36.39,-10.24],[-36.34,-10.17],[-36.45,-10.04],[-36.3,-10.03],[-36.28,-9.94],[-36.12,-9.93],[-36.03,-10.05],[-36.18,-10.2]]]]},"muncod":"2702306","mundescricao":"Coruripe","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.04,-9.81],[-37.22,-9.71],[-37.16,-9.6],[-37.05,-9.62],[-37.07,-9.66],[-36.99,-9.72],[-37.04,-9.81]]]]},"muncod":"2700706","mundescricao":"Batalha","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.44,-9.59],[-36.5,-9.58],[-36.52,-9.5],[-36.49,-9.5],[-36.44,-9.59]]]]},"muncod":"2700805","mundescricao":"Belem","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.11,-9.87],[-37.13,-9.91],[-37.23,-9.9],[-37.31,-9.8],[-37.22,-9.71],[-37.04,-9.81],[-37.11,-9.87]]]]},"muncod":"2700904","mundescricao":"Belo Monte","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.11,-9.7],[-36.25,-9.73],[-36.26,-9.6],[-36.1,-9.64],[-36.07,-9.69],[-36.11,-9.7]]]]},"muncod":"2701001","mundescricao":"Boca da Mata","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.84,-9.47],[-36.84,-9.53],[-36.98,-9.49],[-37.02,-9.37],[-36.83,-9.39],[-36.84,-9.47]]]]},"muncod":"2701209","mundescricao":"Cacimbinhas","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.16,-9.47],[-36.21,-9.45],[-36.18,-9.31],[-36.11,-9.31],[-36.16,-9.47]]]]},"muncod":"2701308","mundescricao":"Cajueiro","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.54,-8.86],[-35.5,-8.89],[-35.5,-8.95],[-35.58,-8.91],[-35.58,-8.86],[-35.54,-8.82],[-35.54,-8.86]]]]},"muncod":"2701357","mundescricao":"Campestre","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.9,-9.87],[-36.03,-9.81],[-36,-9.77],[-35.91,-9.81],[-35.86,-9.79],[-35.9,-9.87]]]]},"muncod":"2700607","mundescricao":"Barra de Sao Miguel","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.21,-9.81],[-36.2,-9.84],[-36.24,-9.87],[-36.18,-9.89],[-36.28,-9.9],[-36.39,-9.77],[-36.32,-9.71],[-36.24,-9.73],[-36.19,-9.79],[-36.21,-9.81]]]]},"muncod":"2701407","mundescricao":"Campo Alegre","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.39,-9.32],[-37.59,-9.29],[-37.48,-9.21],[-37.26,-9.16],[-37.23,-9.23],[-37.41,-9.29],[-37.39,-9.32]]]]},"muncod":"2704609","mundescricao":"Maravilha","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.84,-9.77],[-35.91,-9.81],[-35.99,-9.78],[-35.99,-9.7],[-36.02,-9.65],[-35.98,-9.67],[-35.98,-9.64],[-35.87,-9.63],[-35.86,-9.65],[-35.82,-9.66],[-35.78,-9.7],[-35.84,-9.77]]]]},"muncod":"2704708","mundescricao":"Marechal Deodoro","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.23,-9.52],[-36.19,-9.6],[-36.26,-9.6],[-36.25,-9.62],[-36.38,-9.57],[-36.39,-9.51],[-36.26,-9.52],[-36.22,-9.47],[-36.23,-9.52]]]]},"muncod":"2704807","mundescricao":"Maribondo","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.8,-10.14],[-36.71,-10.05],[-36.69,-9.96],[-36.65,-9.99],[-36.68,-10.13],[-36.74,-10.14],[-36.72,-10.26],[-36.85,-10.19],[-36.8,-10.14]]]]},"muncod":"2707503","mundescricao":"Porto Real do Colegio","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.4,-9.51],[-36.45,-9.51],[-36.46,-9.45],[-36.39,-9.42],[-36.34,-9.46],[-36.34,-9.51],[-36.4,-9.51]]]]},"muncod":"2704906","mundescricao":"Mar Vermelho","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.64,-9.11],[-37.72,-9.2],[-37.84,-9.22],[-37.95,-9.11],[-37.84,-8.98],[-37.82,-8.89],[-37.76,-8.86],[-37.68,-8.97],[-37.7,-8.99],[-37.64,-9.02],[-37.53,-8.96],[-37.64,-9.11]]]]},"muncod":"2705002","mundescricao":"Mata Grande","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.61,-9.61],[-36.61,-9.65],[-36.69,-9.63],[-36.7,-9.58],[-36.81,-9.56],[-36.84,-9.51],[-36.57,-9.49],[-36.53,-9.53],[-36.62,-9.58],[-36.61,-9.61]]]]},"muncod":"2703106","mundescricao":"Igaci","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.56,-10.18],[-36.69,-10.28],[-36.72,-10.26],[-36.74,-10.14],[-36.68,-10.13],[-36.66,-10.03],[-36.52,-10.04],[-36.49,-10.07],[-36.51,-10.15],[-36.56,-10.18]]]]},"muncod":"2703205","mundescricao":"Igreja Nova","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.65,-9.41],[-37.84,-9.21],[-37.62,-9.19],[-37.59,-9.3],[-37.65,-9.41]]]]},"muncod":"2703304","mundescricao":"Inhapi","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.17,-9.64],[-37.24,-9.74],[-37.3,-9.64],[-37.25,-9.63],[-37.26,-9.58],[-37.16,-9.61],[-37.17,-9.64]]]]},"muncod":"2703403","mundescricao":"Jacare dos Homens","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.18,-8.97],[-35.26,-9.07],[-35.36,-8.98],[-35.35,-8.86],[-35.17,-8.89],[-35.18,-8.97]]]]},"muncod":"2704500","mundescricao":"Maragogi","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.05,-9.62],[-37.16,-9.6],[-37.1,-9.46],[-37.03,-9.39],[-36.98,-9.49],[-36.84,-9.53],[-36.88,-9.6],[-37.05,-9.62]]]]},"muncod":"2704401","mundescricao":"Major Isidoro","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.41,-8.84],[-35.34,-8.87],[-35.38,-8.97],[-35.42,-8.92],[-35.45,-8.96],[-35.49,-8.94],[-35.55,-8.84],[-35.47,-8.81],[-35.41,-8.84]]]]},"muncod":"2703502","mundescricao":"Jacuipe","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.29,-9.14],[-35.33,-9.12],[-35.34,-9.03],[-35.24,-9.07],[-35.29,-9.14]]]]},"muncod":"2703601","mundescricao":"Japaratinga","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.75,-9.69],[-36.78,-9.73],[-36.91,-9.62],[-36.83,-9.55],[-36.7,-9.58],[-36.69,-9.64],[-36.75,-9.69]]]]},"muncod":"2702355","mundescricao":"Craibas","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.89,-9.43],[-37.89,-9.54],[-37.98,-9.53],[-38.08,-9.44],[-38.2,-9.42],[-38.24,-9.33],[-38.16,-9.25],[-37.89,-9.43]]]]},"muncod":"2702405","mundescricao":"Delmiro Gouveia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.72,-9.47],[-36.84,-9.49],[-36.8,-9.28],[-36.7,-9.29],[-36.72,-9.47]]]]},"muncod":"2702553","mundescricao":"Estrela de Alagoas","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.61,-9.93],[-36.65,-9.99],[-36.75,-9.88],[-36.72,-9.88],[-36.69,-9.8],[-36.59,-9.87],[-36.61,-9.93]]]]},"muncod":"2702603","mundescricao":"Feira Grande","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.69,-9.28],[-35.69,-9.37],[-35.77,-9.39],[-35.77,-9.31],[-35.84,-9.26],[-35.89,-9.14],[-35.79,-9.16],[-35.73,-9.12],[-35.69,-9.28]]]]},"muncod":"2702801","mundescricao":"Flexeiras","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.76,-9.9],[-36.81,-9.96],[-36.92,-9.88],[-36.9,-9.74],[-36.97,-9.69],[-36.94,-9.63],[-36.89,-9.63],[-36.75,-9.77],[-36.76,-9.9]]]]},"muncod":"2702900","mundescricao":"Girau do Ponciano","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.95,-8.98],[-35.99,-8.9],[-35.9,-8.85],[-35.83,-8.87],[-35.86,-9.05],[-35.95,-9.03],[-35.95,-8.98]]]]},"muncod":"2703007","mundescricao":"Ibateguara","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.95,-9.65],[-36.99,-9.72],[-37.07,-9.64],[-36.89,-9.6],[-36.95,-9.65]]]]},"muncod":"2703700","mundescricao":"Jaramataia","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.71,-10.04],[-36.84,-10.01],[-36.75,-9.88],[-36.67,-9.94],[-36.71,-10.04]]]]},"muncod":"2701506","mundescricao":"Campo Grande","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.42,-9.16],[-37.59,-9.29],[-37.62,-9.19],[-37.69,-9.18],[-37.53,-8.96],[-37.4,-9.05],[-37.42,-9.16]]]]},"muncod":"2701605","mundescricao":"Canapi","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.88,-9.41],[-38.01,-9.34],[-37.96,-9.25],[-38.01,-9.16],[-37.95,-9.11],[-37.77,-9.29],[-37.88,-9.41]]]]},"muncod":"2700102","mundescricao":"Agua Branca","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.28,-9.34],[-37.45,-9.36],[-37.58,-9.3],[-37.39,-9.32],[-37.41,-9.29],[-37.2,-9.22],[-37.16,-9.24],[-37.28,-9.34]]]]},"muncod":"2707206","mundescricao":"Poco das Trincheiras","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.32,-9.15],[-35.37,-9.09],[-35.56,-9.06],[-35.57,-9.01],[-35.42,-8.92],[-35.38,-8.97],[-35.37,-8.94],[-35.3,-9.03],[-35.34,-9.03],[-35.32,-9.15]]]]},"muncod":"2707305","mundescricao":"Porto Calvo","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.3,-9.16],[-35.36,-9.23],[-35.48,-9.19],[-35.43,-9.18],[-35.53,-9.12],[-35.44,-9.07],[-35.37,-9.09],[-35.3,-9.16]]]]},"muncod":"2707404","mundescricao":"Porto de Pedras","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.62,-9.48],[-36.72,-9.47],[-36.7,-9.29],[-36.59,-9.36],[-36.55,-9.34],[-36.45,-9.46],[-36.53,-9.53],[-36.62,-9.48]]]]},"muncod":"2706307","mundescricao":"Palmeira dos Indios","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.3,-9.83],[-37.66,-9.69],[-37.66,-9.52],[-37.57,-9.51],[-37.57,-9.55],[-37.47,-9.65],[-37.3,-9.64],[-37.37,-9.68],[-37.24,-9.74],[-37.3,-9.79],[-37.3,-9.83]]]]},"muncod":"2706406","mundescricao":"Pao de Acucar","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.98,-9.3],[-38.01,-9.34],[-38.16,-9.24],[-38.09,-9.17],[-38.01,-9.16],[-37.96,-9.25],[-37.98,-9.3]]]]},"muncod":"2706422","mundescricao":"Pariconha","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.84,-9.64],[-35.87,-9.63],[-35.82,-9.58],[-35.79,-9.6],[-35.84,-9.64]]]]},"muncod":"2707909","mundescricao":"Santa Luzia do Norte","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.55,-9.47],[-35.62,-9.47],[-35.68,-9.4],[-35.55,-9.4],[-35.52,-9.44],[-35.55,-9.47]]]]},"muncod":"2706448","mundescricao":"Paripueira","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.56,-9.27],[-35.62,-9.13],[-35.55,-9.19],[-35.48,-9.19],[-35.39,-9.29],[-35.46,-9.35],[-35.56,-9.27]]]]},"muncod":"2706505","mundescricao":"Passo de Camaragibe","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.43,-9.36],[-36.39,-9.29],[-36.34,-9.29],[-36.39,-9.43],[-36.46,-9.45],[-36.49,-9.41],[-36.43,-9.36]]]]},"muncod":"2706604","mundescricao":"Paulo Jacinto","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.5,-10.4],[-36.51,-10.43],[-36.56,-10.42],[-36.57,-10.33],[-36.66,-10.24],[-36.52,-10.18],[-36.45,-10.03],[-36.34,-10.18],[-36.4,-10.25],[-36.38,-10.3],[-36.49,-10.35],[-36.5,-10.4]]]]},"muncod":"2706703","mundescricao":"Penedo","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.41,-10.48],[-36.51,-10.37],[-36.4,-10.3],[-36.29,-10.35],[-36.39,-10.5],[-36.41,-10.48]]]]},"muncod":"2706802","mundescricao":"Piacabucu","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.78,-9.47],[-37.84,-9.58],[-37.89,-9.54],[-37.89,-9.43],[-37.77,-9.29],[-37.72,-9.38],[-37.78,-9.47]]]]},"muncod":"2705804","mundescricao":"Olho d\'Agua do Casado","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.76,-10.1],[-36.77,-10.12],[-36.87,-10.06],[-36.86,-10.03],[-36.82,-10],[-36.71,-10.04],[-36.76,-10.1]]]]},"muncod":"2705903","mundescricao":"Olho d\'Agua Grande","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.11,-9.51],[-37.16,-9.58],[-37.27,-9.45],[-37.12,-9.44],[-37.11,-9.51]]]]},"muncod":"2706000","mundescricao":"Olivenca","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.33,-9.87],[-36.4,-9.87],[-36.47,-9.97],[-36.5,-9.87],[-36.55,-9.84],[-36.39,-9.77],[-36.33,-9.87]]]]},"muncod":"2704005","mundescricao":"Junqueiro","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.72,-9.85],[-36.75,-9.88],[-36.76,-9.71],[-36.69,-9.81],[-36.72,-9.85]]]]},"muncod":"2704104","mundescricao":"Lagoa da Canoa","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.53,-9.8],[-36.55,-9.84],[-36.58,-9.69],[-36.38,-9.66],[-36.34,-9.71],[-36.36,-9.77],[-36.53,-9.8]]]]},"muncod":"2704203","mundescricao":"Limoeiro de Anadia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.77,-9.69],[-35.8,-9.71],[-35.77,-9.64],[-35.82,-9.58],[-35.77,-9.39],[-35.69,-9.37],[-35.67,-9.43],[-35.56,-9.49],[-35.68,-9.6],[-35.69,-9.66],[-35.77,-9.69]]]]},"muncod":"2704302","mundescricao":"Maceio","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.49,-10.02],[-36.45,-10.03],[-36.49,-10.07],[-36.52,-10.04],[-36.66,-10.03],[-36.6,-9.87],[-36.54,-9.88],[-36.55,-9.84],[-36.5,-9.87],[-36.49,-10.02]]]]},"muncod":"2708808","mundescricao":"Sao Sebastiao","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.94,-10.08],[-36.97,-9.99],[-37.05,-9.99],[-37.14,-9.9],[-37,-9.76],[-36.98,-9.69],[-36.9,-9.74],[-36.92,-9.88],[-36.8,-9.96],[-36.87,-10.05],[-36.94,-10.08]]]]},"muncod":"2709202","mundescricao":"Traipu","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.03,-9.2],[-36.18,-9.21],[-36.13,-9.15],[-36.11,-9.06],[-35.95,-9.03],[-35.86,-9.05],[-35.89,-9.14],[-36.03,-9.2]]]]},"muncod":"2709301","mundescricao":"Uniao dos Palmares","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.34,-9.46],[-36.39,-9.43],[-36.34,-9.29],[-36.3,-9.31],[-36.24,-9.29],[-36.18,-9.21],[-36.21,-9.45],[-36.3,-9.43],[-36.34,-9.46]]]]},"muncod":"2709400","mundescricao":"Vicosa","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.04,-10.05],[-36.12,-9.93],[-36.28,-9.94],[-36.28,-9.9],[-36.18,-9.89],[-36.24,-9.87],[-36.21,-9.81],[-36.16,-9.86],[-36.13,-9.83],[-36.07,-9.88],[-36.04,-9.86],[-36.02,-9.92],[-35.97,-9.93],[-36.04,-10.05]]]]},"muncod":"2703759","mundescricao":"Jequia da Praia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.42,-9.36],[-36.49,-9.41],[-36.62,-9.33],[-36.57,-9.33],[-36.57,-9.29],[-36.44,-9.21],[-36.36,-9.22],[-36.33,-9.28],[-36.39,-9.29],[-36.42,-9.36]]]]},"muncod":"2707602","mundescricao":"Quebrangulo","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.81,-9.54],[-35.89,-9.59],[-35.96,-9.53],[-35.93,-9.37],[-35.88,-9.41],[-35.77,-9.39],[-35.81,-9.54]]]]},"muncod":"2707701","mundescricao":"Rio Largo","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.16,-9.44],[-37.27,-9.45],[-37.45,-9.36],[-37.28,-9.34],[-37.11,-9.24],[-37.06,-9.32],[-37.16,-9.44]]]]},"muncod":"2708006","mundescricao":"Santana do Ipanema","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.13,-9.16],[-36.18,-9.21],[-36.29,-9.17],[-36.26,-9.17],[-36.27,-9.1],[-36.12,-9.03],[-36.13,-9.16]]]]},"muncod":"2708105","mundescricao":"Santana do Mundau","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.84,-10.17],[-36.91,-10.14],[-36.95,-10.07],[-36.87,-10.05],[-36.82,-10.07],[-36.77,-10.12],[-36.84,-10.17]]]]},"muncod":"2708204","mundescricao":"Sao Bras","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.13,-9.06],[-36.13,-8.96],[-35.98,-8.91],[-35.95,-9.03],[-36.13,-9.06]]]]},"muncod":"2708303","mundescricao":"Sao Jose da Laje","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.41,-9.64],[-37.47,-9.65],[-37.57,-9.55],[-37.57,-9.51],[-37.67,-9.54],[-37.66,-9.41],[-37.31,-9.55],[-37.36,-9.64],[-37.41,-9.64]]]]},"muncod":"2708402","mundescricao":"Sao Jose da Tapera","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.08,-9.66],[-36.18,-9.61],[-36.01,-9.59],[-35.96,-9.53],[-35.87,-9.63],[-35.98,-9.64],[-35.98,-9.67],[-36.08,-9.66]]]]},"muncod":"2706901","mundescricao":"Pilar","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.23,-9.5],[-36.34,-9.51],[-36.35,-9.45],[-36.3,-9.43],[-36.21,-9.45],[-36.23,-9.5]]]]},"muncod":"2707008","mundescricao":"Pindoba","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.74,-9.62],[-37.79,-9.64],[-37.84,-9.57],[-37.79,-9.54],[-37.73,-9.33],[-37.66,-9.41],[-37.68,-9.52],[-37.64,-9.6],[-37.66,-9.68],[-37.74,-9.62]]]]},"muncod":"2707107","mundescricao":"Piranhas","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.52,-9.14],[-35.43,-9.18],[-35.55,-9.19],[-35.63,-9.09],[-35.69,-9.1],[-35.71,-9.05],[-35.68,-9.03],[-35.5,-9.08],[-35.52,-9.14]]]]},"muncod":"2705101","mundescricao":"Matriz de Camaragibe","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.13,-9.83],[-36.16,-9.86],[-36.25,-9.73],[-36.08,-9.67],[-35.99,-9.69],[-36.05,-9.87],[-36.13,-9.83]]]]},"muncod":"2708600","mundescricao":"Sao Miguel dos Campos","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.51,-9.69],[-36.58,-9.56],[-36.53,-9.53],[-36.39,-9.65],[-36.51,-9.69]]]]},"muncod":"2709103","mundescricao":"Taquarana","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.39,-9.61],[-36.39,-9.65],[-36.47,-9.6],[-36.44,-9.59],[-36.47,-9.49],[-36.38,-9.51],[-36.35,-9.59],[-36.39,-9.61]]]]},"muncod":"2709004","mundescricao":"Tanque d\'Arca","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.24,-9.29],[-36.32,-9.3],[-36.35,-9.2],[-36.29,-9.17],[-36.18,-9.21],[-36.24,-9.29]]]]},"muncod":"2701902","mundescricao":"Cha Preta","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.59,-9.36],[-35.69,-9.37],[-35.72,-9.13],[-35.69,-9.1],[-35.62,-9.13],[-35.59,-9.23],[-35.51,-9.31],[-35.52,-9.35],[-35.59,-9.36]]]]},"muncod":"2708501","mundescricao":"Sao Luis do Quitunde","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.96,-9.94],[-36.02,-9.92],[-36.04,-9.81],[-35.91,-9.85],[-35.96,-9.94]]]]},"muncod":"2707800","mundescricao":"Roteiro","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.85,-9.39],[-36.94,-9.38],[-36.92,-9.31],[-36.87,-9.27],[-36.8,-9.28],[-36.8,-9.36],[-36.85,-9.39]]]]},"muncod":"2705309","mundescricao":"Minador do Negrao","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.3,-9.12],[-37.26,-9.16],[-37.3,-9.19],[-37.47,-9.21],[-37.4,-9.05],[-37.3,-9.12]]]]},"muncod":"2706109","mundescricao":"Ouro Branco","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.33,-9.67],[-37.3,-9.64],[-37.24,-9.74],[-37.37,-9.68],[-37.33,-9.67]]]]},"muncod":"2706208","mundescricao":"Palestina","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.52,-8.95],[-35.45,-8.96],[-35.52,-9.01],[-35.59,-8.98],[-35.58,-8.91],[-35.52,-8.95]]]]},"muncod":"2703908","mundescricao":"Jundia","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.37,-10.03],[-36.48,-10.03],[-36.48,-9.96],[-36.4,-9.87],[-36.28,-9.89],[-36.3,-10.03],[-36.37,-10.03]]]]},"muncod":"2709152","mundescricao":"Teotonio Vilela","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.87,-9.63],[-35.88,-9.56],[-35.8,-9.53],[-35.87,-9.63]]]]},"muncod":"2708907","mundescricao":"Satuba","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.06,-9.28],[-36.18,-9.21],[-36.06,-9.22],[-35.89,-9.14],[-36.01,-9.27],[-36.06,-9.28]]]]},"muncod":"2701100","mundescricao":"Branquinha","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.84,-9.15],[-35.89,-9.14],[-35.83,-8.98],[-35.68,-9.03],[-35.71,-9.08],[-35.63,-9.09],[-35.58,-9.16],[-35.68,-9.1],[-35.84,-9.15]]]]},"muncod":"2703809","mundescricao":"Joaquim Gomes","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.57,-8.99],[-35.56,-9.06],[-35.72,-9.02],[-35.65,-8.88],[-35.57,-8.84],[-35.57,-8.99]]]]},"muncod":"2705606","mundescricao":"Novo Lino","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.86,-9.39],[-35.88,-9.41],[-35.93,-9.37],[-36.01,-9.4],[-36,-9.33],[-36.05,-9.33],[-36.07,-9.28],[-36.01,-9.27],[-35.89,-9.14],[-35.83,-9.24],[-35.86,-9.39]]]]},"muncod":"2705507","mundescricao":"Murici","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.12,-9.54],[-37.16,-9.61],[-37.37,-9.52],[-37.27,-9.45],[-37.16,-9.58],[-37.12,-9.54]]]]},"muncod":"2705705","mundescricao":"Olho d\'Agua das Flores","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.33,-9.48],[-37.37,-9.52],[-37.45,-9.45],[-37.32,-9.42],[-37.27,-9.45],[-37.33,-9.48]]]]},"muncod":"2701803","mundescricao":"Carneiros","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.35,-9.65],[-37.31,-9.54],[-37.24,-9.61],[-37.35,-9.65]]]]},"muncod":"2705408","mundescricao":"Monteiropolis","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-37.45,-9.47],[-37.65,-9.41],[-37.59,-9.3],[-37.33,-9.42],[-37.45,-9.47]]]]},"muncod":"2708956","mundescricao":"Senador Rui Palmeira","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.88,-9.41],[-35.84,-9.26],[-35.8,-9.27],[-35.77,-9.39],[-35.88,-9.41]]]]},"muncod":"2705200","mundescricao":"Messias","estuf":"AL","cor":"#99FF99"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-35.36,-9.26],[-35.39,-9.29],[-35.48,-9.19],[-35.38,-9.21],[-35.34,-9.23],[-35.36,-9.26]]]]},"muncod":"2708709","mundescricao":"Sao Miguel dos Milagres","estuf":"AL","cor":"#FFFFFF"},{"poli":{"type":"MultiPolygon","coordinates":[[[[-36.24,-9.68],[-36.25,-9.73],[-36.33,-9.71],[-36.34,-9.74],[-36.39,-9.6],[-36.29,-9.6],[-36.24,-9.68]]]]},"muncod":"2700201","mundescricao":"Anadia","estuf":"AL","cor":"#FFFFFF"}]';
			this.poligonos = {cor:'#BBD8E9',poligono:this.requisicao};
		}else{
			var origensDasRequisicoes = this.origensDasRequisicoes();
			console.log(origensDasRequisicoes.url);
			jQuery.ajax({
				url: origensDasRequisicoes.url,
				type: 'POST',
				data: {
                    chamadoMapas:'montaEstado',
                    params:chamada,
                    retorno:'EstadosSelecionados',
                    origemRequisicao:this.origemRequisicao
                },
				success: function( resposta ){
					console.log(resposta);
					jQuery( '#'+nomeTemporario ).remove();
					that.requisicao = resposta;
					that.poligonos = {cor:'#FFF',poligono:resposta};
					that.desenhaPoligonos();
					// that.atualizaMapa();
					that.atualizaPosicaoLegenda();
				}
			});
		}

	},

	/**
	 * Atualiza mapa com as configurações aplicadas
	 */
	atualizaMapa: function(){
		
		this.mostraPosicao( 'atualizaMapa' );
		this.desenhaPoligonos();
	},

	desenhaPoligonos: function(){
		this.mostraPosicao( 'desenhaPoligonos' );

		var that = this;
		var bordaTexto = 'text-shadow: 1px 0 0 #000, -1px 0 0 #000, 0 1px 0 #000, 0 -1px 0 #000, 1px 1px #000, -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000;';

		// console.log(this.poligonos.poligono);
		if( this.poligonos.poligono != "" ){
			var poli = JSON.parse(this.poligonos.poligono);
		}

		// retira todos os poligonos do mapa e destrói os mesmos
		var i;
        if (that.poligonosDesenhados.length > 0) {
            for (i in that.poligonosDesenhados) {
                that.poligonosDesenhados[i].setMap(null);
                // that.poligonosDesenhados[i] = undefined;
            }
        }

		if( 
			this.origemRequisicao == 'pais-legenda-externo'
			|| this.origemRequisicao == 'pais-externo' ){
			this.aplicaFundoBrasil();
		}

		if( this.poligonos.poligono != "" ){
			
			jQuery.each( poli, function( index, value ){
//console.log(value);
				if( value == null ){ 
					return;
				}
				
				if( value.cor ){ 
					var cor = value.cor; 
				}else{ 
					var cor = this.poligonos.cor; 
				}
				
				if( value.cor == '#f6ead9' ){ 
					value.cor = '#fff'; 
				} // pq é branco quando não possui assessoramento
console.log(value.poli.coordinates);
				var im;
				for (im in value.poli.coordinates) { // LOOP MULTIPOLIGONO

					if( typeof value.poli.coordinates[im][0] != 'undefined' ){
						var i;
						for (var i in value.poli.coordinates[im][0]) {
							value.poli.coordinates[im][0][i] = new google.maps.LatLng( value.poli.coordinates[im][0][i][1], value.poli.coordinates[im][0][i][0] );
						};
					}
                    console.log(im);
					that.poligonosDesenhados[ that.casa ] = new google.maps.Polygon({
						mundescricao: value.mundescricao,
						muncod: value.muncod,
						estuf: value.estuf,
						cor: value.cor,
						situacao: value.situacao,
						paths: value.poli.coordinates[im][0],
						useGeoJSON: true,
					  	strokeColor: '#333',
					  	strokeOpacity: 1,
					  	strokeWeight: 0.5,
					  	fillColor: value.cor,
					  	// fillOpacity: 0.6,
					  	fillOpacity: 1,
					  	zIndex:6,
					  	map: that.map.map
					});

                    /**
                     * Victor Martins Machado - 02/03/2015
                     * Evento que apresenta as informações do município como InfoWindow do google.maps, ultilizando
                     * o html montado na variável 'html_municipio, criado na página.
                     */
					google.maps.event.addListener(that.poligonosDesenhados[ that.casa ], 'click', function(event){
                        infowindow.setContent(html_municipio.replace("{muncod}", value.muncod));
                        infowindow.setPosition(event.latLng);
                        infowindow.open(that.map.map);
					});

					google.maps.event.addListener(that.poligonosDesenhados[ that.casa ], 'mouseover', function(){
						var corAtual = this.fillColor;
				  		this.setOptions({fillColorBkp: corAtual});
				  		this.setOptions({fillColor: "#ccc"});
				  		if( this.situacao == undefined ){ 
				  			if( that.origemRequisicao == 'organizacoesterritoriais' ){
				  				this.situacao = 'Mesoregião não determinada'; 
				  			}else{
				  				this.situacao = 'Não Cadastrado'; 
				  			}
				  		}
				  		that.clearTextArea();
				  		if( 
				  			that.origemRequisicao == 'assessoramento-externo'
                            || that.origemRequisicao == 'mapa-base-nacional-comum'
				  			|| that.origemRequisicao == 'assessoramento-legenda-externo' ){
				  			jQuery( that.idTag+'txt' ).append( '<div style="position:absolute;z-index:10;background:#fff;" class="txts" id="'+this.mundescricao+'txt">'+this.mundescricao+' - '+this.estuf+'' );
				  		}else{
				  			jQuery( that.idTag+'txt' ).append( '<div style="position:absolute;z-index:10;background:#fff;" class="txts" id="'+this.mundescricao+'txt">'+this.mundescricao+' - '+this.estuf+' <font style="color:' + this.cor + ';'+bordaTexto+'">(' + this.situacao + ')</font></div>' );
				  		}
					});

					google.maps.event.addListener(that.poligonosDesenhados[ that.casa ], 'mouseout', function(){
						var corAtual = this.fillColorBkp;
				  		this.setOptions({fillColor: corAtual});
				  		that.clearTextArea();
					});

                    infowindow = new google.maps.InfoWindow();
					that.casa++;
				}; // LOOP MULTIPOLIGONO /

			});
		}

		this.posDesenhoPoligonoAcao();
	},

	/**
	 * Atualiza com chamada a mostragem atual
	 */
	posDesenhoPoligonoAcao: function(){
		this.mostraPosicao( 'posDesenhoPoligono' );

		if( this.posDesenhoPoligono != '' ){
			eval(''+this.posDesenhoPoligono+'()');
		}
	},

	/**
	 * Atualiza com chamada a mostragem atual
	 */
	sincronizaMapaComServidor: function(){
		this.mostraPosicao( 'sincronizaMapaComServidor' );

		this.getEstadoPoligono( this.chamada );
	},

	/**
	 * Determina informações necessárias para atender as diferentes origens das requisicoes
	 */
	 origensDasRequisicoes: function(){
		this.mostraPosicao( 'origensDasRequisicoes' );

	 	var that = this;

	 	switch( this.origemRequisicao ){
	 		// ---------------------------------------- buscas padrao
	 		case 'assessoramento-estado': 
	 		case 'assessoramento':
	 		case 'assessoramento-legenda-estado':
	 		case 'assessoramento-legenda':
            case 'base-nacional-por-tipo':
	 			return {url:'/par/par.php?modulo=principal/mapa&acao=A'};
	 			break;
	 		case 'questoespontuais':
	 		case 'questoespontuais-legenda':
	 			return {url:'sase.php?modulo=principal/questoespontuais&acao=A'};
	 			break;
	 		case 'organizacoesterritoriais':
	 		case 'organizacoesterritoriais-legenda':
	 			that.orgid = jQuery('#orgao').val();
	 			return {url:'sase.php?modulo=principal/organizacoesterritoriais&acao=A'};
	 			break;
	 		// ---------------------------------------- buscas sase_mapas
	 		case 'assessoramento-externo':
	 		case 'assessoramento-legenda-externo':
	 		case 'questoespontuais-externo':
	 		case 'questoespontuais-legenda-externo':
	 		case 'organizacoesterritoriais-externo':
	 			return {url:'sase_mapas.php'};
	 			break;
	 		// ---------------------------------------- buscas sase_mapas
	 		case 'pais-externo':
	 		case 'pais-legenda-externo':
	 			return {url:'sase_pais.php'}
	 			break;

            case 'mapa-base-nacional-comum':
                return {url:'mapa_base_nacional_comum.php'}
                break;
	 	}
	},

	/**
	  * Atualiza as legendas apresentadas para o que é apresentado no mapa apenas com suas quantidades
	  */
	atualizaLegenda: function( estuf, muncod, origemRequisicao ){
		this.mostraPosicao( 'atualizaLegenda' );

		var that = this;
		var selecionados = jQuery(estuf).val();
        var munSelecionados = jQuery(muncod).val();

		if(typeof selecionados === 'string'){ selecionados = [selecionados]; }

		this.origemRequisicao = origemRequisicao;

		this.chamada = {
			estados: selecionados,
            municipios: munSelecionados
		};

		if( this.orgid != '' ){ this.chamada.orgid = this.orgid; }

		var origensDasRequisicoes = this.origensDasRequisicoes();
		jQuery.ajax({
			url: origensDasRequisicoes.url,
			type: 'POST',
			data: {chamadoMapas:'legenda',params:this.chamada,retorno:'legenda',origemRequisicao:this.origemRequisicao},
			success: function( resposta ){
				// console.log(resposta);
				jQuery('#legendaMapa').html( resposta );
				that.acaoPosAplicacaoLegenda();
			}
		});
	},

	atualizaPosicaoLegenda: function(){
		this.mostraPosicao( 'atualizaPosicaoLegenda' );

		switch( this.origemRequisicao ){
	 		case 'assessoramento-externo':
	 		case 'assessoramento-legenda-externo':
	 		case 'questoespontuais-externo':
	 		case 'questoespontuais-legenda-externo':
	 		case 'organizacoesterritoriais-externo':
	 			jQuery('#legendaMapa').appendTo( jQuery('#map_canvas') );
	 			break;
	 	}
	 },

	clearTextArea: function(){
		this.mostraPosicao( 'clearTextArea' );

		jQuery( '.txts' ).remove();
	},

	acaoPosAplicacaoLegenda: function(){
		this.mostraPosicao( 'acaoPosAplicacaoLegenda' );

		if( this.posLegenda != '' ){
			eval(''+this.posLegenda+'()');
		}
	},

	verificaSeCentroPermaneceNoBound: function(){
		this.mostraPosicao( 'verificaSeCentroPermaneceNoBound' );

		var that = this;
		var map = this.map.map;

		that.allowedBounds = map.getBounds();
		google.maps.event.addListenerOnce(map,'idle',function() {
			// var zoomAtual = map.getZoom();

			// map.setZoom( zoomAtual + 1 );

			that.allowedBounds = map.getBounds();

			// map.setZoom( zoomAtual );
		});

		google.maps.event.addListener(map,'center_changed',function() { 
			checkBounds( that.allowedBounds ); 
		});

		function checkBounds() {    
			// console.log(that.allowedBounds);
		    if(! that.allowedBounds.contains(map.getCenter())) {
		      var C = map.getCenter();
		      var X = C.lng();
		      var Y = C.lat();

		      var AmaxX = that.allowedBounds.getNorthEast().lng();
		      var AmaxY = that.allowedBounds.getNorthEast().lat();
		      var AminX = that.allowedBounds.getSouthWest().lng();
		      var AminY = that.allowedBounds.getSouthWest().lat();

		      if (X < AminX) {X = AminX;}
		      if (X > AmaxX) {X = AmaxX;}
		      if (Y < AminY) {Y = AminY;}
		      if (Y > AmaxY) {Y = AmaxY;}

		      map.setCenter(new google.maps.LatLng(Y,X));
		    }
		}
	},

	/**
	 * Aplica imagens para Siglas dos Estados
	 */
	aplicaTituloEstados: function(){
		this.mostraPosicao( 'aplicaTituloEstados' );

		setMarkers(this.map.map, this.centroEstados);

		function setMarkers(map, locations) {
		  var shape = {
		      coords: [1, 1, 1, 20, 18, 20, 18 , 1],
		      type: 'poly'
		  };
		  for (var i = 0; i < locations.length; i++) {
		    var uf = locations[i];
		    var myLatLng = new google.maps.LatLng(uf[1], uf[2]);
		    var marker = new google.maps.Marker({
		        position: myLatLng,
		        map: map,
		        icon: {
		        	url: 'texto_nome_entidade_territorio.php?estuf='+uf[0],
		        	size: new google.maps.Size(60, 32),
				    origin: new google.maps.Point(0,0),
				    anchor: new google.maps.Point(0, 32)
		        },
		        shape: shape,
		        title: uf[0],
		        zIndex: 4
		    });
		  }
		}

	},

	/**
	 * Aplica imagem para fundo 
	 */
	aplicaFundoBrasil: function(){
		this.mostraPosicao( 'aplicaFundoBrasil' );

		var that = this;

		var path = [
			new google.maps.LatLng(8.183668,-75.689105), // nordeste 
			new google.maps.LatLng(-38.655990,-75.689105), // sudeste
			new google.maps.LatLng(-38.655990,-32.534811), // sudoeste
			new google.maps.LatLng(8.183668,-32.534811)  // noroeste
		];
		
		this.fundoBrasil = new google.maps.Polygon({
			paths: path,
			strokeColor: '#efefef',
			strokeOpacity: 1,
			strokeWeight: 0,
			fillColor: '#efefef',
			fillOpacity: 1,
			map: that.map.map
		});

	},

	aplicaComponente: function( estuf, componente, origemRequisicao){
		this.mostraPosicao( 'aplicaComponente' );

		var that = this;
		this.origemRequisicao = origemRequisicao;

		estuf = jQuery(estuf).val();

		var origensDasRequisicoes = that.origensDasRequisicoes();
		jQuery.ajax({
			url: origensDasRequisicoes.url,
			type: 'POST',
			data: {chamadoMapas:componente,params:estuf,origemRequisicao:this.origemRequisicao},
			success: function( resposta ){
				jQuery('#componentesMapa').append( resposta );
			}
		});

	},

	/**
	 * método para atender componente de busca de municipios
	 */
	buscaMunicipio: function( muncod ){
		this.mostraPosicao( 'buscaMunicipio' );

		var chave = null;
		jQuery.each(this.poligonosDesenhados, function( index, value ){
			if( value.muncod == muncod ){
				chave = index;
			}
		});

		var that = this;

		var contornoInicial = this.poligonosDesenhados[ chave ].strokeWeight;
		if( this.buscaMunicipioSuporte[chave] === undefined ){
			this.buscaMunicipioSuporte.push( [chave,contornoInicial] );
		}
		var atualizaContornosAnteriores = function(){
			jQuery.each( that.buscaMunicipioSuporte, function( index, value){
				that.poligonosDesenhados[ value[0] ].setOptions({strokeWeight: value[1]});
			} );
		}
		atualizaContornosAnteriores();

		var contornoFinal = 4;
		this.poligonosDesenhados[ chave ].setOptions({strokeWeight: contornoFinal});


		// ################################################################## POSICAO DO MAPA
		var polMunicipio = JSON.parse(this.poligonos.poligono);
		
		// that.map.removeMarkers();
		var removeMarcadores = function(){
			// console.log(that.marcadores);
			for (var i = that.marcadores.length - 1; i >= 0; i--) {
				that.marcadores[i].setMap(null);
			};
		}
		removeMarcadores();
		
		jQuery.each(polMunicipio, function( key, value ){
			if( key != 'boundsSimplificado' ){
				if( value.muncod == muncod ){
					
					var poligono = value.poli.coordinates[0][0];
					var boundbox = new google.maps.LatLngBounds();
					for ( var i = 0; i < poligono.length; i++ ){
						boundbox.extend(new google.maps.LatLng(poligono[i][1],poligono[i][0]));
						
						if( i == poligono.length-1 ){
							that.map.map.setCenter(boundbox.getCenter());
							that.map.map.fitBounds(boundbox);

							var centro = boundbox.getCenter();
							that.marcadores.push( new google.maps.Marker({
								position: new google.maps.LatLng( centro.lat(), centro.lng() ),
								map: that.map.map,
								icon: '../imagens/sase/direction.png'
							}) );
						}
					}

				}
			}
		});
	}


};



