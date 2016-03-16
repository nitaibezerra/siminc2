<?php
//descomentar em produção
cte_verificaSessao();

foreach ( $_REQUEST as $chave => $valor )
{
	if ( is_string( $valor ) )
	{
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}
//retirar em produção
//$_SESSION["inuid"] = 136;

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$docid = cte_pegarDocid( $_SESSION["inuid"] );
$estado_documento = wf_pegarEstadoAtual( $docid );

$equipeTecnicaMec =
	cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_TECNICA ) ) ||
	$db->testa_superuser();
//retirar em produção	
//$unid = 316;	

	$sql_prgid = "SELECT prgid 
				FROM cte.programa  
				WHERE prgdsc = 'OUTROS'
    ";	
	$prgid_outros = $db->pegaUm($sql_prgid);
	

	
?>


<?php if( $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ): ?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
					<img align="absmiddle" src="/imagens/atencao.png"/>
					<p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico">diagnóstico</a>.</p>
				</td>
			</tr>
		</tbody>
	</table>
<?php endif; ?>


<?php

if( cte_podeElaborarPlanoDeAcoes( $_SESSION["inuid"] ) )
{
	$habil = 'S';
}
else
{ 
	$habil = 'N';
}

// captura dados de entrada geraissbaid=165
//remover em produção
//$sbaid = 674256;

//$aciid = 153169;

//descomentar em produção
$sbaid = (integer) $_REQUEST['sbaid'];
$aciid = (integer) $_REQUEST['aciid'];


if ( !$sbaid && !$aciid )
{
	?>
  	<script type="text/javascript">
  		location.href='cte.php?modulo=principal/par_acao&acao=A&indid=<?= $_REQUEST['indid'] ?>';
  	</script>
   	<?php
   	exit();
}

// ----- REMOVER SUBACAO -------------------------------------------------------
if ( $_REQUEST['action'] == "remov" && $habil == "S" )
{
	$sql = "delete from cte.subacaoindicador where sbaid = " . $sbaid;
	if ( $db->executar( $sql ) )
	{
		$db->commit();
		//$db->rollback();
		if ( $_REQUEST['origem'] )
	    {
	    	?>
	   		<script type="text/javascript">
	   			alert( 'Operação realizada com sucesso' );
			</script>
	   		<script type="text/javascript">
	   			location.href='cte.php?<?= $_REQUEST['origem'] ?>';
	   		</script>
	    	<?php
	    	exit();
	    }
	    else
	    {
			$db->sucesso( 'modulo=principal/par' );
	    }
	}
	else
	{
		$db->rollback();
	}
	exit();
}
// ----- FIM REMOVER SUBACAO -------------------------------------------------------

// ----- CARREGA DADOS DO BANCO ------------------------------------------------
if ( $sbaid )
{
	//recuperando o id da descrição do programa quando esta for igual a "OUTROS"
	$sql_programa = "SELECT prog.prgid 
				FROM cte.programa prog 
				LEFT JOIN cte.subacaoindicador as suba  
				ON (prog.prgid = suba.prgid) WHERE suba.prgid = ".$sbaid."
    ";	
	$prgid = $db->pegaUm($sql_programa);
	
	
	$sql = "select * from cte.subacaoindicador where sbaid = " . $sbaid;
	$dados = $db->recuperar( $sql );
	extract( (array) $dados );
	/*
	if ( is_array( $dados ) )
	{
		$aciid = $dados['aciid'];
		$undid = $dados['undid'];
		$frmid = $dados['frmid'];
		$sbadsc = $dados['sbadsc'];
		$sbastgmpl = $dados['sbastgmpl'];
		$sbaprm = $dados['sbaprm'];
		$sbapcr = $dados['sbapcr'];
		$sba0ano = $dados['sba0ano'];
		$sba1ano = $dados['sba1ano'];
		$sba2ano = $dados['sba2ano'];
		$sba3ano = $dados['sba3ano'];
		$sba4ano = $dados['sba4ano'];
		$sbaunt = $dados['sbaunt'];
		$sbauntdsc = $dados['sbauntdsc'];
		$sba0ini = $dados['sba0ini'];
		$sba0fim = $dados['sba0fim'];
		$sba1ini = $dados['sba1ini'];
		$sba1fim = $dados['sba1fim'];
		$sba2ini = $dados['sba2ini'];
		$sba2fim = $dados['sba2fim'];
		$sba3ini = $dados['sba3ini'];
		$sba3fim = $dados['sba3fim'];
		$sba4ini = $dados['sba4ini'];
		$sba4fim = $dados['sba4fim'];
		$psuid = $dados['psuid'];
		$sbaparecer = $dados['sbaparecer'];
		$ssuid = $dados['ssuid'];
	}
	*/
	
	$sbaprm = $sbaprm == "null" ? "" : $sbaprm;
	$sbapcr = $sbapcr == "null" ? "" : $sbapcr;
	$sba0ano = $sba0ano == "null" ? "" : $sba0ano;
	$sba1ano = $sba1ano == "null" ? "" : $sba1ano;
	$sba2ano = $sba2ano == "null" ? "" : $sba2ano;
	$sba3ano = $sba3ano == "null" ? "" : $sba3ano;
	$sba4ano = $sba4ano == "null" ? "" : $sba4ano;
	$sbauntdsc = $sbauntdsc == "null" ? "" : $sbauntdsc;
	$sba0ini = $sba0ini == "null" ? "" : $sba0ini;
	$sba0fim = $sba0fim == "null" ? "" : $sba0fim;
	$sba1ini = $sba1ini == "null" ? "" : $sba1ini;
	$sba1fim = $sba1fim == "null" ? "" : $sba1fim;
	$sba2ini = $sba2ini == "null" ? "" : $sba2ini;
	$sba2fim = $sba2fim == "null" ? "" : $sba2fim;
	$sba3ini = $sba3ini == "null" ? "" : $sba3ini;
	$sba3fim = $sba3fim == "null" ? "" : $sba3fim;
	$sba4ini = $sba4ini == "null" ? "" : $sba4ini;
	$sba4fim = $sba4fim == "null" ? "" : $sba4fim;
	$foaid = $foaid == "null" ? "" : $foaid;
	
	if(!$sbaunt)
	{
		$sbaunt = "";
		$sbavl0 = "";
		$sbavl1 = "";
		$sbavl2 = "";
		$sbavl3 = "";
		$sbavl4 = "";
	}
	else
	{
		$sbavlT = ( (float)$sbaunt * (integer)$sba0ano ) + ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = number_format($sbavlT,2,',','.');	
		if($sbavl0 == "null")
		{
			$sbavl0 = "";
		}
		else
		{
			$sbavl0 = (float)$sbaunt * (integer)$sba0ano;
			$sbavl0 = number_format($sbavl0,2,',','.');	
		}
		if($sbavl1 == "null")
		{
			$sbavl1 = "";
		}
		else
		{
			$sbavl1 = (float)$sbaunt * (integer)$sba1ano;
			$sbavl1 = number_format($sbavl1,2,',','.');	
		}
		if($sbavl2 == "null")
		{
			$sbavl2 = "";
		}
		else
		{
			$sbavl2 = (float)$sbaunt * (integer)$sba2ano;
			$sbavl2 = number_format($sbavl2,2,',','.');	
		}
		if($sbavl3 == "null")
		{
			$sbavl3 = "";
		}
		else
		{
			$sbavl3 = (float)$sbaunt * (integer)$sba3ano;
			$sbavl3 = number_format($sbavl3,2,',','.');	
		}
		if($sbavl4 == "null")
		{
			$sbavl4 = "";
		}
		else
		{
			$sbavl4 = (float)$sbaunt * (integer)$sba4ano;
			$sbavl4 = number_format($sbavl4,2,',','.');	
		}
		$sbaunt = number_format($sbaunt,2,',','.');	
	}
	
}
// ----- FIM CARREGA DADOS DO BANCO --------------------------------------------

// ----- TRATAMENTO SUBMISSAO --------------------------------------------------
if ( isset( $_REQUEST['formulario'] ) && $habil == 'S' )
{
//	dump($_REQUEST,true);
	//Validar aqui
	
	// trata entradas
	$sbadsc     = $_REQUEST['sbadsc'];
	$sbastgmpl = $_REQUEST['sbastgmpl'];
	$prgid = $_REQUEST['prgid'] != "" ? (integer) $_REQUEST['prgid'] : "null";
	$sbaprm    = $_REQUEST['sbaprm'];
	$sbapcr = $_REQUEST['sbapcr'];
	$sbauntdsc = $_REQUEST['sbauntdsc'];
	$sba0ini = $_REQUEST['sba0ini'];
	$sba0fim = $_REQUEST['sba0fim'];
	$sba1ini = $_REQUEST['sba1ini'];
	$sba1fim = $_REQUEST['sba1fim'];
	$sba2ini = $_REQUEST['sba2ini'];
	$sba2fim = $_REQUEST['sba2fim'];
	$sba3ini = $_REQUEST['sba3ini'];
	$sba3fim = $_REQUEST['sba3fim'];
	$sba4ini = $_REQUEST['sba4ini'];
	$sba4fim = $_REQUEST['sba4fim'];
	$sbauntdsc = $_REQUEST['sbauntdsc'];
	$foaid = (integer) $_REQUEST['foaid'];
	$foaid = $foaid ? $foaid : "null";
	
	$psuid = $_REQUEST['psuid'];
	$sbaparecer = trim( $_REQUEST['sbaparecer'] );
	$ssuid = $_REQUEST['ssuid'];
		$psuid_sql = $psuid ? $psuid : "null";
		$ssuid_sql = $ssuid ? $ssuid : "null";
	
	$sbaparecer = trim( $_REQUEST['sbaparecer'] );
	$ssuid = $_REQUEST['ssuid'];
	
	$undid = $_REQUEST['undid'] != "" ? (integer) $_REQUEST['undid'] : "null";
	$frmid = $_REQUEST['frmid'] != "" ? (integer) $_REQUEST['frmid'] : "null";
	
	$sba0ano = $_REQUEST['sba0ano'] ? (integer) $_REQUEST['sba0ano'] : "null";
	$sba1ano = $_REQUEST['sba1ano'] ? (integer) $_REQUEST['sba1ano'] : "null";
	$sba2ano = $_REQUEST['sba2ano'] ? (integer) $_REQUEST['sba2ano'] : "null";
	$sba3ano = $_REQUEST['sba3ano'] ? (integer) $_REQUEST['sba3ano'] : "null";
	$sba4ano = $_REQUEST['sba4ano'] ? (integer) $_REQUEST['sba4ano'] : "null";
	if( $_REQUEST['sbaunt'])
	{
		$sbaunt = str_replace(".","",$_REQUEST['sbaunt']);
		$sbaunt = str_replace(",",".",$sbaunt);
		$sbaunt = (float)$sbaunt;
	}
	else
	{
		$sbaunt = "null";
		$sbauntdsc = "null";
	}
	
	// ----- INSERT ------------------------------------------------------------
	if ( !$sbaid )
	{
		$sql = "
			insert into cte.subacaoindicador
			(
				aciid,
				undid,				frmid,				sbadsc,
				sbastgmpl,			sbaprm,				sbapcr,
				sba1ano,			sba2ano,			sba3ano,
				sba4ano,			sbaunt,				sbauntdsc,
				sba1ini,			sba1fim,			sba2ini,
				sba2fim,			sba3ini,			sba3fim,
				sba4ini,			sba4fim,            sbadata,
				usucpf,				psuid,				sbaparecer,
				ssuid, sba0ano, sba0ini, sba0fim, foaid, prgid
			)
			values
			(
				%d,
				%s,					%s,					'%s',
				'%s',				'%s',				'%s',
				%s,					%s,					%s,
				%s,					%s,					'%s',
				'%s',				'%s',				'%s',
				'%s',				'%s',				'%s',
				'%s',				'%s',               '%s',
				'%s',				%s,               '%s',
				%s, 				%s,				'%s',               '%s', %s, %s
			)
			returning sbaid
		";
		$sql = sprintf(
			$sql,
				$aciid,
				$undid,				$frmid,				$sbadsc,
				$sbastgmpl,			$sbaprm,			$sbapcr,
				$sba1ano,			$sba2ano,			$sba3ano,
				$sba4ano,			$sbaunt,			$sbauntdsc,
				$sba1ini,			$sba1fim,			$sba2ini,
				$sba2fim,			$sba3ini,			$sba3fim,
				$sba4ini,			$sba4fim,           date( "Y-m-d H:i:s" ),
				$_SESSION['usucpf'], $psuid_sql,        $sbaparecer,
				$ssuid_sql, $sba0ano, $sba0ini,			$sba0fim, $foaid, $prgid
		);
		$sbaid = $db->executar( $sql, false );
		$sbaid = pg_fetch_result($sbaid, 0);
		//dbg((boolean) $sbaid,1);
		$sucesso = (boolean) $sbaid;
	}
	
	// ----- UPDATE ------------------------------------------------------------
	else
	{
		
		$sql = "
			update cte.subacaoindicador set
				undid = %s,				frmid = %s,
				sbadsc = '%s',			sbastgmpl = '%s',
				sbaprm = '%s',			sbapcr = '%s',
				sba1ano = %s,			sba2ano = %s,
				sba3ano = %s,			sba4ano = %s,
				sbaunt = %s,			sbauntdsc = '%s',
				sba1ini = '%s',			sba1fim = '%s',
				sba2ini = '%s',			sba2fim = '%s',
				sba3ini = '%s',			sba3fim = '%s',
				sba4ini = '%s',			sba4fim = '%s',
				sbadata = '%s',         usucpf = '%s',
				psuid = %s,             sbaparecer = '%s',
				ssuid = %s,				sba0ano = %s, 
				sba0ini = '%s',			sba0fim = '%s', foaid = %s, prgid = %s
			where
				sbaid = %d
		";
		$sql = sprintf(
			$sql,
				$undid,					$frmid,
				$sbadsc,				$sbastgmpl,
				$sbaprm,				$sbapcr,
				$sba1ano,				$sba2ano,
				$sba3ano,				$sba4ano,
				$sbaunt,				$sbauntdsc,
				$sba1ini,				$sba1fim,
				$sba2ini,				$sba2fim,
				$sba3ini,				$sba3fim,
				$sba4ini,				$sba4fim,
				date( "Y-m-d H:i:s" ),  $_SESSION['usucpf'],
				$psuid_sql,             $sbaparecer,
				$ssuid_sql,
				$sba0ano,				$sba0ini,
				$sba0fim, $foaid, $prgid,
				$_REQUEST['sbaid']
		);
		//dump( $sql );
		$sucesso = (boolean) $db->executar( $sql, false );
		//dump( $sucesso, true );
	}
	
	// executa ação
	if ( $sucesso )
	{
		$mensagem = "Dados gravados com sucesso!";
		$db->commit();
//		$_REQUEST["acao"] = "A";
		$db->sucesso( $_REQUEST['modulo'], "&sbaid=" . $sbaid );
	}
	else
	{
		$mensagem = "Não foi possível gravar os dados. Tente novamente mais tarde.";
		$db->rollback();
	}
	
	$sbaprm = $sbaprm == "null" ? "" : $sbaprm;
	$sbapcr = $sbapcr == "null" ? "" : $sbapcr;
	$sba0ano = $sba0ano == "null" ? "" : $sba0ano;
	$sba1ano = $sba1ano == "null" ? "" : $sba1ano;
	$sba2ano = $sba2ano == "null" ? "" : $sba2ano;
	$sba3ano = $sba3ano == "null" ? "" : $sba3ano;
	$sba4ano = $sba4ano == "null" ? "" : $sba4ano;
	$sbatotal = (integer)$sba1ano + (integer)$sba2ano + (integer)$sba3ano + (integer)$sba4ano;
	$sbauntdsc = $sbauntdsc == "null" ? "" : $sbauntdsc;
	$sba0ini = $sba0ini == "null" ? "" : $sba0ini;
	$sba0fim = $sba0fim == "null" ? "" : $sba0fim;
	$sba1ini = $sba1ini == "null" ? "" : $sba1ini;
	$sba1fim = $sba1fim == "null" ? "" : $sba1fim;
	$sba2ini = $sba2ini == "null" ? "" : $sba2ini;
	$sba2fim = $sba2fim == "null" ? "" : $sba2fim;
	$sba3ini = $sba3ini == "null" ? "" : $sba3ini;
	$sba3fim = $sba3fim == "null" ? "" : $sba3fim;
	$sba4ini = $sba4ini == "null" ? "" : $sba4ini;
	$sba4fim = $sba4fim == "null" ? "" : $sba4fim;
	$foaid = $foaid == "null" ? "" : $foaid;
	$prgid = $prgid == "null" ? "" : $prgid;
	
	if($sbaunt == "null")
	{
		$sbaunt = "";
		$sbavl0 = "";
		$sbavl1 = "";
		$sbavl2 = "";
		$sbavl3 = "";
		$sbavl4 = "";
	}
	else
	{
		$sbavlT = ( (float)$sbaunt * (integer)$sba0ano ) + ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = number_format($sbavlT,2,',','.');	
		if($sbavl0 == "null")
		{
			$sbavl0 = "";
		}
		else
		{
			$sbavl0 = (float)$sbaunt * (integer)$sba0ano;
			$sbavl0 = number_format($sbavl0,2,',','.');	
		}
		if($sbavl1 == "null")
		{
			$sbavl1 = "";
		}
		else
		{
			$sbavl1 = (float)$sbaunt * (integer)$sba1ano;
			$sbavl1 = number_format($sbavl1,2,',','.');	
		}
		if($sbavl2 == "null")
		{
			$sbavl2 = "";
		}
		else
		{
			$sbavl2 = (float)$sbaunt * (integer)$sba2ano;
			$sbavl2 = number_format($sbavl2,2,',','.');	
		}
		if($sbavl3 == "null")
		{
			$sbavl3 = "";
		}
		else
		{
			$sbavl3 = (float)$sbaunt * (integer)$sba3ano;
			$sbavl3 = number_format($sbavl3,2,',','.');	
		}
		if($sbavl4 == "null")
		{
			$sbavl4 = "";
		}
		else
		{
			$sbavl4 = (float)$sbaunt * (integer)$sba4ano;
			$sbavl4 = number_format($sbavl4,2,',','.');	
		}
		$sbaunt = number_format($sbaunt,2,',','.');	
	}
	
}

// ----- FIM TRATAMENTO SUBMISSAO ----------------------------------------------

if ( !$aciid )
{
	$sql = "
		select
			aciid
		from cte.subacaoindicador
		where
			sbaid = '" . $sbaid . "'
	";
	$aciid = (integer) $db->pegaUm( $sql );
}
$sql = "
	select
		dim.dimid,
		dim.dimcod,
		dim.dimdsc,
		are.ardid,
		are.ardcod,
		are.arddsc,
		ind.indid,
		ind.indcod,
		ind.inddsc,
		aci.acidsc,
		aci.acilocalizador,
		pto.ptoid
	from cte.acaoindicador aci
		inner join cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = 'A'
		inner join cte.indicador ind on ind.indid = pto.indid
		inner join cte.areadimensao are on are.ardid = ind.ardid
		inner join cte.dimensao dim on dim.dimid = are.dimid
	where
		aci.aciid = '" . $aciid . "'
";
$hieraquia = $db->recuperar( $sql );
if ( is_array( $hieraquia ) )
{
	$dimensaoId		 	   = $hieraquia['dimid'];
	$dimensaoCod	 	   = $hieraquia['dimcod'];
	$dimensaoDescricao 	   = $hieraquia['dimdsc'];
	
	$areaDimensaoId		   = $hieraquia['ardid'];
	$areaDimensaoCod	   = $hieraquia['ardcod'];
	$areaDimensaoDescricao = $hieraquia['arddsc'];
	
	$indicadorId    	   = $hieraquia['indid'];
	$indicadorCod    	   = $hieraquia['indcod'];
	$indicadorDescricao    = $hieraquia['inddsc'];
	
	$acaoDescricao         = $hieraquia['acidsc'];
	$acilocalizador        = $hieraquia['acilocalizador'];
	
	if( $acilocalizador == "E" )
	{
		$tipoModulo = "Rede Estadual";
	}
	else
	{
		$tipoModulo = "Rede Municipal";
	}
	
	$ptoid = $hieraquia['ptoid'];
}

if(cte_pegarItrid( $_SESSION['inuid'] ) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
    $capturaTipo = "E";
}

if(cte_pegarItrid( $_SESSION['inuid'] ) == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
	$capturaTipo = "M";
}

$db->cria_aba( $abacod_tela, $url, '&aciid=' . $aciid  . '&indid='. $indicadorId);
cte_montaTitulo( $titulo_modulo, '<img border="0" src="../imagens/obrig.gif"/> Indica Campo Obrigatório.' );

?>

<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>

<style>

table.cronograma { margin: 10px; }
table.cronograma thead {}
table.cronograma thead tr {}
table.cronograma thead tr th { text-align: center; font-weight: bold; }
table.cronograma tbody {}
table.cronograma tbody tr {}
table.cronograma tbody tr td { text-align: left; }
table.cronograma tbody tr td.nome { text-align: right; }
table.cronograma tbody tr td div.data_inicio { float: left; }
table.cronograma tbody tr td div.data_fim { float: right; }


</style>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<form method="POST" name="formulario">					
					<input type="hidden" name="formulario" value="1"/>
					<input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
					<input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
					<table class="tabela" style="width:100%;" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<colgroup>
							<col style="width: 25%;"/>
							<col style="width: 75%;"/>
						</colgroup>
						<tbody>
							<tr>
								<td class="SubTituloDireita">Dimensão:</td>
								<td><?= $dimensaoCod ?>. <?= $dimensaoDescricao; ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Área:</td>
								<td><?= $areaDimensaoCod ?>. <?= $areaDimensaoDescricao; ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Indicador:</td>
								<td>
									<a href="?modulo=principal/formulario_avaliacao&acao=A&indid=<?= $indicadorId ?>">
										<?= $indicadorCod ?>. <?= $indicadorDescricao; ?>
									</a>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Demanda:</td>
								<td><?= $tipoModulo; ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Ação:</td>
								<td>
									<a href="?modulo=principal/par_acao&acao=A&ptoid=<?= $ptoid ?>&acilocalizador=<?= $acilocalizador ?>&aciid=<?= $aciid ?>">
										<?= $acaoDescricao; ?>
									</a>
								</td>
							</tr>
							
							<?php if( cte_podeEditarParecer( $_SESSION['inuid'] ) ): ?>
								<tr>
									<td colspan="2" class="SubtituloEsquerda">Parecer Equipe Técnica </td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Parecer Padrão:</td>
									<td>
										<?php
										$sql =
											"select ps.psuid, ps.psudescricao, gp.gpsid, gp.gpsdescricao
											from cte.parecersubacao ps
											inner join cte.grupoparecersubacao gp on gp.gpsid = ps.gpsid
											order by gp.gpsdescricao, ps.psudescricao";
										$pareceres = $db->carregar( $sql );
										?>
										<?php if( $pareceres ): ?>
										<select name="psuid">
											<option>
											<?php foreach( $pareceres as $chave => $parecer ): ?>
												<?php if( $pareceres[$chave]['gpsid'] != $pareceres[$chave-1]['gpsid'] ): ?>
													<optgroup label="<?= $parecer['gpsdescricao'] ?>">
												<?php endif; ?>
												<option value="<?= $parecer['psuid'] ?>" <?= $psuid == $parecer['psuid'] ? "selected='selected'" : "" ?>><?= $parecer['psudescricao'] ?></option>
												<?php if( $pareceres[$chave]['gpsid'] != $pareceres[$chave+1]['gpsid'] ): ?>
													</optgroup>
												<?php endif; ?>
											<?php endforeach; ?>
										</select>
										<?php endif; ?>
									</td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Parecer:</td>
									<td><?= campo_textarea( 'sbaparecer', 'N', $habil, '', 70, 3, 0 ); ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Status Subação:</td>
									<td>
										<?php
										$sql = sprintf( "select ssuid as codigo, ssudescricao as descricao from cte.statussubacao" );
										$db->monta_combo( "ssuid", $sql, 'S', '&nbsp;', '', '' );
										?>
									</td>
								</tr>
							<?php else: ?>
								<input type="hidden" name="psuid" value="<?= $psuid ?>"/>
								<input type="hidden" name="sbaparecer" value="<?= $sbaparecer ?>"/>
								<input type="hidden" name="ssuid" value="<?= $ssuid ?>"/>
							<?php endif; ?>
							
							
							
							
							
							
							
							
							
							
							
							
							<?php
							$itrid = cte_pegarItrid( $_SESSION['inuid'] );
							$grandeMunicipio =
								$itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ||
								cte_verificaGrandeMunicipio( $_SESSION['inuid'] );
							$habil_subacao = $habil == "S" && $grandeMunicipio;
							
							//remover em produção
							//$habil_subacao = true;
							
							$sufixo_campo = $habil_subacao ? "" : "_fake";
							
							?>
							<script type="text/javascript">
								
								function atualizaCampoSubAcao( id, valor )
								{
									input = document.getElementById( id );
									if ( !input )
									{
										return;
									}
									// input text ou hidden
									if ( input.type && ( input.type == 'text' || input.type == 'hidden' ) )
									{
										input.value = valor;
									}
									// select
									else if ( input.options && input.options.length )
									{
										var estado = input.disabled;
										input.disabled = false;
										for ( var chave = 0; chave < input.options.length; chave++ )
										{
											if ( input.options[chave].value == valor )
											{
												input.options[chave].selected = true;
												break;
											}
										}
										input.disabled = estado;
									}
									// textarea
									else
									{
										input.innerHTML = valor;
									}
								}
								
								function propostaSubAcao( descricao, metodologia, programa, forma, unidade, prgid )
								{
									atualizaCampoSubAcao( 'sbadsc_fake', descricao );
									atualizaCampoSubAcao( 'sbastgmpl_fake', metodologia );
									atualizaCampoSubAcao( 'sbaprm_fake', programa );
									atualizaCampoSubAcao( 'undid_fake', unidade );
									atualizaCampoSubAcao( 'frmid_fake', forma );
									
									atualizaCampoSubAcao( 'sbadsc', descricao );
									atualizaCampoSubAcao( 'sbastgmpl', metodologia );
									atualizaCampoSubAcao( 'sbaprm', programa );
									atualizaCampoSubAcao( 'undid', unidade );
									atualizaCampoSubAcao( 'frmid', forma );
									atualizaCampoSubAcao( 'prgid', prgid );
									exibeCampo();
								}
								
							</script>
							<tr>
								<td colspan="2" class="SubtituloEsquerda">Dados da Subação</td>
							</tr>
							
							
							<?php if ( $equipeTecnicaMec ) : ?>
								<tr>
									<td align='right' class="SubTituloDireita">
										Forma de atendimento:
									</td>
									<td>
										<?php
											$sqlComboFormaAtendimento = "
												select
													foaid as codigo,
													foadsc as descricao
												from cte.formaatendimento
												order by
													foadsc
											";
											$db->monta_combo(
												"foaid" . $sufixo_campo,
												$sqlComboFormaAtendimento,
												$habil_subacao ? 'S' : 'N',
												'Escolha...', '', '','','','N',
												'foaid' . $sufixo_campo
											);
										?>
										<?php if ( !$habil_subacao ) : ?>
											<input
												type="hidden"
												name="foaid"
												id="foaid"
												value="<?= simec_htmlentities( $foaid ) ?>"
											/>
										<?php endif; ?>
									</td>
								</tr>
							<?php else: ?>
								<input type="hidden" id="foaid" name="foaid" value="<?= $foaid ?>"/>
							<?php endif; ?>
							
							
							<tr>
								<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
								<td>
									<?  if( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) : ?>
										<div align="left" >
											<a href="javascript:janelaSecundaria('cte.php?modulo=principal/proposta/proposta_subacao&acao=A&aciid=<?= $aciid ?>')"> 
												Clique neste link para abrir opções de proposta de  Sub ação
											</a>
										</div>
									<? endif; ?>
									<? ${"sbadsc" . $sufixo_campo} = $sbadsc; ?>
									<?= campo_textarea( 'sbadsc'.$sufixo_campo, "S", $habil_subacao ? 'S' : 'N', '', 70, 3, 2000 ); ?>
									<?php if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="sbadsc"
											id="sbadsc"
											value="<?= simec_htmlentities( $sbadsc ) ?>"
										/>
									<?php endif; ?>
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
								<td>
									<? ${"sbastgmpl" . $sufixo_campo} = $sbastgmpl; ?>
									<?= campo_textarea( 'sbastgmpl'.$sufixo_campo,  "S", $habil_subacao ? 'S' : 'N', '', 70, 3, 0 ); ?>
									<?php if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="sbastgmpl"
											id="sbastgmpl"
											value="<?= simec_htmlentities( $sbastgmpl ) ?>"
										/>
									<?php endif; ?>
								</td>
							</tr>
							
													
							
								<tr>
									<td align='right' class="SubTituloDireita">Programa:</td>
									<td>
										<?php
											$sql = "SELECT prgid as codigo, substr(prgdsc, 1, 100) as descricao FROM cte.programa";
											
											$db->monta_combo( 'prgid', $sql, $habil_subacao ? 'S' : 'N', 'Selecione', 'exibeCampo', '','','','S', 'prgid');
										?>									
									</td>
								</tr>
							
							
						    <?	
						    //exibe ou esconde a linha da tabela q contém o campo de texto do programa																								
															
							if( $prgid == $prgid_outros) {	
								
								if($sbaprm == ''){
									$display = "display: none";									
								}else{
									$display = "display: table-row";									
								}								
							}else{									
								$display = "display: none";							
							}
							?>
							
							<tr id="tr_programa" style="<?=$display ?>;">							
							
								<td align='right' class="SubTituloDireita">Programa:</td>
								<td>
								
									<?
										${"sbaprm" . $sufixo_campo} = $sbaprm; 
									?>
									
									<?= campo_texto( 'sbaprm'.$sufixo_campo, 'N', $habil_subacao ? 'S' : 'N', '', 50, 255, '', '', 'left', '', 0, 'id="sbaprm'.$sufixo_campo.'"' );?>
									<?php if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="sbaprm"
											id="sbaprm"
											value="<?= simec_htmlentities( $sbaprm ) ?>"
										/>
									<?php endif; ?>
								</td>
							</tr>
							
							
							
							
							<tr>
								<td align='right' class="SubTituloDireita">Unidade de Medida:</td>
								<td>
									<?php
										$sql = "SELECT undid as codigo, unddsc as descricao FROM cte.unidademedida WHERE undtipo='".$capturaTipo."' ORDER BY descricao";
										${"undid" . $sufixo_campo} = $undid;
										$db->monta_combo( "undid".$sufixo_campo, $sql, $habil_subacao ? 'S' : 'N', 'Escolha...', '', '','','','S', 'undid'.$sufixo_campo );
									?>
									<?php 
																		
									if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="undid"
											id="undid"
											value="<?= simec_htmlentities( $undid ) ?>"
										/>
									<?php endif; ?>
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Forma de Execução:</td>
								<td>
									<?php
										$sql = "SELECT frmid as codigo, frmdsc as descricao FROM cte.formaexecucao WHERE frmtipo='".$capturaTipo."' ORDER BY descricao";
										${"frmid".$sufixo_campo} = $frmid;
										$db->monta_combo( "frmid".$sufixo_campo, $sql, $habil_subacao ? 'S' : 'N', 'Escolha...', 'regraFormaExec', '','','','S', 'frmid'.$sufixo_campo );
									?>
									<?php if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="frmid"
											id="frmid"
											value="<?= simec_htmlentities( $frmid ) ?>"
										/>
									<?php endif; ?>
								</td>
							</tr>
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							<tr>
								<td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
								<td><?=campo_texto( 'sbapcr', '', $habil , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"' );?></td>
							</tr>
							<tr style="background-color: #cccccc">
								<td align='left' colspan="2" style="vertical-align:top; width:25%;"><b>Quantidades e Cronograma Físico</b></td>
							</tr>
							<tr>
								<td colspan="2" align="center">
									<table class="listagem" align="center" border="0" width="95%" cellpadding="5">
										<colgroup>
										</colgroup>
										<thead>
											<tr>
												<th></th>
												<th>2007</th>
												<th>2008</th>
												<th>2009</th>
												<th>2010</th>
												<th>2011</th>
												<th>Total</th>
											</tr>
										</thead>
										<tbody>
											<tr>
												<td align="right"  class="SubTituloDireita">Quantidades:</td>
												<td align="right"><?=campo_texto( 'sba0ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba0ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td>
												<td align="right"><?=campo_texto( 'sba1ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba1ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td>
												<td align="right"><?=campo_texto( 'sba2ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba2ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td>
												<td align="right"><?=campo_texto( 'sba3ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba3ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td>
												<td align="right"><?=campo_texto( 'sba4ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba4ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td>
												<td align="right"><?=campo_texto( 'sbatotal', '', 'N', '', 23, 20, '#######', '', 'right', '', 0, 'id="sbatotal"' );?></td>
											</tr>
											<tr>
												<td align="right"  class="SubTituloDireita">Cronograma Físico:</td>
												<td nowrap align="right">
													<?=campo_texto( 'sba0ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba0ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba0fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba0fim"' );?>
												</td>
												<td nowrap align="right">
													<?=campo_texto( 'sba1ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba1ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba1fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba1fim"' );?>
												</td>
												<td nowrap align="right">
													<?=campo_texto( 'sba2ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba2ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba2fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba2fim"' );?>
												</td>
												<td nowrap align="right">
													<?=campo_texto( 'sba3ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba3ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba3fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba3fim"' );?>
												</td>
												<td nowrap align="right">
													<?=campo_texto( 'sba4ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba4ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba4fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba4fim"' );?>
												</td>
												<td>&nbsp;</td>
											</tr>
										</tbody>
									</table>
								</td>
							</tr>
							<tr id="trValores">
				
								<td colspan="2">
									<table class="tabela" style="width:100%;" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
										<colgroup>
											<col style="width: 25%;"/>
											<col style="width: 75%;"/>
										</colgroup>
										<tr style="background-color: #cccccc">
											<td align='left' colspan="2" style="vertical-align:top; width:25%;"><b>Valores</b></td>
										</tr>
										<tr>
											<td align='right' class="SubTituloDireita">Valor Unitário:</td>
											<td>
												<?=campo_texto( 'sbaunt', 'N', $habil , '', 25, 20, '###.###.###.###.###.###.###.###.###,##', '', 'left', '', 0, 'id="sbaunt" onBlur="MouseBlur(this);"','calculaValoresAnuais();' );?>
												<img border="0" id="imgVU" title="Indica campo obrigatório." src="../imagens/obrig.gif" style="display:none"/>
											</td>
										</tr>
										<tr>
											<td align='right' class="SubTituloDireita">Detalhamento da Composição do Valor Unitário:</td>
											<td>
												<?= campo_textarea( 'sbauntdsc', 'N', $habil , '', 100, 10, 2000 ); ?>
												<img border="0" id="imgDVU" title="Indica campo obrigatório." src="../imagens/obrig.gif" style="display:none"/>
											</td>
										</tr>
										
										<tr>
											
											<td colspan=2>
												<table class="listagem" align="center" border="0" width="95%" cellpadding="5">
													<colgroup>
													</colgroup>
													<thead>
														<tr>
															<th></th>
															<th>2007</th>
															<th>2008</th>
															<th>2009</th>
															<th>2010</th>
															<th>2011</th>
															<th>Total</th>
														</tr>
													</thead>
													<tbody>
														<tr>
															<td align='right' class="SubTituloDireita">Valores Anuais:</td>
															<td align="right"><?=campo_texto( 'sbavl0', '', 'N', '', 23, 20, '', '', 'right', '', 0, 'id="sbavl0"' );?></td> 
															<td align="right"><?=campo_texto( 'sbavl1', '', 'N', '', 23, 20, '', '', 'right', '', 0, 'id="sbavl1"' );?></td> 
															<td align="right"><?=campo_texto( 'sbavl2', '', 'N', '', 23, 20, '###.###.###.###.###.###,##', '', 'right', '', 0, 'id="sbavl2"' );?></td>
															<td align="right"><?=campo_texto( 'sbavl3', '', 'N', '', 23, 20, '###.###.###.###.###.###,##', '', 'right', '', 0, 'id="sbavl3"' );?></td>
															<td align="right"><?=campo_texto( 'sbavl4', '', 'N', '', 23, 20, '###.###.###.###.###.###,##', '', 'right', '', 0, 'id="sbavl4"' );?></td>
															<td align="right"><?=campo_texto( 'sbavlT', '', 'N', '', 23, 20, '###.###.###.###.###.###,##', '', 'right', '', 0, 'id="sbavlT"' );?></td>
														</tr>
													</tbody>
												</table>
											</td>
										</tr>
									</table>
								</td>
							</tr>
							<tr style="background-color: #cccccc">
								<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
								<td>
								<?php if ( $habil == 'S' ): ?>
									<input type="button" name="botao" value="Salvar" onclick="enviar();"/>
								<?php endif; ?>
									<!--
									<input type="button" name="botao" value="Nova Subação" onclick="javascrip:window.location.href='/cte/cte.php?modulo=principal/par_subacao&acao=A&aciid=<?=$aciid?>'"/>
									<input type="button" name="botao" value="Fechar" onclick="javascrip:window.location.href='/cte/cte.php?modulo=principal/par&acao=A'"/>
									-->
								</td>
							</tr>
						</tbody>
					</table>
				</form>
			</td>
		</tr>
	</tbody>
</table>

<?php if( !empty($frmid) && ( $frmid == 1 ) ):?>
	<script type="text/javascript">
		document.getElementById("trValores").style.display = 'none';
				
		document.getElementById("sbaunt").value = '';
		document.getElementById("sbauntdsc").value = '';
		document.getElementById("sbavlT").value = '';
		
		for( i=1 ; i<=4 ; i++ )
		{
			document.getElementById("sbavl" + i).value = '';
		}
	</script>
<?php endif; ?>

<hr style="display: table-row"/>
<script type="text/javascript">



	function exibeCampo(){		
		
			if(document.getElementById("prgid").value == <?=$prgid_outros; ?>){			
				document.getElementById("tr_programa").style.display = 'table-row';
				
				
			}else{
			    document.getElementById("sbaprm").value = '';
				document.getElementById("tr_programa").style.display = 'none';
				
			}					
	}


	function calculaValoresAnuais()
	{
		var st = 0;
		var vln = 0;
		var vlt = 0;
		var valor = 0;
		var s = new Array();
		var vu = 0;
		var vu_ori = document.getElementById("sbaunt").value;
	    var j = 0;
	    
	    vu = parseInt( vu_ori.replace( /[\.,]/g , "" ) ) - 0;
	    if ( vu_ori.indexOf( ',' ) > -1 )
	    {
	    	vu /= 100;
	    }
	    
		s[0] = parseInt( document.getElementById("sba0ano").value.replace( /[\.,]/g , "" ) ) - 0;
		s[1] = parseInt( document.getElementById("sba1ano").value.replace( /[\.,]/g , "" ) ) - 0;
		s[2] = parseInt( document.getElementById("sba2ano").value.replace( /[\.,]/g , "" ) ) - 0;
		s[3] = parseInt( document.getElementById("sba3ano").value.replace( /[\.,]/g , "" ) ) - 0;
		s[4] = parseInt( document.getElementById("sba4ano").value.replace( /[\.,]/g , "" ) ) - 0;
		
		var parcial = 0;
		var parcial_texto = '';
		for( var i = 0 ; i < 5 ; i++ )
		{
			if( !isNaN( parseInt( s[i] ) ) )
			{
				j = i; 
				s[i] = parseInt( s[i] );
				parcial = parseInt( parseFloat( vu * s[i] ) * 100 );
				document.getElementById("sbavl" + j).value = parcial;
				document.getElementById( "sbavl" + j ).value = mascaraglobal( '###.###.###.###.###.###.###.###.###,##', ( parcial + '' ).replace( /[\.,]/g , "" ) );
				vlt += parcial;
				
//				st = st + s[i];
//				j = i + 1; 
//				document.getElementById("sbavl" + j).value = ( s[i] * parseInt( vu ) ) / 100;
//				parcial = parseFloat( s[i] * vu );
//				parcial_texto = parcial += '';
//				if ( parcial_texto.indexOf( '.' ) < 0 )
//				{
//					parcial *= 100;
//				}
//				document.getElementById( "sbavl" + j ).value = mascaraglobal( '###.###.###.###.###.###.###.###.###,##', ( parcial + '' ).replace( /[\.,]/g , "" ) );
//				vlt += parcial;
			}
		}
		
//		vlt = vlt * 100;
//		vlt = "" + vlt + "";
//		vlt = mascaraglobal("###.###.###.###.###.###,##" , vlt);
//		alert( vlt );
		
		document.getElementById("sbavlT").value = mascaraglobal( '###.###.###.###.###.###.###.###.###,##', ( vlt + '' ).replace( /[\.,]/g , "" ) );
		return true;
	}
	function somaQuantidades()
	{
		var st = 0;
		var s = new Array(); 
		var vu = document.getElementById("sbaunt").value.replace( /[\.,]/g , "" );
		
		s[0] = document.getElementById("sba0ano").value;
		s[1] = document.getElementById("sba1ano").value;
		s[2] = document.getElementById("sba2ano").value;
		s[3] = document.getElementById("sba3ano").value;
		s[4] = document.getElementById("sba4ano").value;
		
		for( i=0 ; i<5 ; i++ )
		{
			if( !isNaN( parseInt( s[i] ) ) )
			{
				s[i] = parseInt( s[i] );
				st = st + s[i];
			}
		}
		document.getElementById("sbatotal").value = st;
		if( !isNaN( parseInt( vu ) ) )
		{
			calculaValoresAnuais();
		}
	}
	somaQuantidades();
	function regraFormaExec( vl )
	{
		//Regra: Se a opção "Forma de Execução" selecionada for:
		//"Transferencia Voluntária" ou "Assistência Técnica com complementação financeira"
		//As setas de obrigatoriedade do preenchimento dos campos
		//"Valor Unitário" e "Detalhamento da Composição do Valor Unitário"
		//Deverão ser mostradas
		switch(vl)
		{
			case "2":
			case "3":
				//2 = Transferencia Voluntária
				//3 = Assistência Técnica com complementação financeira
				document.getElementById("imgVU").style.display = '';
				document.getElementById("imgDVU").style.display = '';
				document.getElementById("trValores").style.display = '';
			break;
			case "1":
				//1 = Ações de Execução direta do Estado
				document.getElementById("trValores").style.display = 'none';
				document.getElementById("sbaunt").value = '';
				document.getElementById("sbauntdsc").value = '';
				document.getElementById("sbavlT").value = '';
				
				for( i=1 ; i<=4 ; i++ )
				{
					document.getElementById("sbavl" + i).value = '';
				}
			break;
			default:
				document.getElementById("imgVU").style.display = 'none';
				document.getElementById("imgDVU").style.display = 'none';
				document.getElementById("trValores").style.display = '';
			break;
		}
	}
	function verifica_data( stringData ) { 
		var mes = ( stringData.substring( 0 , 2 ) ); 
		var ano = ( stringData.substring( 3 , 7 ) ); 
		var situacao = true; 
		
		if(stringData.length != 7)
		{
			situacao = false;
		}
		// verifica se o mes e valido 
		if (mes < 01 || mes > 12 ) { 
			situacao = false; 
		}
		// verifica se o mes e valido 
		if (ano < 1950 || ano > 2020 ) { 
			//situacao = false; 
		} 
		return situacao;
    } 
	
	function maior_data( menor , maior ){
		var mesMenor = ( menor.substring( 0 , 2 ) ); 
		var anoMenor = ( menor.substring( 3 , 7 ) );
		var mesMaior = ( maior.substring( 0 , 2 ) ); 
		var anoMaior = ( maior.substring( 3 , 7 ) );
		var situacao = true; 
		
		var datamenor = anoMenor+anoMenor;
		var datamaior = anoMaior+anoMaior;
                   
		if(datamenor > datamaior){
			situacao = false; 
		}
		return situacao;
   } 
   
	function enviar( acao ){
		var objSbadsc   = document.getElementById("sbadsc");
		var objSbastgmpl   = document.getElementById("sbastgmpl");
		var objCBPorg = document.getElementById("prgid");
		var objSbaprm   = document.getElementById("sbaprm");
		var objUndid   = document.getElementById("undid");
		var objFrmid   = document.getElementById("frmid");
		var objSbapcr   = document.getElementById("sbapcr");
		var objSbaunt   = document.getElementById("sbaunt");
		var objSbauntdsc   = document.getElementById("sbauntdsc");
		var objSba0ano   = document.getElementById("sba0ano");
		var objSba1ano   = document.getElementById("sba1ano");
		var objSba2ano   = document.getElementById("sba2ano");
		var objSba3ano   = document.getElementById("sba3ano");
		var objSba4ano   = document.getElementById("sba4ano");
		var objSba0ini   = document.getElementById("sba0ini");
		var objSba1ini   = document.getElementById("sba1ini");
		var objSba2ini   = document.getElementById("sba2ini");
		var objSba3ini   = document.getElementById("sba3ini");
		var objSba4ini   = document.getElementById("sba4ini");
		var objSba0fim   = document.getElementById("sba0fim");
		var objSba1fim   = document.getElementById("sba1fim");
		var objSba2fim   = document.getElementById("sba2fim");
		var objSba3fim   = document.getElementById("sba3fim");
		var objSba4fim   = document.getElementById("sba4fim");
		var objValida = true;
		var objValores = false;
		var msg = "";
		
				
		if(objSbadsc.value==""){
			msg = "Informe a descrição da Subação\n";
			objValida = false;
		}
		
		if(objSbastgmpl.value==""){
			msg += "Informe a Estratégia de Implementação\n";
			objValida = false;
		}
		
		if(objUndid.value==""){
			msg += "Informe a Undiade de medida\n";
			objValida = false;
		}
		
		if(objFrmid.value==""){
			msg += "Informe a Forma de execução\n";
			objValida = false;
		}
		
		switch(true)
		{
			case( trim( objSba0ano.value ) != ""  && trim( objSba0fim.value ) != "" && trim( objSba0ini.value ) != ""  ):
			case( trim( objSba1ano.value ) != ""  && trim( objSba1fim.value ) != "" && trim( objSba1ini.value ) != ""  ):
			case( trim( objSba2ano.value ) != "" && trim( objSba2fim.value ) != "" && trim( objSba2ini.value ) != "" ):
			case( trim( objSba3ano.value ) != "" && trim( objSba3fim.value ) != "" && trim( objSba3ini.value ) != "" ):
			case( trim( objSba4ano.value ) != "" && trim( objSba4fim.value ) != "" && trim( objSba4ini.value ) != "" ):
				objValores = true;
				break;
		}
		
		if( !objValores )
		{
			msg += "Informe pelo menos um dos campos de ano de Quantidade e Cronograma Físico \n";
			objValida = false;
		}
		
		switch(objFrmid.value)
		{
			case "2": 
			case "3":
				//2 = Transferencia voluntaria
				//3 = Assistência Técnica com complementação financeira
				if(objSbaunt.value == "")
				{
					msg += "Informe o Valor Unitário\n";
					objValida = false;
				}
				if(objSbauntdsc.value == "")
				{
					msg += "Informe o Detalhamento da Composição do Valor Unitário\n";
					objValida = false;
				}
			break;
		}

		if( verifica_data( objSba0ini.value ) || verifica_data( objSba0fim.value ) )
		{
			if( !maior_data( objSba0ini.value , objSba0fim.value ) )
			{
				objValida = false;
				msg += "A data final de 2007 está maior que a data inicial\n";
			}
		}
		else
		{
			
			if( trim( objSba0ini.value ) != "" && !verifica_data( objSba0ini.value ) )
			{
				objValida = false;
				msg += "Data inicial de 2007 inválida\n";
			}
			if( trim( objSba0fim.value ) != "" &&  !verifica_data( objSba0fim.value ) )
			{
				objValida = false;
				msg += "Data final de 2007 inválida\n";
			}
		}

		
		if( verifica_data( objSba1ini.value ) || verifica_data( objSba1fim.value ) )
		{
			if( !maior_data( objSba1ini.value , objSba1fim.value ) )
			{
				objValida = false;
				msg += "A data final de 2008 está maior que a data inicial\n";
			}
		}
		else
		{
			
			if( trim( objSba1ini.value ) != "" && !verifica_data( objSba1ini.value ) )
			{
				objValida = false;
				msg += "Data inicial de 2008 inválida\n";
			}
			if( trim( objSba1fim.value ) != "" &&  !verifica_data( objSba1fim.value ) )
			{
				objValida = false;
				msg += "Data final de 2008 inválida\n";
			}
		}
		
		if( verifica_data( objSba2ini.value ) || verifica_data( objSba2fim.value ) )
		{
			if( !maior_data( objSba2ini.value , objSba2fim.value ) )
			{
				objValida = false;
				msg += "A data final de 2009 está maior que a data inicial\n";
			}
		}
		else
		{
			if( trim( objSba2ini.value ) != "" && !verifica_data( objSba2ini.value ) )
			{
				objValida = false;
				msg += "Data inicial de 2009 inválida\n";
			}
			if( trim( objSba2fim.value ) != "" &&  !verifica_data( objSba2fim.value ) )
			{
				objValida = false;
				msg += "Data final de 2009 inválida\n";
			}
		}
		
		if( verifica_data( objSba3ini.value ) || verifica_data( objSba3fim.value ) )
		{
			if( !maior_data( objSba3ini.value , objSba3fim.value ) )
			{
				objValida = false;
				msg += "A data final de 2010 está maior que a data inicial\n";
			}
		}
		else
		{
			if( trim( objSba3ini.value ) != "" && !verifica_data( objSba3ini.value ) )
			{
				objValida = false;
				msg += "Data inicial de 2010 inválida\n";
			}
			if( trim( objSba3fim.value ) != "" &&  !verifica_data( objSba3fim.value ) )
			{
				objValida = false;
				msg += "Data final de 2010 inválida\n";
			}
		}
		
		if( verifica_data( objSba4ini.value ) || verifica_data( objSba4fim.value ) )
		{
			if( !maior_data( objSba4ini.value , objSba4fim.value ) )
			{
				objValida = false;
				msg += "A data final de 2011 está maior que a data inicial\n";
			}
		}
		else
		{
			if( trim( objSba4ini.value ) != "" && !verifica_data( objSba4ini.value ) )
			{
				objValida = false;
				msg += "Data inicial de 2011 inválida\n";
			}
			if( trim( objSba4fim.value ) != "" &&  !verifica_data( objSba4fim.value ) )
			{
				objValida = false;
				msg += "Data final de 2011 inválida\n";
			}
		}
				
		if( !objValida )
		{
			alert(msg);
			return false;
		}
		else
		{
			document.formulario.action = '';
			document.formulario.submit();
		}
	}
	
	function janelaSecundaria( URL )
	{
	   window.open(URL,"janelaProposta","width=400,height=300,scrollbars=yes") 
	}
</script>
<?php if( isset( $mensagem ) ):?>
	<script type="text/javascript">
		alert("<?php echo $mensagem; ?>");
	</script>
<?php endif; ?>