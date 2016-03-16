<?php


switch($_REQUEST['requisicao']) {
	case 'tornarconsultapublica':
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
			$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		exit;
		
	case 'carregarconsulta':
		$sql = sprintf(	"SELECT prtobj FROM public.parametros_tela WHERE prtid = ".$_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		
		$dados = unserialize( $itens );
		print_r(simec_json_encode($dados));
		
		exit;
	
	case 'excluirconsulta':
		$sql = "DELETE FROM public.parametros_tela WHERE prtid='".$_REQUEST['prtid']."'";
		$db->executar($sql);
		$db->commit();
		exit;
		
	case 'carregarrelatorio':
		$sql = sprintf(	"SELECT prtobj FROM public.parametros_tela WHERE prtid = ".$_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		extract( $dados );
		$_REQUEST = $dados;
		
	case 'gerargrafico':

		if($_REQUEST['grfagp']) {
			foreach($_REQUEST['grfagp'] as $grfid => $linha) {
				foreach($linha as $line => $dado) {
					echo "<img src=\"painelacompanhamento.php?consulta=".$dado."&media=".$_REQUEST['media'][$grfid][$line]."&totalizador=".$_REQUEST['totalizador'][$grfid][$line]."&dataini=".md5_encrypt($_REQUEST['dataini'][$grfid][$line])."&datafim=".md5_encrypt($_REQUEST['datafim'][$grfid][$line])."\"><br/><br/>";
                                }
			}
		}
		exit;
	case 'listarconsultas':
		$sql = sprintf(
			"SELECT CASE WHEN prtpublico = false THEN '".(($db->testa_superuser())?"<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;":"")."<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' END AS acao, '<a href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' AS descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
			$_SESSION['mnuid'],
			$_SESSION['usucpf']);
			
		$db->monta_lista_simples( $sql, null, 50, 50, null, null, null );
		exit;
	case 'gravarconsulta':
		unset($_REQUEST['modulo'], 
			  $_REQUEST['acao'], 
			  $_REQUEST['grfid'], 
			  $_REQUEST['theme_simec'], 
			  $_REQUEST['PHPSESSID']);
			  
		$dados = serialize($_REQUEST);
		
		$sql = "INSERT INTO public.parametros_tela(
	            prtdsc, prtobj, prtpublico, prtdata, usucpf, mnuid)
	    		VALUES ('".$_REQUEST['prtdsc']."', '".$dados."', FALSE, NOW(), '".$_SESSION['usucpf']."', '".$_SESSION['mnuid']."');";
		
		$db->executar($sql);
		$db->commit();
	
		die("<script type=\"text/javascript\" src=\"../includes/JQuery/jquery2.js\"></script>
			 <script>
				alert('Consulta gravada com sucesso');
				window.opener.jQuery(\"#td_consultas\").load('demandas.php?modulo=principal/painelAcompanhamento&acao=A', { \"requisicao\": \"listarconsultas\" } );
				window.close();
			 </script>");
		
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Painel de acompanhamento', '&nbsp;');
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js" type="text/javascript" language="javascript"></script>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css">

<script>
function submeter_form() {
	if(document.getElementById("tabela").rows.length == 1) {
		alert('Insira um gráfico');
		return false;
	}
	
	if(document.getElementById('requisicao').value == 'gravarconsulta') {
		if(document.getElementById('prtdsc').value == '') {
			alert('Insira a descrição da consulta');
			return false;
		}
	
	}

	
	var form = document.getElementById('formulario');
	
	for(i=0;i<form.elements.length;i++) {
		if(form.elements[i].id.substr(0,7) == 'dataini' || form.elements[i].id.substr(0,7) == 'datafim') {
			if(form.elements[i].value == '') {
			
				alert('Preencha as datas de início e fim');
				return false;
				
			} else {
			
				if(!validaData(form.elements[i])) {
					alert('Preencha as datas');
					return false;
				}
				
			}
			
		}
		
	}
	
	// submete formulario
	formulario.target = 'graficosdemandas';
	var janela = window.open( '', 'graficosdemandas', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();

}

function excluir_relatorio(prtid) {
	var conf = confirm("Deseja realmente excluir?");
	
	if(conf) {
		jQuery.post("demandas.php?modulo=principal/painelAcompanhamento&acao=A", { "requisicao": "excluirconsulta", "prtid": prtid });
		jQuery("#td_consultas").load('demandas.php?modulo=principal/painelAcompanhamento&acao=A', { "requisicao": "listarconsultas" } );
	}
}

function carregar_consulta(prtid) {
	divCarregando();
	var nlinhas = document.getElementById("tabela").rows.length;
	if(nlinhas > 1) {
		for(var lin=1;lin<nlinhas;lin++) {
			document.getElementById("tabela").deleteRow(document.getElementById("tabela").rows.length-1);
		}
	}
	jQuery.post("demandas.php?modulo=principal/painelAcompanhamento&acao=A", { "requisicao": "carregarconsulta", "prtid": prtid }, function(data){
var d = eval("(" + data + ")");

jQuery.each( d.grfagp, function(i, n){
    jQuery.each( n, function(j, k){
            document.getElementById('grfid').value = i;
            selecionarGrafico(false);
            formulario.elements["grfagp["+i+"]["+j+"]"].value = k;
            formulario.elements["dataini["+i+"]["+j+"]"].value = d.dataini[i][j];
            formulario.elements["datafim["+i+"]["+j+"]"].value = d.datafim[i][j];
            if(d.totalizador) {
                    if(d.totalizador[i]) { 
                            formulario.elements["totalizador["+i+"]["+j+"]"].value = d.totalizador[i][j];
                            formulario.elements["totalizador["+i+"]["+j+"]"].onchange();
                    }
            }
            if(d.media) {
                    if(d.media[i]) {
                            formulario.elements["media["+i+"]["+j+"]"].value = d.media[i][j];
                            formulario.elements["media["+i+"]["+j+"]"].onchange();
                    }
            }
    });

});

                                                                                                                                                                                                                                                                    });
divCarregado();
}

function carregar_relatorio(prtid) {

	document.getElementById('prtid').value=prtid;
	document.getElementById('requisicao').value='carregarrelatorio';	
	// submete formulario
	formulario.target = 'graficosdemandas';
	var janela = window.open( '', 'graficosdemandas', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();

}


function carregarGraficos() {
	document.getElementById('individualpontuacao').innerHTML = "<img src=\"painelacompanhamento.php?consulta=individualpontuacao&ano="+document.getElementById('ano').value+"&mes="+document.getElementById('mes').value+"\">";
	document.getElementById('individualpontuacaoprioridade').innerHTML = "<img src=\"painelacompanhamento.php?consulta=individualpontuacaoprioridade&ano="+document.getElementById('ano').value+"&mes="+document.getElementById('mes').value+"\">";
	document.getElementById('avaliacaoportecnico').innerHTML = "<img src=\"painelacompanhamento.php?consulta=avaliacaoportecnico&ano="+document.getElementById('ano').value+"&mes="+document.getElementById('mes').value+"\">";
	document.getElementById('dentroforaprazo').innerHTML = "<img src=\"painelacompanhamento.php?consulta=dentroforaprazo&ano="+document.getElementById('ano').value+"&mes="+document.getElementById('mes').value+"\">";
	document.getElementById('demandasportecnico').innerHTML = "<img src=\"painelacompanhamento.php?consulta=demandasportecnico&ano="+document.getElementById('ano').value+"&mes="+document.getElementById('mes').value+"\">";
}

function tornar_publico(prtid) {
	jQuery.post("demandas.php?modulo=principal/painelAcompanhamento&acao=A", { "requisicao": "tornarconsultapublica", "prtid": prtid });
	jQuery("#td_consultas").load('demandas.php?modulo=principal/painelAcompanhamento&acao=A', { "requisicao": "listarconsultas" } );
	alert("Consulta publicada.");

}

function selecionarGrafico(carregando) {
	
	if(document.getElementById('grfid').value == '') {
		alert('Selecione um gráfico');
		return false;
	}
	if(carregando) divCarregando();
	
	var dados='';
	
	var linha = document.getElementById("tabela").insertRow(document.getElementById("tabela").rows.length);

	var myAjax = new Ajax.Request(
		'painelacompanhamento.php',
		{
			method: 'post',
			parameters: 'consulta=buscardadosgrafico&linha='+linha.rowIndex+'&grfid='+document.getElementById('grfid').value,
			asynchronous: false,
			onComplete: function(resp) {
				dados = resp.responseText.split("|");
			},
			onLoading: function(){}
		});
	
	var col0 = linha.insertCell(0);
	col0.innerHTML = "<center><img src='../imagens/excluir.gif' style=cursor:pointer; onclick=removerLinha(this); border='0'></center>";
	
	var col1 = linha.insertCell(1);
	col1.innerHTML = dados[1];
	
	var col2 = linha.insertCell(2);
	col2.innerHTML = dados[2];
	
	var col3 = linha.insertCell(3);
	col3.innerHTML = "<input type=\"text\" onblur=\"MouseBlur(this);this.value=mascaraglobal('##/##/####',this.value);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal('##/##/####',this.value);\" class=\"normal\" maxlength=\"10\" style=\"text-align: right;\" size=\"12\" name=\"dataini["+dados[0]+"]["+linha.rowIndex+"]\" id=\"dataini_"+dados[0]+"_"+linha.rowIndex+"\" title=\"Data inicial\"> <img border=\"0\" align=\"absmiddle\" onclick=\"displayCalendar(document.getElementById('dataini_"+dados[0]+"_"+linha.rowIndex+"'),'dd/mm/yyyy',this)\" title=\"Escolha uma Data\" style=\"cursor: pointer;\" src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\">";

	var col4 = linha.insertCell(4);
	col4.innerHTML = "<input type=\"text\" onblur=\"MouseBlur(this);this.value=mascaraglobal('##/##/####',this.value);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal('##/##/####',this.value);\" class=\"normal\" maxlength=\"10\" style=\"text-align: right;\" size=\"12\" name=\"datafim["+dados[0]+"]["+linha.rowIndex+"]\" id=\"datafim_"+dados[0]+"_"+linha.rowIndex+"\" title=\"Data inicial\"> <img border=\"0\" align=\"absmiddle\" onclick=\"displayCalendar(document.getElementById('datafim_"+dados[0]+"_"+linha.rowIndex+"'),'dd/mm/yyyy',this)\" title=\"Escolha uma Data\" style=\"cursor: pointer;\" src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\">";

	var col5 = linha.insertCell(5);
	col5.innerHTML = "<center><select class=\"normal\" name=\"totalizador["+dados[0]+"]["+linha.rowIndex+"]\" id=\"totalizador_"+dados[0]+"_"+linha.rowIndex+"\" onchange=troca_combo('"+dados[0]+"','"+linha.rowIndex+"','media');><option value='-'>-</option><option value='1'>Acrescentar total</option><option value='2'>Somente total</option></select></center>";
	
	var col6 = linha.insertCell(6);
	col6.innerHTML = "<center><select class=\"normal\" name=\"media["+dados[0]+"]["+linha.rowIndex+"]\" id=\"media_"+dados[0]+"_"+linha.rowIndex+"\" onchange=troca_combo('"+dados[0]+"','"+linha.rowIndex+"','totalizador');><option value='-'>-</option><option value='1'>Acrescentar média</option><option value='2'>Somente média</option></select></center>";

	document.getElementById('grfid').value="";
	
	if(carregando) divCarregado();
	
}

function troca_combo(param1,param2,destino,ob) {
	if(destino == 'media') { local='totalizador'; } else { local='media'; }
	if(document.getElementById(local+'_'+param1+'_'+param2).value == '2') {
		document.getElementById(destino+'_'+param1+'_'+param2).value='-';
		document.getElementById(destino+'_'+param1+'_'+param2).disabled=true;
	} else {
		document.getElementById(destino+'_'+param1+'_'+param2).disabled=false;
	}
}

function removerLinha(obj) {
	var linha = obj.parentNode.parentNode.parentNode.rowIndex;
	document.getElementById("tabela").deleteRow(linha);
}

jQuery(document).ready(function () {
	jQuery("#td_consultas").load('demandas.php?modulo=principal/painelAcompanhamento&acao=A', { "requisicao": "listarconsultas" } );
});


</script>

<form name="formulario" method="post" id="formulario">
<input type="hidden" name="requisicao" value="gerargrafico" id="requisicao">
<input type="hidden" name="prtid" id="prtid">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class='SubTituloDireita'>Gráficos:</td>
<td>
<?
$mes = date("m"); 

//Empresa  TYPE MÁQUINAS E SERVIÇOS LTDA.
$perfil = arrayPerfil();
//if( in_array(DEMANDA_PERFIL_EMPRESA_TYPE,$perfil) ){
//	$sql = "SELECT grfid as codigo, grfdsc as descricao FROM demandas.graficos_indicadores where grfid = 7";
//}
//else{
	$sql = "SELECT grfid as codigo, grfdsc as descricao FROM demandas.graficos_indicadores";
//}
$db->monta_combo('grfid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'grfid');
?>
<input type="button" value="Inserir" onclick="selecionarGrafico(true);">
</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Descrição da consulta:</td>
	<td><? echo campo_texto('prtdsc', "S", "S", "Descrição da consulta", 40, 150, "", "", '', '', 0, 'id="prtdsc"' ); ?><input type="button" name="enviarform" value="Salvar consulta" onclick="document.getElementById('requisicao').value='gravarconsulta';submeter_form();"></td>
</tr>
<tr>
	<td class='SubTituloEsquerda' colspan=2><img src="../imagens/menos.gif" onclick="if(document.getElementById('tr_consultas').style.display == ''){document.getElementById('tr_consultas').style.display = 'none'}else{document.getElementById('tr_consultas').style.display = ''}" style="cursor:pointer;"> Consultas</td>
</tr>
<tr id="tr_consultas">
	<td colspan="2" id="td_consultas">
	</td>
</tr>
</table>
<br />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" id="tabela">
<tr>
	<td class='SubTituloCentro'>&nbsp;</td>
	<td class='SubTituloCentro'>Nome do indicador</td>
	<td class='SubTituloCentro'>Agrupamento</td>
	<td class='SubTituloCentro'>Data início</td>
	<td class='SubTituloCentro'>Data fim</td>
	<td class='SubTituloCentro'>Total</td>
	<td class='SubTituloCentro'>Média</td>
</tr>

</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'><input type="button" name="enviarform" value="Enviar" onclick="document.getElementById('requisicao').value='gerargrafico';submeter_form();"></td>
</tr>
</table>
</form>