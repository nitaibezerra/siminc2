<?php 

if($_REQUEST['tapid']){
	
	$sql = "select 
				* 
			from 
				par.pftermoadesaoprograma 
			where 
				tapid = {$_REQUEST['tapid']}";

	$rsTermo = $db->pegaLinha($sql);
	
	if($rsTermo) extract($rsTermo);
}

$prgid = $prgid ? $prgid : $_REQUEST['prgid'];

$linha1 = 'Cadastro de Termo/Pacto'; 
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
//$linha2 = '';

monta_titulo($linha1, $linha2);

$larguraTextarea = '95%';
$laguraLabel = 100;

?>
<style>
.ui-accordion-content{
	height: 410px;
	
}
</style>
<script>
	
	jQuery(function(){

		jQuery('#btnFechar').click(function(){
			jQuery( '#dialog-confirm' ).dialog( 'close' );
		});

		jQuery( "#accordion" ).accordion();

		jQuery('#btnSalvarTermo').click(function(){

			var prgid = jQuery('[name=prgid]').val();
			var tprcod = jQuery('[name=tprcod]').val();
			var tapid = jQuery('[name=tapid]').val();

			if(tinyMCE.get('taptermo').getContent() == ''){
				alert('O campo Termo de Adesão é obrigatório!');
				jQuery("#accordion").accordion("activate", 1);
				tinyMCE.get('taptermo').focus();
				return false;				
			}
			
			if(jQuery('[name=tapano]').val() == ''){
				alert('O campo Ano de Referência é obrigatório!');
				jQuery("#accordion").accordion("activate", 3);
				jQuery('[name=tapano]').focus();
				return false;
			}
			
			jQuery.ajax({
					url		: 'par.php?modulo=sistema/tabelasapoio/feirao_programas/formPrograma&acao=A',
					type	: 'post',
					data	: 'requisicao=verificaEsfera&prgid='+prgid+'&tprcod='+tprcod,
					success	: function(e){						
						if(e && !tapid){
							alert('Já existe um termo cadastrado para a esfera '+e+' neste programa!');
						}else{
							jQuery('[name=requisicao]').val('salvarTermo');
							jQuery('#formTermo').submit();
						}
					}
			});			
		});
	});
	
</script>
<form name="formTermo" id="formTermo" method="post" action="">	
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="tapid" value="<?php echo $tapid ? $tapid : ''; ?>" />
	<input type="hidden" name="prgid" value="<?php echo $prgid ? $prgid : ''; ?>" />

	<center>
		<div id="accordion" style="width:95%;text-align:left;">
		    <h3><a href="#">Texto do Pacto</a></h3>
		    <div style="padding:0px;margin:0px;">
				<?php echo campo_textarea('tappacto', 'N', 'S', '', '92%', 23, ''); ?>
		    </div>
		    <h3><a href="#">Texto do Termo de Adesão&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"></a></h3>
		    <div style="padding:0px;margin:0px;">		
				<?php echo campo_textarea('taptermo', 'N', 'S', '', '92%', 23, ''); ?>
		    </div>
		    <h3><a href="#">Mensagens</a></h3>
		    <div style="padding:0px;margin:0px;">
		    	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		    		<tr>
						<td class="subtituloDireita">Mensagem de aceite pacto</td>
						<td><?php echo campo_textarea('tapmsgaceitepacto', 'N', 'S', '', $larguraTextarea, 4, 3000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Mensagem de não aceite pacto</td>
						<td><?php echo campo_textarea('tapmsgnaoaceitepacto', 'N', 'S', '', $larguraTextarea, 4, 3000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Mensagem de aceite termo</td>
						<td><?php echo campo_textarea('tapmsgaceitetermo', 'N', 'S', '', $larguraTextarea, 4, 3000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Mensagem de não aceite termo</td>
						<td><?php echo campo_textarea('tapmsgnaoaceitetermo', 'N', 'S', '', $larguraTextarea, 4, 3000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Mensagem de e-mail após enviar para pré-análise</td>
						<td><?php echo campo_textarea('tapconteudoemail', 'N', 'S', '', $larguraTextarea, 4, 3000); ?></td>		
					</tr>					
		    	</table>
		    </div>
		    <h3><a href="#">Outros&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"></a></h3>
		    <div style="padding:0px;margin:0px;">
		    	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		    		<tr>
						<td class="subtituloDireita">Ano de Referência</td>
						<td>
							<?php echo campo_texto('tapano', 'S', 'S', '', 10, 4, '####', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Esfera</td>
						<td>
							<?php
							$sql = array(
								array('codigo'=>'','descricao'=>'Todas'),
								array('codigo'=>'1','descricao'=>'Estadual'),
								array('codigo'=>'2','descricao'=>'Municipal')
							); 
							$db->monta_combo('tprcod', $sql, 'S', '', '', '', '', '', 'N'); 
							?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Fluxo Workflow</td>
						<td>
							<?php
							$tpdid = $tpdid ? $tpdid : 41;
							$sql = "select 
										tpdid as codigo, 
										tpddsc as descricao 
									from 
										workflow.tipodocumento 
									where 
										sisid = ".SISID_PAR;
							$db->monta_combo('tpdid', $sql, 'S', '', '', '', '', '', 'N'); 
							?>
						</td>
					</tr>						
					<tr>
						<td class="subtituloDireita">Título Cabeçalho Adesão</td>
						<td>
							<?php echo campo_texto('taptituloadesao', 'N', 'S', '', 65, 255, '', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Instruções</td>
						<td><?php echo campo_textarea('tapinstrucao', 'N', 'S', '', $larguraTextarea, 4, 10000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Mensagem de bem vindo</td>
						<td><?php echo campo_textarea('tapmsg', 'N', 'S', '', $larguraTextarea, 4, 1000); ?></td>		
					</tr>
					<tr>
						<td class="subtituloDireita">Redirecionamento após Adesão</td>
						<td>
							<?php echo campo_texto('tapredireciona', 'N', 'S', '', 65, 255, '', ''); ?>
						</td>
					</tr>
				</table>
		    </div>
		</div>
		<p>
		<input type="button" value="Salvar" id="btnSalvarTermo" />
		<input type="button" value="Fechar" id="btnFechar" />
		</p>
	</center>

<!--  
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">		
		<tr>
			<td colspan="2" class="subtituloCentro">Textos Pacto</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Pacto</td>
			<td><?php echo campo_textarea('tappacto', 'N', 'S', '', '100%', 8, 10000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Mensagem de aceite pacto</td>
			<td><?php echo campo_textarea('tapmsgaceitepacto', 'N', 'S', '', '100%', 4, 1000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Mensagem de não aceite pacto</td>
			<td><?php echo campo_textarea('tapmsgnaoaceitepacto', 'N', 'S', '', '100%', 4, 1000); ?></td>		
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Textos Termo de Adesão</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Termo de Adesão</td>
			<td><?php echo campo_textarea('taptermo', 'N', 'S', '', '100%', 8, 10000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Mensagem de aceite termo</td>
			<td><?php echo campo_textarea('tapmsgaceitetermo', 'N', 'S', '', '100%', 4, 1000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Mensagem de não aceite termo</td>
			<td><?php echo campo_textarea('tapmsgnaoaceitetermo', 'N', 'S', '', '100%', 4, 1000); ?></td>		
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Configurações</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Ano de Referência</td>
			<td>
				<?php echo campo_texto('tapano', 'S', 'S', '', 10, 4, '####', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Título Cabeçalho Adesão</td>
			<td>
				<?php echo campo_texto('taptituloadesao', 'S', 'S', '', 65, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Instruções</td>
			<td><?php echo campo_textarea('tapinstrucao', 'N', 'S', '', '100%', 4, 10000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Mensagem de bem vindo</td>
			<td><?php echo campo_textarea('tapmsg', 'N', 'S', '', '100%', 4, 1000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Redirecionamento após Adesão</td>
			<td>
				<?php echo campo_texto('tapredireciona', 'S', 'S', '', 65, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" />
				<input type="button" value="Fechar" id="btnFechar"/>
			</td>
		</tr>
	</table>
-->
</form>
