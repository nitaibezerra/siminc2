<?php

include APPRAIZ . "includes/registraracesso.php";

cte_verificaSessao();

foreach ( $_REQUEST as $chave => $valor )
{
	if ( is_string( $valor ) )
	{
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}

$sql = sprintf(
	"select p.inuid
	from cte.subacaoindicador si
	inner join cte.acaoindicador ai on ai.aciid = si.aciid
	inner join cte.pontuacao p on p.ptoid = ai.ptoid
	where si.sbaid = %d",
	$_REQUEST["sbaid"]
);
$inuid = $db->pegaUm( $sql );

$itrid = cte_pegarItrid( $inuid );
$podeExcluirSubacao = false;

$docid = cte_pegarDocid( $inuid );
$estado_documento = wf_pegarEstadoAtual( $docid );

$equipeTecnicaMec = false;

$sql_prgid = "
	SELECT
		prgid 
	FROM cte.programa  
	WHERE
		prgdsc = 'OUTROS'
";
$prgid_outros = (integer) $db->pegaUm( $sql_prgid );


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

//if( cte_podeElaborarPlanoDeAcoes( $_SESSION["inuid"] ) )
//{
//	$habil = 'S';
//}
//else
//{
//	$habil = 'N';
//}
$habil = 'N';

// captura dados de entrada gerais
$sbaid = (integer) $_REQUEST['sbaid'];
$aciid = (integer) $_REQUEST['aciid'];

if ( !$sbaid && !$aciid )
{
	?>
  	<script type="text/javascript">
  		location.href='brasilpro.php?modulo=principal/par_acao&acao=A&indid=<?= $_REQUEST['indid'] ?>';
  	</script>
   	<?php
   	exit();
}

// ----- REMOVER SUBACAO -------------------------------------------------------
if ( $_REQUEST['action'] == "remov" && $habil == "S" )
{
	$sbaid = (integer) $sbaid;
	$sql = "
		delete from cte.subacaobeneficiario
		where
			sbaid = " . $sbaid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from cte.composicaosubacao
		where
			sbaid = " . $sbaid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from cte.qtdfisicoano
		where
			sbaid = " . $sbaid . "
	";
	$db->executar( $sql );
	
	$sql = "delete from cte.subacaoindicador where sbaid = " . $sbaid;
	if ( $db->executar( $sql ) )
	{
		$db->commit();
		//$db->rollback();
		if ( $_REQUEST['origem'] )
	    {
	    	?>
	   		<script type="text/javascript">
	   			window.opener.location.reload();
	   			alert( 'Operação realizada com sucesso' );
			</script>
	   		<script type="text/javascript">
	   			location.href='brasilpro.php?<?= $_REQUEST['origem'] ?>';
	   		</script>
	    	<?php
	    	exit();
	    }
	    else
	    {
	    	?>
	   		<script type="text/javascript">
	   			window.opener.location.reload();
			</script>
			<?
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
	$sql = "select * from cte.subacaoindicador where sbaid = " . $sbaid;
	$dados = $db->recuperar( $sql );
	extract( (array) $dados );
	
	$sbaprm = $sbaprm == "null" ? "" : $sbaprm;
	$sbapcr = $sbapcr == "null" ? "" : $sbapcr;
	$sba0ano = $sba0ano == "null" ? "" : $sba0ano;
	$sba1ano = $sba1ano == "null" ? "" : $sba1ano;
	$sba2ano = $sba2ano == "null" ? "" : $sba2ano;
	$sba3ano = $sba3ano == "null" ? "" : $sba3ano;
	$sba4ano = $sba4ano == "null" ? "" : $sba4ano;
	$sbauntdsc = $sbauntdsc == "null" ? "" : $sbauntdsc;
	//$sba0ini = $sba0ini == "null" ? "" : $sba0ini;
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
	$sbaporescola = $sbaporescola == 't';
	
	if(!$sbaunt)
	{
		$sbaunt = "";
		//$sbavl0 = "";
		$sbavl1 = "";
		$sbavl2 = "";
		$sbavl3 = "";
		$sbavl4 = "";
	}
	else
	{
		//$sbavlT = ( (float)$sbaunt * (integer)$sba0ano ) + ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = number_format($sbavlT,2,',','.');
		/*
		if($sbavl0 == "null")
		{
			$sbavl0 = "";
		}
		else
		{
			$sbavl0 = (float)$sbaunt * (integer)$sba0ano;
			$sbavl0 = number_format($sbavl0,2,',','.');	
		}
		*/
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
	//$sba0ini = $_REQUEST['sba0ini'];
	//$sba0fim = $_REQUEST['sba0fim'];
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
	
	$ppsid = (integer) $_REQUEST["ppsid"];
	$ppsid_banco = $ppsid ? $ppsid : "null";
	
	if ( $ppsid )
	{
		$sqlOrdem = "
			select
				ppsordem
			from cte.proposicaosubacao
			where
				ppsid = " . $ppsid . "
		";
		$sbaordem = (integer) $db->pegaUm( $sqlOrdem );
	}
	else
	{
		$sbaordem = 0;
	}
	
	
	$sbaporescola = $_REQUEST["sbaporescola"] == '1';
		$sbaporescola_sql = $sbaporescola ? "true" : "false";
	
	//$psuid = $_REQUEST['psuid'];
	$sbaparecer = trim( $_REQUEST['sbaparecer'] );
	$ssuid = $_REQUEST['ssuid'];
		//$psuid_sql = $psuid ? $psuid : "null";
		$ssuid_sql = $ssuid ? $ssuid : "null";
	
	$psuid     = null;
	$psuid_sql = "null";
	
	$sbaparecer = trim( $_REQUEST['sbaparecer'] );
	$ssuid = $_REQUEST['ssuid'];
	
	$undid = $_REQUEST['undid'] != "" ? (integer) $_REQUEST['undid'] : "null";
	$frmid = $_REQUEST['frmid'] != "" ? (integer) $_REQUEST['frmid'] : "null";
	
	$valoresEscola = $_REQUEST["escola"];
	$valoresEscola = is_array( $valoresEscola ) ? $valoresEscola : array();
	
	//$sba0ano = $_REQUEST['sba0ano'] ? (integer) $_REQUEST['sba0ano'] : "null";
	if ( !$sbaporescola )
	{
		$sba1ano = $_REQUEST['sba1ano'] ? (integer) $_REQUEST['sba1ano'] : "null";
		$sba2ano = $_REQUEST['sba2ano'] ? (integer) $_REQUEST['sba2ano'] : "null";
		$sba3ano = $_REQUEST['sba3ano'] ? (integer) $_REQUEST['sba3ano'] : "null";
		$sba4ano = $_REQUEST['sba4ano'] ? (integer) $_REQUEST['sba4ano'] : "null";
	}
	else
	{
		$sba1ano = 0;
		$sba2ano = 0;
		$sba3ano = 0;
		$sba4ano = 0;
		foreach ( $valoresEscola as $entid => $anos )
		{
			foreach ( $anos as $ano => $valor )
			{
				switch ( $ano )
				{
					case '2008':
						$sba1ano += $valor;
						break;
					case '2009':
						$sba2ano += $valor;
						break;
					case '2010':
						$sba3ano += $valor;
						break;
					case '2011':
						$sba4ano += $valor;
						break;
				}
			}
		}
	}
	
	if( $_REQUEST['sbaunt'])
	{
		$sbaunt = str_replace(".","",$_REQUEST['sbaunt']);
		$sbaunt = str_replace(",",".",$sbaunt);
		$sbaunt = (float)$sbaunt;
	}
	else
	{
		$sbaunt = "null";
		//$sbauntdsc = "null";
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
				ssuid,				foaid,				sbaporescola,
				prgid,				ppsid,				sbaordem
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
				'%s',				%s,               	'%s',
				%s,					%s,					%s,
				%s,					%s,					%d
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
				//$ssuid_sql, $sba0ano, $sba0ini,			$sba0fim, $foaid
				$ssuid_sql,			$foaid,				$sbaporescola_sql,
				$prgid,				$ppsid_banco,		$sbaordem
		);
		$sbaid = $db->executar( $sql, false );
		$sbaid = pg_fetch_result($sbaid, 0);
		//dbg((boolean) $sbaid,1);
		$sucesso = (boolean) $sbaid;
	}
	
	// ----- UPDATE ------------------------------------------------------------
	else
	{
		$sbaid = (integer) $_REQUEST['sbaid'];
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
				ssuid = %s,				foaid = %s,
				sbaporescola = %s,		prgid = %s,
				ppsid = %s,				sbaordem = %d
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
				$ssuid_sql,				$foaid,
				$sbaporescola_sql,		$prgid,
				$ppsid_banco,			$sbaordem,
				$_REQUEST['sbaid']
		);
		//dump( $sql );
		$sucesso = (boolean) $db->executar( $sql, false );
		//dump( $sucesso, true );
	}
	
	// apaga composicoes atuais
	$sql = "
		delete from cte.composicaosubacao
		where
			sbaid = '" . $sbaid . "'
	";
	$db->executar( $sql );
	
	// grava composicoes
	if ( isset( $_REQUEST['composicao']['cosord'] ) && count( $_REQUEST['composicao']['cosord'] ) )
	{
		$sqlBase = "
			insert into cte.composicaosubacao
			( sbaid, cosord, cosdsc, unddid, cosqtd, cosvlruni )
			values
			( %d, %d, '%s', %d, %d, %f )
		";
		foreach ( array_keys( $_REQUEST['composicao']['cosord'] ) as $chave )
		{
			$cosord    = (integer) $_REQUEST['composicao']['cosord'][$chave];
			$cosdsc    = str_replace( "'", "\\'", $_REQUEST['composicao']['cosdsc'][$chave] );
			//$cosunimed = str_replace( "'", "\\'", $_REQUEST['composicao']['cosunimed'][$chave] );
			$cosqtd    = (integer) $_REQUEST['composicao']['cosqtd'][$chave];
			$unddid    = str_replace( "'", "\\'", $_REQUEST['composicao']['unddid'][$chave] );
			
			$cosvlruni = $_REQUEST['composicao']['cosvlruni'][$chave];
			$cosvlruni = str_replace( ".", "", $cosvlruni );
			$cosvlruni = (float) str_replace( ",", ".", $cosvlruni );
			$sql = sprintf(
				$sqlBase,
					$sbaid,
					$cosord,
					$cosdsc,
					//$cosunimed,
					$unddid,
					$cosqtd,
					$cosvlruni
			);
			$db->executar( $sql );
		}
		
	}
	
	// apaga valores escolares atuais
	$sql = "
		delete from cte.qtdfisicoano
		where
			sbaid = " . $sbaid . "
	";
	
	// grava valores escolares
	if ( $sbaporescola )
	{
		foreach ( $valoresEscola as $entid => $anos )
		{
			foreach ( $anos as $ano => $valor )
			{
				$sql = "
					insert into cte.qtdfisicoano
					( sbaid, qfaano, qfaqtd, entid )
					values ( %d, %d, %d, %d )
				";
				$sql = sprintf( $sql, $sbaid, $ano, $valor, $entid );
				$db->executar( $sql );
			}
		}
	}
	
	
	$valoresBen = $_REQUEST["beneficiario"];
	$valoresBen = is_array( $valoresBen ) ? $valoresBen : array();
	$valoresBen["benid"] = is_array( $valoresBen["benid"] ) ? $valoresBen["benid"] : array();
	
	// apaga valores de beneficiários
	$sql = "
		delete from cte.subacaobeneficiario
		where
			sbaid = " . $sbaid . "
	";
	$db->executar( $sql );
	
	// grava valores escolares
	$sqlBaseBen = "
		insert into cte.subacaobeneficiario
		( benid, sbaid, vlrrural, vlrurbano )
		values ( %d, %d, %d, %d )
	";
	$benids = array();
	foreach ( array_keys( $valoresBen["benid"] ) as $chave )
	{
		$benid     = (integer) $valoresBen["benid"][$chave];
		$vlrrural  = (integer) $valoresBen["rural"][$chave];
		$vlrurbano = (integer) $valoresBen["urbano"][$chave];
		if ( in_array( $benid, $benids ) )
		{
			continue;
		}
		array_push( $benids, $benid );
		$sql = sprintf(
			$sqlBaseBen,
				$benid,
				$sbaid,
				$vlrrural,
				$vlrurbano
		);
		$db->executar( $sql );
	}
	
	
	
	
	// executa ação
	if ( $sucesso )
	{
		$mensagem = "Dados gravados com sucesso!";
		$db->commit();
		
		$outraSubacao = (integer) $_REQUEST["irpara"];
		//dump( $outraSubacao, true );
		?>
   		<script type="text/javascript">
   			window.opener.location.reload();
		</script>
		<?
		if ( $outraSubacao )
		{
			$db->sucesso( $_REQUEST['modulo'], "&sbaid=" . $outraSubacao );
		}
		else
		{
			$db->sucesso( $_REQUEST['modulo'], "&sbaid=" . $sbaid );
		}
	}
	else
	{
		$mensagem = "Não foi possível gravar os dados. Tente novamente mais tarde.";
		$db->rollback();
	}
	
	$sbaprm = $sbaprm == "null" ? "" : $sbaprm;
	$sbapcr = $sbapcr == "null" ? "" : $sbapcr;
	//$sba0ano = $sba0ano == "null" ? "" : $sba0ano;
	$sba1ano = $sba1ano == "null" ? "" : $sba1ano;
	$sba2ano = $sba2ano == "null" ? "" : $sba2ano;
	$sba3ano = $sba3ano == "null" ? "" : $sba3ano;
	$sba4ano = $sba4ano == "null" ? "" : $sba4ano;
	$sbatotal = (integer)$sba1ano + (integer)$sba2ano + (integer)$sba3ano + (integer)$sba4ano;
	$sbauntdsc = $sbauntdsc == "null" ? "" : $sbauntdsc;
	//$sba0ini = $sba0ini == "null" ? "" : $sba0ini;
	//$sba0fim = $sba0fim == "null" ? "" : $sba0fim;
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
		//$sbavl0 = "";
		$sbavl1 = "";
		$sbavl2 = "";
		$sbavl3 = "";
		$sbavl4 = "";
	}
	else
	{
		//$sbavlT = ( (float)$sbaunt * (integer)$sba0ano ) + ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = ( (float)$sbaunt * (integer)$sba1ano ) + ( (float)$sbaunt * (integer)$sba2ano ) + ( (float)$sbaunt *  (integer)$sba3ano ) + ( (float)$sbaunt *  (integer)$sba4ano );
		$sbavlT = number_format($sbavlT,2,',','.');	
		/*
		if($sbavl0 == "null")
		{
			$sbavl0 = "";
		}
		else
		{
			$sbavl0 = (float)$sbaunt * (integer)$sba0ano;
			$sbavl0 = number_format($sbavl0,2,',','.');	
		}
		*/
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
		pto.ptoid,
		
		ind.indqtdporescola
		
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
	
	$indqtdporescola = $hieraquia['indqtdporescola'] == 't';
}
else
{
	$indqtdporescola = null;
}

$podeEditarTipoCronograma = $indqtdporescola === null;
$sbaporescola = $indqtdporescola;

if(cte_pegarItrid( $inuid ) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
    $capturaTipo = "E";
}

if(cte_pegarItrid( $inuid ) == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
	$capturaTipo = "M";
}

// CARREGA COMPOSIÇÃO
if ( $sbaid )
{
	$sql = "
		select
			cosord,
			cosdsc,
			--cosunimed,
			unddid,
			cosqtd,
			cosvlruni
		from cte.composicaosubacao
		where
			sbaid = '" . $sbaid . "'
		order by
			cosord
	";
	$composicao = $db->carregar( $sql );
	$composicao = $composicao ? $composicao : array();
}
else
{
	$composicao = array();
}

// carrega escolas
if ( $sbaid )
{
	$sql = "
		select
			q.qfaano,
			q.qfaqtd,
			q.entid,
			e.entnome
		from cte.qtdfisicoano q
			inner join entidade.entidade e on
				e.entid = q.entid
		where
			q.sbaid = " . $sbaid . "
	";
	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();
	$escolas = array();
	foreach ( $dados as $linha )
	{
		$qfaano  = $linha["qfaano"];
		$qfaqtd  = $linha["qfaqtd"];
		$entid   = $linha["entid"];
		$entnome = $linha["entnome"];
		if ( !array_key_exists( $entid, $escolas ) )
		{
			$escolas[$entid] = array(
				"entid"   => $entid,
				"entnome" => $entnome,
				"2008"    => 0,
				"2009"    => 0,
				"2010"    => 0,
				"2011"    => 0
			);
		}
		$escolas[$entid][$qfaano] = $qfaqtd;
	}
}
else
{
	$escolas = array();
}

//dump( $escolas );

//$db->cria_aba( $abacod_tela, $url, '&aciid=' . $aciid  . '&indid='. $indicadorId);
//cte_montaTitulo( $titulo_modulo, '<img border="0" src="../imagens/obrig.gif"/> Indica Campo Obrigatório.' );

$inuid = (integer) $inuid;
$dim_cod = (integer) $dimensaoCod;
$ard_cod = (integer) $areaDimensaoCod;
$ind_cod = (integer) $indicadorCod;
$sql = "
	select
		sai.sbaid,
		dim.dimcod,
		ard.ardcod,
		ind.indcod
	from cte.subacaoindicador sai
		inner join cte.acaoindicador aci on
			aci.aciid = sai.aciid
		inner join cte.pontuacao pto on
			pto.ptoid = aci.ptoid
		inner join cte.indicador ind on
			ind.indid = pto.indid
		inner join cte.areadimensao ard on
			ard.ardid = ind.ardid
		inner join cte.dimensao dim on
			dim.dimid = ard.dimid
	where
		pto.inuid = " . $inuid . " and
		pto.ptostatus = 'A'
	order by
		dim.dimcod,
		ard.ardcod,
		ind.indcod,
		sai.sbaordem,
		sai.sbadsc
";
$outrasSubAcoes = $db->carregar( $sql );
$outrasSubAcoes = $outrasSubAcoes ? $outrasSubAcoes : array();
$sbaid_anterior = null;
$sbaid_proximo  = null;
foreach ( $outrasSubAcoes as $chave => $outraSubAcao )
{
	if ( $outraSubAcao["sbaid"] == $sbaid )
	{
		if ( isset( $outrasSubAcoes[$chave-1] ) )
		{
			$sbaid_anterior = $outrasSubAcoes[$chave-1]["sbaid"]; 
		}
		if ( isset( $outrasSubAcoes[$chave+1] ) )
		{
			$sbaid_proximo = $outrasSubAcoes[$chave+1]["sbaid"]; 
		}
		break;
	}
}

$sql = "
	select
		prgid,
		prgplanointerno
	from cte.programa
";
$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();
$planosinterno = array();
foreach ( $dados as $linha )
{
	$planosinterno[$linha["prgid"]] = $linha["prgplanointerno"];
}

$sql = "
	select
		benid,
		bendsc
	from cte.beneficiario
	order by
		bendsc
";
$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();
$ben = array();
foreach ( $dados as $linha )
{
	$ben[$linha["benid"]] = $linha["bendsc"];
}

$sql = "
	select
		benid,
		vlrurbano,
		vlrrural
	from cte.subacaobeneficiario
	where
		sbaid = " . $sbaid . "
";
$ben_sbi = $db->carregar( $sql );
$ben_sbi = $ben_sbi ? $ben_sbi : array();


?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
	
	var IE = !!document.all;
	
	function campoQuantitativoBloqueado()
	{
		var obj = document.getElementById( 'frmid' );
		if ( !obj )
		{
			return false;
		}
		//alert( obj.value );
		return obj.value == 4 || obj.value == 8;
	}
	
	function atualizarBloqueioCamposQuantitativo()
	{
		var bloqueado = campoQuantitativoBloqueado();
		var desabilitado = bloqueado;
		var campo = new Array();
		/*
		campo[campo.length] = document.getElementById( 'sba1ano' );
		campo[campo.length] = document.getElementById( 'sba2ano' );
		campo[campo.length] = document.getElementById( 'sba3ano' );
		campo[campo.length] = document.getElementById( 'sba4ano' );
		for ( var i = 0; i < esc_nextid; i++ )
		{
			campo[campo.length] = document.getElementById( 'esc_2008_' + i );
			campo[campo.length] = document.getElementById( 'esc_2009_' + i );
			campo[campo.length] = document.getElementById( 'esc_2010_' + i );
			campo[campo.length] = document.getElementById( 'esc_2011_' + i );
		}
		for ( var i = 0; i < campo.length; i++ )
		{
			if ( campo[i] )
			{
				campo[i].disabled = desabilitado;
			}
		}
		*/
		var tr_tit = document.getElementById( 'det_linha_titulo' );
		var tr_con = document.getElementById( 'det_linha_conteudo' );
		var tr_val = document.getElementById( 'trValores' );
		if ( desabilitado )
		{
			tr_tit.style.display = 'none';
			tr_con.style.display = 'none';
			tr_val.style.display = 'none';
		}
		else
		{
			tr_tit.style.display = IE ? 'block' : 'table-row';
			tr_con.style.display = IE ? 'block' : 'table-row';
			tr_val.style.display = IE ? 'block' : 'table-row';
		}
	}
	
	var planosInterno = new Array();
	<?php foreach ( $planosinterno as $programa => $pi ) : ?>
		planosInterno[<?= $programa ?>] = '<?= str_replace( "\n", "", str_replace( "'", "\\'", $pi ) ) ?>';
	<?php endforeach; ?>
	
	function atualizarPlanoInterno()
	{
		var programa = document.getElementById( 'prgid' );
		var campoPI = document.getElementById( 'planointerno' );
		if ( !programa || !campoPI )
		{
			return;
		}
		campoPI.value = planosInterno[programa.value];
	}
	
	function excluirSubacao( sbaid )
	{
		if ( !confirm( "Deseja realmente excluir a subação?" ) )
		{
			return;
		}
		window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid + "&excluir=1";
	}
	
	function irParaSubacao( sbaid )
	{
		window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid;
	}
	
	// recebe em formato 000.000,00
	function textoParaValor( texto )
	{
		var valor =  parseFloat( ( texto + '' ).replace( /\./g, '' ).replace( /,/g, '.' ) );
		if ( isNaN( valor ) )
		{
			return 0;
		}
		return valor;
	}
	
	function textoParaNumero( texto )
	{
		return ( texto + '' ).replace( /\./g, '' ).replace( /,/g, '.' );
	}
	
	function valorParaDinheiro( valor )
	{
		valor = Math.round( ( valor - 0 ) * 100 ) + '';
		return mascaraglobal( '###.###.###.###,##', valor );
	}
	
	var comp_nextid = 0;
	
	function atualizarValorUnitario()
	{
		var campo_detal = document.getElementById( 'costotal' );
		var campo_total = document.getElementById( 'sbatotal' );
		if ( !campo_detal || !campo_total )
		{
			return;
		}
		var valor_detal = textoParaValor( campo_detal.innerHTML );
		var valor_total = textoParaValor( campo_total.value );
		var valor = 0;
		if ( valor_total > 0 )
		{
			valor = valor_detal / valor_total;
		}
		var campo = document.getElementById( 'sbaunt' );
		if ( campo )
		{
			campo.value = valorParaDinheiro( valor );
		}
	}
	
	
</script>
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
<?cte_montaTitulo( $titulo_modulo, '' );?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<form method="POST" name="formulario">
					<input type="hidden" name="formulario" value="1"/>
					<input type="hidden" name="irpara" id="irpara" value=""/>
					<input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
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
									<!--
									<a href="?modulo=principal/formulario_avaliacao&acao=A&indid=<?= $indicadorId ?>">
									-->
										<?= $indicadorCod ?>. <?= $indicadorDescricao; ?>
									<!--
									</a>
									-->
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Demanda:</td>
								<td><?= $tipoModulo; ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Ação:</td>
								<td>
									<!--
									<a href="?modulo=principal/par_acao&acao=A&ptoid=<?= $ptoid ?>&acilocalizador=<?= $acilocalizador ?>&aciid=<?= $aciid ?>">
									-->
										<?= $acaoDescricao; ?>
									<!--
									</a>
									-->
								</td>
							</tr>
							
							<?php if( cte_podeEditarParecer( $inuid ) ): ?>
								<tr>
									<td colspan="2" class="SubtituloEsquerda">Parecer Equipe Técnica </td>
								</tr>
								<?php
								/*
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
								*/
								?>
								<tr>
									<td align='right' class="SubTituloDireita">Parecer:</td>
									<td><?= campo_textarea( 'sbaparecer', 'N', $habil, '', 70, 3, 0 ); ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Status Subação:</td>
									<td>
										<?php 
										$sql = sprintf( "select ssuid as codigo, ssudescricao as descricao from cte.statussubacao WHERE ssutipostatus = 'E'" );
										$db->monta_combo( "ssuid", $sql, $habil, '&nbsp;', '', '' );
										?>
									</td>
								</tr>
							<?php else: ?>
								<input type="hidden" name="psuid" value="<?= $psuid ?>"/>
								<input type="hidden" name="sbaparecer" value="<?= $sbaparecer ?>"/>
								<input type="hidden" name="ssuid" value="<?= $ssuid ?>"/>
							<?php endif; ?>
							
							
							
							
							
							
							
							
							
							
							
							
							<?php
							$itrid = cte_pegarItrid( $inuid );
							$grandeMunicipio =
								$itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ||
								cte_verificaGrandeMunicipio( $inuid );
							$habil_subacao = $habil == "S" && $grandeMunicipio;
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
								
								function propostaSubAcao( descricao, metodologia, programa, forma, unidade, prgid, ppsid )
								{
									atualizaCampoSubAcao( 'sbadsc_fake', descricao );
									atualizaCampoSubAcao( 'sbastgmpl_fake', metodologia );
									atualizaCampoSubAcao( 'sbaprm_fake', programa );
									atualizaCampoSubAcao( 'undid_fake', unidade );
									atualizaCampoSubAcao( 'frmid_fake', forma );
									atualizaCampoSubAcao( 'prgid_fake', prgid );
									
									atualizaCampoSubAcao( 'sbadsc', descricao );
									atualizaCampoSubAcao( 'sbastgmpl', metodologia );
									atualizaCampoSubAcao( 'sbaprm', programa );
									atualizaCampoSubAcao( 'undid', unidade );
									atualizaCampoSubAcao( 'frmid', forma );
									atualizaCampoSubAcao( 'prgid', prgid );
									atualizaCampoSubAcao( 'ppsid', ppsid );
									
									atualizarPlanoInterno();
									exibeCampo();
									atualizarBloqueioCamposQuantitativo();
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
									<?  if( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL && !$sbaid ) : ?>
										<div align="left" >
											<a href="javascript:janelaSecundaria('brasilpro.php?modulo=principal/proposta/proposta_subacao&acao=A&aciid=<?= $aciid ?>')"> 
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
										${"prgid" . $sufixo_campo} = $prgid; 
										$sql = "
											SELECT
												prgid as codigo,
												substr( prgdsc, 1, 100 ) as descricao
											FROM cte.programa
										";
										$db->monta_combo( 'prgid' . $sufixo_campo, $sql, $habil_subacao ? 'S' : 'N', 'Selecione', 'exibeCampo', '', '', '', 'S', 'prgid' . $sufixo_campo );
									?>
									<?php if ( !$habil_subacao ) : ?>
										<input
											type="hidden"
											name="prgid"
											id="prgid"
											value="<?= simec_htmlentities( $prgid ) ?>"
										/>
									<?php endif; ?>					
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Plano Interno:</td>
								<td>
									<?=
									campo_texto(
										"planointertno", "N",
										"N", "Plano Interno",
										51, 100, "", ""
									);
									?>					
								</td>
							</tr>
						    <?php
							    // exibe ou esconde a linha da tabela q contém o campo de texto do programa																								
								if( $prgid == $prgid_outros )
								{
									if ( $sbaprm == '' )
									{
										$display = "display: none";
									}
									else
									{
										$display = "display: table-row";									
									}								
								}
								else
								{									
									$display = "display: none";							
								}
							?>
							<tr id="tr_programa" style="<?=$display ?>;">							
								<td align='right' class="SubTituloDireita">Programa:</td>
								<td>
									<?php
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
								<td align='right' class="SubTituloDireita">Meta/Objetivo:</td>
								<td>
									<?php
										$sql = "
											SELECT
												um.undid as codigo,
												um.unddsc as descricao
											FROM cte.unidademedida um
												inner join cte.indicadorunidademedida iu on
													iu.undid = um.undid
											WHERE
												um.undtipo = '" . $capturaTipo . "' and
												iu.indid = " . ( (integer) $indicadorId ) . "
											ORDER BY
												um.unddsc
										";
										${"undid" . $sufixo_campo} = $undid;
										$db->monta_combo( "undid".$sufixo_campo, $sql, $habil_subacao ? 'S' : 'N', 'Escolha...', '', '','','','S', 'undid'.$sufixo_campo );
									?>
									<?php if ( !$habil_subacao ) : ?>
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
										$db->monta_combo(
											"frmid".$sufixo_campo,
											$sql,
											$habil_subacao ? 'S' : 'N',
											'Escolha...',
											'regraFormaExec',
											'','','','S',
											'frmid'.$sufixo_campo
										);
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
							<tr <?= $podeEditarTipoCronograma ? '' : ' style="display:none;" ' ?>>
								<td align='right' class="SubTituloDireita">
									Cronograma:
								</td>
								<td>
									<script type="text/javascript">
										
										function alterarCronograma()
										{
											var IE = !!document.all;
											var global = document.getElementById( 'sbaporescolaGlobal' ).checked;
											var linhaGlobal = document.getElementById( 'linhaTotalGlobal' );
											var tabelaEscolas = document.getElementById( 'tabelaEscolasCronograma' );
											if ( global )
											{
												linhaGlobal.style.display = IE ? 'block' : 'table-row';
												tabelaEscolas.style.display = 'none';
											}
											else
											{
												linhaGlobal.style.display = 'none';
												tabelaEscolas.style.display = IE ? 'block' : 'table';
											}
										}
										
									</script>
									<input
										type="radio"
										name="sbaporescola"
										value="0"
										onchange="alterarCronograma();"
										onclick="alterarCronograma();"
										id="sbaporescolaGlobal"
										<?= !$sbaporescola ? 'checked="checked"' : '' ?>
									/>
									Global
									&nbsp;&nbsp;&nbsp;
									<input
										type="radio"
										name="sbaporescola"
										value="1"
										onchange="alterarCronograma();"
										onclick="alterarCronograma();"
										<?= $sbaporescola ? 'checked="checked"' : '' ?>
									/>
									Por Escola
								</td>
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
												<!-- <th>2007</th> -->
												<th>2008</th>
												<th>2009</th>
												<th>2010</th>
												<th>2011</th>
												<th>Total</th>
											</tr>
										</thead>
										<tbody>
											<tr id="linhaTotalGlobal">
												<td align="right"  class="SubTituloDireita">Quantidades:</td>
												<!-- <td align="right"><?=campo_texto( 'sba0ano', '', $habil, '', 23, 20, '#######', '', 'right', '', 0, 'id="sba0ano" onBlur="MouseBlur(this);"' , 'somaQuantidades();' );?></td> -->
												<td align="right">
													<?=
													campo_texto(
														'sba1ano',	'',		$habil,		'',
														23,			20,		'#######',	'',
														'right',	'',		0,
														'id="sba1ano" onBlur="MouseBlur(this);"',
														'somaQuantidades();'
													);
													?>
												</td>
												<td align="right">
													<?=
													campo_texto(
														'sba2ano',	'',		$habil,		'',
														23,			20,		'#######',	'',
														'right',	'',		0,
														'id="sba2ano" onBlur="MouseBlur(this);"',
														'somaQuantidades();'
													);
													?>
												</td>
												<td align="right">
													<?=
													campo_texto(
														'sba3ano',	'',		$habil,		'',
														23,			20,		'#######',	'',
														'right',	'',		0,
														'id="sba3ano" onBlur="MouseBlur(this);"',
														'somaQuantidades();'
													);
													?>
												</td>
												<td align="right">
													<?=
													campo_texto(
														'sba4ano',	'',		$habil,		'',
														23,			20,		'#######',	'',
														'right',	'',		0,
														'id="sba4ano" onBlur="MouseBlur(this);"',
														'somaQuantidades();'
													);
													?>
												</td>
												<td align="right">
													<?=
													campo_texto(
														'sbatotal',	'',		'N',		'',
														23,			20,		'#######',	'',
														'right',	'',		0,
														'id="sbatotal"'
													);
													?>
												</td>
											</tr>
											<tr>
												<td align="right"  class="SubTituloDireita">Cronograma Físico:</td>
												<!--
												<td nowrap align="right">
													<?=campo_texto( 'sba0ini', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba0ini"' );?>
													&nbsp;até&nbsp;
													<?=campo_texto( 'sba0fim', '', $habil, '', 7, 7, '##/####', '', 'right', '', 0, 'id="sba0fim"' );?>
												</td>
												-->
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
									
									
									
									
									
									
									
									
									
									
									
									
									
									
									
									
									
									
									<table
										border="0"
										align="center"
										cellpadding="0"
										cellspacing="1"
										style="border: 1px solid #cfcfcf; margin-top: 10px;"
										id="tabelaEscolasCronograma"
										width="100%"
									>
										<tr bgcolor="#cfcfcf">
											<td width="30" align="center" style="padding:3px;">
												&nbsp;
											</td>
											<td align="left" style="padding:3px 0;">
												<b>&nbsp;Escola</b>
											</td>
											<td width="90" align="center" style="padding:3px 0;">
												<b>2008</b>
											</td>
											<td width="90" align="center" style="padding:3px 0;">
												<b>2009</b>
											</td>
											<td width="90" align="center" style="padding:3px 0;">
												<b>2010</b>
											</td>
											<td width="90" align="center" style="padding:3px 0;">
												<b>2011</b>
											</td>
											<td width="90" align="center" style="padding: 3px 16px 0px 0px;">
												<b>Total</b>
											</td>
										</tr>
										<tr>
											<td colspan="7">
												<div
													style="margin:0;padding:0;height:150px;width:100%;"
													class="div_rolagem"
												>
													<table
														id="esc_tabela"
														border="0"
														align="left"
														cellpadding="2"
														cellspacing="1"
														width="100%"
													></table>
												</div>
											</td>
										</tr>
										<tr bgcolor="#cfcfcf">
											<td colspan="2" style="padding:3px;">
												<?php if ( $habil == "S" ) : ?>
													<span
														onmouseover="this.style.cursor = 'pointer';"
														onclick="esc_popupSelecionar();"
													>
														<img
															src="/imagens/gif_inclui.gif"
															align="absmiddle"
														/>
														Inserir Escola
													</span>
												<?php else: ?>
													<img
														src="/imagens/gif_inclui_d.gif"
													/>
												<?php endif; ?>
											</td>
											<td align="right">
												<span id="valorescola_2008">0</span>
											</td>
											<td align="right">
												<span id="valorescola_2009">0</span>
											</td>
											<td align="right">
												<span id="valorescola_2010">0</span>
											</td>
											<td align="right">
												<span id="valorescola_2011">0</span>
											</td>
											<td align="right" style="padding: 3px 16px 0px 0px;">
												<span id="valorescola_total">0</span>
											</td>
										</tr>
									</table>
									<script type="text/javascript">
										
										var esc_tabela = document.getElementById( 'esc_tabela' );
										
										var esc_nextid = 0;
										
										var esc_selecionadas = new Array();
										
										function esc_popupSelecionar()
										{
											atuais = esc_selecionadas.join( ',' );
											atuais = escape( atuais );
											janela = window.open( '?modulo=principal/escolasescolhasubacao&acao=A&atuais=' + atuais,'_blank','height=400,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
											janela.focus();
										}
										
										function esc_adicionarItem( entid, entnome, vlr2008, vlr2009, vlr2010, vlr2011 )
										{
											var novo = '';
											var id = esc_nextid;
											esc_nextid++;
											
											entid   -= 0;
											entnome += '';
											vlr2008 -= 0;
											vlr2009 -= 0;
											vlr2010 -= 0;
											vlr2011 -= 0;
											
											esc_selecionadas[esc_selecionadas.length] = entid;
											
											var safari = navigator.userAgent.indexOf( 'Safari' ) > 0;
											var gecko  = navigator.product == 'Gecko';
											
											var evento = new Function ( 'this.value = mascaraglobal( \'#########\', this.value ); esc_atualizarValores();' );
											
											// CAMPOS
											
											//var desabilitado = campoQuantitativoBloqueado;
											var desabilitado = false;
											
											var input_2008 = document.createElement( 'input' );
												input_2008.disabled        = desabilitado;
												input_2008.type            = 'text';
												input_2008.id              = 'esc_2008_' + id;
												input_2008.maxLength       = '9';
												input_2008.className       = 'normal';
												input_2008.name            = 'escola[' + entid + '][2008]';
												input_2008.value           = vlr2008;
												input_2008.style.width     = '70px';
												input_2008.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_2008.disabled = true;
												<?php else: ?>
													if ( safari || gecko )
													{
														input_2008.addEventListener( "keyup", evento, false );
														input_2008.addEventListener( "blur", evento, false );
													}
													else
													{
														input_2008.attachEvent( 'onkeyup', evento );
														input_2008.attachEvent( 'onblur', evento );
													}
												<?php endif; ?>
											
											var input_2009 = document.createElement( 'input' );
												input_2009.disabled        = desabilitado;
												input_2009.type            = 'text';
												input_2009.id              = 'esc_2009_' + id;
												input_2009.maxLength       = '9';
												input_2009.className       = 'normal';
												input_2009.name            = 'escola[' + entid + '][2009]';
												input_2009.value           = vlr2009;
												input_2009.style.width     = '70px';
												input_2009.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_2009.disabled = true;
												<?php else: ?>
													if ( safari || gecko )
													{
														input_2009.addEventListener( "keyup", evento, false );
														input_2009.addEventListener( "blur", evento, false );
													}
													else
													{
														input_2009.attachEvent( 'onkeyup', evento );
														input_2009.attachEvent( 'onblur', evento );
													}
												<?php endif; ?>
											
											var input_2010 = document.createElement( 'input' );
												input_2010.disabled        = desabilitado;
												input_2010.type            = 'text';
												input_2010.id              = 'esc_2010_' + id;
												input_2010.maxLength       = '9';
												input_2010.className       = 'normal';
												input_2010.name            = 'escola[' + entid + '][2010]';
												input_2010.value           = vlr2010;
												input_2010.style.width     = '70px';
												input_2010.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_2010.disabled = true;
												<?php else: ?>
													if ( safari || gecko )
													{
														input_2010.addEventListener( "keyup", evento, false );
														input_2010.addEventListener( "blur", evento, false );
													}
													else
													{
														input_2010.attachEvent( 'onkeyup', evento );
														input_2010.attachEvent( 'onblur', evento );
													}
												<?php endif; ?>
											
											var input_2011 = document.createElement( 'input' );
												input_2011.disabled        = desabilitado;
												input_2011.type            = 'text';
												input_2011.id              = 'esc_2011_' + id;
												input_2011.maxLength       = '9';
												input_2011.className       = 'normal';
												input_2011.name            = 'escola[' + entid + '][2011]';
												input_2011.value           = vlr2011;
												input_2011.style.width     = '70px';
												input_2011.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_2011.disabled = true;
												<?php else: ?>
													if ( safari || gecko )
													{
														input_2011.addEventListener( "keyup", evento, false );
														input_2011.addEventListener( "blur", evento, false );
													}
													else
													{
														input_2011.attachEvent( 'onkeyup', evento );
														input_2011.attachEvent( 'onblur', evento );
													}
												<?php endif; ?>
											
											var span_total = document.createElement( 'span' );
												span_total.id        = 'esc_total_' + id;
												span_total.innerHTML = vlr2008 + vlr2009 + vlr2010 + vlr2011;
											
											
											var input_fake = document.createElement( 'input' );
												input_fake.type            = 'hidden';
												input_fake.id              = 'esc_fake_' + id;
												input_fake.name            = 'escola[' + entid + '][fake]';
												input_fake.value           = '1';
											
											var botao_excluir = document.createElement( 'img' );
												<?php if ( $habil != 'S' ) : ?>
													botao_excluir.src = '/imagens/excluir_01.gif';
												<?php else : ?>
													botao_excluir.src = '/imagens/excluir.gif';
													var evento_excluir = new Function( " esc_excluirLinha( document.getElementById( 'esc_" + id + "', " + entid + " ) ) " );
													var evento_mouse =  new Function( " this.style.cursor = 'pointer'; " );
													if ( safari || gecko )
													{
														botao_excluir.addEventListener( "click", evento_excluir, false );
														botao_excluir.addEventListener( "click", evento_excluir, false );
														botao_excluir.addEventListener( "mouseover", evento_mouse, false );
														botao_excluir. onmouseover="this.style.cursor = 'pointer';"
													}
													else
													{
														botao_excluir.attachEvent( 'onclick', evento_excluir );
														botao_excluir.attachEvent( 'onclick', evento_excluir );
														botao_excluir.attachEvent( "onmouseover", evento_mouse );
													}
												<?php endif; ?>
											
											// LINHA
											
											var tr    = esc_tabela.insertRow( esc_tabela.rows.length );
												tr.id = 'esc_' + id;
												tr.style.backgroundColor = '#f0f0f0';
											
											//alert( tr.id );
											
											// CELULAS
											
											var td_acao = tr.insertCell( tr.cells.length );
												td_acao.style.width = '28px';
												td_acao.style.textAlign = 'center';
												td_acao.appendChild( botao_excluir );
												td_acao.appendChild( input_fake );
											
											var td_nome = tr.insertCell( tr.cells.length );
												//td_nome.style.width = '298px';
												td_nome.style.textAlign = 'left';
												td_nome.appendChild( document.createTextNode( entnome ) );
											
											var td_2008 = tr.insertCell( tr.cells.length );
												td_2008.style.width = '86px';
												td_2008.style.textAlign = 'right';
												td_2008.appendChild( input_2008 );
											
											var td_2009 = tr.insertCell( tr.cells.length );
												td_2009.style.textAlign = 'right';
												td_2009.style.width = '86px';
												td_2009.appendChild( input_2009 );
											
											var td_2010 = tr.insertCell( tr.cells.length );
												td_2010.style.width = '86px';
												td_2010.style.textAlign = 'right';
												td_2010.appendChild( input_2010 );
											
											var td_2011 = tr.insertCell( tr.cells.length );
												td_2011.style.width = '86px';
												td_2011.style.textAlign = 'right';
												td_2011.appendChild( input_2011 );
											
											var td_total = tr.insertCell( tr.cells.length );
												td_total.style.width = '86px';
												td_total.style.textAlign = 'right';
												td_total.appendChild( span_total );
											
											esc_atualizarValores();
										}
										
										function esc_pegarValorTotal()
										{
											var campo_2008 = null;
											var campo_2009 = null;
											var campo_2010 = null;
											var campo_2011 = null;
											var total = 0;
											for ( var i = 0; i < esc_nextid; i++ )
											{
												campo_2008 = document.getElementById( 'esc_2008_' + i );
												campo_2009 = document.getElementById( 'esc_2009_' + i );
												campo_2010 = document.getElementById( 'esc_2010_' + i );
												campo_2011 = document.getElementById( 'esc_2011_' + i );
												if ( !campo_2008 || !campo_2009 || !campo_2010 || !campo_2011 )
												{
													continue;
												}
												var vlr2008 = campo_2008.value;
												var vlr2009 = campo_2009.value;
												var vlr2010 = campo_2010.value;
												var vlr2011 = campo_2011.value;
												total += vlr2008 + vlr2009 + vlr2010 + vlr2011;
											}
											return total;
										}
										
										function esc_atualizarValores()
										{
											var campo_2008  = null;
											var campo_2009  = null;
											var campo_2010  = null;
											var campo_2011  = null;
											var campo_total = null;
											var total_2008 = 0;
											var total_2009 = 0;
											var total_2010 = 0;
											var total_2011 = 0;
											var total_item = 0;
											var vlr2008 = 0;
											var vlr2009 = 0;
											var vlr2010 = 0;
											var vlr2011 = 0;
											for ( var i = 0; i <= esc_nextid; i++ )
											{
												campo_2008  = document.getElementById( 'esc_2008_' + i );
												campo_2009  = document.getElementById( 'esc_2009_' + i );
												campo_2010  = document.getElementById( 'esc_2010_' + i );
												campo_2011  = document.getElementById( 'esc_2011_' + i );
												campo_total = document.getElementById( 'esc_total_' + i );
												if ( !campo_2008 || !campo_2009 || !campo_2010 || !campo_2011 || !campo_total )
												{
													continue;
												}
												var vlr2008 = campo_2008.value - 0;
												var vlr2009 = campo_2009.value - 0;
												var vlr2010 = campo_2010.value - 0;
												var vlr2011 = campo_2011.value - 0;
												total_item = vlr2008 + vlr2009 + vlr2010 + vlr2011;
												campo_total.innerHTML = total_item;
												total_2008 += vlr2008;
												total_2009 += vlr2009;
												total_2010 += vlr2010;
												total_2011 += vlr2011;
												
											}
											document.getElementById( 'valorescola_2008' ).innerHTML  = total_2008;
											document.getElementById( 'valorescola_2009' ).innerHTML  = total_2009;
											document.getElementById( 'valorescola_2010' ).innerHTML  = total_2010;
											document.getElementById( 'valorescola_2011' ).innerHTML  = total_2011;
											document.getElementById( 'valorescola_total' ).innerHTML = total_2008 + total_2009 + total_2010 + total_2011;
											
											document.getElementById( 'sba1ano' ).value  = total_2008;
											document.getElementById( 'sba2ano' ).value  = total_2009;
											document.getElementById( 'sba3ano' ).value  = total_2010;
											document.getElementById( 'sba4ano' ).value  = total_2011;
											
											var total_esc = total_2008 + total_2009 + total_2010 + total_2011;
											document.getElementById( 'sbatotal' ).value = total_esc;
											
											atualizarValorUnitario();
											
										}
										
										function esc_excluirLinha( tr, entid )
										{
											if ( tr )
											{
												if ( !confirm( 'Deseja realmente excluir a escola do cronograma?' ) )
												{
													return;
												}
												tr.parentNode.removeChild( tr );
												for ( var chave = 0; chave < esc_selecionadas.length; chave++ )
												{
													esc_selecionadas[chave] = false;
												}
												esc_atualizarValores();
											}
										}
										
										function esc_focarFinalTabela()
										{
											var campo_2008 = null;
											for ( var i = esc_nextid - 1; i >= 0; i-- )
											{
												campo_2008 = document.getElementById( 'esc_2008_' + i );
												if ( campo_2008 )
												{
													campo_2008.focus();
													return;
												}
											}
										}
										
										<?php foreach ( $escolas as $esc ): ?>
											esc_adicionarItem( '<?= $esc['entid'] ?>', '<?= $esc['entnome'] ?>', '<?= $esc['2008'] ?>', '<?= $esc['2009'] ?>', '<?= $esc['2010'] ?>', '<?= $esc['2011'] ?>' );
										<?php endforeach; ?>
										
									</script>
									
								</td>
							</tr>
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							<tr style="background-color: #cccccc;" id="det_linha_titulo">
								<td align="left" colspan="2">
									<b>Detalhamento dos Itens que Compõem a Especificação</b>
								</td>
							</tr>
							<tr id="det_linha_conteudo">
								<td colspan="2">
									<table
										border="0"
										align="center"
										cellpadding="0"
										cellspacing="1"
										style="border: 1px solid #cfcfcf;"
										width="100%"
									>
										<tr bgcolor="#dcdcdc">
											<td width="30" align="center" style="padding:3px;">
												&nbsp;
											</td>
											<td width="50" align="center" style="padding:3px 0;"><b>Ordem</b></td>
											<td style="padding:3px 0;"><b>Identifição do Item</b></td>
											<td width="150" style="padding:3px 0;"><b>Unidade de Medida</b></td>
											<td width="50" align="center" style="padding:3px 0;"><b>Qtd.</b></td>
											<td width="70" align="center" style="padding:3px 0;"><b>Vl. Unit.</b></td>
											<td width="110" align="center" style="padding: 3px 16px 0px 0px;"><b>Valor Total</b></td>
										</tr>
										<tr>
											<td colspan="7">
												<div
													style="margin:0;padding:0;height:150px;width:100%;"
													class="div_rolagem"
												>
													<table
														id="comp_tabela"
														border="0"
														align="left"
														cellpadding="2"
														cellspacing="1"
														width="100%"
													></table>
												</div>
											</td>
										</tr>
										<tr bgcolor="#dcdcdc">
											<td colspan="6" style="padding:3px;">
												<?php if ( $habil == "S" ) : ?>
													<span
														onmouseover="this.style.cursor = 'pointer';"
														onclick="adicionarItem( '', '', '', '' , '' );com_focarFinalTabela();"
													>
														<img
															src="/imagens/gif_inclui.gif"
															align="absmiddle"
														/>
														Inserir Item de Composição
													</span>
												<?php else: ?>
													<img
														src="/imagens/gif_inclui_d.gif"
													/>
												<?php endif; ?>
											</td>
											<td align="right" style="padding: 3px 16px 0px 0px;">
												R$ <span id="costotal">0</span>
											</td>
										</tr>
										<tr>
											<td align="right" colspan="2" class="SubTituloDireita">Comentários:</td>
											<td colspan="6">
												<?= campo_textarea( 'sbauntdsc', 'N', $habil , '', 100, 10, 2000 ); ?>
												<img border="0" id="imgDVU" title="Indica campo obrigatório." src="../imagens/obrig.gif" style="display:none"/>
											</td>
										</tr>
									</table>
									
									<script type="text/javascript">
										
										var comp_tabela = document.getElementById( 'comp_tabela' );
										
										function adicionarItem( cosord, coddsc, cosunimed, cosqtd, cosvlruni )
										{
											var novo = '';
											var id = comp_nextid;
											comp_nextid++;
											
											cosord    -= 0;
											coddsc    = coddsc + '';
											cosunimed = cosunimed + '';
											cosqtd    -= 0;
											
											cosvlruni = textoParaValor( cosvlruni );
											
											if ( cosord == 0 )
											{
												cosord = comp_nextid;
											}
											
											var safari = navigator.userAgent.indexOf( 'Safari' ) > 0;
											var gecko  = navigator.product == 'Gecko';
											
											//var evento = com_atualizarValores;
											var evento_qtd = new Function( 'this.value = mascaraglobal( \'#########\', this.value ); com_atualizarValores(); atualizarValorUnitario();calculaValoresAnuais();' );
											var evento_vlr = new Function( 'this.value = mascaraglobal( \'###.###.###,##\', this.value ); com_atualizarValores(); atualizarValorUnitario();calculaValoresAnuais();' );
											
											// CAMPOS
											
											var input_cosord = document.createElement( 'input' );
												input_cosord.type            = 'text';
												input_cosord.maxLength       = '3';
												input_cosord.className       = 'normal';
												input_cosord.name            = 'composicao[cosord][]';
												input_cosord.value           = cosord;
												input_cosord.style.width     = '25px';
												input_cosord.style.textAlign = 'center';
												<?php if ( $habil != 'S' ) : ?>
													input_cosord.disabled = true;
												<?php endif; ?>
											
											var input_coddsc = document.createElement( 'input' );
												input_coddsc.type        = 'text';
												input_coddsc.maxLength   = '70';
												input_coddsc.className   = 'normal';
												input_coddsc.name        = 'composicao[cosdsc][]';
												input_coddsc.value       = coddsc;
												input_coddsc.style.width = '240px';
												<?php if ( $habil != 'S' ) : ?>
													input_coddsc.disabled = true;
												<?php endif; ?>
											
											/*
											var input_cosunimed = document.createElement( 'input' );
												input_cosunimed.type        = 'text';
												input_cosunimed.maxLength   = '50';
												input_cosunimed.className   = 'normal';
												input_cosunimed.name        = 'composicao[cosunimed][]';
												input_cosunimed.value       = cosunimed;
												input_cosunimed.style.width = '140px';
												<?php if ( $habil != 'S' ) : ?>
													input_cosunimed.disabled = true;
												<?php endif; ?>
											*/
											var select_cosunimed = document.createElement( 'select' );
												select_cosunimed.name        = 'composicao[unddid][]';
												select_cosunimed.className   = 'normal';
												select_cosunimed.style.width = '140px';
												<?php
												$sql = "select unddid, undddsc from cte.unidademedidadetalhamento";
												$uni_med_det = $db->carregar( $sql );
												$uni_med_det = $uni_med_det ? $uni_med_det : array();
												?>
												//alert( cosunimed );
												<?php foreach( $uni_med_det as $item ) : ?>
													selecionado = cosunimed == '<?= $item["unddid"] ?>';
													select_cosunimed.options[select_cosunimed.length] = new Option( '<?= $item["undddsc"] ?>', '<?= $item["unddid"] ?>', selecionado, selecionado );
												<?php endforeach; ?>
											
											var input_cosqtd = document.createElement( 'input' );
												input_cosqtd.id              = 'cosqtd_' + id;
												input_cosqtd.type            = 'text';
												input_cosqtd.maxLength       = '6';
												input_cosqtd.className       = 'normal';
												input_cosqtd.name            = 'composicao[cosqtd][]';
												input_cosqtd.value           = cosqtd;
												input_cosqtd.style.width     = '40px';
												input_cosqtd.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_cosqtd.disabled = true;
												<?php endif; ?>
												if ( safari || gecko )
												{
													input_cosqtd.addEventListener( "keyup", evento_qtd, false );
													input_cosqtd.addEventListener( "blur", evento_qtd, false );
												}
												else
												{
													input_cosqtd.attachEvent( 'onkeyup', evento_qtd );
													input_cosqtd.attachEvent( 'onblur', evento_qtd );
												}
											
											var input_cosvlruni = document.createElement( 'input' );
												input_cosvlruni.id              = 'cosvlruni_' + id;
												input_cosvlruni.type            = 'text';
												input_cosvlruni.maxLength       = '12';
												input_cosvlruni.className       = 'normal';
												input_cosvlruni.name            = 'composicao[cosvlruni][]';
												input_cosvlruni.value           = valorParaDinheiro( cosvlruni );
												input_cosvlruni.style.width     = '60px';
												input_cosvlruni.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_cosvlruni.disabled = true;
												<?php endif; ?>
												if ( safari || gecko )
												{
													input_cosvlruni.addEventListener( "keyup", evento_vlr, false );
													input_cosvlruni.addEventListener( "blur", evento_vlr, false );
												}
												else
												{
													input_cosvlruni.attachEvent( 'onkeyup', evento_vlr );
													input_cosvlruni.attachEvent( 'onblur', evento_vlr );
												}
											
											var span_total = document.createElement( 'span' );
												span_total.id        = 'costotal_' + id;
												span_total.innerHTML = cosqtd * cosvlruni;
											
											var botao_excluir = document.createElement( 'img' );
												<?php if ( $habil != 'S' ) : ?>
													botao_excluir.src = '/imagens/excluir_01.gif';
												<?php else : ?>
													botao_excluir.src = '/imagens/excluir.gif';
													var evento_excluir = new Function( " com_excluirLinha( document.getElementById( 'cos_" + id + "' ) ) " );
													var evento_mouse =  new Function( " this.style.cursor = 'pointer'; " );
													if ( safari || gecko )
													{
														botao_excluir.addEventListener( "click", evento_excluir, false );
														botao_excluir.addEventListener( "mouseover", evento_mouse, false );
														botao_excluir. onmouseover="this.style.cursor = 'pointer';"
													}
													else
													{
														botao_excluir.attachEvent( 'onclick', evento_excluir );
														botao_excluir.attachEvent( "onmouseover", evento_mouse );
													}
												<?php endif; ?>
											
											// LINHA
											
											var tr    = comp_tabela.insertRow( comp_tabela.rows.length );
												tr.id = 'cos_' + id;
												tr.style.backgroundColor = '#f0f0f0';
											
											// CELULAS
											
											var td_acao = tr.insertCell( tr.cells.length );
												td_acao.style.textAlign = 'center';
												td_acao.appendChild( botao_excluir );
											
											var td_cosord = tr.insertCell( tr.cells.length );
												td_cosord.style.width = '46px';
												td_cosord.style.textAlign = 'center';
												td_cosord.appendChild( input_cosord );
											
											var td_coddsc = tr.insertCell( tr.cells.length );
												//td_coddsc.style.width = '246px';
												td_coddsc.appendChild( input_coddsc );
											
											var td_cosunimed = tr.insertCell( tr.cells.length );
												td_cosunimed.style.width = '146px';
												td_cosunimed.appendChild( select_cosunimed );
											
											var td_cosqtd = tr.insertCell( tr.cells.length );
												td_cosqtd.style.width = '46px';
												td_cosqtd.style.textAlign = 'right';
												td_cosqtd.appendChild( input_cosqtd );
											
											var td_cosvlruni = tr.insertCell( tr.cells.length );
												td_cosvlruni.style.width = '66px';
												td_cosvlruni.style.textAlign = 'right';
												td_cosvlruni.appendChild( input_cosvlruni );
											
											var td_total = tr.insertCell( tr.cells.length );
												td_total.style.width = '106px';
												td_total.style.textAlign = 'right';
												td_total.appendChild( span_total );
											
											com_atualizarValores();
										}
										
										function com_excluirLinha( tr )
										{
											if ( tr )
											{
												if ( !confirm( 'Deseja realmente excluir o item de composição?' ) )
												{
													return;
												}
												tr.parentNode.removeChild( tr );
												com_atualizarValores();
											}
										}
										
										function com_pegarValorTotal()
										{
											var campo_cosqtd    = null;
											var campo_cosvlruni = null;
											var total = 0;
											for ( var i = 0; i < comp_nextid; i++ )
											{
												campo_cosqtd    = document.getElementById( 'cosqtd_' + i );
												campo_cosvlruni = document.getElementById( 'cosvlruni_' + i );
												if ( !campo_cosqtd || !campo_cosvlruni )
												{
													continue;
												}
												total += ( campo_cosqtd.value - 0 ) * ( campo_cosvlruni.value - 0 );
											}
											return total;
										}
										
										function com_atualizarValores()
										{
											var campo_cosqtd    = null;
											var campo_cosvlruni = null;
											var campo_total     = null;
											var qtd = 0;
											var uni = 0;
											var total_item = 0;
											var total_total = 0;
											for ( var i = 0; i < comp_nextid; i++ )
											{
												campo_cosqtd    = document.getElementById( 'cosqtd_' + i );
												campo_cosvlruni = document.getElementById( 'cosvlruni_' + i );
												campo_total     = document.getElementById( 'costotal_' + i );
												if ( !campo_cosqtd || !campo_cosvlruni || !campo_total )
												{
													continue;
												}
												qtd = campo_cosqtd.value - 0;
												uni = textoParaValor( campo_cosvlruni.value );
												//document.title = qtd + ' ' + uni;
												total_item = qtd * uni;
												campo_total.innerHTML = valorParaDinheiro( total_item );
												total_total += total_item;
												
												//campo_cosvlruni.value = valorParaDinheiro( textoParaValor( campo_cosvlruni.value ) );
											}
											document.getElementById( 'costotal' ).innerHTML = valorParaDinheiro( total_total );
										}
										
										function com_focarFinalTabela()
										{
											var campo_cosqtd = null;
											for ( var i = comp_nextid - 1; i >= 0; i-- )
											{
												campo_cosqtd = document.getElementById( 'cosqtd_' + i );
												if ( campo_cosqtd )
												{
													campo_cosqtd.focus();
													return;
												}
											}
										}
										
										<?php foreach ( $composicao as $item ) : ?>
											adicionarItem( '<?= $item['cosord'] ?>', '<?= $item['cosdsc'] ?>', '<?= $item['unddid'] ?>', '<?= $item['cosqtd'] ?>', '<?= number_format( $item['cosvlruni'], 2, ",", "." ) ?>' );
										<?php endforeach; ?>
										
									</script>
									
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
												<?
													$sbaunt_original = $sbaunt;
													$sbaunt = number_format( $sbaunt, 2, ",", "." );
													echo campo_texto(
														'sbaunt', 'N', 'N',
														'', 25, 20,
														'###.###.###.###.###.###.###,##',
														'', 'left', '',
														0, 'id="sbaunt" onBlur="MouseBlur(this);"'
														//,'calculaValoresAnuais();'
													);
													$sbaunt = $sbaunt_original;
												?>
												<img
													border="0"
													id="imgVU"
													title="Indica campo obrigatório."
													src="../imagens/obrig.gif"
													style="display:none"
												/>
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
															<!-- <th>2007</th> -->
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
															<!-- <td align="right"><?=campo_texto( 'sbavl0', '', 'N', '', 23, 20, '', '', 'right', '', 0, 'id="sbavl0"' );?></td> --> 
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
							
							<tr style="background-color: #cccccc;">
								<td align="left" colspan="2">
									<b>Beneficiário da Sub-ação</b>
								</td>
							</tr>
							<tr>
								<td colspan="2">
									
									<table
										border="0"
										align="center"
										cellpadding="0"
										cellspacing="1"
										style="border: 1px solid #cfcfcf; margin-top: 10px;"
										id="tabelaEscolasCronograma"
										width="100%"
									>
										<tr bgcolor="#cfcfcf">
											<td width="30" align="center" style="padding:3px;">
												&nbsp;
											</td>
											<td align="left" style="padding:3px 0;">
												<b>&nbsp;Beneficiário</b>
											</td>
											<td width="110" align="center" style="padding:3px 0;">
												<b>Zona Rural</b>
											</td>
											<td width="110" align="center" style="padding:3px 0;">
												<b>Zona Urbana</b>
											</td>
											<td width="120" align="center" style="padding:3px 0;">
												<b>Total</b>
											</td>
										</tr>
										<tr>
											<td colspan="7">
												<div
													style="margin:0;padding:0;height:150px;width:100%;"
													class="div_rolagem"
												>
													<table
														id="ben_tabela"
														border="0"
														align="left"
														cellpadding="2"
														cellspacing="1"
														width="100%"
													></table>
												</div>
											</td>
										</tr>
										<tr bgcolor="#cfcfcf">
											<td colspan="2" style="padding:3px;">
												<?php if ( $habil == "S" ) : ?>
													<span
														onmouseover="this.style.cursor = 'pointer';"
														onclick="ben_Adicionar( 0, 0, 0 );"
													>
														<img
															src="/imagens/gif_inclui.gif"
															align="absmiddle"
														/>
														Inserir Beneficiário
													</span>
												<?php else: ?>
													<img
														src="/imagens/gif_inclui_d.gif"
													/>
												<?php endif; ?>
											</td>
											<td align="right">
												<span id="ben_total_zonarural">0</span>
											</td>
											<td align="right">
												<span id="ben_total_zonaurbana">0</span>
											</td>
											<td align="right" style="padding: 3px 16px 0px 0px;">
												<span id="ben_total">0</span>
											</td>
										</tr>
									</table>
									<script type="text/javascript">
										
										var ben_tabela = document.getElementById( 'ben_tabela' );
										
										var ben_nextid = 0;
										
										var ben_selecionadas = new Array();
										
										var beneficiarios = new Array();
										
										<?php foreach ( $ben as $benid => $bendsc ) : ?>
											beneficiarios[beneficiarios.length] = new Array( '<?= $bendsc ?>', '<?= $benid ?>' );
										<?php endforeach; ?>
										
										function ben_Adicionar( benid, rural, urbano )
										{
											var novo = '';
											var id = ben_nextid;
											ben_nextid++;
											
											benid  -= 0;
											rural  -= 0;
											urbano -= 0;
											
											var safari = navigator.userAgent.indexOf( 'Safari' ) > 0;
											var gecko  = navigator.product == 'Gecko';
											
											//var evento = com_atualizarValores;
											var evento = new Function( 'this.value = mascaraglobal( \'#########\', this.value ); ben_atualizarValores();' );
											
											// CAMPOS
											
											var select_benef = document.createElement( 'select' );
												select_benef.name = 'beneficiario[benid][]';
												//select_benef.style.width = '200px';
											for ( var i = 0; i < beneficiarios.length; i++ )
											{
												select_benef.options[i] = new Option( beneficiarios[i][0], beneficiarios[i][1] );
												if ( benid == beneficiarios[i][1] )
												{
													select_benef.options[i].selected = true;
												}
											}
											
											var input_rural = document.createElement( 'input' );
												input_rural.id              = 'ben_rural_' + id;
												input_rural.type            = 'text';
												input_rural.maxLength       = '10';
												input_rural.className       = 'normal';
												input_rural.name            = 'beneficiario[rural][]';
												input_rural.value           = rural;
												input_rural.style.width     = '100px';
												input_rural.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_cosunimed.disabled = true;
												<?php endif; ?>
												if ( safari || gecko )
												{
													input_rural.addEventListener( "keyup", evento, false );
													input_rural.addEventListener( "blur", evento, false );
												}
												else
												{
													input_rural.attachEvent( 'onkeyup', evento );
													input_rural.attachEvent( 'onblur', evento );
												}
											
											var input_urbano = document.createElement( 'input' );
												input_urbano.id              = 'ben_urbano_' + id;
												input_urbano.type            = 'text';
												input_urbano.maxLength       = '10';
												input_urbano.className       = 'normal';
												input_urbano.name            = 'beneficiario[urbano][]';
												input_urbano.value           = urbano;
												input_urbano.style.width     = '100px';
												input_urbano.style.textAlign = 'right';
												<?php if ( $habil != 'S' ) : ?>
													input_cosunimed.disabled = true;
												<?php endif; ?>
												if ( safari || gecko )
												{
													input_urbano.addEventListener( "keyup", evento, false );
													input_urbano.addEventListener( "blur", evento, false );
												}
												else
												{
													input_urbano.attachEvent( 'onkeyup', evento );
													input_urbano.attachEvent( 'onblur', evento );
												}
											
											var span_total = document.createElement( 'span' );
												span_total.id        = 'ben_total_' + id;
												span_total.innerHTML = rural + urbano;
											
											var botao_excluir = document.createElement( 'img' );
												<?php if ( $habil != 'S' ) : ?>
													botao_excluir.src = '/imagens/excluir_01.gif';
												<?php else : ?>
													botao_excluir.src = '/imagens/excluir.gif';
													var evento_excluir = new Function( " ben_excluirLinha( document.getElementById( 'cos_" + id + "' ) ) " );
													var evento_mouse =  new Function( " this.style.cursor = 'pointer'; " );
													if ( safari || gecko )
													{
														botao_excluir.addEventListener( "click", evento_excluir, false );
														botao_excluir.addEventListener( "mouseover", evento_mouse, false );
														botao_excluir. onmouseover="this.style.cursor = 'pointer';"
													}
													else
													{
														botao_excluir.attachEvent( 'onclick', evento_excluir );
														botao_excluir.attachEvent( "onmouseover", evento_mouse );
													}
												<?php endif; ?>
											
											// LINHA
											
											var tr    = ben_tabela.insertRow( ben_tabela.rows.length );
												tr.id = 'cos_' + id;
												tr.style.backgroundColor = '#f0f0f0';
											
											// CELULAS
											
											var td_acao = tr.insertCell( tr.cells.length );
												td_acao.style.width = '32px';
												td_acao.style.textAlign = 'center';
												td_acao.appendChild( botao_excluir );
											
											var td_ben = tr.insertCell( tr.cells.length );
												td_ben.style.textAlign = 'left';
												td_ben.appendChild( select_benef );
											
											var td_rural = tr.insertCell( tr.cells.length );
												td_rural.style.width = '106px';
												td_rural.style.textAlign = 'center';
												td_rural.appendChild( input_rural );
											
											var td_urbano = tr.insertCell( tr.cells.length );
												td_urbano.style.width = '106px';
												td_urbano.style.textAlign = 'center';
												td_urbano.appendChild( input_urbano );
											
											var td_total = tr.insertCell( tr.cells.length );
												td_total.style.width = '100px';
												td_total.style.textAlign = 'right';
												td_total.appendChild( span_total );
											
											select_benef.focus();
											
											ben_atualizarValores();
										}
										
										function ben_atualizarValores()
										{
											var campo_rural      = null;
											var campo_urbano     = null;
											var campo_total_item = null;
											var rural            = 0;
											var urbano           = 0;
											var total_item       = 0;
											var total_rural      = 0;
											var total_urbano     = 0;
											var total_total      = 0;
											for ( var i = 0; i < ben_nextid; i++ )
											{
												campo_rural      = document.getElementById( 'ben_rural_' + i );
												campo_urbano     = document.getElementById( 'ben_urbano_' + i );
												campo_total_item = document.getElementById( 'ben_total_' + i );
												if ( !campo_rural || !campo_urbano || !campo_total_item )
												{
													continue;
												}
												rural                      = campo_rural.value - 0;
												urbano                     = campo_urbano.value - 0;
												total_item                 = rural + urbano;
												campo_total_item.innerHTML = total_item;
												total_rural                += rural;
												total_urbano               += urbano;
											}
											total_total = total_rural + total_urbano;
											document.getElementById( 'ben_total_zonarural' ).innerHTML  = total_rural;
											document.getElementById( 'ben_total_zonaurbana' ).innerHTML = total_urbano;
											document.getElementById( 'ben_total' ).innerHTML            = total_total;
										}
										
										function ben_excluirLinha( tr )
										{
											if ( tr )
											{
												if ( !confirm( 'Deseja realmente excluir o beneficiário?' ) )
												{
													return;
												}
												tr.parentNode.removeChild( tr );
												com_atualizarValores();
											}
										}
										
										<?php foreach ( $ben_sbi as $item ) : ?>
											ben_Adicionar( '<?= $item["benid"] ?>', '<?= $item["vlrrural"] ?>', '<?= $item["vlrurbano"] ?>' );
										<?php endforeach; ?>
										
									</script>
									
								</td>
							</tr>
							
							<!--
							<tr style="background-color: #cccccc">
								<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
								<td>
									<?php if ( $habil == 'S' ): ?>
										<?php if ( $sbaid_anterior ): ?>
											<input
												type="button"
												name="botao"
												value="Salvar e Anterior"
												onclick="enviar( '<?= $sbaid_anterior ?>' );"
											/>
										<?php else: ?>
											<input
												type="button"
												name="botao"
												value="Salvar e Anterior"
												disabled="disabled"
											/>
										<?php endif; ?>
										&nbsp;
										<input
											type="button"
											name="botao"
											value="Salvar"
											onclick="enviar( '' );"
										/>
										&nbsp;
										<?php if ( $sbaid_proximo ): ?>
											<input
												type="button"
												name="botao"
												value="Salvar e Próximo"
												onclick="enviar( '<?= $sbaid_proximo ?>' );"
											/>
										<?php else: ?>
											<input
												type="button"
												name="botao"
												value="Salvar e Próximo"
												disabled="disabled"
											/>
										<?php endif; ?>
									<?php endif; ?>
									&nbsp;
									<?php if ( $sbaid_anterior ): ?>
										<input
											type="button"
											name="Anterior"
											value="Anterior"
											onclick="irParaSubacao( '<?= $sbaid_anterior ?>' )"
										/>
									<?php else: ?>
										<input
											type="button"
											name="Anterior"
											value="Anterior"
											disabled="disabled"
										/>
									<?php endif; ?>
									&nbsp;
									<?php if ( $sbaid_proximo ): ?>
										<input
											type="button"
											name="Próximo"
											value="Próximo"
											onclick="irParaSubacao( '<?= $sbaid_proximo ?>' )"
										/>
									<?php else: ?>
										<input
											type="button"
											name="Próximo"
											value="Próximo"
											disabled="disabled"
										/>
									<?php endif; ?>
									&nbsp;
									<?php if ( $podeExcluirSubacao ): ?>
										<input
											type="button"
											name="Excluir"
											value="Excluir"
											onclick="excluirSubacao( '<?= $sbaid ?>' );"
										/>
									<?php else: ?>
										<input
											type="button"
											name="Excluir"
											value="Excluir"
											disabled="disabled"
										/>
									<?php endif; ?>
								</td>
							</tr>
							-->
						</tbody>
					</table>
				</form>
			</td>
		</tr>
	</tbody>
</table>

<?php if( !empty($frmid) && ( $frmid == 1 ) ):?>
	<script type="text/javascript">
		
		document.getElementById( "trValores" ).style.display = 'none';
		
		document.getElementById( "sbaunt" ).value = '';
		document.getElementById( "sbauntdsc" ).value = '';
		document.getElementById( "sbavlT" ).value = '';
		
		for ( i = 1; i <= 4; i++ )
		{
			document.getElementById( "sbavl" + i ).value = '';
		}
		
	</script>
<?php endif; ?>

<script type="text/javascript">
	
	function exibeCampo()
	{		
		if( document.getElementById("prgid").value == '<?= $prgid_outros; ?>' )
		{			
			document.getElementById("tr_programa").style.display = 'table-row';
		}
		else
		{
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
			}
		}
		
		document.getElementById("sbavlT").value = mascaraglobal( '###.###.###.###.###.###.###.###.###,##', ( vlt + '' ).replace( /[\.,]/g , "" ) );
		return true;
	}
	
	function somaQuantidades()
	{
		var st = 0;
		var s = new Array(); 
		var vu = ( document.getElementById("sbaunt").value + '' ).replace( /[\.,]/g , "" );
		
		//s[0] = document.getElementById("sba0ano").value;
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
		atualizarValorUnitario();
		if( !isNaN( parseInt( vu ) ) )
		{
//			calculaValoresAnuais();
		}
	}
	
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
		
		atualizarBloqueioCamposQuantitativo();
	}
	
	function verifica_data( stringData )
	{ 
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
	
	function maior_data( menor , maior )
	{
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
   
	function enviar( irPara )
	{
		var objSbadsc   = document.getElementById("sbadsc");
		var objSbastgmpl   = document.getElementById("sbastgmpl");
		var objSbaprm   = document.getElementById("sbaprm");
		var objUndid   = document.getElementById("undid");
		var objFrmid   = document.getElementById("frmid");
		var objSbapcr   = document.getElementById("sbapcr");
		var objSbaunt   = document.getElementById("sbaunt");
		var objSbauntdsc   = document.getElementById("sbauntdsc");
		//var objSba0ano   = document.getElementById("sba0ano");
		var objSba1ano   = document.getElementById("sba1ano");
		var objSba2ano   = document.getElementById("sba2ano");
		var objSba3ano   = document.getElementById("sba3ano");
		var objSba4ano   = document.getElementById("sba4ano");
		//var objSba0ini   = document.getElementById("sba0ini");
		var objSba1ini   = document.getElementById("sba1ini");
		var objSba2ini   = document.getElementById("sba2ini");
		var objSba3ini   = document.getElementById("sba3ini");
		var objSba4ini   = document.getElementById("sba4ini");
		//var objSba0fim   = document.getElementById("sba0fim");
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
			msg += "Informe a Meta/Objetivo\n";
			objValida = false;
		}
		
		if(objFrmid.value==""){
			msg += "Informe a Forma de execução\n";
			objValida = false;
		}
		
		switch(true)
		{
			//case( trim( objSba0ano.value ) != ""  && trim( objSba0fim.value ) != "" && trim( objSba0ini.value ) != ""  ):
			case ( trim( objSba1ano.value ) != "0" || trim( objSba1ano.value ) != "" ) || ( trim( objSba1ano.value ) != "" && trim( objSba1fim.value ) != "" && trim( objSba1ini.value ) != ""  ):
			case ( trim( objSba2ano.value ) != "0" || trim( objSba2ano.value ) != "" ) || ( trim( objSba2ano.value ) != "" && trim( objSba2fim.value ) != "" && trim( objSba2ini.value ) != "" ):
			case ( trim( objSba3ano.value ) != "0" || trim( objSba3ano.value ) != "" ) || ( trim( objSba3ano.value ) != "" && trim( objSba3fim.value ) != "" && trim( objSba3ini.value ) != "" ):
			case ( trim( objSba4ano.value ) != "0" || trim( objSba4ano.value ) != "" ) || ( trim( objSba4ano.value ) != "" && trim( objSba4fim.value ) != "" && trim( objSba4ini.value ) != "" ):
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
		
		// verifica valores das escolas e da composição
		var global = document.getElementById( 'sbaporescolaGlobal' ).checked;
		if ( !global )
		{
			// pega valor do global
			var totalQuantidade = esc_pegarValorTotal() - 0;
		}
		else
		{
			// pega valor das escolas
			var totalQuantidade = document.getElementById( 'sbatotal' ).value - 0;
		}
		// pega total do cronograma
		var totalComposicao = com_pegarValorTotal() - 0;
		if ( totalComposicao > totalQuantidade )
		{
			alert( 'O total distribuído na composição é maior que o detalhado no cronograma.' );
			return;
		}
		
		document.getElementById( 'irpara' ).value = irPara;
		
		if( !objValida )
		{
			alert( msg );
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
	   var janela = window.open( URL, "janelaProposta", "width=400,height=300,scrollbars=yes" );
	   janela.focus(); 
	}
	
	
	somaQuantidades();
	alterarCronograma();
	atualizarPlanoInterno();
	atualizarBloqueioCamposQuantitativo();
	
	<?php if ( $sbaunt && count( $composicao ) == 0 ) : ?>
		// verifica se existe valor unitário do banco
		// se tiver e não existir item de composicao, é inserido um item para que o valor seja válido
		var fake_total = textoParaValor( document.getElementById( 'sbatotal' ).value );
		if ( fake_total )
		{
			var fake_unitario = fake_total / <?= (float) $sbaunt ?>;
			adicionarItem( 1, 'Item de Composição', null, '1', fake_unitario );
		}
	<?php endif;?>
	
	atualizarValorUnitario();
	somaQuantidades();
	alterarCronograma();
	atualizarPlanoInterno();
	atualizarBloqueioCamposQuantitativo();
	
	
</script>
<?php if( isset( $mensagem ) ):?>
	<script type="text/javascript">
		alert("<?php echo $mensagem; ?>");
	</script>
<?php endif; ?>




</body>
</html>