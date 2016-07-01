<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<?php

verificaCodigoTermoSessao();

if ($emCadastramentoCorrecao && in_array(UO_EQUIPE_TECNICA, $perfis) && $boEmAlteracao) {
	$habilita 				= 'S';
	$habilita_Und_G 		= 'S';
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';
	$habilita_Und_G 		= 'S';
	$habilita_pro_con 		= 'S';
}

if ((in_array(UO_EQUIPE_TECNICA, $perfis)) ||
    (in_array(PERFIL_CGSO, $perfis)) ||
    (in_array(PERFIL_AREA_TECNICA_FNDE, $perfis))) {
    $habilitaSalvar 		= 'S';
}

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

$habilitaInserir = quemPodeAnexarArquivo($habilitaInserir);
?>
<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">	
	<input type="hidden" id="tcpid" 	name="tcpid" 	value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" 		name="req" 		value="" />
	<input type="hidden" id="aba" 		name="aba" 		value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" 	name="acaoAba" 	value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="arptipo" name="arptipo" value="A">
	
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="arqidSubTituloDireita" valign="bottom"><center><b>Anexos</b></center></td>
		</tr>
	</table>
	<table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >Arquivo</td>
			<td class="subtitulocentro" >Descrição</td>
            <td class="subtitulocentro" >Usuário</td>
            <td class="subtitulocentro" >Data</td>
			<td class="subtitulocentro" >Visualizar arquivo</td>
		</tr>
		<?php listaAnexos('A'); ?>
	</table>

	<?php if($habilitaInserir == 'S'):?>
		<script type="text/javascript">
			$(function(){
				//anexoPrincipal
				
				if($('.excluirLinha')){				
					$.each($('.excluirLinha'), function(i,v){
						if(i==0){
							$('[name="arpdsc[]"]').val(1);										
							$('.anexoPrincipal, .voltar, .continuar').hide();
							$('.navegar').show();
						}
					});
				}
			});
		</script>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
					<td colspan="3">
						<a href="#" class="inserirAnex"> 
							<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Demais Arquivos
						</a>
					</td>
			</tr>
			<!--  <tr id="tr_<?=$request['cod'] ?>" class="anexoPrincipal">  -->
			<tr id="tr_1" class="anexoPrincipal">
				<td align="center"> Projeto básico ou termo de referência
					<!--  <input type="hidden" name="anexoCod[]" value="<?=$request['cod'] ?>"/>  -->
					<input type="hidden" name="anexoCod[]" value="1"/>
					<!-- <input type="file" name="anexo_<?=$request['cod'] ?>"/> -->
					<input type="file" name="anexo_1"/>
				</td>
				<td align="center">
					<?=campo_textarea( 'arpdsc[]', 'S', 'S', '', 150, 2, 255, '', '', '', '', '', $dados['arpdsc'], array("id"=>"arpdsc")) ?>
				</td>
			</tr>
			
		</table>
	<?php endif; ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?>
					<input type="button" id="<?php echo in_array(UO_EQUIPE_TECNICA, $perfis) ? 'previsao' : 'parecertecnico' ?>" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="tramite" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>