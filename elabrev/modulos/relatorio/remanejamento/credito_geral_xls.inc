<?php

$ano = $_SESSION['exercicio'];


$sql_momento = <<<EOT
	select
		mcrid as codigo,
		mcrdsc as descricao
	from
		elabrev.momentocredito
	where
		mcrstatus = 'A' and mcrano = '$ano'
	order by
		mcrid desc
EOT;

// captura proposta ativa
$sql =
	" select " .
		" ppoid " .
	" from elabrev.propostaorcamento " .
	" where " .
		" ppoano = '" . $ano . "' and " .
		" tppid = 3 and " .
		" ppostatus = 'A'";
$ppoid = (integer) $db->pegaUm( $sql );

$tcrid = $_REQUEST['tcrid'];
$unicod = $_REQUEST['unicod'];
$mcrid = (integer) $_REQUEST['mcrid'];
$ungcod = $_REQUEST['ungcod'];

$valor_unidade = $unicod ? " a.unicod = '" . $unicod . "' and " : " ";
$valor_momento = $mcrid ? " = " . $mcrid . " " : " is not null ";

$valor_unidade_gestora = $ungcod ? " d.ungcod = '" . $ungcod . "' and " : " ";

$uni_wherejoin = $unicod ? " a.unicod = '" . $unicod . "' and " : "";

//array de colunas default, caso não selecione algum filtro diferente de "Todas".
$colunas = array("Unidade","Tipo de Crédito", "Programa", "Código da Ação", "Localizador", "PO", "Natureza" ,"Fonte" ,"IDOC", "IDUSO",  "Dotação", "Suplementação", "Cancelamento", "Sup. Excesso", "Sup. Superávit", "Diferença", "Saldo");

//condição para add filtro na query, quando opção diferente de "Todas".
if($mcrid != 0){
	$colunas   = array("Unidade", "Situação","Tipo de Crédito", "Programa", "Código da Ação", "Localizador", "PO", "Natureza" ,"Fonte" ,"IDOC", "IDUSO",  "Dotação", "Suplementação", "Cancelamento", "Sup. Excesso", "Sup. Superávit", "Diferença", "Saldo");
	$where    = "where tmp.mcrid =".$mcrid;
	$tmpmcrid = ", tmp.mcrid";
	$dmcrid   = ", d.mcrid";
	$case     = " CASE tmp.mcrid
                    WHEN tmp.mcrid THEN ( 
                    	SELECT mc.mcrdsc FROM elabrev.momentocredito mc WHERE mc.mcrid = tmp.mcrid 
					)
                	ELSE '--'
               	  END as mcrdsc,";
}//fim if

$sql ="
    SELECT 
    		   tmp.unicod,
			   $case
               tc.tcrcod,
               '=\"' || tmp.prgcod::text || '\"' AS prgcod,
               '=\"' || tmp.acacod::text || '\"' AS acacod,
               '=\"' || tmp.loccod::text || '\"' AS loccod,
               '=\"' || tmp.plocod::text || '\"' AS plocod,
               n.ndpcod,
               tmp.foncod,
               i.idocod,
               tmp.iducod,
               sum(tmp.dot) AS dot,
               sum(tmp.sup) AS sup,
               sum(tmp.can) AS can,
               sum(tmp.supe) AS supe,
               sum(tmp.sups) AS sups,
               (SUM(tmp.sup) + SUM(tmp.supe) + SUM(tmp.sups)) - SUM(tmp.can) AS diferenca,
               (SUM(tmp.dot) + SUM(tmp.sup) + SUM(tmp.supe) + SUM(tmp.sups)) - SUM(tmp.can) AS saldo 
      FROM(
			-- valor can
			select
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid,
				0 as dot,
				0 as sup,
				abs( sum( d.dpavalor ) ) as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				a.prgano = '$ano' and
				d.dpavalor < 0 and
				(d.dpatipovalor in(1) or d.dpatipovalor is null) and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid
		union
			-- valor sup
			select
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid,
				0 as dot,
				sum( d.dpavalor ) as sup,
				0 as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				(d.dpatipovalor in(2) or d.dpatipovalor is null) and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid
		union
			-- valor sup excesso
			select
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid,
				0 as dot,
				0 as sup,
				0 as can,
				sum( d.dpavalor ) as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				d.dpatipovalor = 3 and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid
		union
			-- valor sup superavit
			select
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid,
				0 as dot,
				0 as sup,
				0 as can,
				0 as supe,
				sum( d.dpavalor ) as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				d.dpatipovalor = 4 and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid
		union
			-- valor dot
			select
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid,
				sum( d.dpavalor ) as dot,
				0 as sup,
				0 as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				a.prgano = '$ano' and
				d.ppoid = $ppoid and
				d.mcrid is not null
			group by
				a.unicod, a.acacod, a.acatitulo,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod, d.tcrid $dmcrid
	
	) as tmp
		inner join public.naturezadespesa n on
			n.ndpid = tmp.ndpid
		inner join public.idoc i on
			i.idoid = tmp.idoid 
		inner join elabrev.tipocredito tc on
			tc.tcrid = tmp.tcrid
		--inner join elabrev.momentocredito mc on
			--tmp.mcrid = mc.mcrid
	$where
	group by
		tmp.unicod, tmp.acacod, tmp.acatitulo,
		tmp.loccod, tmp.prgcod, n.ndpcod,
		tmp.foncod, i.idocod,   tmp.iducod, tmp.plocod, tc.tcrcod $tmpmcrid
	having
		sum( tmp.sup ) > 0 or sum( tmp.can ) > 0 or sum( tmp.supe ) > 0 or sum( tmp.sups ) > 0
	order by
		tmp.unicod, tc.tcrcod, tmp.acacod, tmp.acatitulo,
		tmp.loccod, tmp.prgcod, n.ndpcod,
		tmp.foncod, i.idocod,   tmp.iducod, tmp.plocod $tmpmcrid
";

if( $_POST['requisicao'] == 'gerar_xls') {
	
	$cabecalho = $colunas;
		
	ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_Relatorio_credito_geral".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_Relatorio_credito_geral".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'S','100%',$par2);
		exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Relatório de Créditos - Geral (xls)' );

$unicod = (string) $_REQUEST['unicod'];
$tcrid = (integer) $_REQUEST['tcrid'];
$mcrid = (integer) $_REQUEST['mcrid'];

?>
<script type="text/javascript">

</script>
<form action="" name="filtro" method="post">
	<table align="center" border="0" class="tabela" cellpadding="5" cellspacing="0" style="border-top: none;border-bottom: none; padding-top: 1px;">
		<input type="hidden" name="requisicao" value="gerar_xls" id="requisicao" />
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Descrição
			</td>
			<td width="80%">
				<?= $db->monta_combo( 'mcrid', $sql_momento, 'S', 'Todas', '', '', '', '', '', '' ) ?>
			</td>
		</tr>
		<tr style="background-color:#cccccc">
                    <td align='right' style="vertical-align:top;width:25%">&nbsp;</td>
                    <td>
                        <input type="submit" value="Gerar Relatório xls" />
                    </td>
                </tr>
		
	</table>
</form>