<?
ini_set("memory_limit", "2048M");

$perfis = pegaPerfilGeral();

$consulta = false;

$estado = wf_pegarEstadoAtual( $_SESSION['sismedio']['gestorescola']['docid'] );

if((!$db->testa_superuser())) {
	
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		
		if($estado['esdid'] == ESD_ESCOLA_VALIDADO) {
			$consulta = true;
		}
		
	} else {
	
		if($estado['esdid'] != ESD_FLUXOESCOLA_EMELABORACAO) {
			$consulta = true;
		}
	
	}

}

if(!$consulta) $consulta = verificaPermissao();

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirProfessores() {
	window.open('sismedio.php?modulo=principal/escola/inserirprofessores&acao=A','Professores','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
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
		
		var permissao = true;
			
		if(iusd) {
	
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
			calcularNumProfessoresCadastrados();
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

	var conf = confirm('ATENÇÃO: após definir os Professores e Coordenadores Pedagógicos que participarão do curso de formação, é necessário indicar na próxima tela qual(is) deles atuará(ão) como Orientador(es) de Estudo. A quantidade de Orientadores é definida em função do número de profissionais cadastrados, na proporção de 1 Orientador para cada 30 Professores e Coordenadores. Assim, a inclusão ou exclusão de nomes pode afetar a quantidade final de Orientadores. Confirma a inclusão dos Professores/ Coordenadores Pedagógicos?');

	if(conf) {

		divCarregando();	
		jQuery('#goto').val(goto);
		
		document.getElementById('formulario').submit();

	}
}

function calcularNumProfessoresCadastrados() {
	
   	var numprofcadtotal = jQuery("[name^='pflcod['][value='<?=PFL_PROFESSORALFABETIZADOR ?>']").length;
	var numprofcadcpflivre = jQuery("[name^='tipo['][value='cpflivre']").length;
	var numprofcad = numprofcadtotal-numprofcadcpflivre;
	jQuery('#td_numprofessorescadastrados').html(numprofcad);
}

jQuery(document).ready(function() {
<? if($consulta) : ?>
jQuery("[name^='inseriraluno']").hide();
jQuery("[name^='salvarprofessoresalfabetizadores']").hide();
<? endif; ?>
});


</script>

<?
$professorescontagem = carregarDadosIdentificacaoUsuario(array("iuscodigoinep" => $_SESSION['sismedio']['gestorescola']['lemcodigoinep'],"pflcod" => array(PFL_PROFESSORALFABETIZADOR, PFL_ORIENTADORESTUDO),"iustipoprofessor" => "censo")); 
$professores = carregarDadosIdentificacaoUsuario(array("iuscodigoinep" => $_SESSION['sismedio']['gestorescola']['lemcodigoinep'],"pflcod" => array(PFL_PROFESSORALFABETIZADOR,PFL_COORDENADORPEDAGOGICO,PFL_ORIENTADORESTUDO))); 
?>

<form method="post" id="formulario" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Definir Professores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($abaativa); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de professores indicados do Censo Escolar 2013</td>
		<td id="td_numprofessoresprevisto"><?=$_SESSION['sismedio']['gestorescola']['lemdoctotal'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número total de professores cadastrados (bolsistas)</td>
		<td><?=count($professorescontagem) ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número total de professores cadastrados (bolsistas+não bolsistas)</td>
		<td id="td_numprofessorescadastrados"><?=count($professores) ?></td>
	</tr>
	<? if(!$consulta) : ?>
	<tr>
		<td colspan="2"><input type="button" name="inserirprofessores" value="Inserir Professores" onclick="inserirProfessores();"></td>
	</tr>
	<? endif; ?>
	<tr>
		<td colspan="2">
		
	<input type="hidden" name="requisicao" value="inserirProfessoresAlfabetizadores">
	<input type="hidden" name="goto" id="goto" value="">
	<input type="hidden" name="alteracaodados" id="alteracaodados" value="0">
		
	<table class="listagem" id="tabelaprofessoresalfabetizadores" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="5%">&nbsp;</td>
		<td class="SubTituloCentro" width="10%">CPF</td>
		<td class="SubTituloCentro" width="15%">Nome</td>
		<td class="SubTituloCentro" width="15%">E-mail</td>
		<td class="SubTituloCentro" width="10%">Tipo</td>
	</tr>
	<? if($professores) : ?>
	<? foreach($professores as $pa) : ?>
	<tr>
		<td width="5%">
		<? if($consulta) : ?>
		&nbsp;
		<? else : ?>
		<center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer;" id="img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>" onclick="removerProfessorAlfabetizador('<?=$pa['iusd'] ?>', this);"></center>
		<? endif; ?>
		</td>
		<td width="10%"><?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?></td>
		<td width="15%"><?=$pa['iusnome'] ?></td>
		<td width="15%" id="td_img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>"><?=$pa['iusemailprincipal'] ?></td>
		<td>
		<input type="hidden" name="pflcod[<?=$pa['iuscpf'] ?>]" value="<?=$pa['pflcod'] ?>"> <input type="hidden" name="tipo[<?=$pa['iuscpf'] ?>]" value="<?=$pa['iustipoprofessor'] ?>">
		<?
		if($pa['pflcod'] == PFL_COORDENADORPEDAGOGICO) {

			echo $pa['pfldsc']." ( Bolsista )";
			
		} else {
		
			echo $pa['pfldsc'];
			
			if($pa['iustipoprofessor']=="cpflivre") {
				
				echo " ( Não bolsista )";
				
			} else {
				echo " ( Bolsista )";
			}
		
		}
		
		?>
		</td>
	</tr>
	<? endforeach; ?>
	<? endif; ?>
	</table>
		
		
		</td>
	</tr>
	<tr>
		<td colspan="2">Total de cadastrados : <?=count($professores) ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=dadosgestor';">
			<? if(!$consulta) : ?>
			<input type="button" name="salvarorientadoresestudo" value="Salvar" onclick="salvarProfessoresAlfabetizadores('sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirprofessores');">
			<input type="button" name="salvarorientadoresestudo2" value="Salvar e Continuar" onclick="salvarProfessoresAlfabetizadores('sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirorientadores');">
			<? endif; ?>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirorientadores';">
		</td>
	</tr>
</table>
</form>
