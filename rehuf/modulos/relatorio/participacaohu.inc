<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "participacaohu_resultado.inc";
		exit;
                break;
	case '2':
		include "participacaohu_resultado.inc";
		exit;
                break;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Gerencial de Participação dos HUs sobre o Total de Itens do Pregão", "" );

$permissoes = verificaPerfilRehuf();
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script> 
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	formulario.buscar.value='1';
	
        selectAllOptions( document.getElementById( 'entid' ) );
        selectAllOptions( document.getElementById( 'preid' ) );
        
	formulario.target = 'participacaohu_resultado';
	var janela = window.open('', 'participacaohu_resultado', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}

function exibirXls() {
	var formulario = document.formulario;
	formulario.buscar.value='2';
	
        selectAllOptions( document.getElementById( 'entid' ) );
        selectAllOptions( document.getElementById( 'preid' ) );
        
	formulario.target = 'participacaohu_resultado';
	var janela = window.open('', 'participacaohu_resultado', 'width=200,height=100,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
       
}

$(document).ready(function(){
    $('#entid').attr('title','Permite filtrar a consulta por hospital. Caso não seja informado, exibirá lista com a participação de todos os HUs.');
    $('#preid').attr('title','Permite filtrar a consulta por pregão. Caso não seja informado, exibirá lista com todos os pregões.');
    $('#preinsinicial').attr('title','Permite filtrar a consulta por pregões no período. Caso não seja informado, exibirá lista com todos os pregões.');
    $('#preinsfinal').attr('title','Permite filtrar a consulta por pregões no período. Caso não seja informado, exibirá lista com todos os pregões.');
});
</script>


<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script> 
<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td>Filtros</td></tr>
               <tr>
			<td class="SubTituloDireita" valign="top">Hospitais :</td>
			<td>
			<?
			$permissoes = verificaPerfilRehuf();
			if(count($permissoes['verhospitais']) > 0) {
				$sqlHospitais = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
								 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
								 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
								 ORDER BY ena.entsig";
				combo_popup( "entid", $sqlHospitais, "Hospitais", "292x650", 0, array(), "", "S", false, false, 5, 400 );				
				
			} else {
				echo "Seu perfil não possui hospitais";	
			}
			?>
			</td>
		</tr>
                <tr>
			<td class="SubTituloDireita" valign="top">Pregões :</td>
			<td>
			<?
				$sql = "SELECT preid as codigo, preobjeto||' ('||COALESCE(precodigo,'-')||')' as descricao FROM rehuf.pregao WHERE prestatus = 'A'";

				combo_popup( "preid", $sql, "Pregões", "292x650", 0, array(), "", "S", false, false, 5, 400 );				
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Período Vigência :</td>
                        <td>
                         <?php echo campo_data2( 'preinsinicial', 'N', 'S', '', 'S' ); ?>
                         <?php echo campo_data2( 'preinsfinal', 'N', 'S', '', 'S' ); ?>
                        </td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;">
		   <input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/>&nbsp;&nbsp;
		   <input type="button" name="filtrarxls" value="Visualizar XLS" onclick="exibirXls();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/>
		</td>
	</tr>
</table>