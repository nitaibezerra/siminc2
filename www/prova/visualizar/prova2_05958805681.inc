<?php 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

/*
 * DICAS:
 * 
 * Com a função "executar" você executa uma query, por exemplo para inseri-la no banco. Ex:
 * 
 * 		$sql = "INSERT INTO data (dia, mes, ano) VALUES ('24','03','1984')";
 * 		$db->executar($sql); 
 * 		$db->commit(); 
 * 
 */

if (isset($_POST['acao'])) {
	switch ($_POST['acao']) {
		case 'salvar':
			// -- Tratando strings de inserção
			$sTurnome = pg_escape_string($_POST['turnome']);
			// -- @todo Adicionar validação de data (dia existente e período válido)
			$sTurdtabr = pg_escape_string($_POST['turdtabr']);
			$sTurdtfec = pg_escape_string($_POST['turdtfec']);
			$sQuery = <<<QUERY
INSERT INTO prova.turma(turnome, turdtabr, turdtfec)
  VALUES ('{$sTurnome}', TO_TIMESTAMP('{$sTurdtabr}', 'DD/MM/YYYY'), TO_TIMESTAMP('{$sTurdtfec}', 'DD/MM/YYYY'))
QUERY;
			if ($db->executar($sQuery)) {
				$db->commit();
				$_SESSION['prova']['salvar'] = 'Operação realizada com sucesso.';
			} else {
				$_SESSION['prova']['salvar'] = 'Não foi possível salvar a nova turma.';
			}
			break;
		case 'excluir':
			// -- Tratando strings de inserção
			$sTurid = pg_escape_string($_POST['turid']);
			$sQuery = <<<QUERY
DELETE FROM prova.turma
  WHERE turid = {$sTurid}
QUERY;
			$db->executar($sQuery);
			$db->commit();
			break;
		case 'alterar':
			$sTurid = pg_escape_string($_POST['turid']);
			$sTurnome = pg_escape_string($_POST['turnome']);
			// -- @todo Adicionar validação de data (dia existente e período válido)
			$sTurdtabr = pg_escape_string($_POST['turdtabr']);
			$sTurdtfec = pg_escape_string($_POST['turdtfec']);
			$sQuery = <<<QUERY
UPDATE prova.turma
  SET turnome = '{$sTurnome}',
  	  turdtabr = TO_TIMESTAMP('{$sTurdtabr}', 'DD/MM/YYYY'), 
  	  turdtfec = TO_TIMESTAMP('{$sTurdtfec}', 'DD/MM/YYYY')
  WHERE turid = {$sTurid}
QUERY;
			$db->executar($sQuery);
			$db->commit();
			break;
	}
	header("Location: {$_SERVER['HTTP_REFERER']}");
	exit;	
	
	
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript">
/*
	Masked Input plugin for jQuery
	Copyright (c) 2007-2013 Josh Bush (digitalbush.com)
	Licensed under the MIT license (http://digitalbush.com/projects/masked-input-plugin/#license)
	Version: 1.3.1
*/
(function(e){function t(){var e=document.createElement("input"),t="onpaste";return e.setAttribute(t,""),"function"==typeof e[t]?"paste":"input"}var n,a=t()+".mask",r=navigator.userAgent,i=/iphone/i.test(r),o=/android/i.test(r);e.mask={definitions:{9:"[0-9]",a:"[A-Za-z]","*":"[A-Za-z0-9]"},dataName:"rawMaskFn",placeholder:"_"},e.fn.extend({caret:function(e,t){var n;if(0!==this.length&&!this.is(":hidden"))return"number"==typeof e?(t="number"==typeof t?t:e,this.each(function(){this.setSelectionRange?this.setSelectionRange(e,t):this.createTextRange&&(n=this.createTextRange(),n.collapse(!0),n.moveEnd("character",t),n.moveStart("character",e),n.select())})):(this[0].setSelectionRange?(e=this[0].selectionStart,t=this[0].selectionEnd):document.selection&&document.selection.createRange&&(n=document.selection.createRange(),e=0-n.duplicate().moveStart("character",-1e5),t=e+n.text.length),{begin:e,end:t})},unmask:function(){return this.trigger("unmask")},mask:function(t,r){var c,l,s,u,f,h;return!t&&this.length>0?(c=e(this[0]),c.data(e.mask.dataName)()):(r=e.extend({placeholder:e.mask.placeholder,completed:null},r),l=e.mask.definitions,s=[],u=h=t.length,f=null,e.each(t.split(""),function(e,t){"?"==t?(h--,u=e):l[t]?(s.push(RegExp(l[t])),null===f&&(f=s.length-1)):s.push(null)}),this.trigger("unmask").each(function(){function c(e){for(;h>++e&&!s[e];);return e}function d(e){for(;--e>=0&&!s[e];);return e}function m(e,t){var n,a;if(!(0>e)){for(n=e,a=c(t);h>n;n++)if(s[n]){if(!(h>a&&s[n].test(R[a])))break;R[n]=R[a],R[a]=r.placeholder,a=c(a)}b(),x.caret(Math.max(f,e))}}function p(e){var t,n,a,i;for(t=e,n=r.placeholder;h>t;t++)if(s[t]){if(a=c(t),i=R[t],R[t]=n,!(h>a&&s[a].test(i)))break;n=i}}function g(e){var t,n,a,r=e.which;8===r||46===r||i&&127===r?(t=x.caret(),n=t.begin,a=t.end,0===a-n&&(n=46!==r?d(n):a=c(n-1),a=46===r?c(a):a),k(n,a),m(n,a-1),e.preventDefault()):27==r&&(x.val(S),x.caret(0,y()),e.preventDefault())}function v(t){var n,a,i,l=t.which,u=x.caret();t.ctrlKey||t.altKey||t.metaKey||32>l||l&&(0!==u.end-u.begin&&(k(u.begin,u.end),m(u.begin,u.end-1)),n=c(u.begin-1),h>n&&(a=String.fromCharCode(l),s[n].test(a)&&(p(n),R[n]=a,b(),i=c(n),o?setTimeout(e.proxy(e.fn.caret,x,i),0):x.caret(i),r.completed&&i>=h&&r.completed.call(x))),t.preventDefault())}function k(e,t){var n;for(n=e;t>n&&h>n;n++)s[n]&&(R[n]=r.placeholder)}function b(){x.val(R.join(""))}function y(e){var t,n,a=x.val(),i=-1;for(t=0,pos=0;h>t;t++)if(s[t]){for(R[t]=r.placeholder;pos++<a.length;)if(n=a.charAt(pos-1),s[t].test(n)){R[t]=n,i=t;break}if(pos>a.length)break}else R[t]===a.charAt(pos)&&t!==u&&(pos++,i=t);return e?b():u>i+1?(x.val(""),k(0,h)):(b(),x.val(x.val().substring(0,i+1))),u?t:f}var x=e(this),R=e.map(t.split(""),function(e){return"?"!=e?l[e]?r.placeholder:e:void 0}),S=x.val();x.data(e.mask.dataName,function(){return e.map(R,function(e,t){return s[t]&&e!=r.placeholder?e:null}).join("")}),x.attr("readonly")||x.one("unmask",function(){x.unbind(".mask").removeData(e.mask.dataName)}).bind("focus.mask",function(){clearTimeout(n);var e;S=x.val(),e=y(),n=setTimeout(function(){b(),e==t.length?x.caret(0,e):x.caret(e)},10)}).bind("blur.mask",function(){y(),x.val()!=S&&x.change()}).bind("keydown.mask",g).bind("keypress.mask",v).bind(a,function(){setTimeout(function(){var e=y(!0);x.caret(e),r.completed&&e==x.val().length&&r.completed.call(x)},0)}),y()}))}})})(jQuery);
</script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" id="acao" name="acao">
	<input type="hidden" id="turid" name="turid">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50"
			           value="<?php if(isset($_SESSION['prova']['turnome'])){ echo ($_SESSION['prova']['turnome']); } ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50"
					   value="<?php if(isset($_SESSION['prova']['turdtabr'])){ echo ($_SESSION['prova']['turdtabr']); } ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" size="50"
					   value="<?php if(isset($_SESSION['prova']['turdtfec'])){ echo ($_SESSION['prova']['turdtfec']); } ?>"></td>
		</tr>
		<tr id="tr_confirmacao" style="display:none;">
			<td><input type="button" id="confirmar" value="Confirmar" style="margin-left:150px" /></td>
			<td><input type="reset" id="cancelar" value="Cancelar" style="margin-left:150px" /></td>
		</tr>
	</table>
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = <<<QUERY
SELECT '<img title="Alterar" src="../imagens/alterar.gif" class="alterar" value="'|| turid ||'" />&nbsp;'
	   || '<img title="Excluir" src="../imagens/excluir.gif" class="excluir" value="'|| turid ||'" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome
QUERY;
/*
$sql = "SELECT '<img title=\"Alterar\" src=\"../imagens/alterar.gif\" class=\"alterar\" value=\"\" />&nbsp;'
			   || '<img title=\"Excluir\" src=\"../imagens/excluir.gif\" class=\"excluir\" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";*/

$cabecalho = array("Ações","Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');
?>

<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input value="Questão anterior" type="button" onclick="navega(1)" id="anterior">
				</div>
				<div style="float:left; width:30%; text-align: center">
					<input value="Salvar" type="button" id="salvar">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>
<script language="javascript" type="text/javascript">
/**
* Função de verificação de campos obrigatórios do formulário.
* Verifica se o campo não estão vazio e emite mensagem de erro se necessário.
* @param string seletor Seletor jquery do campo a ser validado;
* @param string nome Nome do campo para compor a mensagem de erro;
* @return boolean
*/
function estaPreenchido(seletor, nome) {
	var $campo = $(seletor);
	if ('' == $.trim($campo.attr('value'))) {
		alert("O campo '"+nome+"' é obrigatório e não pode ser deixado em branco.");
		$campo.focus().select();
		return false;
  	}
  	return true;
}
$(document).ready(function(){
	// -- Máscara dos campos de data
	$("#turabr").mask("99/99/9999");
	$("#turfec").mask("99/99/9999");
	// -- Validação e envio do formulário para adicionar uma nova turma
	$('#salvar').click(function(){
		if (estaPreenchido('#turdsc', 'Nome da Turma')
			&& estaPreenchido('#turabr', 'Data de abertura da Turma')
			&& estaPreenchido('#turfec', 'Data de fechamento da Turma')) {
			$('#acao').attr('value', 'salvar');
			$('#formulario').submit();
		}
	});
	// -- Envio do formulário para exclusão de uma turma
	$('.excluir').each(function(){
		var $this = $(this);
		$this.css('cursor', 'pointer').click(function(){
			if (confirm('Tem certeza que deseja excluir a turma?')) {
				$('#acao').attr('value', 'excluir');
				$('#turid').attr('value', $(this).attr('value'));
				$('#formulario').submit();
			}
		});
	});
	// -- Envio do formulário para edição de uma turma
	$('.alterar').each(function(){
		var $this = $(this);
		$this.css('cursor', 'pointer').click(function(){
			// -- Controles de edição
			$('#tr_confirmacao').show();
			// -- Nome da turma
			$('#turdsc').attr('value', $this.parent().next().text());
			// -- Data de abertura
			$('#turabr').attr('value', $this.parent().next().next().text());
			// -- Data de fechamento
			$('#turfec').attr('value', $this.parent().next().next().next().text());
			// -- ID da turma
			$('#turid').attr('value', $this.attr('value'));
			// -- Ação
			$('#acao').attr('value', 'alterar');
			// -- Escondendo salvar
			$('#salvar').hide();
		});
	});
	$('#cancelar').click(function(){
		$('#salvar').show();
		$('#tr_confirmacao').hide();
	});
	$('#confirmar').click(function(){
		// -- Validação
		if (estaPreenchido('#turdsc', 'Nome da Turma')
				&& estaPreenchido('#turabr', 'Data de abertura da Turma')
				&& estaPreenchido('#turfec', 'Data de fechamento da Turma')) {
			$('#acao').attr('value', 'alterar');
			$('#formulario').submit();
		}
	});
	
	// -- Mensagem retornada pela ação de adicionar uma nova turma
	<?php if(isset($_SESSION['prova']['salvar'])): ?>
	alert('<?php echo($_SESSION['prova']['salvar']); ?>');
	<?php endif; ?>
});
</script>
<?php unset($_SESSION['prova']); ?>