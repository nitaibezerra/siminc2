<?php

class RelatorioGeralTraducao
{
	// TRADUÇÃO CÓDIGO
	// 'nome' => array(
	//		'cod_campo' => '',
	//		'cod_alias' => '',
	//		'dsc_campo' => '',
	//		'dsc_alias' => '',
	//		'titulo' 	=> '',
	//		'tabela' 	=> '',
	//		'join' 		=> '',
	//		'join_dep' 	=> array()
	// )
	protected static $traducao = array(
		'ano'	=> array(
			'cod_campo' => 'EXTRACT(year from dmddatainclusao)',
			'cod_alias' => 'ano',
			'dsc_campo' => 'EXTRACT(year from dmddatainclusao)',
			'dsc_alias' => 'dscano',
			'titulo'    => 'Ano de Atendimento',
			'tabela'    => '',
			'join'		=> '',
			'join_dep'	=> array()
		),
		'usucpfdemandante'	=> array(
			'cod_campo' => 'demandante.usucpf',
			'cod_alias' => 'usucpf',
			//'dsc_campo' => 'demandante.usunome',
			'dsc_campo' => 'case when ( demandante.usunome is null ) then d.dmdnomedemandante else demandante.usunome end',
			'dsc_alias' => 'usunome',
			'titulo'    => 'Analista responsável',
			'tabela'    => 'seguranca.usuario',
			'join'		=> ' LEFT JOIN seguranca.usuario demandante ON demandante.usucpf = d.usucpfdemandante ',
			'join_dep'	=> array()
		),
		
		'esdid'	=> array(
			'cod_campo' => 'doc.esdid',
			'cod_alias' => 'docid',
			'dsc_campo' => 'estDoc.esddsc',
			'dsc_alias' => 'esddsc',
			'titulo'    => 'Situação',
			'tabela'    => 'workflow.documento',
			'join'		=> ' LEFT JOIN workflow.documento doc ON d.docid = doc.docid
							 LEFT JOIN workflow.estadodocumento estDoc ON estDoc.esdid = doc.esdid ',
			'join_dep'	=> array(  )
			#'join_dep'	=> array( 'uo' )
		),
		'mes'	=> array(
			'cod_campo' => 'EXTRACT(month from dmddatainclusao)',
			'cod_alias' => 'mes',
			'dsc_campo' => "CASE EXTRACT(month from dmddatainclusao)
							WHEN '1' THEN 'Janeiro'
							WHEN '2' THEN 'Fevereiro'
							WHEN '3' THEN 'Março'
							WHEN '4' THEN 'Abril'
							WHEN '5' THEN 'Maio'
							WHEN '6' THEN 'Junho'
							WHEN '7' THEN 'Julho'
							WHEN '8' THEN 'Agosto'
							WHEN '9' THEN 'Setembro'
							WHEN '10' THEN 'Outubro'
							WHEN '11' THEN 'Novembro'
							WHEN '12' THEN 'Dezembro'
							END ",
			'dsc_alias' => 'dscmes',
			'titulo'    => 'Mês de atendimento',
			'tabela'    => "(select '1'::text as mes, 'Janeiro'::text as dscmes
							union
							select '2'::text as mes, 'Fevereiro'::text as dscmes
							union
							select '3'::text as mes, 'Março'::text as dscmes
							union
							select '4'::text as mes, 'Abril'::text as dscmes
							union
							select '5'::text as mes, 'Maio'::text as dscmes
							union
							select '6'::text as mes, 'Junho'::text as dscmes
							union
							select '7'::text as mes, 'Julho'::text as dscmes
							union
							select '8'::text as mes, 'Agosto'::text as dscmes
							union
							select '9'::text as mes, 'Setembro'::text as dscmes
							union
							select '10'::text as mes, 'Outubro'::text as dscmes
							union
							select '11'::text as mes, 'Novembro'::text as dscmes
							union
							select '12'::text as mes, 'Dezembro'::text as dscmes
							) as tm",
			'join'		=> '',
			'join_dep'	=> array()
		),
		'usucpfgerente'	=> array(
			'cod_campo' => 'gerente.usucpf',
			'cod_alias' => 'usucpf',
			'dsc_campo' => 'gerente.usunome',
			'dsc_alias' => 'usunome',
			'titulo'    => 'Gerente',
			'tabela'    => 'seguranca.usuario',
			'join'		=> 'LEFT JOIN demandas.sistemadetalhe sd ON d.sidid = sd.sidid 
							LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.sidid = sd.sidid AND ur.rpustatus = \'A\'
							LEFT JOIN seguranca.usuario gerente ON gerente.usucpf = ur.usucpf',
			'join_dep'	=> array()
		),
		'priid'	=> array(
			'cod_campo' => 'd.priid',
			'cod_alias' => 'priid',
			'dsc_campo' => 'pri.pridsc',
			'dsc_alias' => 'pridsc',
			'titulo'    => 'Prioridade',
			'tabela'    => 'demandas.prioridade',
			'join'		=> ' LEFT JOIN demandas.prioridade pri ON pri.priid = d.priid ',
			'join_dep'	=> array()
		),
		'usucpfexecutor'	=> array(
			'cod_campo' => 'executor.usucpf',
			'cod_alias' => 'usucpfexecutor',
			'dsc_campo' => 'executor.usunome',
			'dsc_alias' => 'executor_nome',
			'titulo'    => 'Técnico',
			'tabela'    => 'seguranca.usuario',
			'join'		=> ' LEFT JOIN seguranca.usuario executor ON executor.usucpf = d.usucpfexecutor ',
			'join_dep'	=> array()
		),
		'ordid'	=> array(
			'cod_campo' => 'od.ordid',
			'cod_alias' => 'ordid',
			'dsc_campo' => 'od.orddescricao',
			'dsc_alias' => 'orddescricao',
			'titulo'    => 'Origem da Demanda',
			'tabela'    => 'demandas.origemdemanda',
			'join'		=> 'LEFT JOIN demandas.tiposervico ts on ts.tipid = d.tipid 
							LEFT JOIN demandas.origemdemanda od ON od.ordid = ts.ordid ',
			'join_dep'	=> array()
		),
		'ordid'	=> array(
			'cod_campo' => 'ua.unaid',
			'cod_alias' => 'unaid',
			'dsc_campo' => 'ua.unadescricao',
			'dsc_alias' => 'unadescricao',
			'titulo'    => 'Setor',
			'tabela'    => 'demandas.unidadeatendimento',
			'join'		=> 'LEFT JOIN demandas.unidadeatendimento ua on d.unaid = ua.unaid',
			'join_dep'	=> array()
		),
		
		'ungcod'	=> array(
			'cod_campo' => 'ug.ungcod',
			'cod_alias' => 'ungcod',
			'dsc_campo' => 'ug.ungdsc',
			'dsc_alias' => 'ungdsc',
			'titulo'    => 'Orgão',
			'tabela'    => 'public.unidadegestora',
			'join'		=> 'LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf
							LEFT JOIN public.unidadegestora ug ON u.ungcod = ug.ungcod OR u.ungcod = ug.ungcod ',
			'join_dep'	=> array()
		),
		'sidid'	=> array(
			'cod_campo' => 'sde.sidid',
			'cod_alias' => 'sidid',
			'dsc_campo' => 'sde.siddescricao',
			'dsc_alias' => 'siddescricao',
			'titulo'    => 'Sistema',
			'tabela'    => 'demandas.sistemadetalhe',
			'join'		=> ' LEFT JOIN demandas.sistemadetalhe sde ON sde.sidid = d.sidid ',
			'join_dep'	=> array()
		),
		'atendforaprazo' => array(
			'cod_campo' => "case when ( d.dmddatafimprevatendimento > dataatendfinalizado ) then 'DENTROPRAZO' else 'FORAPRAZO' end",
			'cod_alias' => 'atend',
			'dsc_campo' => "case when ( d.dmddatafimprevatendimento > dataatendfinalizado ) then 'DENTROPRAZO' else 'FORAPRAZO' end",
			'dsc_alias' => 'dscatend',
			'titulo'    => 'Atend. Dentro e Fora do Prazo',
			'tabela'    => '',
			'join'		=> " LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, max(a.htddata) as dataatendfinalizado						
												from 	workflow.historicodocumento a
													inner join workflow.documento c on c.docid = a.docid
											where a.aedid in (146, 191) 
											group by a.docid
											) ) as hst ON hst.docid = d.docid ",
			'join_dep'	=> array()
		)
	);
	
	public static function existe( $nome )
	{
		return array_key_exists( $nome, self::$traducao );
	}
	
	private static function pegar( $nome, $tipo )
	{
		if ( isset( self::$traducao[$nome][$tipo] ) == false )
		{
			return '';
		}
		return self::$traducao[$nome][$tipo];
	}
	
	public static function pegarAliasCodigo( $nome )
	{
		return self::pegar( $nome, 'cod_alias' );
	}
	
	public static function pegarAliasDescricao( $nome )
	{
		return self::pegar( $nome, 'dsc_alias' );
	}
	
	public static function pegarCampoCodigo( $nome )
	{
		return self::pegar( $nome, 'cod_campo' );
	}
	
	public static function pegarCampoDescricao( $nome )
	{
		return self::pegar( $nome, 'dsc_campo' );
	}
	
	public static function pegarTabela( $nome )
	{
		return self::pegar( $nome, 'tabela' );
	}
	
	public static function pegarTitulo( $nome )
	{
		return self::pegar( $nome, 'titulo' );
	}
	
	public static function pegarJoin( $nome )
	{
		return self::pegar( $nome, 'join' );
	}
	
	public static function pegarJoinDependencia( $nome )
	{
		return self::pegar( $nome, 'join_dep' );
	}
	
	public static function pegarAgrupadores()
	{
		//$proibidos = array( 'ano', 'ug' );
		$proibidos = array(); // array('ano');
		$retorno = array();
		foreach ( self::$traducao as $codigo => $dados )
		{
			if ( in_array( $codigo, $proibidos ) == true )
			{
				continue;
			}
			$novoItem = array(
				'codigo' => $codigo,
				'descricao' => $dados['titulo']
			);
			array_push( $retorno, $novoItem );
		}
		return $retorno;
	}
	
}


class RelatorioGeral
{
	
	
	protected $tabela = 'demandas.demanda d';
	
	protected $agrupador = array();
	
	/**
	 * Enter description here...
	 *
	 * @var integer
	 */
	protected $escala = 1;
	
	protected $filtro = array();
	
	protected $filtroExcludente = array();
	
	protected $titulo = 'Relatório de Indicadores';
	
	protected $filtrados = array();
	
	/**
	 * Adiciona um filtro a ser acrescentado à requisição.
	 * 
	 * @param string $campo
	 * @param string $valor
	 * @param boolean $excludente
	 * @return void
	 */
	public function adicionarFiltro( $campo, $valor, $excludente = false )
	{
		$campoCod = RelatorioGeralTraducao::pegarCampoCodigo( $campo );
		if ( array_key_exists( $campoCod, $this->filtro ) == false )
		{
			$this->filtro[$campoCod] = array();
		}
		if ( is_array( $valor ) == true )
		{
			foreach ( $valor as $item )
			{
				array_push( $this->filtro[$campoCod], $item );
			}
		}
		else
		{
			array_push( $this->filtro[$campoCod], $valor );
		}
		if ( $excludente == true )
		{
			$this->filtroExcludente[$campo] = $campoCod;
		}
		array_push( $this->filtrados, $campo );
	}
	
	public function adicionarAgrupador( $campo )
	{
		if ( RelatorioGeralTraducao::existe( $campo ) == false || in_array( $campo, $this->agrupador ) == true )
		{
			return;
		}
		array_push( $this->agrupador, $campo );
		array_push( $this->filtrados, $campo );
	}
	
	protected function agrupar( &$itens, $nivelAgrupador )
	{
		if ( !$itens || count( $itens ) < 1 || array_key_exists( $nivelAgrupador, $this->agrupador ) == false )
		{
			return array();
		}
		$this->agruparPorCampo( $itens, $nivelAgrupador  );
		if ( $nivelAgrupador < count( $this->agrupador ) - 2 )
		{
			foreach ( $itens as &$item )
			{
				$this->agrupar( $item['itens'], $nivelAgrupador + 1 );
			}
		}
	}
	
	protected function agruparPorCampo( &$itens, $nivelAgrupador )
	{
		$agrupador = $this->agrupador[$nivelAgrupador];
		$aliasCod = RelatorioGeralTraducao::pegarAliasCodigo( $agrupador );
		$aliasDsc = RelatorioGeralTraducao::pegarAliasDescricao( $agrupador );
		//dbg( $itens, 1 );
		$resultado = array();
		foreach ( $itens as $item )
		{
			$chaveGrupo = $item[$aliasCod] . $item[$aliasDsc];
			
			// captura a descrição do item no nivel que está agrupado
			// os itens sempre terão os índices 'cod' e 'dsc' que conterão o código e descrição no agrupador atual
			if ( count( $item ) >= 3 )
			{
				$nivelSelecionado = $nivelAgrupador;
				if ( array_key_exists( $nivelAgrupador + 1, $this->agrupador ) == true )
				{
					$nivelSelecionado++;
				}
				$campoCod = RelatorioGeralTraducao::pegarAliasCodigo( $this->agrupador[$nivelSelecionado] );
				$campoDsc = RelatorioGeralTraducao::pegarAliasDescricao( $this->agrupador[$nivelSelecionado] );
				$item['cod'] = $item[$campoCod];
				$item['dsc'] = $item[$campoDsc];
			}
			
			// registra item na lista de resultado
			// caso só exista um nível de agrupador o array terá apenas um nível de profundidade
			// caso contrário os itens agrupador por índices de seus grupos
			if ( count( $this->agrupador ) > 1 )
			{
				if ( array_key_exists( $chaveGrupo, $resultado ) == false )
				{
					$resultado[$chaveGrupo] = array(
						'cod' => $item[$aliasCod],
						'dsc' => $item[$aliasDsc],
						'itens' => array()
					);
				}
				$chaveItem = $item['cod'].$item['dsc'];
				array_push( $resultado[$chaveGrupo]['itens'], $item );
			}
			else
			{
				$resultado[$chaveGrupo] = $item;
			}
		}
		$itens = $resultado;
	}
	
	public function alterarEscala( $escala )
	{
		$this->escala = abs( (integer) $escala );
	}
	
	public function alterarTitulo( $titulo )
	{
		$this->titulo = (string) $titulo;
	}
	
	public function pegarAgrupadores( $titulo = false )
	{
		if ( $titulo == false )
		{
			return $this->agrupador;
		}
		$agrupadores = array();
		foreach ( array_keys( $this->agrupador ) as $nivel )
		{
			array_push( $agrupadores, $this->pegarTituloAgrupador( $nivel ) );
		}
		return $agrupadores;
	}
	
	public function pegarEscala()
	{
		return $this->escala;
	}
	
	public function pegarFixos()
	{
		return ' count( dmdid ) as nrDemandas, round( ( count( dmdid ) * 100 ) / sum(distinct a.total), 2 ) as percentual ';
	}
	
	public function pegarTitulo()
	{
		return $this->titulo;
	}
	
	public function pegarTituloAgrupador( $nivel = null )
	{
		$titulo = array();
		if ( $nivel === null )
		{
			foreach ( $this->agrupador as $agrupador )
			{
				array_push( $titulo, RelatorioGeralTraducao::pegarTitulo( $agrupador ) );
			}
		}
		else
		{
			array_push( $titulo, RelatorioGeralTraducao::pegarTitulo( $this->agrupador[$nivel] ) );
		}
		return implode( '/', $titulo );
	}
	
	/**
	 * Captura os campos selecionados. Caso o primeiro parâmetro seja verdadeiro
	 * é adicionado o alias do campos.
	 *
	 * @param boolean $alias
	 * @return string
	 */
	protected function pegarSelecionados( $alias )
	{
		$selecionados = array();

		foreach ( $this->agrupador as $campo )
		{
			$codigo = RelatorioGeralTraducao::pegarCampoCodigo( $campo );
			$descricao = RelatorioGeralTraducao::pegarCampoDescricao( $campo );
			if ( $alias )
			{
				$codigo .= ' as ' . RelatorioGeralTraducao::pegarAliasCodigo( $campo );
				$descricao .= ' as ' . RelatorioGeralTraducao::pegarAliasDescricao( $campo );
			}
			array_push( $selecionados, $codigo );
			array_push( $selecionados, $descricao );
		}

		return $selecionados;
	}
	
	public function pegarValoresFiltro( $campo )
	{
		$nomeCampo = RelatorioGeralTraducao::pegarCampoCodigo( $campo );
		if ( array_key_exists( $nomeCampo, $this->filtro ) == false )
		{
			return array();
		}
		return $this->filtro[$nomeCampo];
	}
	
	protected function montaFiltroSql()
	{
		$where = '';
		foreach ( $this->filtro as $campo => $valores )
		{
			$quantidadeValores = count( $valores );
			$excludente = in_array( $campo, $this->filtroExcludente );
			$valores = '\'' . implode( '\',\'', $valores ) . '\'';
			if ( $quantidadeValores > 1 )
			{
				$operador = $excludente ? ' not in ' : ' in ' ;
				$valores = ' ( ' . $valores . ' ) ';
			}
			else
			{
				$operador = $excludente ? ' != ' : ' = ' ;
			}
			$where .= ' AND ' . $campo . $operador . $valores ;
		}
		if ( $where != '' )
		{
			$where = ' WHERE ' . substr( $where, 5 );
		}
		return $where;
	}
	
	protected function pegarCamposFiltrados()
	{
		return array_unique( $this->filtrados );
	}
	
	/**
	 * Monta requisição a ser executada para a realização da consulta. O texto
	 * retornado retorna os campos agrupados pelo agrupador e sumarizador, após
	 * a reqlização da requisição com o selecte retornado é necessário chamar o
	 * método ConsultaFinanceiro::agrupar()
	 * Caso o primeiro parâmetro seja falso é retornado somente o esqueleto da
	 * query, onde não há campos selecionados nem agrupamento e ordenação.
	 * 
	 * @param string $campos
	 * @return string
	 */
	public function montaRequisicao( $campos = true )
	{
		// verifica se há campos a ser selecionados
		$selecionados = $this->pegarSelecionados( true );
		if ( count( $selecionados ) < 1 || count( $this->agrupador ) < 1 )
		{
			return '';
		}

		// define campos selecionados, agrupamento e ordenação
		$select = '';
		$join  = '';
		$groupby = '';
		$orderby = '';
		if ( $campos == true )
		{
			$select = implode( ', ', $selecionados );
			$camposSemAlias = implode( ', ', $this->pegarSelecionados( false ) );
			$groupby = ' GROUP BY ' . $camposSemAlias;
			$orderby = ' ORDER BY ' . $camposSemAlias;
			$selecionados = array_merge( $this->agrupador, array_keys( $this->filtro ), array_keys( $this->filtroExcludente ) );
			$join = array();
			foreach ( $this->pegarCamposFiltrados() as $campo )
			{
				foreach ( RelatorioGeralTraducao::pegarJoinDependencia( $campo ) as $campoJoinDep )
				{
					$join[$campoJoinDep] = RelatorioGeralTraducao::pegarJoin( $campoJoinDep );
				}
				$join[$campo] = RelatorioGeralTraducao::pegarJoin( $campo );
			}
			$join = implode( ' ', $join );
		}

		// define filtro
		$where = $this->montaFiltroSql();
		
		// define escala dos valores
		$escala = '';
		if ( $this->escala > 1 )
		{
			$escala = ' / ' . $this->escala;
		}
		
		$fixos = $this->pegarFixos();
		$fixos = empty( $fixos ) ? '' : ', '. $fixos;

		// monta requisição
		$sql =
			' SELECT  
				' . $select . '
				' . $fixos . '
			 FROM ' . $this->tabela . '
			 '. $join . ' 
			 LEFT JOIN ( select count(*) as total from demandas.demanda ) a ON 1=1
			' . $where . '
			' . $groupby . '
			' . $orderby;
		//dbg($sql,1);
		return $sql;
	}
	
	/**
	 * Realiza consulta com os dados presentes na classe.
	 * 
	 * @return string[]
	 */
	public function consultar()
	{
		global $db;
		$sql = $this->montaRequisicao();
/*
echo '<pre>' . $sql;
dbg( $sql );
die;
*/

		if ( $sql == '' )
		{
			return array();
		}

		$itens = $db->carregar( $sql );
		$this->agrupar( $itens, 0 );
		return array( 'itens' => $itens );
	}
	
	public function filtroExcludente( $campo )
	{
		return array_key_exists( $campo, $this->filtroExcludente );
	}

}

class ConsultaFinanceiroOrcamento extends RelatorioGeral 
{

	protected $tabela = 'financeiro.execucaomec ';
	
	static protected $selecionados = array(
		'orc.gstcod' => 'gstcod',
		'orc.frscod' => 'frscod'
	);

	protected function pegarSelecionados( $alias )
	{
		$selecionados = array();

		foreach ( ConsultaFinanceiroOrcamento::$selecionados as $campo => $apelido )
		{
			if ( $alias )
			{
				$campo .= ' as '. $apelido;
			}
			array_push( $selecionados, $campo );
		}
		return array_merge( parent::pegarSelecionados( $alias ), $selecionados );
	}

	public function pegarFixos()
	{

		if( $_REQUEST['rap'] == null ) 
		{
			return
			' sum( coalesce( execredrecdestadmdir , 0 ) + coalesce( execredrecdestadmind , 0 ) + coalesce( execredrecprov , 0 ) ) / ' . $this->escala . ' as credito_recebido, ' .
			' sum( coalesce( exeempemit, 0 ) ) / ' . $this->escala . ' as empenhos_emitidos, ' .
			' sum( coalesce( exeempliqpagosapagar, 0 ) ) / ' . $this->escala . ' as empenhos_liquidados, ' .
			' sum( coalesce( exevalrepasreceb, 0 ) ) as repasse_recebido, ' . 
			' sum( coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as valores_pagos, ' .
			' sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as liquidado_pagar, ' .
			' sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' as saldo_financeiro, ' .
			' sum( coalesce( exevallimsaque, 0 ) ) / ' . $this->escala . ' as limite_saque, ' .
			' case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			' ( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) )  / ' . $this->escala . ' ) ' .
			' else 0 end as valor_recompor, ' .
			' case when sum( coalesce( exevallimsaque, 0) ) < sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			' ( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) - sum( coalesce( exevallimsaque, 0) ) / ' . $this->escala . ') ' .
			' else 0 end as valor_detalhar, ' .
	
			' case sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) ' .
			
			' when 0 then ' .
			'	case when sum( coalesce( exevallimsaque, 0 ) ) > 0 then sum( coalesce( exevallimsaque, 0 ) ) / ' . $this->escala . ' else 0 end ' .
	
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			'		( sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ')  ' .
			'	end ' .
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevallimsaque, 0 ) ) = sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then 0 end ' .
			'	end ' .
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < sum( coalesce( exevallimsaque, 0 ) ) then sum( coalesce( exevallimsaque, 0 ) ) - sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' end ' .
			'	end ' .
	
			' when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) then ' .
			'	case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > 0 then ' .
			'			case when sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) > sum( coalesce( exevallimsaque, 0 ) ) then sum( coalesce( exevallimsaque, 0 ) ) - sum( coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' end ' .
			'	end ' .
			
			' end as valore_desdetalhar, ' .
			
			' case when sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalrepasreceb, 0 ) + coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) < 0 then ' .
			'		sum( coalesce( exeempliqpagosapagar, 0 ) - coalesce( exevalrepasreceb, 0 ) + coalesce( exevalrepasreceb, 0 ) - coalesce( exevalliqpagos, 0 ) ) / ' . $this->escala . ' else 0 end as repassado_excesso, ' .
			
			' sum( coalesce( exeempliqpagosapagar, 0 ) ) / ' . $this->escala . ' as valor_repassar, ' .
			' sum( coalesce( 0, 0 ) ) / ' . $this->escala . ' as valor_proposto ';
			}
		else
		{
					
			return
			' sum( coalesce( exerapliquidado , 0 ) ) / ' . $this->escala . ' as processado_mes0, ' .
			' sum( coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as nao_processado_mes0, ' .
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as total_mes0, ' .
			' sum( coalesce( exerapliquidado , 0 ) ) / ' . $this->escala . ' as processado_atual, ' .
			' sum( coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as nao_processado_atual, ' .
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as total_atual, ' .
			' sum( coalesce( exerappago, 0 ) ) as pago, ' . 
			' sum( coalesce( exerapliquidado, 0 ) + coalesce( exerapaliqbloq, 0 ) ) / ' . $this->escala . ' as atual_total, ' .
			' sum( coalesce( exerapliqexercantrapprocapagar, 0 ) ) / ' . $this->escala . ' as processado_apagar, ' .
			' sum( coalesce( exeraptransfreceb, 0 ) ) / ' . $this->escala . ' as nao_processado_apagar, ' .
			' sum( coalesce( exerapaliq, 0 ) ) / ' . $this->escala . ' as total_apagar, ' .
			' sum( coalesce( exeraptransf, 0 ) ) / ' . $this->escala . ' as valora_repassar '; 

		}
		
	}

}
	


?>