<?php

/**
 * classe Mapas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto que retorna informações (dados em formato geoJSON) direcionadas a popular os mapas
 */
class Mapas extends Modelo{
	
	public $Solicitacao;

	protected $Joins = array();

	// atributos ligados a requisicao
		public $origemRequisicao;
		public $params;
		public $chamadoMapas;

	// Partes dos Mapas
		protected $Poligonos;
		protected $MetaDados;

	/**
	 *	Construct
	 */
	public function __construct(){
		$this->Poligonos = new Poligonos();
		$this->MetaDados = new MetaDados();
	}

	/**
	 * Retorna o resultado da requisicao
	 * 
	 * @todo testar
	 */
	public function requisicaoPost(){
		
		if( $this->params['estado'] )
			$this->params['estado'] = $this->garanteQueSejaArray( $this->params['estado'] );

		$this->construtorPoligonos(); // montaEstado, montaEstadoLegenda ...

		$this->construtorMetaDados(); // aplica cores aos dados em json.

		// escreve resultado
		ob_clean();
		echo $this->Poligonos->poligono;
		die();

	}

	/**
	 * Construtor de Poligonos
	 *
	 * @internal aqui encapsulam as regras da construção de polígonos 
	 *			 - praticamente baseadas em $this->origemRequisicao
	 * @internal Factory Method
	 * @return void
	 * @internal preenche os respectivos objetos que compoem a solicitação
	 */
	protected function construtorPoligonos(){
		
		switch ( $this->origemRequisicao ) {
			case 'assessoramento':
			case 'assessoramento-externo':
            case 'planocarreira-municipio':
            case 'acompanhamento-pne-municipio':
				$this->Poligonos->estuf = $this->params['estado'];
				$teste = $this->Poligonos->getMunicipios();
				break;

            case 'planocarreira-estado':
			case 'assessoramento-estado':
			case 'sase-mapas-estado-externo':
				$this->Poligonos->estuf = $this->params['estado'];
				$this->Poligonos->getEstado();
				break;


			case 'questoespontuais':
			case 'questoespontuais-externo':
				$this->Poligonos->estuf = $this->params['estado'];
				$this->Poligonos->getMunicipios();
				break;

			case 'organizacoesterritoriais':
			case 'organizacoesterritoriais-externo':
				$this->Poligonos->estuf = $this->params['estado'];
				$this->Poligonos->orgid = $this->params['orgid'];
                             
				$this->Poligonos->getMunicipios();
				break;

			case 'pais-externo':
            case 'planocarreira-estado-externo':
				$this->Poligonos->getPais();
				break;

            case 'municipio-externo':
                $this->Poligonos->getMunicipios();
                break;
		}

	}

	/**
	 * Construtor de MetaDados
	 *
	 * @internal aqui encapsulam as regras da construção de metadados
	 * @internal Factory Method
	 * @internal método $this->MetaDados->aplicaCoresAosMunicipios adicionado no switch pq para 
	 *			 cada origem pode demandar uma requisicao diferente.
	 * @return void
	 */
	protected function construtorMetaDados(){
		
		switch( $this->origemRequisicao ){
			case 'assessoramento-estado':
				$this->MetaDados->estuf = $this->params['estado'];
				$this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoEstado' );
				$this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
				break;
				
			case 'assessoramento':
				$this->MetaDados->estuf = $this->params['estado'];
				$this->MetaDados->montaDado( 'DadoSituacaoAssessoramento' );
				$this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
				break;

			case 'assessoramento-externo':
				$this->MetaDados->estuf = $this->params['estado'];
				$this->MetaDados->montaDado( 'DadoMacroCategoria' );
				$this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
				break;

            case 'planocarreira-municipio':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->tpdid = $this->params['tpdid'];
                $this->MetaDados->montaDado( 'DadoPlanoCarreiraMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'planocarreira-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->tpdid = $this->params['tpdid'];
                $this->MetaDados->montaDado( 'DadoPlanoCarreiraEstado' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'acompanhamento-pne-municipio':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->subid  = $this->params['subid'];
                $this->MetaDados->pneano = $this->params['pneano'];
                $this->MetaDados->montaDado( 'DadoAcompanhoPneMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

			case 'questoespontuais':
			case 'questoespontuais-externo':
				$this->MetaDados->estuf = $this->params['estado'];
				$this->MetaDados->montaDado( 'DadoSituacaoQuestoesPontuais' );
				$this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
				break;

			case 'organizacoesterritoriais':
			case 'organizacoesterritoriais-externo':
				$this->MetaDados->estuf = $this->params['estado'];
				$this->MetaDados->orgid = $this->params['orgid'];
				$this->MetaDados->montaDado( 'DadoMesoregiao' );
				$this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
				break;

			case 'pais-externo':
				$this->MetaDados->montaDado( 'DadoEstadosPais' );
				$this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
				break;

            case 'planocarreira-estado-externo':
                $this->MetaDados->tpdid = $this->params['tpdid'];
                $this->MetaDados->montaDado('DadoPlanocarreiraEstadoExterno');
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'municipio-externo':
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramento' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
		}

	}

	/**
	 * Garante que variável seja array
	 *
	 * @param $var 
	 * @return $array
	 */
	protected function garanteQueSejaArray( $var ){
		if( 
			$var 
			&& !is_array($var) 
		) 
			$var = array($var);

		return $var;
	}

	/**
	 *
	 */
	public function requisicaoPostLegenda(){
		global $db;
		
		if( count($this->params['estados']) > 0 ){ 
			ob_clean(); 
			switch(	$this->origemRequisicao ){
				case 'assessoramento-legenda-estado':

					include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
					$SituacaoAssessoramento = new SituacaoAssessoramento();
					$SituacaoAssessoramento->montaLegendaEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

					break;
                case 'planocarreira-municipio-legenda':
                    include APPRAIZ . 'sase/classes/Planodecarreira.class.inc';
                    $planodecarreira = new Planodecarreira();
                    $planodecarreira->getLegenda((( !is_array($this->params['estados']) )?array():$this->params['estados']), $this->params['tpdid']);
                    break;
                case 'planocarreira-estado-legenda':
                    include APPRAIZ . 'sase/classes/PlanodecarreiraEstado.class.inc';
                    $planodecarreira = new PlanodecarreiraEstado();
                    $planodecarreira->getLegenda((( !is_array($this->params['estados']) )?array():$this->params['estados']), $this->params['tpdid']);
                    break;
				case 'assessoramento-legenda':

					include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
					$SituacaoAssessoramento = new SituacaoAssessoramento();
					$SituacaoAssessoramento->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

					break;
				case 'assessoramento-legenda-externo':

					include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
					$SituacaoAssessoramento = new SituacaoAssessoramento();
					$SituacaoAssessoramento->montaLegendaMacroCategoria( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );
					
					break;
				case 'pais-legenda-externo':

					include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
					$SituacaoAssessoramento = new SituacaoAssessoramento();
					$SituacaoAssessoramento->montaLegendaMacroCategoriaPais( );


					break;

                case 'planocarreira-estado-legenda-externo':
                    include APPRAIZ . 'sase/classes/PlanodecarreiraEstado.class.inc';
                    $planodecarreira = new PlanodecarreiraEstado();
                    $planodecarreira->getLegenda(array(), $this->params['tpdid']);
                    break;

                case 'municipio-legenda-externo':
                    include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                    $SituacaoAssessoramento = new SituacaoAssessoramento();
                    $SituacaoAssessoramento->montaLegendaMunicipioExterno();
                    break;

				case 'questoespontuais-legenda':
				case 'questoespontuais-legenda-externo':

					include_once APPRAIZ . 'sase/classes/QuestoesPontuaisPar.class.inc';
					$QuestoesPontuaisPar = new QuestoesPontuaisPar();
					$QuestoesPontuaisPar->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

					break;
				case 'organizacoesterritoriais-legenda':
				case 'organizacoesterritoriais-legenda-externo':
					include_once APPRAIZ . 'sase/classes/Territorio.class.inc';
					$Territorio = new Territorio();
					$Territorio->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']), (( !$this->params['orgid'] )?'':$this->params['orgid']) );
					break;

				default:
					break;
			}
			die();
		}

	}

	/**
	 * Componentes em _componentes.php
	 *
	 * @param $componentes - nome da função
	 * @param $params - parametros a serem passados para funcao
	 */
	public function componente( $componente, $params ){

		$funcao = $componente.'("'.implode('","', $params).'");';
		eval( $funcao );

	}

}

// ############################################################################ CAPTURA DE REQUISICAO POST
$chamadoMapas = new Mapas();

if( $_POST['chamadoMapas'] ){
	// if( $_POST['origemRequisicao']=='sase-mapas-estado-externo' ){ print_r($_POST['params']);exit; }
	
	$chamadoMapas->origemRequisicao = $_POST['origemRequisicao'];
	$chamadoMapas->params = $_POST['params'];
	$chamadoMapas->chamadoMapas = $_POST['chamadoMapas'];
	if( $_POST['chamadoMapas'] == 'legenda' ){
		$chamadoMapas->requisicaoPostLegenda();

	}else if( $_POST['chamadoMapas'] == 'componente-buscamunicipios' ){
		$chamadoMapas->componente('BuscaMunicipio', array('estuf' => $chamadoMapas->params) );

	}else
		$chamadoMapas->requisicaoPost();

}
// ########################################################################################################

