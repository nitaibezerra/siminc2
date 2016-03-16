<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

ini_set("memory_limit", "1024M");


// Cria o cookie de um 12h para segurança de sessao (1:dia * 12:hora * 3600)
setcookie('SESSAOPAINEL', 'OKLOGADO', (time() + (1 * 12 * 3600)));


if ($_REQUEST['ordidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaCelula($_POST['ordidAjax']);
	exit;
}

if($_REQUEST['requisicaoAjax'] == 'mudaResponsavelDemandas' ){
	$usucpfexecutor = $_REQUEST['cpfdestino'];
	$dmdid = $_REQUEST['demanda'];

	$sql = "UPDATE
			         demandas.demanda
			        SET
			         usucpfexecutor			   = '".$usucpfexecutor."'
			        WHERE
			         dmdid = ".$dmdid;
	if($db->executar($sql)){
		$db->commit();
		echo 'Responsável pela demanda foi atualizada com sucesso.';
	}
	exit;
}

if ($_GET['dmdidver']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdidver'];
	header( "Location: demandas.php?modulo=principal/dadosDemanda&acao=A" );
	exit();
}

//Ajax pausa
if ($_REQUEST['dmdidPausaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaPausa($_REQUEST['dmdidPausaAjax']);
	exit;
}

//Ajax Dados Demanda SuperTitle
if ($_REQUEST['dadosDemandaSuperTile']) {
	header('content-type: text/html; charset=ISO-8859-1');
	dadosDemandaSuperTile($_GET['dmdid']);
	exit;
}



function montaCelula($ordid){
	global $db;
 	
	$sql = "SELECT
			 c.celid AS codigo,
			 c.celnome as DESCRICAO
			FROM
			 demandas.celula c
			WHERE
			 c.ordid = ".$ordid." AND
			 c.celstatus = 'A'
			ORDER BY
			 c.celnome;";
	$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', '', '','','','','celid');
}

function dadosDemandaSuperTile($dmdid)
{
	global $db;
	$sql = "select
				usu.usunome,
				dmddsc,
				to_char(dmddatainiprevatendimento,'DD:MM:YYYY HH:MI:SS') as dt1,
				CASE WHEN dmddatafimprevatendimento is not null THEN
					(CASE WHEN to_char(dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
			 			THEN '<font title=\"Demanda com vencimento hoje!\">' || to_char(dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font> - Demanda com vencimento hoje!' 
			 			ELSE 
							(CASE WHEN dmddatafimprevatendimento < CURRENT_DATE THEN 
										'<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font> - Demanda em atraso!' 
					 			  ELSE 
					 			  		'<font color=\"green\" title=\"Demanda em dia!\">' || to_char(dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font> - ' || CASE WHEN position( 'day' in (dmddatafimprevatendimento - now())::text ) > 0 THEN 'Falta(m) ' || substring( (dmddatafimprevatendimento - now() )::text ,0, position( 'day' in (dmddatafimprevatendimento - now())::text ) ) || 'dia(s).' ELSE 'Falta(m) ' || to_char(dmddatafimprevatendimento - now(), 'HH24:MI:SS') || '.' END 
					 		END)
					END)
					 ELSE
					 	''				 		
			 		END as dt2
			from
				demandas.demanda dmd
			inner join
				seguranca.usuario usu ON usu.usucpf = dmd.usucpfdemandante
			where
				dmdid = $dmdid";
	$arrCab = array("Solicitante","Descrição","Data Inicial","Data Final");
	$arrDados = $db->pegaLinha($sql);
	$n = 0;
	if($arrDados){
		foreach($arrDados as $dado){
			if((strlen($dado) > 150)){
				$nova_string = substr($dado,0,150); //pega só os primeiros 200 caracteres
				$nova_string = strrev($nova_string); //inverte a string
				$pos_ini = strpos($nova_string," ");  //pega o uútimo espaço em branco
				if($pos_ini){
					$pos_fim = strlen($dado);
					$nova_string = substr($nova_string,$pos_ini,$pos_fim);
					$dado = strrev($nova_string);
				}
				$dado.= "...";
			}		
			echo "<b>{$arrCab[$n]}</b>: ".$dado."<br />";
			$n++;
		}
	}else{
		echo "<b>Não existem informações.</b>";
	}
}



function montaPausa($dmdid){
	global $db;
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	$classdata = new Data;

	
				//verifica pausa da demanda
				$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
						from demandas.pausademanda p 
						inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
						where p.dmdid = ". (int) $dmdid;
						
				$dadosp = $db->carregar($sql);	
				
				$flagIndeterminado = '';
				$tempototalpausa = 0;
				$textotempopausa = "<div align='left'>";
				
				if($dadosp){
					foreach($dadosp as $dadop){
						
						if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){
							
							$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
							$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
							$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
							$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
							$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
				
							$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
							$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
							$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
							$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
							$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);
							
							$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
							$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final
							
							// pega o tempo total da pausa
							$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);
							
							
							$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
							$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';
							
						}
	
						//monta o texto da tempopausa
						$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
						$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
						$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
						if($dadop['datapausafim']){
							$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
						}else{
							$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
						}
						
						if($dadop['pdmdatafimpausa']){
							$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
							if(!$tempop) $tempop = '0 minuto';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
						}else{
							$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
						}
						
						$textotempopausa .= "<BR><BR>";
						
					}
					
					
					
					//if($flagIndeterminado == '1')
					//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
					//else{
						$datainiaux = date('Y-m-d H:i').':00';
						$ano_aux	= substr($datainiaux,0,4);
						$mes_aux	= substr($datainiaux,5,2);
						$dia_aux	= substr($datainiaux,8,2);
						$hor_aux	= substr($datainiaux,11,2);
						$min_aux	= substr($datainiaux,14,2);
						
						$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
						$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
						$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
					//}
					
				}
				
				
				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
		        $intHoras 		= floor($horas);
		        if($intHoras >= 1){	//se houver horas
		            $horasx = $intHoras;
		            $resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
		        }
		                    
		        $minutos 		= $resto/60; //quantidade de minutos
		        $intMinutos 	= floor($minutos);
		        if($intMinutos >= 1){ //se houver minutos
		            $minutosx = $intMinutos;
		            $resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
		        }				
		        
	            if(!$horasx) $horasx = "00";
	            if(strlen($horasx) == 1) $horasx = "0".$horasx;
	            if(!$minutosx) $minutosx = "00";
	            if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
	            
	            $hormin = $horasx.":".$minutosx;
	            
	            
				//pega prioridade e data termino e data conclusão
				$sql = "select dmdhorarioatendimento,
							   to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimento,
							   --datahist as dataconc
							   (select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid
								 	where aedid in (146, 191) 
								 	and c.esdid in (93,95,109,111)
								 	and a.docid = d.docid
								) as dataconc
						from demandas.demanda d
						/*
						LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
						*/						
						where d.dmdid = ". (int) $dmdid;
				$dadosdmd = $db->carregar($sql);	
				
				//pega o ordid para passar como parametro
				$sql = "SELECT	t.ordid FROM demandas.demanda d
						LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid	 			
				 		WHERE d.dmdid=".(int) $dmdid;
				$ordid = $db->PegaUm($sql);				
	            
				
				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimento'], $hormin, $dadosdmd[0]['dmdhorarioatendimento'], $dadosdmd[0]['dataconc'], $ordid);
				
				
				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
					//$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$horasx.":".$minutosx."</font>";
				}
				
				$textotempopausa .= "</div>"; 
	 
				echo $textotempopausa;
	exit;
}


function dscMes($mes){
	
	$mes = (int) $mes;
	
	switch ($mes) {
    case 1:
        echo "JANEIRO";
        break;
    case 2:
        echo "FEVEREIRO";
        break;
    case 3:
        echo "MARÇO";
        break;
    case 4:
        echo "ABRIL";
        break;
    case 5:
        echo "MAIO";
        break;
    case 6:
        echo "JUNHO";
        break;
    case 7:
        echo "JULHO";
        break;
    case 8:
        echo "AGOSTO";
        break;
    case 9:
        echo "SETEMBRO";
        break;
    case 10:
        echo "OUTUBRO";
        break;
    case 11:
        echo "NOVEMBRO";
        break;
    case 12:
        echo "DEZEMBRO";
        break;
	}    
}


if(!$_REQUEST['modopopup']){
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Painel Gerencial', 'Visão geral das demandas em aberto por técnico' );
}


$codigo = $_REQUEST['codigo'];
$palavra_chave = $_REQUEST['palavra_chave'];
$mesini = $_REQUEST['mesini'];
$mesfim = $_REQUEST['mesfim'];
$ano = $_REQUEST['ano'];

/*
if(!$mesini || !$mesfim || !$ano){
	
	$mesfim = date('m');
	$mes = date('m');
	if(strlen($mes) == 1) $mes = "0".$mes;
	
	$ano = date('Y');
	
	if($mes == '01'){
		$mesini = "12";
		$anoini = $ano - 1;
	}
	else{
		$mesini = date('m')-1;
		if(strlen($mesini) == 1) $mesini = "0".$mesini;
		$anoini = $ano;
	}
}
*/

 
	

$celidx = $db->pegaUm("SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus='A' and celid is not null and usucpf = '".$_SESSION['usucpf']."'");
$celid = $_REQUEST['celid'] ? $_REQUEST['celid'] : $celidx;

if($celid){
	$ordid = $db->pegaUm(" SELECT ordid from demandas.celula where celstatus='A' and celid = ".$celid);
	$ordid = $_REQUEST['ordid'] ? $_REQUEST['ordid'] : $ordid;
}
else{
	$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and not ordid is null");
	$ordid  = $_REQUEST['ordid'] ? $_REQUEST['ordid'] : $ordidx[0];
}

$displayCel = 'none';
$displayCargo = 'none';

if($ordid == 1){
	$displayCel = '';
	$displayCargo = '';
}
else if($ordid == 2 || $ordid == 23 || $ordid == 24){
	$displayCel = '';
	$displayCargo = 'none';
}
/*
else if($ordid == 2 || $ordid == 13 || $ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21){
	$displayCel = '';
	$displayCargo = 'none';
}
*/

?>

<?php if($_REQUEST['modopopup']){?>
 <style type="text/css">
 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
 .linha1{background-color:#f5f5f5;}
 .linha2{background-color:#fdfdfd;}
 </style>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

 <script type="text/javascript" src="../includes/funcoes.js"></script>
 <script type="text/javascript" src="../includes/remedial.js"></script>
 <script type="text/javascript" src="../includes/superTitle.js"></script>
 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
 
 <link rel="stylesheet" href="/includes/LyteBox/lytebox.css" type="text/css" media="screen" />
 <script type="text/javascript" src="/includes/LyteBox/lytebox.js" ></script>
<?php }?>
 <!-- 
 <script type="text/javascript" src="../includes/prototype.js"></script>
  -->
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
 <script type="text/javascript" src="../includes/remedial.js"></script>
 <script type="text/javascript" src="../includes/superTitle.js"></script>
 

 <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
 <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
  <script>jQuery.noConflict();</script>
 <link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css">
 <script src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.core.js"></script>
 <script src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.widget.js"></script>
 <script src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.mouse.js"></script>
 <script src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.sortable.js"></script>
 <link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/demos/demos.css">

<form action="" method="POST" name="formulario">
		
	<input type="hidden" name="submete" value="1">
	<input type="hidden" name="modopopup" value="<?=$_REQUEST['modopopup']?>">
		
	<?php if(!$_REQUEST['modopopup']){?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="width:39.5%;">Código:</td>
				<td>
					<?= campo_texto( 'codigo', 'N', 'S', '', 10, 15, '##########', '', '','','','','buscaCodigo(event)' ); ?>
				</td>
			</tr>	
			<tr>
				<td class="SubTituloDireita" >Assunto / Descrição:</td>
				<td>
					<?= campo_texto( 'palavra_chave', 'N', 'S', '', 40, 40, '', '', '', '', '', 'id="palavra_chave"', '', ''); ?>
				</td>
			</tr>	
			<tr>
				<td class="SubTituloDireita" style="width:39.5%;">Origem:</td>
				<td>
					<? 
						if(!$ordidx) $ordidx= $db->carregarColuna(" SELECT ordid from demandas.origemdemanda where ordstatus='A'");
						
						$sqlord = "SELECT
								 ordid AS codigo,
								 orddescricao AS descricao
								FROM
								 demandas.origemdemanda
								WHERE
								 ordstatus = 'A'
								 and ordid in (".implode(',',$ordidx).")
								ORDER BY
								 orddescricao;";
						
						$db->monta_combo( "ordid", $sqlord, 'S', '-- Informe a origem --', 'filtraCelula', '','','','','ordid');
					?>					
				</td>
			</tr>
			<tr id="tr_celula" style="display:<?=$displayCel?>;">
				<td class="SubTituloDireita" >Célula:</td>
				<td id="td_celula">
					<? 

					if(!$db->testa_superuser()){
						$sql = "select 
										ur.celid 
								from demandas.usuarioresponsabilidade ur
								where rpustatus = 'A' 
								and usucpf = '".$_SESSION['usucpf']."'
								and ur.celid is not null
								and pflcod not in (235) --(235 = perfil demandante) 
								";
						$dadosCel = $db->carregarColuna($sql);
						
						if($dadosCel) $andCel = " AND ur.celid in (".implode(",", $dadosCel).")";
					}
					
					if($ordid){
						$sql = "select celid  as codigo, 
									   celnome as descricao
								   FROM demandas.celula as ur
								   WHERE
								   		--celid in (2,7) AND
								   		ordid in ($ordid) AND
								   		celstatus = 'A'
								   		$andCel
								   	order by 2";
					}
					
					//$celid = $_REQUEST["celid"] ? $_REQUEST["celid"] : $dadosCel[0];	
					//$db->monta_combo( "celid", $sql, 'S', '--', '', '','','','','celid');
					$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', '', '','','','','celid');
	
				?>
				</td>
			</tr>	
			<tr id="tr_cargo" style="display:<?=$displayCargo?>;">
				<td class="SubTituloDireita" >Cargo/Função:</td>
				<td>
					<? 
					$sql = "select pflcod as codigo, 
									  pfldsc as descricao
							   FROM seguranca.perfil
							   WHERE
							   		sisid=".$_SESSION['sisid']." AND
							   		pflcod in (237,238)";
					$funcao = $_POST["funcao"];	
					$db->monta_combo( "funcao", $sql, 'S', '--', '', '','','','','funcao');
	
				?>
				</td>
			</tr>	
			<tr>
				<td class="SubTituloDireita">Período:</td>
				<td>
					<?php 
						if($ano == '2010'){
							$sql = "select '06' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '07' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '08' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '09' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";
						}
						else{
							$sql = "select '01' as codigo, 'Janeiro' as descricao
	  	   						union all
	  	   						select '02' as codigo, 'Fevereiro' as descricao
	  	   						union all
	  	   						select '03' as codigo, 'Março' as descricao
	  	   						union all
	  	   						select '04' as codigo, 'Abril' as descricao
	  	   						union all
	  	   						select '05' as codigo, 'Maio' as descricao
	  	   						union all
	  	   						select '06' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '07' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '08' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '09' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";
						}
						$db->monta_combo( 'mesini', $sql, 'S', '--','', '' );									
					?>
					&nbsp;&nbsp;
					até
					&nbsp;&nbsp;
					<?php 
						if($ano == '2010'){
							$sql = "select '06' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '07' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '08' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '09' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";
						}
						else{
							$sql = "select '01' as codigo, 'Janeiro' as descricao
	  	   						union all
	  	   						select '02' as codigo, 'Fevereiro' as descricao
	  	   						union all
	  	   						select '03' as codigo, 'Março' as descricao
	  	   						union all
	  	   						select '04' as codigo, 'Abril' as descricao
	  	   						union all
	  	   						select '05' as codigo, 'Maio' as descricao
	  	   						union all
	  	   						select '06' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '07' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '08' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '09' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";
						}
					
						$db->monta_combo( 'mesfim', $sql, 'S', '--','', '' );									
					?>		
					
					<?php 
						$anoini = (int) 2010;
						$anofim = (int) date("Y");
						
						for($i=$anoini; $i<=$anofim; $i++){
							$sqlano .= " select '$i' as codigo, '$i' as descricao ";
							if($i != $anofim) $sqlano .= " union ";
						}
						
						$db->monta_combo( 'ano', $sqlano, 'S', '--','', '' );									
					?>
				</td>
			</tr>
			<tr>
				<td bgcolor="#dfdfdf" colspan=2 align="center">
					&nbsp;&nbsp;
					<input style="cursor:pointer;" type="button" onclick="pesquisa();" name="pesquisar" value="Pesquisar"/>
					&nbsp;&nbsp;
					<input style="cursor:pointer;" type="button" onclick="limpa();" name="limpar" value="Limpar"/>
					&nbsp;&nbsp;
					<input style="cursor:pointer;" type="button" onclick="modopop();" name="poupup" value="Modo Popup"/>
				</td>
			</tr>
			<?php if($mesini){?>
			<tr>
				<td colspan=2 align=center>
					<br>
					<FONT COLOR=#00008B><B>PERÍODO: <?=dscMes($mesini)?> ATÉ <?=dscMes($mesfim)?> DE <?=$ano?></B></FONT> 
				</td>
			</tr>
			<?php }?>
			
		</table>
		
	<?}else{?>
		<input type="hidden" name="codigo" value="<?=$_REQUEST['codigo']?>">
		<input type="hidden" name="palavra_chave" value="<?=$_REQUEST['palavra_chave']?>">
		<input type="hidden" name="funcao" value="<?=$_REQUEST['funcao']?>">
		<input type="hidden" name="celid" value="<?=$_REQUEST['celid']?>">
		<input type="hidden" name="mesini" value="<?=$_REQUEST['mesini']?>">
		<input type="hidden" name="mesfim" value="<?=$_REQUEST['mesfim']?>">
		<input type="hidden" name="ano" value="<?=$_REQUEST['ano']?>">
	<?php }?>  		
		<table border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> 
                
                <?
                if($_POST['submete'] == '1'){
                	
                	if($_POST["funcao"]) $andTec = " and ur.pflcod = ".$_POST["funcao"];
                	if($_POST["celid"]){
						$andCel = " and ur.celid = ".$_POST["celid"];
					}else{
						if($andCel){
							$andCel = $andCel;
						}
						else{
							$andCel = " and ur.celid = 2";	
						}
						
					}
					
					if($_POST["ordid"] != '1' && $_POST["ordid"] != '2' && $_POST["ordid"] != '23' && $_POST["ordid"] != '24'){
						$andCel = '';
						$andTec = '';
						if($_POST["ordid"]) $andOrd = " and ur.ordid = ".$_POST["ordid"];
					}
					if($_POST["ordid"] == '2'){
						$andTec = '';
					}
                	
					$sql = "SELECT DISTINCT
						u.usucpf,
						u.usunome
					FROM
						seguranca.usuario AS u
					INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf	
					INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
					WHERE
						ur.rpustatus = 'A' AND
						us.susstatus = 'A' AND
						us.suscod = 'A' 		 
						$andTec
						$andCel
						$andOrd
					ORDER BY u.usunome";				
					//dbg($sql,1);
					$listas = $db->carregar( $sql );
					
					if($listas){
						foreach($listas as $lts){
							$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;
							
							echo ($a%4) == 0 ? "<TR>" : "";
							
							if($lts['usunome']){
								$n = explode(' ', $lts['usunome']);
								$n1 = $n[0];
								$n2 = $n[count($n)-1];
							}
							
							$qtd = totalDemandasPorTec($lts['usucpf']);
							if($qtd == 0) $qtd = "<font color=red>($qtd)</font>";
							else $qtd = "<font color=blue>($qtd)</font>";	
							
							?><td valign='top' align="center" style="width: 133px">
										<table width="100%" class='tabela'> 
											<tr> 
												<th><font color='#00008B'><?=$n1 ." ". $n2 ." ". $qtd?></font></th> 
											</tr> 
											<tr> 
												<td valign='top' align="center" style="width: 133px">
													<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="finalizadas">
														<?=painelGerencia($lts['usucpf'])?>
													</div>
												</td>
											</tr>
										</table>
								   </td>
							<?
							echo ($a+1%4) == 0 ? "</TR>" : "";
							
							$a++;
							
						}
					}
                }
				?>
				
        </table> 
		
	

<style type="text/css">
<?php
	if($listas){ 
		foreach($listas as $lts){
			$usucpf = $lts['usucpf'];
			
?>
	#sortable_<?php echo $usucpf;  ?> { list-style-type: none; margin: 0; padding: 0 0 2.5em; float: left; margin-right: 10px; }
	#sortable_<?php echo $usucpf;  ?> li { margin: 2px 0 0 5px; padding: 0px; font-size: 1.2em; width: 190px; }
<?php } } ?>

	.ui-state-disabled{ color: red; opacity: 100; }
</style>

<script type="text/javascript">
	jQuery(function() {
		<?php if($listas){ 
				foreach($listas as $lts){ 
					$usucpf = $lts['usucpf'];
		?>
		
		jQuery( "#sortable_<?php echo $usucpf;?>" ).sortable({
			connectWith: ".connectedSortable",
			cursor: "move",
			opacity: 0.7,
			items: "li:not(.ui-state-disabled)",
			receive: function(event, ui) 
			{ 	
				var arrID = this.id.split("_");
				var cpfdestino = arrID[1];
				var idDemanda = ui.item.attr('id');
				//console.log(ui.sender.attr('id')); 
				
				jQuery.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=mudaResponsavelDemandas&cpfdestino=" + cpfdestino + "&demanda=" + idDemanda,
					   success: function(resp){
							alert(resp);
							var totalLinhas = (parseInt(ui.sender.find('li').length)-1); 
							if(totalLinhas == 0){
								ui.sender.attr('id')
								var arrIDOrigem = ui.sender.attr('id').split("_");
								jQuery('#semdemanda_' + arrIDOrigem[1]).toggle();
							
							}
							jQuery('#semdemanda_<?php echo $usucpf;?>').hide();
						}
				});


			 }
			 
		}).disableSelection();
		<?php } } ?>
	});
</script>
						
								  
			  
			 	
</form>


<?php

function totalDemandasPorTec($usucpf){
	
	global $db;
	
	/*
	if($_POST['dtinip'] && $_POST['dtfimp']){
		
		$dtinip = formata_data_sql($_POST['dtinip']);
		$dtfimp = formata_data_sql($_POST['dtfimp']);
		
		
		$andperiodo = "
						AND (
						     (
						     to_char(dmddatainiprevatendimento, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
							or 
						     to_char(dmddatafimprevatendimento, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
						     )
						     or 
						     (
						     '$dtinip' BETWEEN to_char(dmddatainiprevatendimento, 'YYYY-MM-DD') and to_char(dmddatafimprevatendimento, 'YYYY-MM-DD')
							or 
						     '$dtfimp' BETWEEN to_char(dmddatainiprevatendimento, 'YYYY-MM-DD') and to_char(dmddatafimprevatendimento, 'YYYY-MM-DD')
						     )
						   )		
					  ";
	}
	*/	
	
	$and = " and doc.esdid in(91,107,92,108) ";
	
	$codigo 		= (isset($_POST["codigo"]) && $_POST["codigo"]!="") ? $_POST["codigo"] : NULL;
	$palavra_chave 	= (isset($_POST["palavra_chave"]) && $_POST["palavra_chave"]!="") ? $_POST["palavra_chave"] : NULL;
	$mesini 		= (isset($_POST["mesini"]) && $_POST["mesini"]!="") ? $_POST["mesini"] : NULL;
	$mesfim 		= (isset($_POST["mesfim"]) && $_POST["mesfim"]!="") ? $_POST["mesfim"] : NULL;
	$ano 			= (isset($_POST["ano"]) && $_POST["ano"]!="") ? $_POST["ano"] : NULL;
	
	
	
	if ($codigo){
		$codigo = str_replace("#","",$codigo);
		$and .= " and d.dmdid = {$codigo}";
	}
	if ($palavra_chave){
		$and .= " and (d.dmdtitulo ILIKE '%{$palavra_chave}%' OR d.dmddsc ILIKE '%{$palavra_chave}%')";
	}	
	
	if ($mesini && $mesfim && $ano){
		$lastday = date('t',strtotime($ano.'-'.$mesfim.'-01'));
		$and .= " and d.dmddatainiprevatendimento between '$ano/$mesini/01 00:00:00' and '$ano/$mesfim/$lastday 23:59:59'";
	}
	
				
	
	$sql = "SELECT 
				count(*) as qtd
			FROM
				demandas.demanda as d
			LEFT JOIN 
				workflow.documento doc ON doc.docid 	  = d.docid
			LEFT JOIN 
				workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE 
				d.usucpfexecutor = '".$usucpf."'
				AND d.usucpfdemandante is not null
				AND d.dmdstatus = 'A'
				AND ed.esdstatus = 'A' 
				AND doc.esdid in (91,92,107,108)
				$and
			";
	//dbg($sql);
	$totalRegistro = $db->PegaUm( $sql );
	
	return $totalRegistro ? $totalRegistro : 0;
											
}



function painelGerencia($usucpf){
	
	global $db;
	
	//$and = " and d.dmdidorigem is null ";
	//$and .= " and (doc.esdid is null or doc.esdid in(91,107,92,108) ) ";
	$and .= " and doc.esdid in(91,107,92,108) ";
	
	$codigo 		= (isset($_POST["codigo"]) && $_POST["codigo"]!="") ? $_POST["codigo"] : NULL;
	$palavra_chave 	= (isset($_POST["palavra_chave"]) && $_POST["palavra_chave"]!="") ? $_POST["palavra_chave"] : NULL;
	$mesini 		= (isset($_POST["mesini"]) && $_POST["mesini"]!="") ? $_POST["mesini"] : NULL;
	$mesfim 		= (isset($_POST["mesfim"]) && $_POST["mesfim"]!="") ? $_POST["mesfim"] : NULL;
	$ano 			= (isset($_POST["ano"]) && $_POST["ano"]!="") ? $_POST["ano"] : NULL;
	
	
	
	if ($codigo){
		$codigo = str_replace("#","",$codigo);
		$and .= " and d.dmdid = {$codigo}";
	}
	if ($palavra_chave){
		$and .= " and (d.dmdtitulo ILIKE '%{$palavra_chave}%' OR d.dmddsc ILIKE '%{$palavra_chave}%')";
	}	
	
	if ($mesini && $mesfim && $ano){
		$lastday = date('t',strtotime($ano.'-'.$mesfim.'-01'));
		$and .= " and d.dmddatainiprevatendimento between '$ano/$mesini/01 00:00:00' and '$ano/$mesfim/$lastday 23:59:59'";
	}
		
	$orderby = "d.dmddatafimprevatendimento,";

	$sql = "

				SELECT
					 DISTINCT
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid)
							 from demandas.pausademanda pp
							 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
							 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
							 where pp.dmdid = d.dmdid) > 0 
						AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) is null 
						OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') )
						THEN
							'<img src=\'../imagens/pause.gif\' style=\'cursor:pointer\'  border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onclick=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelGerencia&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\">'
						ELSE
							''	
					 END) || '<img src=\'../imagens/alterar.gif\' style=\'cursor:pointer\'  border=0  align=\'absmiddle\' onmouseout=\'SuperTitleOff(this);\' onclick=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelGerencia&acao=A&dadosDemandaSuperTile=1&dmdid='|| d.dmdid ||'\',this);\" > <a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/painelGerencia&acao=A&dmdidver=' || d.dmdid || '\' target=\'_blank\' >' || d.dmdtitulo  || '</a>' AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY') AS dataabertura,
					 
					 to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY') AS dataprevisaotermino,
					 
					 to_char(d.dmddatainiprevatendimento, 'DD/MM/YYYY') AS dataprevistainicio,
					 
					d.dmdid,

					d.dmdidorigem,
					
					d.dmddatafimprevatendimento
					 
				FROM
				
					 demandas.demanda d
					 /*
					 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
					 */
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					WHERE 
					 d.dmdstatus = 'A'
					 AND d.usucpfexecutor = '".$usucpf."'	
					 $and	 
					ORDER BY $orderby d.dmdid limit 50";
		
	//dbg($sql,1);
				 
	if($sql) $RS = $db->carregar($sql);

	

?>

		<table width='100%' align='left' border='0' cellspacing='0' cellpadding='2' class='listagem'>
			<tr>
				<td width="50%" align='left' valign='top' class='title' bgcolor='#dfdfdf' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Assunto</strong>
				<td width="25%" align='left' valign='top' class='title' bgcolor='#dfdfdf' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Data Inicio</strong>
				<td width="25%" align='left' valign='top' class='title' bgcolor='#dfdfdf' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;'><strong>Data Fim</strong>
			</tr>
		</table>	
				<? if ( $RS ) : ?>
				
					<?$cor = ''; ?>
					<?$ct = 0; ?>
					<ul id="sortable_<? echo $usucpf; ?>" class="connectedSortable" style="width: 310px;">
						<li id="semdemanda_<?php echo $usucpf;?>" class="ui-state-default ui-state-disabled"  style="background-color: <?php echo $cor; ?>;  width: 100%;" >
							Não existem demandas.
						</li>
					<?foreach ( $RS as $dado ) : 
					
						$cor = $cor == '#E2E2E2' ? '#ffffff' : '#E2E2E2' ;
						
						?>
						
							<li style="background-color: <?php echo $cor; ?>; width: 310px; " id="<? echo $dado['dmdid'];?>" class="ui-state-default" value="<? echo $dado['dmdid'];?>">
								<table width='100%' align='center' border='1' cellspacing='0' cellpadding='0' >
									<tr>
										<td width="50%" style="background-color: <?php echo $cor; ?>;  padding: 2px 2px 2px 5px ; "><?=$dado['assunto']?></td>
										<td width="25%" style="background-color: <?php echo $cor; ?> "><?=$dado['dataprevistainicio'] ?></td>
										<td width="25%" style="background-color: <?php echo $cor; ?> "><?=$dado['dataprevisaotermino'] ?></td>
									</tr>
								</table>
							</li>
						
						
					<?
						echo '<script>jQuery( "#semdemanda_'.$usucpf.'").hide();</script>';
					endforeach;?>
					</ul>
					
				<?php else : ?>
				<ul id="sortable_<? echo $usucpf; ?>" class="connectedSortable" style="width: 310px;">
						<li style="background-color: <?php echo $cor; ?>; width: 310px;" id="semdemanda_<?php echo $usucpf;?>" class="ui-state-default ui-state-disabled">
						Não existem demandas.
						</li>
				</ul>
				<?php endif; ?>
				
				
		

<?php }?>


<?php 
function dmdFilhoSub ($dmdid,$width,$profundidade = null,$cor = null, $usucpf){
	global $db;

	/*
	if($profundidade == 1){
		$sql = "select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdid = $dmdid order by dmdid desc";
		$dadosDemanda = $db->carregar($sql);
		$caminho = $_SERVER ['REQUEST_URI'];
		$caminho = explode("&dmdid=",$caminho);
		$caminho = $caminho[0]."&dmdid={$dadosDemanda[0]['dmdid']}";
		($_SESSION['dmdid'] == $dadosDemanda[0]['dmdid'])? $cor = "font-weight:bold" : $cor="";
		$tr_filhos .= "<div style=\"text-align: left;background: rgb(238, 238, 238);$cor\" ><a href=\"$caminho\"> Cód. # {$dadosDemanda[0]['dmdid']} -  {$dadosDemanda[0]['dmdtitulo']}</a></div>";
	}
	*/
	($profundidade)? $profundidade++ : $profundidade = $profundidade;
	//$sql="select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdidorigem = $dmdid order by dmdid desc";
	
	$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelGerencia&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid)
							 from demandas.pausademanda pp
							 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
							 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
							 where pp.dmdid = d.dmdid) > 0 
						AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) is null 
						OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') )
						THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onclick=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelGerencia&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END) || '<img src=\'../imagens/alterar.gif\' style=\'cursor:pointer\'  border=0  align=\'absmiddle\' onmouseout=\'SuperTitleOff(this);\' onclick=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelGerencia&acao=A&dadosDemandaSuperTile=1&dmdid='|| d.dmdid ||'\',this);\" > <a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/painelGerencia&acao=A&dmdidver=' || d.dmdid || '\'>' || d.dmdtitulo  || '</a>' AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS dataabertura,
					 
					 to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI') AS dataprevisaotermino,
					 
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 /*
					 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
					 */
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					WHERE 
					 d.dmdstatus = 'A'	
					 and d.dmdidorigem = $dmdid
					 AND d.usucpfexecutor = '".$usucpf."'		 
					ORDER BY d.dmdid ";
	
	//dbg($sql,1);
	$filhos = $db->carregar($sql);
	if($filhos){
		
		$nivel = 1;
		
		foreach($filhos AS $fl){

			$arvore = "1.$x$nivel";
			//$caminho = $_SERVER ['REQUEST_URI'];
			//$caminho = explode("&dmdid=",$caminho);
			//$caminho = $caminho[0]."&dmdid={$fl['dmdid']}";
			//($_SESSION['dmdid'] == $fl['dmdid'])? $cor = "font-weight:bold" : $cor="";
			//$tr_filhos .= ("<div style=\"text-align: left;background: rgb(238, 238, 238);padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\" ><img src='../imagens/seta_filho.gif' ><a href=\"$caminho\" > Cód. # {$fl['dmdid']} - {$fl['dmdtitulo']}</a></div>");
			
			//$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
			//$cor = '#ffffcc';
			$tr_filhos .= "
						<tr bgcolor=".$cor." onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
							<td nowrap><div style=\"text-align: left;padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\"><img src='../imagens/seta_filho.gif'>{$fl['id']}</div></td>
							<td >".$fl['assunto']."</td>
							<td >".$fl['dataprevisaotermino']."</td>
						</tr>
			";
			
			$filho = dmdFilhoSub($fl['dmdid'],$width,$profundidade,$cor,$usucpf);
			($filho)? $profundidade = $profundidade : "" ;
			$tr_filhos .= $filho;
			$nivel++;
		}

	}
	
	return $tr_filhos;
}
?>

<script type="text/javascript">

	// Define delay do tooltip.
	SetglobalIntFadeSpeed(100);

	//varial global
	var d = document;

	function pesquisa() {
		<?//if($db->testa_superuser()){?>
			if(document.formulario.ordid.value == ''){
				alert("O campo Origem é obrigatório.");
				document.formulario.ordid.focus();
				return false;
			}
			var ordid = document.formulario.ordid.value;
			//if(ordid==1||ordid==2||ordid==13||ordid==18||ordid==19||ordid==20||ordid==21){
			if(ordid==1||ordid==2||ordid==23||ordid==24){
				if(document.formulario.celid.value == ''){
					alert("O campo Célula é obrigatório.");
					document.formulario.celid.focus();
					return false;
				}
			}
		<?//}?>
		document.formulario.target = '';
		document.formulario.modopopup.value='';
		document.formulario.action = 'demandas.php?modulo=principal/painelGerencia&acao=A';
		document.formulario.submit();
	}

	function limpa() {
		/*
		document.formulario.target = '';
		document.formulario.modopopup.value='';
		document.formulario.codigo.value = '';
		document.formulario.palavra_chave.value = '';
		document.formulario.funcao.value = '';
		document.formulario.celid.value = '';
		document.formulario.mesini.value = '';
		document.formulario.mesfim.value = '';
		document.formulario.ano.value = '';
		document.formulario.submit();
		*/
		location.href='demandas.php?modulo=principal/painelGerencia&acao=A';
	}	

	function modopop(){
		<?//if($db->testa_superuser()){?>
			if(document.formulario.ordid.value == ''){
				alert("O campo Origem é obrigatório.");
				document.formulario.ordid.focus();
				return false;
			}
			var ordid = document.formulario.ordid.value;
			//if(ordid==1||ordid==2||ordid==13||ordid==18||ordid==19||ordid==20||ordid==21){
			if(ordid==1||ordid==2||ordid==23||ordid==24){
				if(document.formulario.celid.value == ''){
					alert("O campo Célula é obrigatório.");
					document.formulario.celid.focus();
					return false;
				}
			}
		<?//}?>
		document.formulario.modopopup.value='1';
		//document.formulario.action = 'demandas.php?modulo=principal/painelGerencia&acao=A';
		if(document.formulario.ordid.value == 1 && document.formulario.celid.value == 2){
			document.formulario.action = 'popPainelGerencia.php';
		}
		else if(document.formulario.ordid.value == 23 && document.formulario.celid.value == 49){
			document.formulario.action = 'popPainelGerenciaGabinete.php';
		}
		else{
			document.formulario.action = 'popPainelGerencia2.php';
		}
		var janela = window.open( '', 'modopop', 'width=780,height=465,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		document.formulario.target = 'modopop';
		document.formulario.submit();
		
		//var janela = window.open( 'demandas.php?modulo=principal/painelGerencia&acao=A', 'painel', 'width=780,height=465,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
		//janela.focus();
	}



	function buscarAval(dmdid){
	    inicialytebox('popCadAvaliacao.php?codseg=simecok&dmdid=' + dmdid, dmdid);
	}
	
	function inicialytebox(url, dmdid) {
	    var anchor = this.document.createElement('a'); // cria um elemento de link
	    anchor.setAttribute('rev', 'width: 800px; height: 600px; scrolling: auto;');
	    anchor.setAttribute('title', '');
	    anchor.setAttribute('href', url);
	    anchor.setAttribute('rel', 'lyteframe');
	    // paramentros para do lytebox //
	    myLytebox.maxOpacity = 50;
	    myLytebox.outerBorder = true;
	    myLytebox.doAnimations = false;
	    myLytebox.start(anchor, false, true);
		/*
	    document.getElementById('lbClose').onclick = function() { 
	        alert(dmdid); window.location.reload();
	        //document.getElementById('div'+dmdid).innerHTML = "<font color=blue><b>SIM</b></font>";
	        myLytebox.end(); 
	        return false; 
	    }
	    */
	    return false;
	}

	
	function buscaCodigo(e){

		var key; 
		if(e && e.which){
			key = e.which;
		}
		else if(window.event){
			e = window.event;
			key = e.keyCode;
		}

		if(key == 13 && document.formulario.codigo.value!='') document.formulario.submit();
	}	
	
	function filtraCelula(ordid) {
		
		trCel    = d.getElementById('tr_celula');
		tdCel  	 = d.getElementById('td_celula');
		trCargo  = d.getElementById('tr_cargo');
		
		trCel.style.display = 'none';
		tdCel.style.display = 'none';
		trCargo.style.display = 'none';
		
		if(ordid == 1){ //Sistemas de Informação
			trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
			tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
			trCargo.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		else if(ordid==2 || ordid==23 || ordid==24){ 
			trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
			tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		/*
		else if(ordid==2||ordid==13||ordid==18||ordid==19||ordid==20||ordid==21){ 
			trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
			tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		*/
		
		//if(ordid==1||ordid==2||ordid==13||ordid==18||ordid==19||ordid==20||ordid==21){
		if(ordid==1||ordid==2||ordid==23||ordid==24){
			// Faz uma requisição ajax, passando o parametro 'ordid', via POST
			jQuery.ajax({
						   type: "POST",
						   url: "demandas.php?modulo=principal/painelGerencia&acao=A",
						   data: "ordidAjax=" + ordid,
						   success: function(resp){
								tdCel.innerHTML = resp;
							}
					});
		}
	}

	<?php if($_POST['modopopup'] != ''){?>
		window.onload = function () {
			setTimeout('document.formulario.submit();', 5*60*1000);
		}
	<?php }?>
	
	

</script>