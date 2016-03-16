<?php if(!empty($mvbid)): ?>

	<script type="text/javascript">
	
		/**
		 * Redireciona para a pagina de exclusão
		 * @param id - Chave para exclusao
		 * @return void
		 */
		function redirecionaExcluir(id,pagina){
	
			if($('uendidnova')){
				var uendidnova = $('uendidnova').getValue();
			}
			

			if(confirm('Deseja excluir o registro da Movimentação ?')){

				if(pagina == 'editarMovimentacaoLocalidade'){
					location.href = "?modulo=principal/bensmovimentacao/excluirRgp&acao=A&rgpid=" + id + "&mvbid=<?php echo $mvbid; ?>&mvbtipo=<?php echo $mvbtipo; ?>&pagina="+pagina+"&uendidnova="+uendidnova;
				}
				else{
					location.href = "?modulo=principal/bensmovimentacao/excluirRgp&acao=A&rgpid=" + id + "&mvbid=<?php echo $mvbid; ?>&mvbtipo=<?php echo $mvbtipo; ?>&pagina="+pagina+"&uendidnova="+uendidnova;
				}
			}
	
		}

		/**
		 * Redireciona para a página de edição (Quando Movimentnação Geral)
		 * @param id - Chave para edição
		 * @return void
		 */
		function redirecionaEditar(id){
			if($('mvbstatus')){
				var mvbstatus = $('mvbstatus').getValue();
			}

			if(mvbstatus == 'I'){
				location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoGeral&acao=A&rgpid=' + id + '&mvbid=<?php echo $mvbid; ?>';
			}
			else if(mvbstatus == 'A'){
				location.href = '?modulo=principal/bensmovimentacao/editarMovimentacaoGeral&acao=A&rgpid=' + id + '&mvbid=<?php echo $mvbid; ?>';
			}
		}
	
	</script>

	<div class="tituloLista"> Patrim&ocirc;nios da Movimenta&ccedil;&atilde;o </div>
		
	<?php 
		
		if($mvbtipo == 'G')
			$oBensMovimentacaoRgp->listarRgpsGeral($mvbid,$pagina);
		else
			$oBensMovimentacaoRgp->listarRgps($mvbid,$pagina);
		 
	?>
	
<?php endif; ?>