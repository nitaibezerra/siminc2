<?php

// ---------- FUNCOES ----------

	// ----- PEGAR
	
	function tc_pegar( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql = "select * from elabrev.tipocredito where tcrid = " . $tcrid;
		$retorno = $db->recuperar( $sql );
		return $retorno ? $retorno : array();
	}
	
	function tc_pegar_unidades( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql =
			" select " .
				" u.unicod as codigo, u.unidsc as descricao " .
			" from elabrev.tipocreditounidade tc " .
				" inner join public.unidade u on " .
					" u.unicod = tc.unicod and " .
					" u.unitpocod = tc.unitpocod " .
			" where " .
				" tc.tcrid = " . $tcrid;
		$retorno = $db->carregar( $sql );
		return $retorno ? $retorno : array();
	}
	
	function tc_pegar_gnds( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql =
			" select " .
				" gnd.gndcod as codigo, gnd.gnddsc as descricao " .
			" from elabrev.tipocreditognd tc " .
				" inner join public.gnd on " .
					" gnd.gndcod = tc.gndcod " .
			" where " .
				" tc.tcrid = " . $tcrid;
		$retorno = $db->carregar( $sql );
		return $retorno ? $retorno : array();
	}
	
	function tc_pegar_acoes( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql =
			" select " .
				" a.acacod as codigo, a.acadsc as descricao " .
			" from elabrev.tipocreditoacao tc " .
				" inner join monitora.acao a on " .
					" a.acaid = tc.acaid " .
			" where " .
				" tc.tcrid = " . $tcrid . 
			" group by " .
				" a.acacod, a.acadsc ";
		$retorno = $db->carregar( $sql );
		return $retorno ? $retorno : array();
	}
	
	function tc_pegar_grupos_unidade( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql =
			" select " .
				" gu.gunid as codigo, gu.gundsc as descricao " .
			" from elabrev.tipocreditogrupounidade tc " .
				" inner join public.grupounidade gu on " .
					" gu.gunid = tc.gunid " .
			" where " .
				" tc.tcrid = " . $tcrid;
		$retorno = $db->carregar( $sql );
		return $retorno ? $retorno : array();
	}
	
	// ----- REMOVER
	
	function tc_remover( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		//tc_remover_acoes( $tcrid );
		//tc_remover_unidades( $tcrid );
		//tc_remover_gnds( $tcrid );
		//tc_remover_tipos_acao( $tcrid );
		//$sql = "delete from elabrev.tipocredito where tcrid = " . $tcrid;
		$sql = "update elabrev.tipocredito set tcrstatus = 'I' where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}
	
	function tc_remover_acoes( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql = "delete from elabrev.tipocreditoacao where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}

	function tc_remover_unidades( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql = "delete from elabrev.tipocreditounidade where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}
	
	function tc_remover_gnds( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql = "delete from elabrev.tipocreditognd where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}

	function tc_remover_grupos_unidade( $tcrid )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql = "delete from elabrev.tipocreditogrupounidade where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}
	
	// ----- INSERIR
	
	function tc_inserir( $tcrcod, $tcrdsc, $tcrdscfonte, $tcrdscautorizacao, $tcrtipoporcento, $tcrporcentocancelamento, $tcrporcentosuplementacao, $tcrnaturezapropria )
	{
		global $db;
		$sql = "select nextval( 'elabrev.tipocredito_tcrid_seq' ) as tcrid";
		$tcrid = (integer) $db->pegaUm( $sql );
		$sql =
			" insert into elabrev.tipocredito " .
				" ( " .
					" tcrid, " .
					" tcrcod, " .
					" tcrdsc, " .
					" tcrdscfonte, " .
					" tcrdscautorizacao, " .
					" tcrtipoporcento, " .
					" tcrporcentocancelamento, " .
					" tcrporcentosuplementacao, " .
					" tcrano, " .
					" tcrnaturezapropria " .
				" )" .
			" values " .
				" ( " .
					"'" . $tcrid . "', " .
					"'" . $tcrcod . "', " .
					"'" . $tcrdsc . "', " .
					"'" . $tcrdscfonte . "', " .
					"'" . $tcrdscautorizacao . "', " .
					"'" . $tcrtipoporcento . "', " .
					"'" . $tcrporcentocancelamento . "', " .
					"'" . $tcrporcentosuplementacao . "', " .
					"'" . $_SESSION['exercicio'] . "', " .
					( $tcrnaturezapropria ? 'true' : 'false' ) .
				" )";
		return $db->executar( $sql ) ? $tcrid : 0;
	}
	
	function tc_inserir_acoes( $tcrid, $cods )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$cods = (array) $cods; 
		// limpa acoes atuais
		tc_remover_acoes( $tcrid );
		if ( count( $cods ) == 0 || !$cods[0] )
		{
			return;
		}
		// transforma acacods em acaid
		$sql = "select acaid from monitora.acao where acacod in ( '" . implode( "','", $cods ) . "' ) group by acaid";
		$cods = $db->carregar( $sql );
		if ( !$cods )
		{
			return;
		}
		// faz relacionamentos
		foreach ( $cods as $cod )
		{
			$sql = "insert into elabrev.tipocreditoacao ( tcrid, acaid ) values ( " . $tcrid . ", '" . $cod ['acaid'] . "' ) ";
			$db->executar( $sql );
		}
	}
	
	function tc_inserir_unidades( $tcrid, $cods )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$cods = (array) $cods;
		// limpa acoes atuais
		tc_remover_unidades( $tcrid );
		if ( count( $cods ) == 0 || !$cods[0] )
		{
			return;
		}
		// faz relacionamentos
		foreach ( $cods as $cod )
		{
			$sql = "insert into elabrev.tipocreditounidade ( tcrid, unicod ) values ( " . $tcrid . ", '" . $cod . "' ) ";
			$db->executar( $sql );
		}
	}

	function tc_inserir_gnds( $tcrid, $cods )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$cods = (array) $cods;
		// limpa acoes atuais
		tc_remover_gnds( $tcrid );
		if ( count( $cods ) == 0 || !$cods[0] )
		{
			return;
		}
		// faz relacionamentos
		foreach ( $cods as $cod )
		{
			$sql = "insert into elabrev.tipocreditognd ( tcrid, gndcod ) values ( " . $tcrid . ", '" . $cod . "' ) ";
			$db->executar( $sql );
		}
	}
	
	function tc_inserir_grupos_unidade( $tcrid, $cods )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$cods = (array) $cods;
		// limpa acoes atuais
		tc_remover_grupos_unidade( $tcrid );
		if ( count( $cods ) == 0 || !$cods[0] )
		{
			return;
		}
		// faz relacionamentos
		foreach ( $cods as $cod )
		{
			$sql = "insert into elabrev.tipocreditogrupounidade ( tcrid, gunid ) values ( " . $tcrid . ", '" . $cod . "' ) ";
			$db->executar( $sql );
		}
	}
	
	// ----- EDITAR
	
	function tc_editar( $tcrid, $tcrcod, $tcrdsc, $tcrdscfonte, $tcrdscautorizacao, $tcrtipoporcento, $tcrporcentocancelamento, $tcrporcentosuplementacao, $tcrnaturezapropria )
	{
		global $db;
		$tcrid = (integer) $tcrid;
		$sql =
			" update " .
				" elabrev.tipocredito set " .
					" tcrcod = '" . $tcrcod . "', " .
					" tcrdsc = '" . $tcrdsc . "', " .
					" tcrdscfonte = '" . $tcrdscfonte . "', " .
					" tcrdscautorizacao = '" . $tcrdscautorizacao . "', " .
					" tcrtipoporcento = '" . $tcrtipoporcento . "', " .
					" tcrporcentocancelamento = '" . $tcrporcentocancelamento . "', " .
					" tcrporcentosuplementacao = '" . $tcrporcentosuplementacao . "', " .
					" tcrnaturezapropria = " . ( $tcrnaturezapropria ? 'true' : 'false' ) .
			" where tcrid = " . $tcrid;
		return (boolean) $db->executar( $sql );
	}
