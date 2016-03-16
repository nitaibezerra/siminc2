<?

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

function definirOrientadoresEstudo(goto) {
	
	divCarregando();	
	jQuery('#goto').val(goto);
	
	document.getElementById('formulario').submit();
}

function validarMaximoOrientadores(obj) {
	<? if(!$db->testa_superuser()) : ?>
	var nummaxorientadores = parseInt(jQuery('#td_numorientadores').html());
	var nummarcados = parseInt(jQuery("[name^='orientador[]']:enabled:checked").length);
	if(nummarcados > nummaxorientadores) {
		alert('Excedeu o número máximo de Orientadores de Estudo');
		obj.checked = false;
	}
	<? endif; ?>
	
}

jQuery(document).ready(function() {
});


</script>

<? 
$professorescontagem = carregarDadosIdentificacaoUsuario(array("iuscodigoinep" => $_SESSION['sismedio']['gestorescola']['lemcodigoinep'],"pflcod" => array(PFL_PROFESSORALFABETIZADOR,PFL_ORIENTADORESTUDO),"iustipoprofessor" => "censo"));
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
	<? if(count($professores) >= MIN_PROFESSORES) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Número Máximo de Orientadores</td>
		<td id="td_numorientadores"><?=calcularOrientadoresEstudo(array('numprofessores' => count($professores))) ?></td>
	</tr>
	<? endif; ?>
	
	<tr>
		<td colspan="2">
		
	<? if(count($professores) >= MIN_PROFESSORES || $db->testa_superuser()) : ?>
	<input type="hidden" name="requisicao" value="definirOrientadoresEstudo">
	<input type="hidden" name="goto" id="goto" value="">
		
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
	<? $is_orientador = $db->pegaUm("SELECT doeid FROM sismedio.definicaoorientadoresestudo WHERE iusd='".$pa['iusd']."' AND doecodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'")?>
	<tr>
		<td width="5%">
		<? if($consulta) : ?>
		
		<?=(($is_orientador)?"<center><img src=../imagens/pju_vinculado.png></center>":"&nbsp;")?>
		
		<? else : ?>
		<center><input type="checkbox" name="orientador[]" value="<?=$pa['iusd'] ?>" onclick="validarMaximoOrientadores(this);" <?=(($is_orientador)?"checked":"")?> ></center>
		<? endif; ?>
		</td>
		<td width="10%"><?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?></td>
		<td width="15%"><?=$pa['iusnome'] ?></td>
		<td width="15%" id="td_img_<?=mascaraglobal($pa['iuscpf'],"###.###.###-##") ?>"><?=$pa['iusemailprincipal'] ?></td>
		<td>
		<?
		
		echo $pa['pfldsc'];
		
		if($pa['pflcod'] == PFL_COORDENADORPEDAGOGICO) {

			echo " ( Bolsista )";
			
		} else {
			
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
	<tr>
		<td colspan="2">Total de cadastrados : <?=count($professores) ?></td>
	</tr>
	<? endif; ?>
	</table>
	
	<? else : ?>
	
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro">A escola não possui o número mínimo de professores (<?=MIN_PROFESSORES ?> professores) para definir um Orientador de Estudo, este será definido posteriomente.</td>
	</tr>
	</table>
	
	<? endif;?>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirprofessores';">
			<? if((count($professores) >= MIN_PROFESSORES || $db->testa_superuser()) && !$consulta) : ?>
			<input type="button" name="salvarorientadoresestudo" value="Salvar" onclick="definirOrientadoresEstudo('sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirorientadores');">
			<input type="button" name="salvarorientadoresestudo2" value="Salvar e Continuar" onclick="definirOrientadoresEstudo('sismedio.php?modulo=principal/escola/escola&acao=A&aba=visualizarcadastramento');">
			<? endif; ?>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=visualizarcadastramento';">
		</td>
	</tr>
</table>
</form>
