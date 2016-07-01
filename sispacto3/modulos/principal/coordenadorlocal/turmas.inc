<?
ini_set("memory_limit", "2048M");
$consulta = verificaPermissao();

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['docidturma'] );

if($estado['esdid'] == ESD_FECHADO_TURMA) {
	$consulta = true;
}

$perfis = pegaPerfilGeral();

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sispacto3.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas&turid='+turid;
}


function excluirAlunoTurma(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurma&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
		carregarAlunosTurma();
	}
}

function atualizarSeriesProfessorAlfabetizador(iusd, serie) {
	alert(iusd);
	alert(serie);
}

function exibirSeriesProfessorAlfabetizador(iusd, iusserieprofessor) {
	var html = '';
	html += '<form method="post" id="formulario_serie"><input type="hidden" name="requisicao" value="atualizarSerieProfessorAlfabetizador"><input type="hidden" name="iusd" value="'+iusd+'">';
	html += '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	<? if($_SERIE_TURMA) : ?>
		html += '<tr>';
		html += '<td>';
		<? foreach($_SERIE_TURMA as $sercod => $serdsc) : ?>
			var checked = iusserieprofessor.indexOf("<?=$sercod ?>");
			if(checked >= 0) {
				checked = 'checked';
			} else {
				checked = '';
			}
			
			html += '<input type="checkbox" name="sercod[]" '+checked+' value="<?=$sercod ?>"> <?=$serdsc ?><br>';
			
		<? endforeach; ?>
		
		html += '</td>';
		html += '</tr>';
	<? endif; ?>
	html += '<tr>';
	html += '<td colspan="2" align="center"><input type="button" name="atualizar" value="Atualizar" onclick="jQuery(\'#formulario_serie\').submit();"></td>';
	html += '</tr>';
	html += '</table>';
	html += '</form>';

	jQuery("#modalImportarProfessoresSispactoAnterior").html(html);

	jQuery("#modalImportarProfessoresSispactoAnterior").dialog({
        draggable:true,
        resizable:true,
        width: 300,
        height: 300,
        modal: true,
     	close: function(){} 
    });

	
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


function importarProfessoresSispactoAnterior(turid,iuscpf) {

	divCarregando();
	
	ajaxatualizar('requisicao=carregarProfessoresalfabetizadoresSispacto2014&turid='+turid+'&iuscpf='+iuscpf,'modalImportarProfessoresSispactoAnterior');
	
	jQuery("#modalImportarProfessoresSispactoAnterior").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 600,
        modal: true,
     	close: function(){} 
    });

    divCarregado();
	
}

function importarProfessores(turid, iusd) {
	var conf = confirm('A importação só ocorrerá caso o Orientador tenha participado do SISPACTO 2014. Confirma a importação?');

	if(conf) {
		window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=importarProfessores2013';
	}
	
}

jQuery(document).ready(function() {
<? if($consulta) : ?>
jQuery("[name^='inseriraluno']").hide();
jQuery("[name^='salvarprofessoresalfabetizadores']").hide();
jQuery("[name^='importaraluno']").hide();
<? endif; ?>
});


</script>

<div id="modalImportarProfessoresSispactoAnterior" style="display:none;" ></div>

<?
$ar = array("estuf" 	  => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'],
			"muncod" 	  => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'],
			"dependencia" => $_SESSION['sispacto3']['esfera']);

$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
$professoesalfabetizados = carregarDadosIdentificacaoUsuario(array("picid" => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'],"pflcod" => PFL_PROFESSORALFABETIZADOR));
?>

<form method="post" id="formulario" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas"); ?></td>
	</tr>
	<? if($_REQUEST['turid']) : ?>
	<? $turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid'])); ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><a href="sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas">Lista de turmas</a> >> <?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da turma</td>
		<td><?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientador responsável</td>
		<td><?=$turma['iusnome'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de turmas indicadas do Censo Escolar 2013</td>
		<td id="td_numprofessoresprevisto"><?=$totalalfabetizadores['total'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número total de professores cadastrados</td>
		<td id="td_numprofessorescadastrados"><?=count($professoesalfabetizados) ?></td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" name="inseriraluno" value="Inserir Professores" onclick="abrirAlunosTurma('<?=$turma['turid'] ?>');"> <input type="button" name="importaraluno" value="Importar Professores" onclick="importarProfessoresSispactoAnterior('<?=$turma['turid'] ?>','<?=$turma['iuscpf'] ?>');"></td>
	</tr>
	<tr>
		<td colspan="2">
		
	<input type="hidden" name="requisicao" value="inserirProfessoresAlfabetizadores">
	<input type="hidden" name="goto" id="goto" value="">
	<input type="hidden" name="turid" id="turid" value="<?=$turma['turid'] ?>">
	<input type="hidden" name="alteracaodados" id="alteracaodados" value="0">
		
	<table class="listagem" id="tabelaprofessoresalfabetizadores" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="5%">&nbsp;</td>
		<td class="SubTituloCentro" width="10%">CPF</td>
		<td class="SubTituloCentro" width="15%">Nome</td>
		<td class="SubTituloCentro" width="15%">E-mail</td>
		<td class="SubTituloCentro" width="10%">Ano/Etapa</td>
		<td class="SubTituloCentro" width="10%">Documento comprobatório</td>
		<td class="SubTituloCentro" width="10%">Tipo</td>
	</tr>
	<? $professoesalfabetizados = carregarDadosIdentificacaoUsuario(array("picid" => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'],"pflcod" => PFL_PROFESSORALFABETIZADOR,"turid" => $_REQUEST['turid'])); ?>
	<? if($professoesalfabetizados) : ?>
	<? foreach($professoesalfabetizados as $pa) : ?>
	<tr>
		<td width="5%">
		<? if($consulta) : ?>
		&nbsp;
		<? else : ?>
		<center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer;" id="img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>" onclick="removerProfessorAlfabetizador(<?=$pa['iusd'] ?>, this);"></center>
		<? endif; ?>
		</td>
		<td width="10%"><?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?></td>
		<td width="15%"><?=$pa['iusnome'] ?></td>
		<td width="15%" id="td_img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>"><?=$pa['iusemailprincipal'] ?></td>
		<td width="10%" id="td2_img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>" title="<?=$pa['iusserieprofessor'] ?>">
		<?
		$cods = explode(";",$pa['iusserieprofessor']);
		if($cods) {
			foreach($cods as $cod) {
				echo $_SERIE_TURMA[$cod]."&nbsp";
			}
		}
		if($db->testa_superuser() || in_array(PFL_COORDENADORLOCAL,$perfis)) {
			echo '<img src="../imagens/arrow_v.png" style="cursor:pointer;" align="absmiddle" onclick="exibirSeriesProfessorAlfabetizador('.$pa['iusd'].',\''.$pa['iusserieprofessor'].'\');">';
		}
		?>
		</td>
		<td>
		<?
		if($pa['iustipoprofessor']=="cpflivre") {
			
			$sql = "SELECT arqnome||'.'||arqextensao as arquivo, a.arqid, p.ponid
					FROM public.arquivo a 
					INNER JOIN sispacto3.portarianomeacao p ON a.arqid = p.arqid 
					WHERE iusd='".$pa['iusd']."'";
			
			$arquivo = $db->pegaLinha($sql);
			
			if($arquivo) {
				echo "<img src=\"../imagens/anexo.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=".$arquivo['arqid']."';\"> ".$arquivo['arquivo'];
				if(!$consulta) echo " <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=removerAnexoPortaria&ponid=".$arquivo['ponid']."';\">";
			} else {
				echo "<input type=\"file\" name=\"anexoportaria[".mascaraglobal($pa['iuscpf'],"###.###.###-##")."]\">";
			}
			
		} else {
			echo "&nbsp;";
		}
		
		?>
		</td>
		<td>
		<?
		if($pa['iustipoprofessor']=="cpflivre") {
			
			echo "Não bolsista";
			
		} else {
			echo "Bolsista";
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
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Voltar para lista" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas';">
			<input type="button" name="salvarprofessoresalfabetizadores" value="Salvar" onclick="salvarProfessoresAlfabetizadores('sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas&turid=<?=$turma['turid'] ?>');">
		</td>
	</tr>
	<? else : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de turmas indicadas do Censo Escolar 2014</td>
		<td><?=$totalalfabetizadores['total'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número total de professores cadastrados</td>
		<td><?=count($professoesalfabetizados) ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<table width="100%">
		<tr>
			<td valign="top"><?=carregarTurmasOrientadores(array("picid"=>$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO)) ?></td>
			<td valign="top">
			<?
			/* Barra de estado atual e ações e Historico */
			wf_desenhaBarraNavegacao( $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['docidturma'], array('picid' => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'], 'uncid'=> false) );
			?>
			</td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
		</td>
	</tr>
	<? endif; ?>
</table>
</form>
