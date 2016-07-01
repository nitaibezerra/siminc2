<?php

/**
 *
 */
interface RelatorioConjur
{
	/**
	 *
	 */
	public function preparaDados();

	/**
	 *
	 */
	public function montaRelatorio();
}

/**
 * Relatório de Manifestãções Conjur
 */
class RelatorioConjurManifestacoes extends Modelo implements RelatorioConjur
{

	protected $dadosParaRelatorio;

	public $sql = array();
	protected $aryWhere = array();
	protected $arrayTPDID = array();
	protected $where = '';
	protected $sqlParaExecutar = '';
	protected $listaProcessos = array();
	protected $arrayCpfs = array();

	public $dados = array();
	public $advogados = array();
	public $coordenacoes = array();
	public $periodos = array();
	public $contexto = array();


	/**
	 * 
	 */
	public function  __construct( Array $dados ){

		$this->advogados	= $dados['advogados'];
	 	$this->coordenacoes	= $dados['coordenacoes'];
	 	$this->periodos		= $dados['periodos'];
	 	$this->contexto		= $dados['contexto'];

	 	$this->preparaDados();
	 	$this->montaRelatorio();
	}

	/**
	 * Realiza necessários os ajustes dos dados
	 */
	public function preparaDados(){

		$this->arrayTPDID = array(
		    	TP_PARECER => 'parecer',
		    	TP_COTA => 'cota',
		    	TP_NOTA => 'nota',
		    	TP_DESPACHO => 'despacho',
		    	TP_OFICIO => 'oficio',
		    	TP_INFORMACAO => 'informacao' );

		$this->aryWhere[] = " num.spdstatus = 'A' ";

		$this->advogadosWhere();

		$this->datasWhere();
	}

	/**
	 * Monta Relatório
	 */
	public function montaRelatorio(){

		if( $this->contexto == 'advogado' || $this->contexto == 'todos' ){

		    foreach ($this->arrayTPDID as $key => $value) {
		      	$this->where = ( is_array($this->aryWhere) && count($this->aryWhere) ? ' and ' . implode(' AND ', $this->aryWhere) : '' );
		      	$this->where .= " and td.tpdid in ($key) ";

		      	array_push($this->sql, " SELECT
									p.prcid,
									coalesce(a.spdid,0) as nudidanexo,
									a.arqid, 
									td.tpdid,
									td.tpddsc, 
									p.prcnumsidoc, 
									num.spdnumero,
									num.spdid,
									to_char(num.spdddatainclusao, 'DD/MM/YYYY') as nuddatageracao,
									coalesce(ent.entnome,'ADMINISTRATIVO') as advogado,
									ent.entnumcpfcnpj as entnumcpfcnpj,
									us.usunome,
									us.usucpf
								FROM conjur.processoconjur p
								INNER JOIN conjur.sapiensdocumento num ON p.prcid  = num.prcid
								INNER JOIN conjur.tipodocumento       td ON num.tpdid = td.tpdid
								LEFT  JOIN conjur.sapiensanexo        a ON a.spdid = num.spdid AND a.spastatus = 'A'
								LEFT  JOIN conjur.advogados          adv ON adv.advid = num.advid
								LEFT  JOIN entidade.entidade         ent ON ent.entid = adv.entid
								LEFT  JOIN seguranca.usuario          us ON us.usucpf = num.usucpf
								WHERE 1=1 " . $this->where ." ");
		    }

		    $this->sqlParaExecutar = implode(' UNION ALL ', $this->sql);

		    $this->listaProcessos = $this->carregar( $this->sqlParaExecutar );
		    
		    $this->arrayCpfs = $this->organizaPorCpf();

		    $this->dados = $this->preparaParaMontaLista();

		    $this->relatorio();
		} // fim relatorio advogados
		else if( $this->contexto == 'coordenacoes' || $this->contexto == 'todos' ){ // TODO: por organizar ainda
		    
		    foreach ($this->arrayTPDID as $key => $value) {
		      	$this->where = ( is_array($this->aryWhere) && count($this->aryWhere) ? ' and ' . implode(' AND ', $this->aryWhere) : '' );
		      	$this->where .= " and td.tpdid in ($key) ";

		      	array_push($this->sql, " SELECT
									p.prcid,
									coalesce(a.nudid,0) as nudidanexo,
									a.arqid, 
									td.tpdid,
									td.tpddsc, 
									p.prcnumsidoc, 
									num.nudnumero,
									num.nudid,
									to_char(num.nuddatageracao, 'DD/MM/YYYY') as nuddatageracao,
									coalesce(ent.entnome,'ADMINISTRATIVO') as advogado,
									ent.entnumcpfcnpj as entnumcpfcnpj,
									us.usunome,
									us.usucpf
								FROM conjur.processoconjur p
								LEFT JOIN (SELECT max(hadid), advid, prcid, dtatribuicao 
									FROM conjur.historicoadvogados 
								   	GROUP BY hadid,advid, prcid, dtatribuicao
								   	ORDER BY hadid DESC ) had ON had.prcid = p.prcid
								INNER JOIN conjur.numeracaodocumento num ON p.prcid  = num.prcid
								INNER JOIN conjur.tipodocumento       td ON num.tpdid = td.tpdid
								LEFT  JOIN conjur.anexos               a ON a.nudid = num.nudid AND a.anxstatus = 'A'
								LEFT  JOIN conjur.advogados          adv ON adv.advid = num.advid
								LEFT  JOIN entidade.entidade         ent ON ent.entid = adv.entid
								LEFT  JOIN seguranca.usuario          us ON us.usucpf = num.nudusucpf
								WHERE 1=1 " . $this->where ." ");
		      	
		    }
		    $this->sqlParaExecutar = implode(' UNION ALL ', $this->sql);

		    $this->listaProcessos = $this->carregar( $this->sqlParaExecutar );
		    
		    $this->arrayCpfs = $this->organizaPorCpf();

		    $this->dados = $this->preparaParaMontaLista();

		    $this->relatorio();
		} // fim relatorio cooredenacoes
	}

	/**
	 * Organiza por cpf dos advogados requerentes a lista de processos obtida
	 */
	private function organizaPorCpf(){
		$this->arrayCpfs = array();

		// ver($this->listaProcessos);
		foreach ($this->listaProcessos as $key => $value) {

			if( $this->arrayCpfs[ $value['entnumcpfcnpj'] ][ $value['tpdid'] ]['contagem'] ){

				$this->arrayCpfs[ $value['entnumcpfcnpj'] ][ $value['tpdid'] ]['contagem']++;
			}else {

				$this->arrayCpfs[ $value['entnumcpfcnpj'] ]['nome'] = $value['advogado'];

				$this->arrayCpfs[ $value['entnumcpfcnpj'] ][ $value['tpdid'] ] = array();
				$this->arrayCpfs[ $value['entnumcpfcnpj'] ][ $value['tpdid'] ]['descricao'] = $value['tpddsc'];
				$this->arrayCpfs[ $value['entnumcpfcnpj'] ][ $value['tpdid'] ]['contagem'] = 1;
			}
		}
		// ver($this->arrayCpfs,d);

		return $this->arrayCpfs;
	}
	
	/**
	 * Método organiza para funcao monta_lista()
	 *
	 * Formato esperado pelo o monta_lista
	 * $array['dados'] = Array (
	 * 		[0] => Array(
	 *      	[entnome] => ADRIENNE PINHEIRO DA ROCHA LIMA DE MELO
	 *          [parecer] => 0
	 *          [cota] => 30
	 *          [nota] => 0
	 *         	[despacho] => 15
	 *          [oficio] => 27
	 *          [informacao] => 0
	 *          [total] => 72 ) );
	 * $array['alinhamento'] = array();
	 * $array['cabecalho'] = array();
	 * $array['params'] = array();
	 *
	 */
	private function preparaParaMontaLista(){

		$array = array(
				'dados'=>array(),
				'alinhamento'=>array(),
				'cabecalho'=>array(),
				'params'=>array() );

		// dados
			foreach ($this->arrayCpfs as $key => $value) {
				// ver($value,d);
				$total = 0;
				$arrayVez = array();
				$arrayVez['entnome'] = $value['nome'];
				foreach ($this->arrayTPDID as $key2 => $value2) {
					$total = $total + $value[ $key2 ]['contagem'];
					$arrayVez[$value2] = ($value[ $key2 ]['contagem'])?$value[ $key2 ]['contagem']:0;
				}
				$arrayVez['total'] = $total;
				array_push($array['dados'],$arrayVez);
			}
		// --

		// restante
			$array['params']['totalLinhas'] = false;
			$array['alinhamento'] = array('','right','right','right','right','right','right','right');
			$array['cabecalho'] = array('Advogado Requerente','Parecer', 'Cota', 'Nota','Despacho','Ofício','Informação','TOTAL');
		// --
		// ver($array,d);

		return $array;
	}

	/**
	 *
	 */
	public function relatorio(){
		
		$this->monta_lista($this->dados['dados'], $this->dados['cabecalho'], '250','10', 'S', 'center', 'N', '', '', $this->dados['alinhamento'], '', $this->dados['params']);

	}

	/**
	 *
	 */
	private function advogadosWhere(){

		if( $this->contexto == 'advogado' || $this->contexto == 'todos' ){
			if($this->advogados){

		    	$advogados = implode(',',$this->advogados);
		      	$this->aryWhere[] = " ( adv.advid IN ($advogados) OR adv.advid IS NULL ) ";
		    }
		}else{
		    if($this->advogados){

		    	$advogados = implode(',',$this->advogados);
		      	$this->aryWhere[] = " ( adv.advid IN ($advogados) ) ";
		    }
		}

	}

	/**
	 * 
	 */
	private function datasWhere(){

		if( $this->contexto == 'advogado' || $this->contexto == 'todos' ){

			if( $this->periodos['inicio'] && $this->periodos['fim'] ){
		    	$htddatainicial = formata_data_sql( $this->periodos['inicio'] );
		    	$htddatfinal = formata_data_sql( $this->periodos['fim'] );
		    	$this->aryWhere[] = " num.spdddatainclusao >= TIMESTAMP '{$htddatainicial}' AND num.spdddatainclusao <= TIMESTAMP '{$htddatfinal}' ";
		    }
		}else{

			if( $this->periodos['inicio'] && $this->periodos['fim'] ){
		    	$htddatainicial = formata_data_sql( $this->periodos['inicio'] );
		    	$htddatfinal = formata_data_sql( $this->periodos['fim'] );
		    	$this->aryWhere[] = " num.spdddatainclusao >= TIMESTAMP '{$htddatainicial}' AND num.spdddatainclusao <= TIMESTAMP '{$htddatfinal}' ";
		    }
		}
	}
}



/**
 * Relatório de Médias de Processos Conjur
 */
class RelatorioConjurMediasProcessos extends Modelo implements RelatorioConjur
{

	protected $dadosParaRelatorio;

	public $sql = array();
	
	public $dados = array();
	public $advogados = array();
	public $coordenacoes = array();
	public $periodos = array();

	public $selectAdd = "";
	public $agrupamentoAdd = "";
	public $agrupamentoCoordenacao = "";
	public $agrupamentoAdvogados = "";

	/**
	 * 
	 */
	public function  __construct( Array $dados ){

		$this->advogados	= $dados['advogados'];
	 	$this->coordenacoes	= $dados['coordenacoes'];
	 	$this->periodos		= $dados['periodos'];
	 	if( $dados['agrupamentoCoordenacao'] )
	 		$this->agrupamentoCoordenacao = $dados['agrupamentoCoordenacao'];
	 	if( $dados['agrupamentoAdvogados'] )
			$this->agrupamentoAdvogados = $dados['agrupamentoAdvogados'];

		$this->preparaDados();
	 	$this->montaRelatorio();
	}

	/**
	 * Realiza necessários os ajustes dos dados
	 */
	public function preparaDados(){
		// ver($this->advogados,$this->coordenacoes,d);
		
	}

	/**
	 * Monta Relatório
	 */
	public function montaRelatorio(){

		if( $this->agrupamentoAdvogados ){

			$this->selectAdd = "advidrequerente, advogadorequerente";
			$this->agrupamentoAdd = "advidrequerente, advogadorequerente";
			$this->obtemDadosComquery();

			$this->dados['cabecalho'] = array('Advogado','Médias de Processos', 'Processos Abaixo do Prazo de 15 Dias', 'Processos Acima do Prazo de 15 Dias');

			$this->dados['dados'] = $this->aplicaFaltantes( $this->dados['dados'] );

			$this->relatorio();

		}

		if( $this->agrupamentoCoordenacao ){

			$this->selectAdd = "coordenacao";
			$this->agrupamentoAdd = "coonid, coordenacao";
			$this->obtemDadosComquery();

			$this->dados['cabecalho'] = array('Coordenação','Médias de Processos', 'Processos Abaixo do Prazo de 15 Dias', 'Processos Acima do Prazo de 15 Dias');

			$this->relatorio();

		}

	}
	
	/**
	 * Obtem relatorio de medias
	 *
	 * @internal originalmente o calculo da media era pra ser assim: 
	 *			 DATE_PART('day',SUM(intervaloultimapenultima)) / COUNT(*) as media
	 */
	private function obtemDadosComquery(){
		
		$combo_advogados = $this->advogados;
		
		if($combo_advogados[0]){
			$whereadv = " where advidrequerente IN (".(implode(",",$combo_advogados)).") ";
		}
		
					// 
		$sql = " SELECT 
					{$this->selectAdd},
					COUNT(*)::float / 2 as media,
					SUM(maiorqueprazo) as totalmaiorqueprazo,
					SUM(menorqueprazo) as totalmenorqueprazo,
					COUNT(*) as totalprocessos, 
					DATE_PART('day',SUM(intervaloultimapenultima)) as totaldiasemprocessos
				FROM (

					SELECT 
						prc.prcid as prcid,

						( SELECT advsq.coonid FROM ( SELECT MAX(nudid), advid 
						FROM conjur.numeracaodocumento 
						WHERE prc.prcid = prcid AND advid IS NOT NULL
						GROUP BY advid LIMIT 1) AS query
						JOIN conjur.advogados advsq ON advsq.advid = query.advid ) AS coonid,

						( SELECT coosq.coodsc FROM ( SELECT MAX(nudid), advid 
						FROM conjur.numeracaodocumento 
						WHERE prc.prcid = prcid AND advid IS NOT NULL
						GROUP BY advid LIMIT 1) AS query
						JOIN conjur.advogados advsq ON advsq.advid = query.advid
						JOIN conjur.coordenacao coosq ON coosq.coonid = advsq.coonid ) AS coordenacao,

						CASE WHEN 
							DATE_PART('day', ( COALESCE(his.htddata, prc.prcdtinclusao) - COALESCE( ( SELECT htddata FROM ( SELECT MAX(hstid) as hstid, htddata
							FROM workflow.historicodocumento
							WHERE docid = doc.docid AND his.hstid != hstid
							GROUP BY htddata limit 1 ) AS query ), his.htddata, prc.prcdtinclusao) ) ) > 14
							THEN 1
							ELSE 0
							END AS maiorqueprazo,

						CASE WHEN 
							DATE_PART('day', ( COALESCE(his.htddata, prc.prcdtinclusao) - COALESCE( ( SELECT htddata FROM ( SELECT MAX(hstid) as hstid, htddata
							FROM workflow.historicodocumento
							WHERE docid = doc.docid AND his.hstid != hstid
							GROUP BY htddata limit 1 ) AS query ), his.htddata, prc.prcdtinclusao) ) ) < 15
							THEN 1
							ELSE 0
							END AS menorqueprazo,

						( SELECT query.advid FROM ( SELECT MAX(nudid), advid 
						FROM conjur.numeracaodocumento 
						WHERE prc.prcid = prcid AND advid IS NOT NULL
						GROUP BY advid LIMIT 1) AS query ) AS advidrequerente,

						( SELECT entsq.entnome FROM ( SELECT MAX(nudid), advid 
						FROM conjur.numeracaodocumento 
						WHERE prc.prcid = prcid AND advid IS NOT NULL
						GROUP BY advid LIMIT 1) AS query
						JOIN conjur.advogados advsq ON advsq.advid = query.advid
						JOIN entidade.entidade entsq ON entsq.entid = advsq.entid ) AS advogadorequerente,

						( SELECT MAX(hstid) as hstid
						FROM workflow.historicodocumento
						WHERE docid = doc.docid AND his.hstid != hstid ) as idpenultimatramitacao,

						his.htddata as dataultimatramitacao,

						( SELECT htddata FROM ( SELECT MAX(hstid) as hstid, htddata
						FROM workflow.historicodocumento
						WHERE docid = doc.docid AND his.hstid != hstid
						GROUP BY htddata limit 1 ) AS query ) as datapenultimatramitacao,

						( COALESCE(his.htddata, prc.prcdtinclusao) - COALESCE( ( SELECT htddata FROM ( SELECT MAX(hstid) as hstid, htddata
						FROM workflow.historicodocumento
						WHERE docid = doc.docid AND his.hstid != hstid
						GROUP BY htddata limit 1 ) AS query ), his.htddata, prc.prcdtinclusao) ) as intervaloultimapenultima
						
					FROM conjur.processoconjur prc
					INNER JOIN conjur.estruturaprocesso 	est ON est.prcid = prc.prcid
					LEFT JOIN workflow.documento 		doc ON doc.docid = est.docid
					LEFT JOIN workflow.historicodocumento 	his ON his.hstid = doc.hstid
					WHERE 
						his.htddata >= TIMESTAMP '{$this->periodos['inicio']}'
						AND his.htddata <= TIMESTAMP '{$this->periodos['fim']}'
						AND prc.prcstatus = 'A'

				) AS query
				$whereadv
				GROUP BY {$this->agrupamentoAdd} ";
		 //ver($sql,d);
		
		$this->dados['dados'] = $this->carregar( $sql );

		$this->verificacaoDados();

		// ver($this->dados,d);
		// $this->aplicaLinhaTotal();

		$this->dados['params']['totalLinhas'] = false;
		$this->dados['alinhamento'] = array('','right','right','right','right','right','right','right');

	}

	/**
	 * 
	 */
	private function verificacaoDados(){
		$dados = array();

		$totalProcessosAbaixo = 0;
		$totalProcessosAcima = 0;

		foreach ($this->dados['dados'] as $key => $value) {
			
			unset($value['totaldiasemprocessos']);
			unset($value['totalprocessos']);

			$totalProcessosAbaixo = $totalProcessosAbaixo + $value['totalmenorqueprazo'];
			$totalProcessosAcima = $totalProcessosAcima + $value['totalmaiorqueprazo'];
			$dados = $this->retiraDadosIncompletos( $value, $dados );

		}

		$array = array( 'Total', '-', $totalProcessosAbaixo, $totalProcessosAcima );
		array_push( $dados, $array );

		$this->dados['dados'] = $dados;
	}

	/**
	 * Insere com valor zerado os que não foram trazidos do banco de dados
	 *
	 * @todo arrumar
	 */
	private function aplicaFaltantes( Array $dados ){

		$sql = " SELECT adv.advid
				 FROM conjur.advogados adv
				 INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
				 WHERE adv.advid IN ( ".(implode(',',$this->advogados))." ) AND adv.advstatus = 'A' ";
		// ver($sql,d);
		$listaIdAdvogados = $this->carregar( $sql );

		$listaLimpa = array();
		foreach ($listaIdAdvogados as $key => $value) 
			array_push($listaLimpa, $value['advid']);
		// ver($listaLimpa,$dados,d);

		foreach ($dados as $key => $value) {
			// ver($value['advidrequerente'], $listaLimpa);
			// dbg(array_search($value['advidrequerente'], $listaLimpa));
			if( array_search($value['advidrequerente'], $listaLimpa) !== false )
				unset($listaLimpa[array_search($value['advidrequerente'], $listaLimpa)]);
			unset($dados[$key]['advidrequerente']);
		}

		$sql = " SELECT ent.entnome, adv.advid
				 FROM conjur.advogados adv
				 INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
				 WHERE adv.advid IN ( ".(implode(',',$listaLimpa))." ) AND adv.advstatus = 'A' ";
		// ver($sql,d);
		$listaNomeAdvogados = $this->carregar( $sql );
		// ver($listaNomeAdvogados,d);

		// ver($dados);
		foreach ($listaNomeAdvogados as $key => $value) {
			array_unshift($dados, array(
				'advogadorequerente' => $value['entnome'],
				'media' => '0',
				'totalmaiorqueprazo' => '0',
				'totalmenorqueprazo' => '0') );
		}
		// ver($dados,d);

		return $dados;
	}


	/**
	 * Retira, para advogados, as linhas cujo 'advogadorequerente' não consta,
	 * e para o caso das coordenacoes, o campo 'cordenacao'
	 */
	private function retiraDadosIncompletos( $value, $dados ){
		
		if( !empty($this->advogados[0]) )
			if( !empty($value['advogadorequerente']) )
				array_push($dados, $value);

		if( !empty($this->coordenacoes[0]) )
			if( !empty($value['coordenacao']) )
				array_push($dados, $value);

		return $dados;
	}

	/**
	 *
	 */
	private function aplicaLinhaTotal(){



		$this->dados['dados'] = array();

	}

	/**
	 * @todo fazer esse metodo participar de 
	 *		 uma classe abstrata, visto que é 
	 *		 a mesma em todas as classes dessa 
	 * 		 interface
	 */
	public function relatorio(){
		
		$this->monta_lista($this->dados['dados'], $this->dados['cabecalho'], '250','10', 'N', 'center', 'N', '', '', $this->dados['alinhamento'], '', $this->dados['params']);
		echo "<br/>";

	}
}