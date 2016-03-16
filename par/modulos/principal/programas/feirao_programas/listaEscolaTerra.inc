<?php

define("ESD_ADESAO_EM_ELABORACAO", 840);


function listaAdesaoProgramaEscolas($dados) {
	global $db;
	
	$sql = "(
			SELECT l.letcodigoinep, l.letnome, l.letprofessores::text, '<input type=\"text\" style=\"text-align:left;\" name=\"apeprofessoressolicitados['||a.apeid||']\" size=\"8\" maxlength=\"6\" value=\"'||COALESCE(a.apeprofessoressolicitados::text,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);somarQtdProfessores();\" title=\"\" class=\" normal\" onkeyup=\"this.value=mascaraglobal(\'######\',this.value);somarQtdProfessores();\">' as input FROM escolaterra.adesaoprogramaescola a 
			INNER JOIN escolaterra.listaescolasterra l ON l.letid = a.letid 
			WHERE adpid='".$dados['adpid']."'
			) UNION ALL (
			SELECT '', '', '<b>Total:</b>', '<input type=\"text\" style=\"text-align:left;\" name=\"total\" id=\"total\" size=\"8\" maxlength=\"6\" value=\"' || COALESCE((SELECT SUM(apeprofessoressolicitados) FROM escolaterra.adesaoprogramaescola WHERE adpid='".$dados['adpid']."')::integer,0) || '\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"\" class=\" disabled\" readonly=\"readonly\">'
			)";
	
	$cabecalho = array("Código INEP","Escola","Qtd Professores CENSO","Qtd Professores Solicitados");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2);
}

function atualizarAdesaoProgramaEscola($dados) {
	global $db;
	
	if($dados['apeprofessoressolicitados']) {
		foreach($dados['apeprofessoressolicitados'] as $apeid => $apeprofessoressolicitados) {
			
			$sql = "UPDATE escolaterra.adesaoprogramaescola
   					SET apeprofessoressolicitados='".$apeprofessoressolicitados."'
 					WHERE apeid='".$apeid."'";

			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	die('<script type="text/javascript">
		 alert("Escolas atualizadas com sucesso");
		 window.location.href="par.php?modulo=principal/programas/feirao_programas/listaEscolaTerra&acao=A";
		 </script>');
	
}

function deletarAdesaoProgramaEscola($dados) {
	global $db;
	
	$sql = "DELETE FROM escolaterra.adesaoprogramaescola WHERE letid='".$dados['letid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function inserirAdesaoProgramaEscola($dados) {
	global $db;

	$sql = "SELECT apeid FROM escolaterra.adesaoprogramaescola WHERE letid='".$dados['letid']."'";
	$apeid = $db->pegaUm($sql);

	if(!$apeid) {
		
		$sql = "INSERT INTO escolaterra.adesaoprogramaescola(
	            adpid, letid)
	    		VALUES ('".$_SESSION['par']['adpid']."', '".$dados['letid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
}

function listaEscolasTerra($dados) {
	global $db;
	
	$sql = "SELECT estuf, muncod FROM par.instrumentounidade WHERE inuid='".$_SESSION['par']['inuid']."'";
	$instrumentounidade = $db->pegaLinha($sql);
	
	if($instrumentounidade['estuf']) {
		$f = "letesfera='E' AND m.estuf='".$instrumentounidade['estuf']."'";
	}elseif($instrumentounidade['muncod']) {
		$f = "letesfera='M' AND m.muncod='".$instrumentounidade['muncod']."'";
	} else {
		$f = "1=2";
	}
	
	echo "<p><input type=checkbox id=marcartodos onclick=marcarTodos(true); > Marcar todos | <input type=checkbox id=desmarcartodos onclick=marcarTodos(false);> Desmarcar todos</p>";
	
	$sql = "SELECT '<input type=checkbox name=\"chk[]\" value=\"' || l.letid || '\" onclick=\"gerenciarAdesaoProgramaEscola(' || l.letid || ',this)\" ' || CASE WHEN (SELECT a.apeid FROM escolaterra.adesaoprogramaescola a WHERE a.adpid='".$_SESSION['par']['adpid']."' AND a.letid=l.letid) IS NOT NULL THEN 'checked' ELSE '' END || '>' as chk, letnome, letprofessores, m.estuf || ' / ' || m.mundescricao as localizacao 
			FROM escolaterra.listaescolasterra l 
			INNER JOIN territorios.municipio m ON m.muncod = l.muncod 
			WHERE ".$f;

	$cabecalho = array("&nbsp;","Escola","Qtd Professores","UF/Município");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true,false,false,true);
	
	echo "<p align=center><input type=button value=Fechar onclick=jQuery(\"#modalEscolas\").dialog('close');></p>";
}

include_once APPRAIZ.'includes/workflow.php';


// funções permitidas neste menu
$funcoes_autorizadas = array("listaAdesaoProgramaEscolas",
							 "atualizarAdesaoProgramaEscola",
							 "deletarAdesaoProgramaEscola",
							 "inserirAdesaoProgramaEscola",
							 "listaEscolasTerra");

if(in_array($_REQUEST['requisicao'],$funcoes_autorizadas)) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['par']['adpid'] || !$_SESSION['par']['prgid'] || !$_SESSION['par']['inuid']) {
	die('<script type="text/javascript">alert("Sessão Expirou.\nFavor selecione o programa novamente!");window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";</script>');
}

$docid = pgCriarDocumento( $_SESSION['par']['adpid'] );

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

if($esdid == ESD_ADESAO_EM_ELABORACAO) $consulta = false;
else $consulta = true;

include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/listaEscolaTerra&acao=A');

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/> 
<script>

function marcarTodos(boleano) {

	jQuery("#marcartodos").attr('checked',false);
	jQuery("#desmarcartodos").attr('checked',false);
	
	if(boleano) {
		jQuery("#marcartodos").attr('checked',true);
	} else {
		jQuery("#desmarcartodos").attr('checked',true);
	}

	jQuery("[name^='chk[']").attr('checked',boleano);
	
	jQuery("[name^='chk[']").each(function() {
		gerenciarAdesaoProgramaEscola(jQuery(this).val(),this);
	});
	
}

function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

function inserirEscolas() {

	ajaxatualizar('requisicao=listaEscolasTerra','modalEscolas');

	jQuery("#modalEscolas").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
                  
}

function gerenciarAdesaoProgramaEscola(letid,obj) {
	
	if(obj.checked) {
		ajaxatualizar('requisicao=inserirAdesaoProgramaEscola&letid='+letid,'');
	} else {
		ajaxatualizar('requisicao=deletarAdesaoProgramaEscola&letid='+letid,'');
	}
	
	ajaxatualizar('requisicao=listaAdesaoProgramaEscolas&adpid=<?=$_SESSION['par']['adpid'] ?>','td_listaAdesaoProgramaEscolas');

}

function atualizarAdesaoProgramaEscola() {
	
	var invalidar = false;

	jQuery("[name^='apeprofessoressolicitados[']").each(function() {
		if(jQuery(this).val()=='') {
			invalidar = true;
		}
	});
	
	if(invalidar) {
		alert('É obrigatório o preenchimento da qtd dos professores');
		return false;
	}
	
	document.getElementById('formulario').submit();
	
}

function somarQtdProfessores() {
	
	var somaTotal = 0;

	jQuery("[name^='apeprofessoressolicitados[']").each(function() {
		if(jQuery(this).val()!='') {
			somaTotal += parseInt(jQuery(this).val());
		}
	});
	
	jQuery('#total').val(somaTotal);
	
}

<? if($consulta) : ?>
jQuery(document).ready(function() {
	jQuery("[name^='apeprofessoressolicitados[']").attr('class','disabled');
	jQuery("[name^='apeprofessoressolicitados[']").attr('disabled','disabled');
	jQuery("[name^='enviarescolas']").css('display','none');
});
<? endif; ?>

</script>

<div id="modalEscolas" style="display:none;"></div>

<? cabecalho(); ?>

<form name="formulario" id="formulario" method="post">
<input type="hidden" name="requisicao" value="atualizarAdesaoProgramaEscola">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Definindo Escolas</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
	<? if(!$consulta) : ?>
	<span style="cursor:pointer;" onclick="inserirEscolas();"><img src="../imagens/gif_inclui.gif" style="cursor:pointer;" align="absmiddle"> Inserir escolas</span>
	<? endif; ?>
	</td>
	<td width="5%" rowspan="2" valign="top"><? wf_desenhaBarraNavegacao( $docid, array( 'inuid' => $_SESSION['par']['inuid'] ) ); ?></td>
</tr>
<tr>
	<td valign="top" id="td_listaAdesaoProgramaEscolas"><? listaAdesaoProgramaEscolas(array('adpid' => $_SESSION['par']['adpid'])); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="enviarescolas" value="Salvar" onclick="atualizarAdesaoProgramaEscola()"></td>
</tr>
</table>
</form>