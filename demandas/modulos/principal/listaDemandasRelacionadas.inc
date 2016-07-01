<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past


if ($_GET['dmdid']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdid'];
	header( "Location: demandas.php?modulo=principal/dadosDemanda&acao=A" );
	exit();
}

$dmdid = $_SESSION['dmdid'];

extract($_POST);



//Ajax pausa
if ($_REQUEST['dmdidPausaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaPausa($_REQUEST['dmdidPausaAjax']);
	exit;
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

if ($dataini){
	$dataini = explode("/",$dataini);
	$dataini = "{$dataini[2]}/{$dataini[1]}/{$dataini[0]}"; 
}

if ($datafim){
	$datafim = explode("/",$datafim);
	$datafim = "{$datafim[2]}/{$datafim[1]}/{$datafim[0]}"; 
}

	//Ajax
if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSubLista($_POST['dmdidAjax']);
	exit;
}	
	
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';

/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

#include APPRAIZ . 'includes/Agrupador.php';
//print '<br/>';
monta_titulo( 'Demandas Relacionadas - Cód. # '.$dmdid, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
<?php 
    $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
    if( in_array($estadoAtualDaDemanda['esdid'], array(
    DEMANDA_ESTADO_EM_ANALISE
    ,DEMANDA_ESTADO_EM_ATENDIMENTO
    ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
    ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
    ) ) ) { ?>
        jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
<?php } ?>
});
</script>
<html>
	<head>
			 <style type="text/css">
			 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
			 .linha1{background-color:#f5f5f5;}
			 .linha2{background-color:#fdfdfd;}
			 </style>
			 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			 <script src="../includes/calendario.js"></script>
			 <script src="./js/ajax.js" type="text/javascript"></script>
			 <script src="./js/demandas.js" type="text/javascript"></script>
			 <script type="text/javascript" src="../includes/prototype.js"></script>
			 <script type="text/javascript" src="../includes/funcoes.js"></script>
			 <script type="text/javascript" src="../includes/remedial.js"></script>
			 <script type="text/javascript" src="../includes/superTitle.js"></script>
			 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
	</head>
<body>
<form action="" method="POST" name="formulario">

		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr >
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; "  >
					<b>Cód</b>		 					
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Prioridade</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Assunto</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Origem Demanda</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Situação</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Usuário</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Responsável</b>
				</td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
					<b>Data Abertura</b>
				</td>
			</tr>

			<?
			$sql = lista($dmdid, 1); 
			$listas = $db->carregar( $sql['sql'] );
			$listas = $listas ? $listas : array();
			foreach($listas as $lts){
				$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;
			?>
			 	<tr bgcolor="<?=$cor?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
			 		<td style='width:2.3%'>
			 			<?=$lts['id']?>
			 		</td>
			 		<td style='width:5.8%'>
			 			<?=$lts['prioridade']?>
			 		</td>
			 		<td style='width:20.6%'>
			 			<?=str_replace("principal/lista","principal/listaDemandasRelacionadas",$lts['tit'])?>
			 		</td>
			 		<td style='width:25%'>
			 			<?=$lts['origemdemanda']?>
			 		</td>
			 		<td style='width:8.8%'>
			 			<?=$lts['situacao']?>
			 		</td>
			 		<td style='width:14.2%'>
			 			<?=$lts['usuario']?>
			 		</td>
			 		<td style='width:14.1%'>
			 			<?=$lts['responsavel']?>
			 		</td>
			 		<td>
			 			<?=$lts['datainclusao']?>
			 		</td>
			 	</tr>
			 	<tr><td id="td_<?=$lts['id']?>" style="display:none" colspan="8"></td></tr>
			
			<?
			}
			if(count($listas) == 0){
				echo "<tr><td colspan=8 align=center><font color=red>Não existem registros</font></td></tr>";
			}
			?>
			</table>

</form>

</body>
</html>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
<?php 
    $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
    if( in_array($estadoAtualDaDemanda['esdid'], array(
    DEMANDA_ESTADO_EM_ANALISE
    ,DEMANDA_ESTADO_EM_ATENDIMENTO
    ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
    ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
    ) ) ) { ?>
        jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
<?php } ?>
});
</script>
<script type="text/javascript">

	function montaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/listaDemandasRelacionadas&acao=A', {
				        method:     'post',
				        parameters: '&dmdidAjax=' + dmdid,
				        onComplete: function (res)
				        {	
							td.style.display = '';
							img_mais.style.display = 'none';
							img_menos.style.display = '';
							td.innerHTML = res.responseText;
				        }
				  });
	
	}

	function desmontaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		td.innerHTML = '';
		td.style.display = 'none';
		img_mais.style.display = '';
		img_menos.style.display = 'none';
	
	}

</script>