<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function(){
		$('.salvar').click(function(){
			if($('[name=cpf]').val() == ''){
				alert('O campo CPF é obrigatório!');
				$('[name=cpf]').focus();
				return false;
			}
			if($('[name=nome]').val() == ''){
				alert('O campo NOME é obrigatório!');
				$('[name=nome]').focus();
				return false;
			}
			if($('[name=email]').val() == ''){
				alert('O campo E-MAIL é obrigatório!');
				$('[name=email]').focus();
				return false;
			}
			if($('[name=ug]').val() == ''){
				alert('Não existe nenhuma UG selecionada!');
				$('[name=ug]').focus();
				return false;
			}
			$('#req').val('salvarDados');
			$('#formulario').submit();
		});

		$('.voltar').click(function(){
			window.location = 'elabrev.php?modulo=sistema/tabapoio/termoCooperacao/listaRepresentanteLegal&acao=A';
		});
	});
</script>

<?php
 
if($_REQUEST['req'] == 'salvarDados'){
	
	if($_POST) extract($_POST);
	
	$sql = "select ug from elabrev.representantelegal where ug = '{$_REQUEST['ug']}'";
	$ug = $db->pegaUm($sql);
	
	$cpf = $cpf ? str_replace(array('.','-'), '', $cpf) : '';
	$cpfanterior = $cpfanterior ? str_replace(array('.','-'), '', $cpfanterior) : '';
	
	if($ug){
		$sql = "
		update elabrev.representantelegal set
			cpf 	= '{$cpf}',
			nome	= '{$nome}',
			email 	= '{$email}'
		where ug = '{$ug}';
		";
	}else{
		$sql = "
			insert into elabrev.representantelegal (cpf, nome, email, ug, status)
			values ('{$cpf}','{$nome}','{$email}','{$ug}','A');
		";
	}
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('sistema/tabapoio/termoCooperacao/formRepresentanteLegal', '&cpf='.$cpf.'&ug='.$ug);
	}else{
		$db->insucesso('Falha na operação!', '&cpf='.$cpfanterior.'&ug='.$ug, 'sistema/tabapoio/termoCooperacao/formRepresentanteLegal');
	}
}

require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo( "Representante Legal", 'Editar Registro' );

$sql = "
SELECT 
	uni.unicod,
	uni.unidsc,
	ung.ungdsc,
	rpl.ug,
	rpl.cpf,
	rpl.nome,
	rpl.email
FROM elabrev.representantelegal rpl
JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
LEFT JOIN public.unidade uni ON uni.unicod = ung.unicod
WHERE rpl.cpf = '{$_REQUEST['cpf']}' AND rpl.ug = '{$_REQUEST['ug']}'
";

$rs = $db->pegaLinha($sql);

if($rs) extract($rs);

$cpf = $cpf ? formatar_cpf($cpf) : '';

?>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="ug" name="ug" value="<?php echo $ug; ?>" />
	<input type="hidden" id="cpfanterior" name="cpfanterior" value="<?php echo $cpf; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?php if($unicod): ?>			
			<tr>
				<td class="subtituloDireita">Unidade Orçamentária</td>
				<td><?php echo $unicod; ?> - <?php echo strtoupper($unidsc); ?></td>
			</tr>
		<?php endif; ?>		
		<tr>
			<td class="subtituloDireita">Unidade Gestora</td>
			<td><?php echo $ug; ?> - <?php echo strtoupper($ungdsc); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">CPF</td>
			<td><?php echo campo_texto('cpf', 'S', 'S', '', 25, '', '###.###.###-##', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome</td>
			<td><?php echo campo_texto('nome', 'S', 'S', '', 45, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">E-mail</td>
			<td><?php echo campo_texto('email', 'S', 'S', '', 45, '', '', ''); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" class="salvar" />
				<input type="button" value="Voltar" class="voltar" />
			</td>
		</tr>
	</table>
</form>