<?php
verificaCodigoTermoSessao();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script>
	$(function(){
		$('.inserirCro').click(function(){
			var tamanho = $('#cronograma tr:last').attr('id').replace('tr_','');
			if( tamanho == 'titulo' ){
				tamanho = 0;
			}
			tamanho = parseInt(tamanho)+1;
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=novaLinhaCronograma&cod="+tamanho,
				async: false,
				success: function(msg){
						$('#cronograma tr:last').after(msg);
				}
			});
		});
	});
</script>
<?php 
if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();
?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom"><center><b>Cronograma de Desembo</b></center></td>
		</tr>
	</table>
	<table id="cronograma" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="2" align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >N° da Parcela</td>
			<td class="subtitulocentro" >Ação</td>
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >Valor</td>
			<td class="subtitulocentro" ></td>
		</tr>
		<?php listaCronograma(); ?>
	</table>
	<?php if($habilitaInserir == 'S'):?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="2" cellspacing="1" id="">
			<tr>
				<td colspan="2">
					<a href="#" class="inserirCro"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos"> Inserir Cronograma
					</a>
				</td>
			</tr>
		</table>
	<?php endif; ?>
	
	
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<input type="button" id="voltarAba" value="Voltar" class="navegar" style="display:none;"/>						
				<?php			
				
				if($habilitaSalvar == 'S'): 
					if( $_SESSION['elabrev']['termo']['aba'] != 'tramite' && $_SESSION['elabrev']['termo']['aba'] != 'gerapdf'){
						
					?>
					<?php if( $_SESSION['elabrev']['termo']['aba'] != '' && $_SESSION['elabrev']['termo']['aba'] != 'proponente'){?>
						<input type="button" id="volta" value="Gravar e Voltar" class="voltar"/> 
					<?php }?>
					<?php if($habilita == 'S'): ?>
						<input type="button" id="salva" value="Gravar" class="salvar"/>
					<?php if( $_SESSION['elabrev']['termo']['aba'] != 'tramite' ){?>
						<input type="button" id="continua" value="Gravar e Continuar" class="continuar"/>
					<?php }?>
					<?php endif; ?>
				<?php }elseif( $_SESSION['elabrev']['termo']['aba'] == 'tramite' && $_SESSION['elabrev']['termo']['aba'] != 'gerapdf'){ 
				
						$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
						if( $_SESSION['elabrev']['termo']['aba'] == 'tramite' && $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD && (in_array(PERFIL_COORDENADOR_SEC, $perfis))){
							echo '<input type="button" id="salva" value="Gravar" class="salvar">';
						}
						?>
						<input type="button" value="Voltar" class="voltarT" onclick="history.back();">
				<?php }elseif( $_SESSION['elabrev']['termo']['aba'] == 'gerapdf'){ ?>
						<input type="button" value="Gerar PDF" class="gerarPDF"/>
				<?php }
				endif;
				?>
				<input type="button" id="proximaAba" value="Continuar" class="navegar" style="display:none;"/>
			</td>
		</tr>
	</table>
</form>