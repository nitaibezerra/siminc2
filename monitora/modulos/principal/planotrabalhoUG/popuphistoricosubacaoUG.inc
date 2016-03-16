<?php
die;
if($_REQUEST['requisicao'] == 'vincular'){
	
	require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
	require_once APPRAIZ . "monitora/classes/Pi_SubacaoHistorico.class.inc";

	extract($_POST);
	
	$retorno = false;
	$pi_SubacaoHistorico = new Pi_SubacaoHistorico();
	$pi_SubacaoHistorico->sbaid = $sbaid;
	//$pi_SubacaoHistorico->sahobs = $sahobs;
	$pi_SubacaoHistorico->usucpf = $_SESSION['usucpf'];
	$pi_SubacaoHistorico->sahdaa = date('Y-m-d H:i:s');
	$pi_SubacaoHistorico->sahsituacao = $situacao;
	$pi_SubacaoHistorico->salvar();
	
	$pi_Subacao = new Pi_Subacao($sbaid);
	$pi_Subacao->sbasituacao = $situacao;
	$pi_Subacao->salvar();
	
	if($pi_SubacaoHistorico->commit()){
		$retorno = true;
	}
	unset($pi_Subacao);
	unset($pi_SubacaoHistorico);
	
	echo $retorno;
	die;
	
}

extract($_GET);

require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_SubacaoHistorico.class.inc";


ver("select unicod from monitora.pi_subacaounidade where sbaid = $sbaid",d);

$unicod = $db->pegaUm("select unicod from monitora.pi_subacaounidade where sbaid = $sbaid");

$obPi_Subacao = new Pi_Subacao($sbaid);

if($obPi_Subacao->sbaid){
	$sql_lista = "SELECT
					dtl.ptrid,
					dtl.ptres,
					trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) as descricao,
					uni.unidsc,
					SUM(dtl.ptrdotacao) as dotacaoinicial,
					coalesce(cast(SUM(dt.valor) as varchar),'0.00') as detalhamento
					--(SUM(dtl.ptrdotacao)-coalesce(SUM(dt.valor),0.00)) as diferenca
				FROM monitora.acao aca
					INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
					INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
					INNER JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid
					LEFT JOIN ( select ptrid,
								sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								group by ptrid ) dt ON dtl.ptrid = dt.ptrid
					WHERE aca.prgano = '".$_SESSION['exercicio']."' 
						  and aca.unicod = '".$_SESSION['monitora_var']['unicod']."'
					      and aca.acasnrap = false
					      and sad.sbaid = ".$obPi_Subacao->sbaid."
					GROUP BY dtl.ptrid, dtl.ptres,
								trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) ,
								uni.unidsc";
	$ptresacao = $db->carregar($sql_lista);
}
$ptresacao = ($ptresacao) ? $ptresacao : array();

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
  </head>
<br>    
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="tarid" id="tarid" value="" />
	<tr>
		<td class = "subtitulodireita" colspan="2">
			<center> 
			<h3>Cadastro Subação</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td align="center" colspan="2">
			<?php echo $unidade = $db->pegaUm("SELECT ungcod || ' - ' || ungdsc FROM public.unidadegestora WHERE ungcod = '".$_SESSION['monitora_var']['ungcod']."'"); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
				<tr>
				<td style="background-color: #C9C9C9;" colspan="9" align="center"><b>Detalhamento Orçamentário</b></td>
				</tr>
		        <tr>
					<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="ptrid"></td>
					<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Autorizada</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado na SubAção</b></td>
				    <td style="background-color: #C9C9C9; width:100px;" align="center"><b>Saldo Disponível</b></td>
				    <td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
				</tr>
				<? 
				if($ptresacao[0]) {
					$valortotal = 0;
					$cor = 0;
					foreach($ptresacao as $acpl) { 
				?>
		        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptrid_<? echo $acpl['ptrid']; ?>">
					<td align="center"><? echo $acpl['ptres']; ?></td>
					<td align="left"><? echo $acpl['descricao']; ?><input type='hidden' name='ptrid[<? echo $acpl['ptrid']; ?>]' value='<? echo $acpl['ptrid']; ?>'></td>
				    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
				    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
				    <td align="right"><? echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); ?></td>
				    <td align="center"><input type="text" name="valor[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['detalhamento'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<?=$acpl['ptrid']?>','<?=number_format($acpl['detalhamento'],2,',','.')?>');" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
				<? 
						$cor++;
						$valortotal = $valortotal + $acpl['detalhamento']; 
					}
				}
				?>
				<tr style="height: 30px;">
					<td align="right" valign="top" colspan="5"><b>TOTAL :</b></td>
					<td align="center" valign="top"><input type="text" name="valortotal" id="valortotal" size="28" maxlength="" value="<? echo number_format($valortotal,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">
			Código:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbacod; ?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">
			Título:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbatitulo; ?>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita">
			Descrição / Funcionalidade:
		</td>
		<td>
			<?php echo $obPi_Subacao->sbadsc; ?>
		</td>
	</tr>
	<tr>
		<td>
			<?php

			$sql = "SELECT ptres FROM monitora.pi_subacaodotacao sd
						INNER JOIN monitora.ptres p on sd.ptrid = p.ptrid
					WHERE sd.sbaid = $obPi_Subacao->sbaid AND p.ptrano = '{$_SESSION['exercicio']}'";
			
			$arPtres = $db->carregar($sql);
			$arPtres = ($arPtres) ? $arPtres : array();			
			
			$nPtres = array();
			$boPtres = false;
			foreach($arPtres as $ptres){
				array_push($nPtres,$ptres['ptres']);
				$boPtres = true;
			}
			
			 
			if(!$boPtres){
				echo "<font color=\"blue\">É necessário vincular ação orçamentária para aprovar subação no exercício de {$_SESSION['exercicio']}.</font>";					
			}
			?>
		</td>
	</tr>
	<tr>
	<td colspan="2">
		<?php 
		$sql = "SELECT  
						to_char(sah.sahdaa, 'DD/MM/YYYY HH24:MI:SS') as sahdaa,
						CASE WHEN sah.sahsituacao = 'P' THEN ' <font color=\"red\">Pendente de Aprovação</font> '
							 WHEN sah.sahsituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
							 WHEN sah.sahsituacao = 'R' THEN ' <font color=\"#3F85FF\">Revisado</font> '
							 WHEN sah.sahsituacao = 'E' THEN ' <font color=\"#EAC117\">Enviado para Revisão</font> ' 
						END as situacao,
						sah.sahobs,
						u.usunome 
				FROM monitora.pi_subacaohistorico sah
					INNER JOIN seguranca.usuario u on sah.usucpf = u.usucpf
				WHERE sbaid = $obPi_Subacao->sbaid 
				ORDER BY sah.sahdaa DESC";
		
		
		$sql2 = "SELECT sah.sbaid, 
						sah.sahsituacao
				FROM monitora.pi_subacaohistorico sah
					INNER JOIN seguranca.usuario u on sah.usucpf = u.usucpf
				WHERE sbaid = $obPi_Subacao->sbaid 
				ORDER BY sah.sahdaa DESC";
		
		$arUltimaSituacao = $db->pegaLinha($sql2);
		
		$cabecalho = array("Data Hora", "Situação", "Observação", "Cadastrado Por");
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
		?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td align="left" colspan="2">
		<?php if($arUltimaSituacao['sahsituacao'] == 'E'){ ?>
			<?php if($boPtres){ ?>
				<input type="button" value="Aprovar" onclick="vincular('A')" style="cursor: pointer;"/>
			<?php } ?>
		<?php } ?>
		<?php if($arUltimaSituacao['sahsituacao'] == 'A'){ ?>
			<input type="button" value="Enviar para Revisão" onclick="vincular('E')" style="cursor: pointer;"/>
		<?php } ?>
		</td>
	</tr>
	</form>
</table>
<script>
function vincular(situacao){

	var sbaid = '<?php echo $arUltimaSituacao['sbaid']; ?>';
	
 	var url = window.location.href;
	var parametros = "requisicao=vincular&sbaid="+sbaid+'&situacao='+situacao ;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(r) {
				//alert(r.responseText);
				//$('dibDegug').update(r.responseText);
				if(r.responseText){
					alert('Dados gravados com Sucesso.');
					// feito isso por causa da presa.
					//window.location.reload();
					//window.opener.location.reload();
					window.opener.document.formulario.submit();
					window.close();
				}
			}
		}
	);
 	
	}
</script>
<div id="dibDegug"></div>