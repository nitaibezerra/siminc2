<? 

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_avaliador));

if(!$_SESSION['sispacto'][$sis]['uncid']) {
	$al = array("alert"=>"Usuário não vinculado com nenhuma universidade","location"=>"sispacto.php?modulo=inicio&acao=C");
	alertlocation($al);
}

$consulta_pfl = verificaPermissao();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>
function excluirAvaliacao(mavid) {

	if(mavid=='0') {
		alert('Avaliação em branco');
		return false;
	}
	
	var conf = confirm('Deseja realmente apagar avaliação?');
	
	if(conf) {
		window.location=window.location+'&requisicao=excluirAvaliacoesMensario&mavid='+mavid+'&fpbid=<?=$_REQUEST['fpbid'] ?>';
	}

}

function calcularNotaFinal(iusd) {
	
	if(jQuery('#pfreq_'+iusd).length) {
		var pfreq = jQuery('#pfreq_'+iusd).val();
	} else {
		var pfreq = 0;
	}
	
	if(jQuery('#pativ_'+iusd).length) {
		var pativ = jQuery('#pativ_'+iusd).val();
	} else {
		var pativ = 0;
	}
	
	var frequencianulo = false;
	
	if(jQuery('#frequencia_'+iusd).length) {
		if(jQuery('#frequencia_'+iusd).val()!='') {
			var frequencia 			 = jQuery('#frequencia_'+iusd).val();
		} else {
			var frequencia 			 = 0;
			frequencianulo 			 = true;
		}
	} else {
		var frequencia 			 = 0;
	}
	
	var atividadesrealizadasnulo = false;
	
	if(jQuery('#atividadesrealizadas_'+iusd).length) {
		if(jQuery('#atividadesrealizadas_'+iusd).val()!='') {
			var atividadesrealizadas = jQuery('#atividadesrealizadas_'+iusd).val();
		} else {
			var atividadesrealizadas = 0;
			atividadesrealizadasnulo = true;
		}
	} else {
		var atividadesrealizadas = 0;
	}
	
	if(jQuery('#monitoramento_'+iusd).length) {
		if(jQuery('#monitoramento_'+iusd).val()!='') {
			var monitoramento 	 = jQuery('#monitoramento_'+iusd).val();
		} else {
			var monitoramento 	 = 0;
		}
	} else {
		var monitoramento 	 = 0;
	}
	var total = parseInt(monitoramento)+(parseFloat(frequencia)*parseInt(pfreq))+(parseFloat(atividadesrealizadas)*parseInt(pativ));
	
	if(total < 7) {
		jQuery('#img_'+iusd).attr('src','../imagens/valida6.gif');
	} else {
		jQuery('#img_'+iusd).attr('src','../imagens/valida4.gif');
	}
	if(frequencianulo==false || atividadesrealizadasnulo==false) {
		jQuery('#total_'+iusd).val(total.toFixed(2));
	} else {
		jQuery('#total_'+iusd).val('');		
	}


}

function avaliarEquipe(goto) {

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "avaliarEquipe");
	document.getElementById("formulario").appendChild(input);
	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "iusdavaliador");
	input.setAttribute("value", "<?=$_SESSION['sispacto'][$sis]['iusd'] ?>");
	document.getElementById("formulario").appendChild(input);

	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "goto");
	input.setAttribute("value", goto);
	document.getElementById("formulario").appendChild(input);

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "fpbid");
	input.setAttribute("value", "<?=$_REQUEST['fpbid'] ?>");
	document.getElementById("formulario").appendChild(input);
	
	document.getElementById('formulario').submit();

}

function certificarEquipe(goto) {

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "certificarEquipe");
	document.getElementById("formulario").appendChild(input);
	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "iusdavaliador");
	input.setAttribute("value", "<?=$_SESSION['sispacto'][$sis]['iusd'] ?>");
	document.getElementById("formulario").appendChild(input);

	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "goto");
	input.setAttribute("value", goto);
	document.getElementById("formulario").appendChild(input);

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "fpbid");
	input.setAttribute("value", "<?=$_REQUEST['fpbid'] ?>");
	document.getElementById("formulario").appendChild(input);
	
	document.getElementById('formulario').submit();

}


jQuery(document).ready(function() {

jQuery(window).keydown(function(event){
  if(event.keyCode == 13) {
    event.preventDefault();
    return false;
  }
});
<? if($consulta) : ?>
jQuery("[name^='frequencia[']").attr('disabled','disabled');
jQuery("[name^='iusd_avaliados[']").attr('disabled','disabled');
jQuery("[name^='atividadesrealizadas[']").attr('disabled','disabled');
jQuery("#salvar").hide();
jQuery("#salvarcontinuar").hide();
jQuery("[src^='../imagens/reject.png']").hide();
<? endif; ?>

<? if($consulta_pfl) : ?>
jQuery("#salvarcontinuar").css('display','none');
jQuery("#salvar").css('display','none');
<? endif; ?>

});

function verAjuda(fatid) {

	ajaxatualizar('requisicao=carregarAjudaAvaliacao&fatid='+fatid,'ajudatd');
	
	jQuery("#modalAjuda").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 1024,
	                        height: 720,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function exibirAvaliacaoSub(requisicao, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.innerHTML = '&nbsp;';
		var ncol     = nlinha.insertCell(1);
		ncol.colSpan = 9;
		var d = new Date();
		ncol.id      = 'av_coluna_'+d.toTimeString().substr(0,8)+'_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarAvaliacaoEquipeSub&consulta=true'+requisicao,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}

}


function mostrarCorrecaoAprovado(iusd) {

	jQuery('#iusdcorrecao').val(iusd);

	jQuery("#modalCorrecaoAprovado").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}


function marcarTodosFrequencia(opt) {
	jQuery("[name^='frequencia[']").val(opt);
	jQuery("[name^='frequencia[']").change();
}

function marcarTodosAtividadesRealizadas(opt) {
	jQuery("[name^='atividadesrealizadas[']").val(opt);
	jQuery("[name^='atividadesrealizadas[']").change();
}

function fecharReavaliarNota(iusd, obj) {
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	var conf = confirm('Desejar realmente fechar?');
	
	if(conf) {
	
		jQuery("[name^='iusd_avaliados[]'][value^='"+iusd+"']").attr('disabled','disabled');
			
		if(document.getElementById('atividadesrealizadas_'+iusd)) {
			jQuery('#atividadesrealizadas_'+iusd).attr('disabled','disabled');
		}
		
		if(document.getElementById('frequencia_'+iusd)) {
			jQuery('#frequencia_'+iusd).attr('disabled','disabled');
		}
	
		tabela.deleteRow(linha.rowIndex-1);
	}

}

function reavaliarEquipe(iusdcorrecao) {

	if(document.getElementById('atividadesrealizadas_'+jQuery('#iusdcorrecao').val())) {
		if(jQuery('#atividadesrealizadas_'+jQuery('#iusdcorrecao').val()).val()=='') {
			alert('Para salvar você deve reavaliar o bolsista no box abaixo.');
			return false;
		}
	}
	
	if(document.getElementById('frequencia_'+jQuery('#iusdcorrecao').val())) {
		if(jQuery('#frequencia_'+jQuery('#iusdcorrecao').val()).val()=='') {
			alert('Para salvar você deve reavaliar o bolsista no box abaixo.');
			return false;
		}
	}

	divCarregando();
	avaliarEquipe('<?=$goto_sal ?>');

}

function reavaliarNota() {

	if(jQuery('#mavdsc').val()=='') {
		alert('É necessário preencher a justificativa');
		return false;
	}

	var btn = document.getElementById('corrigir_'+jQuery('#iusdcorrecao').val());
	
	var tabela = btn.parentNode.parentNode.parentNode;
	var linha = btn.parentNode.parentNode;
	
	var nlinha   = tabela.insertRow(linha.rowIndex-1);
	var ncol0     = nlinha.insertCell(0);
	ncol0.innerHTML = '<img src=../imagens/seta_baixo.png style=cursor:pointer; onclick="fecharReavaliarNota('+jQuery('#iusdcorrecao').val()+',this);"> ';
	var ncol1     = nlinha.insertCell(1);
	ncol1.colSpan = 9;
	
	var html  = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		html += '<tr><td class="SubTituloDireita">Responsável:</td><td nowrap>'+jQuery('#labelresponsavel').html()+'<input type=hidden name="cpfresponsavel['+jQuery('#iusdcorrecao').val()+']" value="'+jQuery('#cpfresponsavel').val()+'"></td>';
		html += '    <td class="SubTituloDireita">Justificativa:</td><td>'+jQuery('#mavdsc').val()+'<input type=hidden name="mavdsc['+jQuery('#iusdcorrecao').val()+']" value="'+jQuery('#mavdsc').val()+'"></td>';
		html += '    <td><input type="button" value="Salvar" id="salvar" onclick="reavaliarEquipe();"></td></tr>';
		html += '</table>'; 
	
	ncol1.innerHTML = html;
	
	jQuery("[name^='iusd_avaliados[]'][value^='"+jQuery('#iusdcorrecao').val()+"']").attr('disabled','');
		
	if(document.getElementById('atividadesrealizadas_'+jQuery('#iusdcorrecao').val())) {
		jQuery('#atividadesrealizadas_'+jQuery('#iusdcorrecao').val()).attr('disabled','');
	}
	
	if(document.getElementById('frequencia_'+jQuery('#iusdcorrecao').val())) {
		jQuery('#frequencia_'+jQuery('#iusdcorrecao').val()).attr('disabled','');
	}

	jQuery("#modalCorrecaoAprovado").dialog('close');


}


function gravarJustificativa(cpf) {
	
	jQuery("#mavrecomendadocertificacaojustificativa_"+cpf).val(jQuery("#justificativa").val());

	jQuery("#modalAjuda").dialog('close');
	
}

function cancelarJustificativa(cpf) {

	jQuery("#mavrecomendadocertificacaojustificativa_"+cpf).val('');

	jQuery("#certificacao_"+cpf+"_"+jQuery("#recomendacao_"+cpf).val()).attr('checked','checked');

	
}

function cancelModalAjuda() {

	jQuery("#modalAjuda").dialog('close');
}

function exibirJustificativa(cpf) {

	jQuery("#ajudatd").html(jQuery("#mavrecomendadocertificacaojustificativa_"+cpf).val());

	jQuery("#modalAjuda").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 400,
        modal: true,
     	close: function(){} 
    });

	
}

function recomendarCertificacao(obj,cpf) {

	if(obj.value != jQuery('#recomendacao_'+cpf).val()) {

		jQuery("#ajudatd").html("<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=100%><tr><td class=SubTituloCentro colspan=2>Motivo para alterar a recomendação proposta</td></tr><tr><td class=SubTituloDireita width=20%>Justificativa</td><td><textarea  id=justificativa name=justificativa cols=70 rows=4   onmouseover=MouseOver( this ); onfocus=MouseClick( this );  onmouseout=MouseOut( this );  class=\"obrigatorio txareanormal\" ></textarea> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /></td></td></tr><tr><td class=SubTituloCentro colspan=2><input type=button name=justificativa value=\"Gravar\" onclick=\"gravarJustificativa('"+cpf+"');\"> <input type=button name=cancelar value=\"Cancelar\" onclick=\"cancelModalAjuda();\"></td></tr><tr><td></td></tr></table>");
		
		jQuery("#modalAjuda").dialog({
            draggable:true,
            resizable:true,
            width: 1024,
            height: 720,
            modal: true,
         	close: function(){
         			
         			if(jQuery("#mavrecomendadocertificacaojustificativa_"+cpf).val()=='' && obj.value != jQuery('#recomendacao_'+cpf).val()) {
         				cancelarJustificativa(cpf);
         			}

         			if(jQuery("#mavrecomendadocertificacaojustificativa_"+cpf).val()=='') {
         				jQuery("#imgc_"+cpf).css('display','none');
         			} else {
         				jQuery("#imgc_"+cpf).css('display','');
         			}

         			if(jQuery("#certificacao_"+cpf+"_1").attr('checked')) {
						jQuery("#img_"+cpf).attr('src','../imagens/valida4.gif');
         			}

         			if(jQuery("#certificacao_"+cpf+"_2").attr('checked')) {
         				jQuery("#img_"+cpf).attr('src','../imagens/valida6.gif');
         			}
             	} 
        });

        
	
	} else {

		cancelarJustificativa(cpf);

		jQuery("#imgc_"+cpf).css('display','none');

		if(obj.value=='1') {
			jQuery("#img_"+cpf).attr('src','../imagens/valida4.gif');
		}

		if(obj.value=='2') {
			jQuery("#img_"+cpf).attr('src','../imagens/valida6.gif');
		}
		

	}

	
}

</script>
<div id="modalAjuda" style="display:none;">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td id="ajudatd"></td>
</tr>
</table>
</div>

<div id="modalCorrecaoAprovado" style="display:none;">
<form method="post" id="formulario3" name="formulario3">
<input type="hidden" name="iusdcorrecao" id="iusdcorrecao">
<input type="hidden" name="cpfresponsavel" id="cpfresponsavel" value="<?=$_SESSION['usucpf'] ?>">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Motivo para reavaliação da Nota</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Responsável:</td>
	<td id="labelresponsavel"><? echo mascaraglobal($_SESSION['usucpf'],"###.###.###-##")." - ".$_SESSION['usunome'];?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'mavdsc', 'S', 'S', '', '70', '4', '200'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="corrigir" value="Reavaliar Nota" onclick="reavaliarNota();"></td>
</tr>
</table>
</form>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Avaliar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto/sispacto.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=avaliarusuario"); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('uncid'=>$_SESSION['sispacto'][$sis]['uncid'],'fpbid'=>$_REQUEST['fpbid'],'pflcod_avaliador'=>$pflcod_avaliador));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>

<?

$funcaoavaliacao = $db->pegaUm("SELECT tpatipoavaliacao FROM sispacto.tipoavaliacaoperfil WHERE pflcod='".$pflcod_avaliador."' AND uncid='".$_SESSION['sispacto'][$sis]['uncid']."' AND fpbid='".$_REQUEST['fpbid']."'");

if(!$funcaoavaliacao) $funcaoavaliacao = 'carregarAvaliacaoEquipe';

?>

<tr>
	<td colspan="2">
	<? if($funcaoavaliacao == 'carregarAvaliacaoEquipe') : ?>
	<form method=post name="formbuscar" id="formbuscar">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr><td class="SubTituloDireita" width="25%">CPF</td><td><?=campo_texto('filtro[iuscpf]', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', $_REQUEST['iuscpf']); ?></td></tr>
    <tr><td class="SubTituloDireita" width="25%">Nome</td><td><?=campo_texto('filtro[iusnome]', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td></tr>
    <tr><td class="SubTituloDireita" width="25%">Perfil</td><td><? 
    $sql = "SELECT pfldsc as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_ORIENTADORESTUDO."','".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORIES."')";
    $db->monta_combo('filtro[pfldsc]', $sql, 'S', 'Selecione', 'selecionarPerfilGerenciar', '', '', '200', 'N', 'pfldsc','', $_REQUEST['pfldsc']); ?> 
    </td></tr>
	<tr><td class="SubTituloCentro" colspan=2><input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();"><input type="button" name="vertodos" value="Ver todos" onclick="window.location=window.location;"></td></tr>
	</table>
	</form>
	<? endif; ?>
	
	<form method="post" id="formulario" name="formulario">
	<table width="100%">
	<tr>
	<td width="95%" valign="top">
	<?
	if($funcaoavaliacao == 'carregarAvaliacaoEquipe') :
	?>
	<table cellSpacing="1" cellPadding="3" align="center">
	<tr>
	<td class="SubTituloCentro">Selecionar Todos:</td>
	<td class="SubTituloDireita">Frequência :</td>
	<td><? $db->monta_combo('todosfrequencia', $OPT_AV['frequencia'], 'S', 'Selecione', 'marcarTodosFrequencia', '', '', '', 'N', 'todosfrequencia',''); ?></td>
	<td class="SubTituloDireita">Atividades Realizadas :</td>
	<td><? $db->monta_combo('todosatividadesrealizadas', $OPT_AV['atividadesrealizadas'], 'S', 'Selecione', 'marcarTodosAtividadesRealizadas', '', '', '', 'N', 'todosatividadesrealizadas',''); ?></td>
	</tr>
	</table>
	<?
	else :
	?>
	<script>
	jQuery(document).ready(function() {

		jQuery("#salvar").show();
		jQuery("#salvarcontinuar").show();

		});
			

	</script>
	<?
	endif;
	
	$funcaoavaliacao(array("sql"=>$sql_avaliacao,"fpbid"=>$_REQUEST['fpbid'],"uncid"=>$_SESSION['sispacto'][$sis]['uncid'],"iusd"=>$_SESSION['sispacto'][$sis]['iusd'],"esdid"=>$dados_mensario['esdid'],"sis"=>$sis,"filtro"=>$_REQUEST['filtro']));
	?>
	</td>
	<td width="5%" valign="top">
	<? 
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $dados_mensario['docid'], array("fpbid" => $_REQUEST['fpbid'],"pflcod" => $pflcod_avaliador) );			
	?>
	</td>
	</tr>
	</table>
	</form>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<? if($goto_ant) : ?>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? endif; ?>
	<? if($goto_sal) : ?>
	
	<? if($funcaoavaliacao == 'carregarCertificacaoEquipe') : ?>
	<input type="button" value="Salvar" id="salvar" onclick="divCarregando();certificarEquipe('<?=$goto_sal ?>');">
	<? else :?>
	<input type="button" value="Salvar" id="salvar" onclick="divCarregando();avaliarEquipe('<?=$goto_sal ?>');">
	<? endif; ?>
	
	<? if($goto_pro) : ?>
	
	
	<? if($funcaoavaliacao == 'carregarCertificacaoEquipe') : ?>
	<input type="button" value="Salvar e Continuar" id="salvarcontinuar" onclick="divCarregando();certificarEquipe('<?=$goto_pro ?>');">
	<? else :?>
	<input type="button" value="Salvar e Continuar" id="salvarcontinuar" onclick="divCarregando();avaliarEquipe('<?=$goto_pro ?>');">
	<? endif; ?>
		
	<? endif; ?>
	<? endif; ?>
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>
	</td>
</tr>
</table>