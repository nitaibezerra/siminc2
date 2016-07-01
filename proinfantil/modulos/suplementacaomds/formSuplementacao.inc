<?php

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if($_REQUEST['alaid']){
		$sql = "update proinfantil.mdsalunoatendidopbf set 
					alaquantidade = '{$_REQUEST['alaquantidade']}'
				where
					alaid = {$_REQUEST['alaid']}";
	}else{
		$sql = "insert into proinfantil.mdsalunoatendidopbf
					(alaquantidade, mdsid)
				values
					('{$_REQUEST['alaquantidade']}', '{$_REQUEST['mdsid']}');";
	}	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('suplementacaomds/formSuplementacao');
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

$abacod_tela = 57559;

$arMnuid = array();
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Cadastro de Suplementações MDS', $titulo2);
cabecalhoMds();

$sql = "select 
			alt.alaid,
			alt.alaquantidade,
			mds.mdsid
		from 
			proinfantil.suplementacaomds mds
		left join 
			proinfantil.mdsalunoatendidopbf alt on mds.mdsid = alt.mdsid
		where 
			mds.entcodent = '{$_SESSION['proinfantil']['mds']['entcodent']}'";

$rs = $db->pegaLinha($sql);

if($rs){
	extract($rs);
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function(){
		$('#btn_salvar').click(function(){
			if($('[name=alaquantidade]').val() == ''){
				alert('O campo quantidade é obrigatório!');
				$('[name=alaquantidade]').focus();
				return false;
			}	
			$('[name=requisicao]').val('salvaDados');
			$('#formulario').submit();
		});
	});
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="alaid" value="<?php echo $alaid ? $alaid : '' ?>" />
	<input type="hidden" name="mdsid" value="<?php echo $mdsid ? $mdsid : '' ?>" />
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Quantidade de alunos</td>
			<td>
				<?php echo campo_texto('alaquantidade', 'S', 'S', '', 20, '', '###.###', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btn_salvar" />
			</td>
		</tr>
	</table>
</form>	