<?
ini_set("memory_limit", "2048M");

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function inserirProfessores() {
	window.open('sismedio.php?modulo=principal/escola/inserirprofessores&acao=A','Professores','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas&turid='+turid;
}


function excluirAlunoTurma(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurma&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
		carregarAlunosTurma();
	}
}

function removerProfessorAlfabetizador(iusd, obj) {
	var conf = confirm("Deseja realmente excluir este professor?");
	
	if(conf) {
		divCarregando();
	
		if(iusd) {
			var permissao = true;		
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: 'requisicao=removerProfessorAlfabetizador&iusd='+iusd,
		   		async: false,
		   		success: function(html){
		   			if(html!='') {
		   				alert(html);
		   				permissao = false;
		   			}
		   		}
			});
		
		}
		
		if(permissao) {
			var tabela = obj.parentNode.parentNode.parentNode.parentNode;
			var linha = obj.parentNode.parentNode.parentNode;
			tabela.deleteRow(linha.rowIndex);
		}
		
		divCarregado();
	}
	

}

function salvarProfessoresAlfabetizadores(goto) {
	
	var invalidaanexo=false;
	jQuery("[name^='anexoportaria[']").each(function() {
		if(this.value=='') {
			invalidaanexo=true;
		}
	});
	
	if(invalidaanexo) {
		alert('É obrigatório anexar documentação comprobatória');
		return false;	
	}

	divCarregando();	
	jQuery('#goto').val(goto);
	
	document.getElementById('formulario').submit();
}

function calcularNumProfessoresCadastrados() {
	var numprofcad = parseInt(jQuery('#td_numprofessorescadastrados').html())+1;
	jQuery('#td_numprofessorescadastrados').html(numprofcad);
}

jQuery(document).ready(function() {
<? if($consulta) : ?>
jQuery("[name^='inseriraluno']").hide();
jQuery("[name^='salvarprofessoresalfabetizadores']").hide();
<? endif; ?>
});


</script>

<? $professores = carregarDadosIdentificacaoUsuario(array("iuscodigoinep" => $_SESSION['sismedio']['gestorescola']['lemcodigoinep'],"pflcod" => array(PFL_PROFESSORALFABETIZADOR,PFL_COORDENADORPEDAGOGICO))); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="3">Cadastramento dos Professores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td colspan="2"><? echo carregarOrientacao($abaativa); ?></td>
	</tr>
	<tr>
		<td colspan="2" valign="top">
		
		<? 
		
		$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, i.iusemailprincipal, p.pfldsc FROM sismedio.identificacaousuario i 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				WHERE i.iusstatus='A' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.iuscodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'";
		
		$cabecalho = array("CPF","Nome","Email","Perfil");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
		
		?>
		</td>
		<td width="5%" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sismedio']['gestorescola']['docid'], array() );
		?>
		</td>
	</tr>
	<? if(count($professores) >= MIN_PROFESSORES) : ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Definição dos Orientadores de Estudos</td>
	</tr>
	<tr>
		<td colspan="2" valign="top">
		
		<? 
		
		$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, i.iusemailprincipal, p.pfldsc 
				FROM sismedio.identificacaousuario i 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				INNER JOIN sismedio.definicaoorientadoresestudo d ON d.iusd = i.iusd 
				WHERE i.iusstatus='A' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."','".PFL_ORIENTADORESTUDO."') AND i.iuscodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."' AND d.doecodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'";
		
		$cabecalho = array("CPF","Nome","Email","Perfil");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
		
		?>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td colspan="2">
			<input type="button" value="Anterior" onclick="divCarregando(); window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirorientadores';">
		</td>
	</tr>
	
</table>

