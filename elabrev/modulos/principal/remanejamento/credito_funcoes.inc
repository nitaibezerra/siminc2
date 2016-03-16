<?php

// ---------- FUNCOES ----------
	
	// ----- CHECAGEM

	function cre_edicao_ativa()
	{
		global $db;
		$mcrid = (integer) $_SESSION['credito_mcrid'];
		$sql = "select * from elabrev.momentocredito where mcrid = " . $mcrid;
		$momento = $db->recuperar( $sql );
		if ( !$momento || $momento['mcrstatus'] != 'A' )
		{
			return false;
		}
		$data_inicio = '';
		$data_fim    = '';
		if ( $db->usuarioPossuiPermissaoTodasUnidades() )
		{
			$data_inicio = $momento['mcrdatainiciocgo'];
			$data_fim    = $momento['mcrdatafimcgo'];
		} 
		else
		{
			$data_inicio = $momento['mcrdatainiciouo'];
			$data_fim    = $momento['mcrdatafimuo'];
		}
		$data_inicio = explode( '-', reset( explode( ' ', $data_inicio ) ) );
		$data_fim    = explode( '-', reset( explode( ' ', $data_fim ) ) );
		$agora  = mktime( 0, 0, 0 );
		$inicio = mktime( 0, 0, 0, $data_inicio[1], $data_inicio[2], $data_inicio[0] );
		$fim    = mktime( 0, 0, 0, $data_fim[1], $data_fim[2], $data_fim[0] );
		/*
		dbg( $data_inicio );
		dbg( $data_fim );
		dbg( $agora . ' >= ' . $inicio . ' && ' . $agora . ' <= ' . $fim );
		*/
		return $agora >= $inicio && $agora <= $fim;
	}

	// ----- FORMATAÇÂO

	function cre_pegar_numero_cru( $numero )
	{
		return str_replace( '.', '', $numero );
	}

	// ----- EXISTENCIA
	
	function cre_ndpcod_existe( $cod )
	{
		static $cods = array();
		if ( array_key_exists( $cod, $cods ) )
		{
			return $cods[$cod];
		}
		global $db;
		$sql = "select count(*) as existe from public.naturezadespesa where ndpstatus = 'A' and ndpcod = '"  . $cod . "'";
		$cods[$cod] = (boolean) $db->pegaUm( $sql );
		return $cods[$cod];
	}
	
	function cre_nrccod_existe( $cod )
	{
		static $cods = array();
		if ( array_key_exists( $cod, $cods ) )
		{
			return $cods[$cod];
		}
		global $db;
		$sql = "select count(*) as existe from public.naturezareceita where nrcstatus = 'A' and nrccod = '"  . $cod . "'";
		$cods[$cod] = (boolean) $db->pegaUm( $sql );
		return $cods[$cod];
	}

	function cre_foncod_existe( $cod )
	{
		static $cods = array();
		if ( array_key_exists( $cod, $cods ) )
		{
			return $cods[$cod];
		}
		global $db;
		$sql = "select count(*) as existe from public.fonterecurso where fonstatus = 'A' and foncod = '"  . $cod . "'";
		$cods[$cod] = (boolean) $db->pegaUm( $sql );
		return $cods[$cod];
	}

	function cre_idocod_existe( $cod )
	{
		static $cods = array();
		if ( array_key_exists( $cod, $cods ) )
		{
			return $cods[$cod];
		}
		global $db;
		$sql = "select count(*) as existe from public.idoc where idostatus = 'A' and idocod = '"  . $cod . "'";
		$cods[$cod] = (boolean) $db->pegaUm( $sql );
		return $cods[$cod];
	}
	
	function cre_iducod_existe( $cod )
	{
		static $cods = array();
		if ( array_key_exists( $cod, $cods ) )
		{
			return $cods[$cod];
		}
		global $db;
		$sql = "select count(*) as existe from public.identifuso where idustatus = 'A' and iducod = '"  . $cod . "'";
		$cods[$cod] = (boolean) $db->pegaUm( $sql );
		return $cods[$cod];
	}
	
	// ----- REMOVER
	
	// FUNÇÃO DESATIVADA
	function cre_remover( $tcrid, array $unicod, array $prgcod, array $acacod, array $loccod )
	{
		dbg( '!!! método desativado !!!' );
		return;
		
		global $db;
		
		$ppoid = cre_pegar_proposta_ativa();
		$tcrid = (integer) $tcrid;
		
		// captura os acaid das ações com o unicod, prgcod, acacod e loccod
		$where_acaid = array();
		if ( $acacod && $acacod[0] )
		{
			array_push( $where_acaid, " acacod in ( '" . implode( "','", $acacod ) . "' ) " );
		}
		if ( $unicod && $unicod[0] )
		{
			array_push( $where_acaid, " unicod in ( '" . implode( "','", $unicod ) . "' ) " );
		}
		if ( $prgcod && $prgcod[0] )
		{
			array_push( $where_acaid, " prgcod in ( '" . implode( "','", $prgcod ) . "' ) " );
		}
		if ( $loccod && $loccod[0] )
		{
			array_push( $where_acaid, " loccod in ( '" . implode( "','", $loccod ) . "' ) " );
		}
		$acaid = array();
		if ( $where_acaid )
		{
			$sql =
				" select " .
					" acaid " .
				" from monitora.acao " . 
				" where " .
					implode( ' and ', $where_acaid );
			$res = $db->carregar( $sql );
			if ( $res )
			{
				foreach ( $res as $linha )
				{
					if ( !in_array( $linha['acaid'], $acaid ) )
					{
						array_push( $acaid, $linha['acaid'] );
					}
				}
			}
		}
		// verifica quantidade de acaids encontrados
		if ( !count( $acaid ) )
		{
			return array();
		}
		
		// adiciona filtro referente ao tipo de credito
		$condicao = array();
			// acaid
			// une ações do tipo com as ações filtradas
			$sql = "select acaid from elabrev.tipocreditoacao where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['acaid'] );
				}
				$acaid = array_unique( array_merge( $acaid, $cods ) );
				array_push( $condicao, " d.acaidloa in ( '" . implode( "','", $acaid ) . "' ) " );
			}
			// gnd
			$sql = "select gndcod from elabrev.tipocreditognd where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['gndcod'] );
				}
				array_push( $condicao, " n.gndcod in ( '" . implode( "','", $cods ) . "' ) " );
			}
			// grupo unidade
			$sql = "select gunid from elabrev.tipocreditogrupounidade where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['gunid'] );
				}
				array_push( $condicao, " u.gunid in ( '" . implode( "','", $cods ) . "' ) " );
			}
			// unidade
			$sql = "select unicod, unitpocod from elabrev.tipocreditounidade where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods_cod = array();
				$cods_tpo = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods_cod, $linha['unicod'] );
					array_push( $cods_tpo, $linha['unitpocod'] );
				}
				array_push( $condicao, " u.unicod in ( '" . implode( "','", $cods_cod ) . "' ) " );
				array_push( $condicao, " u.unitpocod in ( '" . implode( "','", $cods_tpo ) . "' ) " );
			}
		array_push( $condicao , " d.tcrid = " . $tcrid );
		array_push( $condicao , " d.ppoid = " . $ppoid );
		array_push( $condicao , " d.mcrid = " . $_SESSION['credito_mcrid'] );
		
		$where = implode( " and ", $condicao );
		$sql = <<<EOF
			select
				dpaid
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
				inner join public.naturezadespesa n on
					n.ndpid = d.ndpid
				inner join public.unidade u on
					u.unicod = a.unicod and
					u.unitpocod = a.unitpocod
			where
				$where
EOF;
		$dados = $db->carregar( $sql );
		if ( !$dados )
		{
			return true;
		}
		$dpaid = array();
		foreach ( $dados as $linha )
		{
			array_push( $dpaid, $linha['dpaid'] );
		}
		$sql = 
			" delete from elabrev.despesaacao " . 
			" where " .
				" dpaid in ( " . implode( ",", $dpaid ) . " ) ";
		return (boolean) $db->executar( $sql );
	}
	
	// ----- PEGAR
	
	function cre_pegar_receitas( $tcrid, $unicod )
	{
		global $db;
		$ano = $_SESSION['exercicio'];
		$sql = <<<EOT
select
	r.*,
	f.fondsc,
	n.nrccod,
	n.nrcdsc,
	coalesce( dot.valor, 0 ) as dot,
	coalesce( outros_acr.valor, 0 ) as outros_acr,
	coalesce( outros_red.valor, 0 ) as outros_red
from elabrev.receitaunidade r
	inner join public.fonterecurso f on
		f.foncod = r.foncod
	inner join public.naturezareceita n on
		n.nrcid = r.nrcid
	left join (
		select
			f.foncod,
			n.nrcid,
			r.unicod,
			r.rcdvalor as valor
		from elabrev.receitaunidadedotacao r
			inner join public.fonterecurso f on
				f.foncod = r.foncod
			inner join public.naturezareceita n on
				n.nrcid = r.nrcid
		where
			r.rcdano = '$ano'
	) dot on
		dot.foncod = r.foncod and
		dot.nrcid = r.nrcid and
		dot.unicod = r.unicod
	left join (
		select
			foncod,
			nrcid,
			unicod,
			sum( rcuacrescimo ) as valor
		from elabrev.receitaunidade
		where
			rcuano = '$ano' and
			(
				tcrid != $tcrid or
				mcrid != {$_SESSION['credito_mcrid']}
			)
		group by
			foncod,
			nrcid, unicod
	) outros_acr on
		outros_acr.foncod = r.foncod and
		outros_acr.nrcid = r.nrcid and
		outros_acr.unicod = r.unicod
	left join (
		select
			foncod,
			nrcid,
			unicod,
			sum( rcureducao ) as valor
		from elabrev.receitaunidade
		where
			rcuano = '$ano' and
			(
				tcrid != $tcrid or
				mcrid != {$_SESSION['credito_mcrid']}
			)
		group by
			foncod,
			nrcid, unicod
	) outros_red on
		outros_red.foncod = r.foncod and
		outros_red.nrcid = r.nrcid and
		outros_red.unicod = r.unicod
where
	r.rcuano = '$ano' and
	r.tcrid = $tcrid and
	r.unicod = '$unicod' and
	r.mcrid = {$_SESSION['credito_mcrid']}
order by
	n.nrccod,
	f.foncod
EOT;

		$receitas = $db->carregar( $sql );
		return !$receitas ? array () : $receitas;
	}
	
	function cre_pegar_acaid( $unicod, $prgcod, $acacod, $loccod )
	{
		static $acaid = array();
		$unicod = (string) $unicod;
		$prgcod = (string) $prgcod;
		$acacod = (string) $acacod;
		$loccod = (string) $loccod;
		$chave = $unicod . $prgcod . $acacod . $loccod;
		if ( array_key_exists( $chave, $acaid ) == true )
		{
			return $acaid[$chave];
		}
		global $db;
		$sql = sprintf(
			" select acaid from monitora.acao " .
				" where unicod = '%s' and prgcod = '%s' and acacod = '%s' and loccod = '%s' and prgano = '%s' and acasnrap = '%s' ",
			$unicod,
			$prgcod,
			$acacod,
			$loccod,
			$_SESSION['exercicio'],
			'f'
		);
		$acaid[$chave] = (integer) $db->pegaUm( $sql );
		return $acaid[$chave];
	}
	
	function cre_pegar_dados_despesas( array $dpaid )
	{
		global $db;
		$dpaid = array_map( 'intval', $dpaid );
		$sql =
			" select " .
				" d.*, " .
				" a.unicod, " .
				" a.loccod, " .
				" a.acacod, " .
				" n.ndpcod, " .
				" i.idocod, " .
				" u.unicod " .
			" from elabrev.despesaacao d " .
				" inner join monitora.acao a on " .
					" a.acaid = d.acaidloa " .
				" inner join public.naturezadespesa n on " .
					" n.ndpid = d.ndpid " .
				" inner join public.idoc i on " .
					" i.idoid = d.idoid " .
				" inner join public.unidade u on " .
					" u.unicod = a.unicod and " .
					" u.unitpocod = a.unitpocod " .
			" where " .
				" d.dpaid in ( " . implode( ",", $dpaid ) . " ) ";
		$dados = $db->recuperar( $sql );
		return $dados ? $dados : array();
	}
	
	function cre_pegar_dados_despesa( $dpaid )
	{
		global $db;
		$dpaid = (integer) $dpaid;
		$sql =
			" select " .
				" d.*, " .
				" a.unicod, " .
				" a.loccod " .
			" from elabrev.despesaacao d " .
				" inner join monitora.acao a on " .
					" d.acaidloa = a.acaid " .
			" where " .
				" d.dpaid = " . $dpaid;
		$dados = $db->recuperar( $sql );
		return $dados ? $dados : array();
	}
	
	function cre_pegar_dotacao_inicial( $cod, $tcrtipoporcento )
	{
		global $db;
		$ppoid = cre_pegar_proposta_ativa();
		$campo = $tcrtipoporcento == 'L' ? 'a.loccod' : 'a.unicod';
		$sql =
			" select " .
				" sum( coalesce( d.dpavalor, 0 ) ) as dot " .
			" from elabrev.despesaacao d " .
				" inner join monitora.acao a on " .
					" a.acaid = d.acaidloa " .
			" where " .
				" d.tcrid is null and " .
				" d.ppoid = " . $ppoid . " and " .
				" d.mcrid = {$_SESSION['credito_mcrid']} " .
			" group by " .
				$campo;
		return (integer) $db->pegaUm( $sql );
	}
	
	function cre_pegar_proposta_ativa()
	{
		static $ppoid = null;
		if ( !$ppoid )
		{
			global $db;
			//$ano = $db->pega_ano_atual();
			$ano = $_SESSION['exercicio'];
			$sql =
				" select " .
					" ppoid " .
				" from elabrev.propostaorcamento " .
				" where " .
					" ppoano = '" . $ano . "' and " .
					///" tppid = " . TIPO_LOA . " and " .
					" tppid = 3 and " .
					" ppostatus = 'A'";
			$ppoid = (integer) $db->pegaUm( $sql );
		}
		return $ppoid;
	}
		
	function cre_pegar_remanejamentos( $tcrid, array $unicod = array(), array $prgcod = array(), array $acacod = array(), array $loccod = array(), $ungcod = null )
	{
		global $db;
		
		// verifica tipo de crédito
		$tcrid = (integer) $tcrid;
		if ( !$tcrid )
		{
			return array();
		}
		else{
			$sql = "select tcrcod from elabrev.tipocredito where tcrid = " . $tcrid;
			$tcrcod = $db->pegaUm( $sql );
		}
		
		// captura proposta que está sendo trabalhada
		$ppoid = cre_pegar_proposta_ativa();
		
		$condicao = array();
		$condicao_invalido_fixo = array();
		array_push( $condicao, " a.prgano = '" . $_SESSION['exercicio'] . "' " );
		array_push( $condicao, " a.acasnrap = 'f' " );
		array_push( $condicao, " d.ppoid = " . $ppoid );
		array_push( $condicao, " d.tcrid is null " );
		array_push( $condicao_invalido_fixo, " a.prgano = '" . $_SESSION['exercicio'] . "' " );
		array_push( $condicao_invalido_fixo, " a.acasnrap = 'f' " );
		array_push( $condicao_invalido_fixo, " d.ppoid = " . $ppoid );
		if ( $unicod && $unicod[0] )
		{
			if ( count( $unicod ) > 1 )
			{
				array_push( $condicao, " a.unicod in ( '" . implode( "','", $unicod ) . "' ) " );
				array_push( $condicao_invalido_fixo, " a.unicod in ( '" . implode( "','", $unicod ) . "' ) " );
			}
			else
			{
				array_push( $condicao, " a.unicod = '" . $unicod[0] . "' " );
				array_push( $condicao_invalido_fixo, " a.unicod = '" . $unicod[0] . "' " );
			}
		}
		if ( $prgcod && $prgcod[0] )
		{
			if ( count( $prgcod ) > 1 )
			{
				array_push( $condicao, " a.prgcod in ( '" . implode( "','", $prgcod ) . "' ) " );
				array_push( $condicao_invalido_fixo, " a.prgcod in ( '" . implode( "','", $prgcod ) . "' ) " );
			}
			else
			{
				array_push( $condicao, " a.prgcod = '" . $prgcod[0] . "' " );
				array_push( $condicao_invalido_fixo, " a.prgcod = '" . $prgcod[0] . "' " );
			}
		}
		if ( $acacod && $acacod[0] )
		{
			if ( count( $acacod ) > 1 )
			{
				array_push( $condicao, " a.acacod in ( '" . implode( "','", $acacod ) . "' ) " );
				array_push( $condicao_invalido_fixo, " a.acacod in ( '" . implode( "','", $acacod ) . "' ) " );
			}
			else
			{
				array_push( $condicao, " a.acacod = '" . $acacod[0] . "' " );
				array_push( $condicao_invalido_fixo, " a.acacod = '" . $acacod[0] . "' " );
			}
		}
		if ( $loccod && $loccod[0] )
		{
			if ( count( $loccod ) > 1 )
			{
				array_push( $condicao, " a.loccod in ( '" . implode( "','", $loccod ) . "' ) " );
				array_push( $condicao_invalido_fixo, " a.loccod in ( '" . implode( "','", $loccod ) . "' ) " );
			}
			else
			{
				array_push( $condicao, " a.loccod = '" . $loccod[0] . "' " );
				array_push( $condicao_invalido_fixo, " a.loccod = '" . $loccod[0] . "' " );
			}
		}
		
		// adiciona filtro referente ao tipo de credito
			// acaid
			$sql = "select acaid from elabrev.tipocreditoacao where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['acaid'] );
				}
				array_push( $condicao, " d.acaidloa in ( '" . implode( "','", $cods ) . "' ) " );
			}
			// gnd
			$sql = "select gndcod from elabrev.tipocreditognd where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['gndcod'] );
				}
				array_push( $condicao, " n.gndcod in ( '" . implode( "','", $cods ) . "' ) " );
			}
			// grupo unidade
			$sql = "select gunid from elabrev.tipocreditogrupounidade where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods, $linha['gunid'] );
				}
				array_push( $condicao, " u.gunid in ( '" . implode( "','", $cods ) . "' ) " );
			}
			// unidade
			$sql = "select unicod, unitpocod from elabrev.tipocreditounidade where tcrid = " . $tcrid;
			$dados = $db->carregar( $sql );
			if ( $dados )
			{
				$cods_cod = array();
				$cods_tpo = array();
				foreach ( $dados as $linha )
				{
					array_push( $cods_cod, $linha['unicod'] );
					array_push( $cods_tpo, $linha['unitpocod'] );
				}
				array_push( $condicao, " u.unicod in ( '" . implode( "','", $cods_cod ) . "' ) " );
				array_push( $condicao, " u.unitpocod in ( '" . implode( "','", $cods_tpo ) . "' ) " );
			}
		
		$where = implode( " and ", $condicao );
		
		// captura os créditos que NÃO pertencem ao filtro (dados cadastrados erroneamente pelo usuário)
		$where_invalidos = array();
		$condicao_invalido_fixo = implode( ' and ', $condicao_invalido_fixo );
		$ano = $_SESSION['exercicio'];
		
		if( $ungcod )
		{
			$innerUngcod = " inner join elabrev.progugexercicio b ON b.prgcod = a.prgcod and b.acacod = a.acacod ";
			$whUngCod	 = " b.ungcod = '".$ungcod."' and b.exercicio = '".$_SESSION['exercicio']."' and ";
		}
		else
		{
			$innerUngcod = "";
			$whUngCod 	 = "";
		}
		
		$sql = <<<EOT
			select
--				d.acaidloa,
--				d.ndpid,
--				d.foncod,
--				d.idoid,
--				d.iducod
				d.*
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
				inner join public.unidade u on
					u.unicod = a.unicod
				inner join public.naturezadespesa n on
					n.ndpid = d.ndpid
				inner join public.fonterecurso f on
					f.foncod = d.foncod
				inner join elabrev.tipocredito t on
					t.tcrid = d.tcrid
				$innerUngcod
					where
				$whUngCod
				d.dpavalor != 0 and
				d.tcrid = $tcrid and
				$condicao_invalido_fixo and
				(
					d.acaidloa not in ( select acaid from elabrev.tipocreditoacao where tcrid = $tcrid ) or
					n.gndcod not in ( select gndcod from elabrev.tipocreditognd where tcrid = $tcrid ) or
					u.gunid not in ( select gunid from elabrev.tipocreditogrupounidade where tcrid = $tcrid ) or
					a.unicod not in ( select unicod from elabrev.tipocreditounidade where tcrid = $tcrid )
				) and
				d.mcrid = {$_SESSION['credito_mcrid']}
EOT;
		
		$invalidos = $db->carregar( $sql );
		//dbg( $sql, 1 );
		$invalidos = $invalidos ? $invalidos : array();
		foreach ( $invalidos as $item )
		{
			array_push(
				$where_invalidos,
				" ( " .
					" $condicao_invalido_fixo and " .
					" d.acaidloa = " . $item['acaidloa'] . " and " .
					" d.ndpid = " . $item['ndpid'] . " and " .
					" d.foncod = '" . $item['foncod'] . "' and " .
					" d.idoid = " . $item['idoid'] . " and " .
					" d.iducod = '" . $item['iducod'] . "' and " .
												// 
					" d.tcrid is null and " .	// caracteriza dotação inicial 
					" d.mcrid is null " .		// 
												// 
				" ) "
			);
		}
		// dbg( $where );
		// dbg( $where_invalidos, 1 );
		if ( count( $where_invalidos ) == 0 )
		{
			$where_invalidos = ' false ';
		}
		else
		{
			//for ($i=0; $i<count($where_invalidos); $i++)
			$where_invalidos = implode( " or ", $where_invalidos );
		}
		// $where_invalidos = ' false ';

		// só captura os creditos adicionais que possuam dotação inicial ou cancelamento ou remanejamento
		$sql_dot =
<<<EOT
		select
			
			d.iducod, d.foncod, d.ndpid, d.nrcid, d.acaidloa, d.idoid, d.ppoid, d.tcrid,
			
			d.dpaid,		a.acacod,		a.loccod,		n.ndpcod,
			nr.nrccod,		d.foncod,		i.idocod,		a.prgcod,
			u.unicod,		d.iducod, d.plocod,
			d.dpavalor as dot
		from elabrev.despesaacao d
			inner join monitora.acao a on a.acaid = d.acaidloa
			left join public.naturezadespesa n on n.ndpid = d.ndpid
			left join public.naturezareceita nr on nr.nrcid = d.nrcid
			inner join public.idoc i on i.idoid = d.idoid
			inner join public.unidade u on u.unicod = a.unicod and u.unitpocod = a.unitpocod
			inner join ELABREV.ACAOTIPOCREDITO ac on ac.acacod = a.acacod and ac.tcrcod = '$tcrcod'
			$innerUngcod
		where
		ac.atcano = '{$_SESSION['exercicio']}' 
		AND 
		$whUngCod
			(
				(
					$where and
					d.dpavalor > 0 and
					d.tcrid is null and
					d.mcrid is null
				)
				or
				(
					$where_invalidos
				)
			)
		order by
			a.acacod,
			a.loccod
EOT;
		//dbg( $sql_dot,1 );
		$retorno = $db->carregar( $sql_dot );
		$retorno = $retorno ? $retorno : array();
		$ano = $_SESSION['exercicio'];
		
		$sql_base_out = 
<<<EOT
		select
			abs( sum( coalesce( d.dpavalor, 0 ) ) ) as valor
		from elabrev.despesaacao d
			inner join elabrev.momentocredito m on d.mcrid = m.mcrid
		where
			d.iducod       = %s
			and d.foncod   = %s
			and d.ndpid      %s
			and d.nrcid      %s
			and d.acaidloa = %s
			and d.idoid    = %s
			and d.ppoid    = %s
			and d.dpavalor %s 0
			and d.tcrid is not null
			and m.mcrano = '$ano'
			and
			(
				d.tcrid != %s
				or
				d.mcrid != {$_SESSION['credito_mcrid']}
			)
EOT;

		
		$sql_base_in_can =
<<<EOT
		select
			abs( sum( coalesce( d.dpavalor, 0 ) ) ) as valor
		from elabrev.despesaacao d
		where
			    d.iducod   = %s
			and d.foncod   = %s
			and d.ndpid      %s
			and d.nrcid      %s
			and d.acaidloa = %s
			and d.idoid    = %s
			and d.ppoid    = %s
			and d.dpavalor %s 0
			and d.tcrid is not null
			and (d.dpatipovalor in(1) or d.dpatipovalor is null)
			and
			(
				d.tcrid = %s
				and
				d.mcrid = {$_SESSION['credito_mcrid']}
			)
EOT;

		
		$sql_base_in_sup =
<<<EOT
		select
			abs( sum( coalesce( d.dpavalor, 0 ) ) ) as valor
		from elabrev.despesaacao d
		where
			    d.iducod   = %s
			and d.foncod   = %s
			and d.ndpid      %s
			and d.nrcid      %s
			and d.acaidloa = %s
			and d.idoid    = %s
			and d.ppoid    = %s
			and d.dpavalor %s 0
			and d.tcrid is not null
			and (d.dpatipovalor in(2) or d.dpatipovalor is null)
			and
			(
				d.tcrid = %s
				and
				d.mcrid = {$_SESSION['credito_mcrid']}
			)
EOT;
		
		
$sql_base_in_excesso =
<<<EOT
		select
			abs( sum( coalesce( d.dpavalor, 0 ) ) ) as valor
		from elabrev.despesaacao d
		where
			    d.iducod   = %s
			and d.foncod   = %s
			and d.ndpid      %s
			and d.nrcid      %s
			and d.acaidloa = %s
			and d.idoid    = %s
			and d.ppoid    = %s
			and d.dpavalor %s 0
			and d.tcrid is not null
			and d.dpatipovalor = 3
			and
			(
				d.tcrid = %s
				and
				d.mcrid = {$_SESSION['credito_mcrid']}
			)
EOT;

$sql_base_in_superavit =
<<<EOT
		select
			abs( sum( coalesce( d.dpavalor, 0 ) ) ) as valor
		from elabrev.despesaacao d
		where
			    d.iducod   = %s
			and d.foncod   = %s
			and d.ndpid      %s
			and d.nrcid      %s
			and d.acaidloa = %s
			and d.idoid    = %s
			and d.ppoid    = %s
			and d.dpavalor %s 0
			and d.tcrid is not null
			and d.dpatipovalor = 4
			and
			(
				d.tcrid = %s
				and
				d.mcrid = {$_SESSION['credito_mcrid']}
			)
EOT;

		foreach ( $retorno as $chave => $item )
		{
			$iducod   = "'".$item['iducod']."'";
			$foncod   = "'".$item['foncod']."'";
			$ndpid    = $item['ndpid'] ? ' = ' . $item['ndpid'] : 'is null';
			$nrcid    = $item['nrcid'] ? ' = ' . $item['nrcid'] : 'is null';
			$acaidloa = $item['acaidloa'];
			$idoid    = $item['idoid'];
			$ppoid    = $item['ppoid'];
			
			$sql_can = sprintf(
				$sql_base_in_can,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '<', $tcrid
			);			
			$can = $db->pegaUm( $sql_can );
			
			$sql_can_out = sprintf(
				$sql_base_out,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '<', $tcrid
			);
			$can_out = $db->pegaUm( $sql_can_out );
			
			$sql_sup = sprintf(
				$sql_base_in_sup,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '>', $tcrid
			);
			//dbg($sql_sup);
			$sup = $db->pegaUm( $sql_sup );
			
			$sql_sup_out = sprintf(
				$sql_base_out,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '>', $tcrid
			);
			$sup_out = $db->pegaUm( $sql_sup_out );
			
			$sql_excesso = sprintf(
				$sql_base_in_excesso,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '>', $tcrid
			);
			$vlexcesso = $db->pegaUm( $sql_excesso );
			
			$sql_superavit = sprintf(
				$sql_base_in_superavit,
					$iducod, $foncod, $ndpid, $nrcid, $acaidloa, $idoid, $ppoid, '>', $tcrid
			);
			$vlsuperavit = $db->pegaUm( $sql_superavit );
			
			
			if ( $item['dot'] || $vlexcesso || $vlsuperavit || $can || $can_out || $sup || $sup_out )
			{
				$retorno[$chave]['can'] = (integer) $can;
				$retorno[$chave]['can_out'] = (integer) $can_out;
				$retorno[$chave]['sup'] = (integer) $sup;
				$retorno[$chave]['sup_out'] = (integer) $sup_out;
				$retorno[$chave]['vlexcesso'] = (integer) $vlexcesso;
				$retorno[$chave]['vlsuperavit'] = (integer) $vlsuperavit;
				$retorno[$chave]['saldo'] = $item['dot']
					- $retorno[$chave]['can']
					- $retorno[$chave]['can_out']
					+ $retorno[$chave]['sup']
					+ $retorno[$chave]['sup_out']
					+ $retorno[$chave]['vlexcesso']
					+ $retorno[$chave]['vlsuperavit'];
					
				$retorno[$chave]['saldo_out'] = $item['dot']
					- $retorno[$chave]['can_out']
					+ $retorno[$chave]['sup_out'];
			}
			else
			{
				unset( $retorno[$chave] );
			}
		}
		//dbg( $retorno, 1 );
		return array_values( $retorno );
		
		/*
		$sql = <<<EOF
			select
				d.dpaid,		a.acacod,		a.loccod,		n.ndpcod,
				nr.nrccod,		d.foncod,		i.idocod,		a.prgcod,
				u.unicod,		d.iducod,
				d.dpavalor as dot,
				abs( sum( coalesce( can.dpavalor, 0 ) ) ) as can,
				sum( coalesce( sup.dpavalor, 0 ) ) as sup,
				d.dpavalor +
					sum( coalesce( can.dpavalor, 0 ) ) +
					sum( coalesce( sup.dpavalor, 0 ) ) +
					sum( coalesce( can_out.dpavalor, 0 ) ) +
					sum( coalesce( sup_out.dpavalor, 0 ) )
					as saldo,
				abs( sum( coalesce( can_out.dpavalor, 0 ) ) ) as can_out,
				sum( coalesce( sup_out.dpavalor, 0 ) ) as sup_out,
				d.dpavalor +
					sum( coalesce( can_out.dpavalor, 0 ) ) +
					sum( coalesce( sup_out.dpavalor, 0 ) )
					as saldo_out
			from elabrev.despesaacao d
				inner join monitora.acao a on a.acaid = d.acaidloa
				left join public.naturezadespesa n on n.ndpid = d.ndpid
				left join public.naturezareceita nr on nr.nrcid = d.nrcid
				inner join public.idoc i on i.idoid = d.idoid
				inner join public.unidade u on u.unicod = a.unicod and u.unitpocod = a.unitpocod
				left join elabrev.despesaacao can on
					can.foncod = d.foncod and
					(
						can.ndpid = d.ndpid or can.nrcid = d.nrcid
					) and
					can.acaidloa = d.acaidloa and can.idoid = d.idoid and
					can.iducod = d.iducod and can.tcrid = $tcrid and
					can.dpavalor < 0
				left join elabrev.despesaacao sup on
					sup.foncod = d.foncod and
					(
						sup.ndpid = d.ndpid or sup.nrcid = d.nrcid
					) and
					sup.acaidloa = d.acaidloa and sup.idoid = d.idoid and
					sup.iducod = d.iducod and sup.tcrid = $tcrid and
					sup.dpavalor > 0
				left join elabrev.despesaacao can_out on
					can_out.foncod = d.foncod and
					(
						can_out.ndpid = d.ndpid or can_out.nrcid = d.nrcid
					) and
					can_out.acaidloa = d.acaidloa and can_out.idoid = d.idoid and
					can_out.iducod = d.iducod and can_out.tcrid != $tcrid and
					can_out.tcrid is not null and can_out.dpavalor < 0
				left join elabrev.despesaacao sup_out on
					sup_out.foncod = d.foncod and
					(
						sup_out.ndpid = d.ndpid or sup_out.nrcid = d.nrcid
					) and
					sup_out.acaidloa = d.acaidloa and sup_out.idoid = d.idoid and
					sup_out.iducod = d.iducod and sup_out.tcrid != $tcrid and
					sup_out.tcrid is not null and sup_out.dpavalor > 0
			where
				( $where )
				or
				( $where_invalidos )
			group by
				d.dpaid,	a.acacod,		a.loccod,		n.ndpcod,
				nr.nrccod,	d.foncod,		i.idocod,		a.prgcod,
				u.unicod,	d.iducod,	d.dpavalor
			having
				d.dpavalor > 0 or
				abs( sum( coalesce( can.dpavalor, 0 ) ) ) > 0 or
				sum( coalesce( sup.dpavalor, 0 ) ) > 0
			order by
				a.acacod,	a.loccod
EOF;
		dbg( $sql );
		$retorno = $db->carregar( $sql );
		return $retorno ? $retorno : array();
*/
	}
	
	/**
	 * Captura saldo de uma despesa a partir do seu identificador
	 * 
	 * @param integer $dpaid
	 * @return integer
	 */
	function cre_pegar_saldo_atual( $dpaid )
	{
		global $db;
		$dpaid = (integer) $dpaid;
		$sql =
			" select " .
				" sum( coalesce( d2.dpavalor, 0 ) ) as dotacao " .
			" from elabrev.despesaacao d1 " .
				" inner join elabrev.despesaacao d2 on " .
					" d1.foncod = d2.foncod and " .
					" d1.ndpid = d2.ndpid and " .
					" d1.acaidloa = d2.acaidloa and " .
					" d1.idoid = d2.idoid " .
			" where " .
				" d1.dpaid = " . $dpaid . " and " .
				" d1.mcrid = {$_SESSION['credito_mcrid']} " .
			" group by " .
				" d2.foncod, " .
				" d2.ndpid, " .
				" d2.acaidloa, " .
				" d2.idoid ";
		return (integer) $db->pegaUm( $sql );
	}
