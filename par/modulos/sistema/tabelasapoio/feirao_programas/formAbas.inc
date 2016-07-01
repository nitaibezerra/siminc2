<?php 

if($_REQUEST['pfcid']){
	
	$sql = "select 
				* 
			from 
				par.pfcurso 
			where 
				pfcid = {$_REQUEST['pfcid']}";

	$rsCurso = $db->pegaLinha($sql);
	
	if($rsCurso) extract($rsCurso);
}

$prgid = $prgid ? $prgid : $_REQUEST['prgid'];

$linha1 = 'Cadastro de Curso/Aba'; 
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';

monta_titulo($linha1, $linha2);

?>
<script>

	jQuery(function(){

		jQuery("[name=pfcdescricao]").addClass("required");	
			
		jQuery("#formAbas").validate();

		jQuery('#btnFechar').click(function(){
			jQuery( '#dialog-confirm' ).dialog( 'close' );
		});

		jQuery('#btnSalvarAba').click(function(){

			selectAllOptions( document.getElementById( 'pfcidsfuncaoatual' ) );
			selectAllOptions( document.getElementById( 'pfcidsformacao' ) );
			
			jQuery('[name=requisicao]').val('salvarAba');
			jQuery('#formAbas').submit();
		});
		
	});

	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
</script>
<!-- 
  funid integer,
  pfcidsfuncaoatual character varying(500),
  pfcidsformacao character varying(500),
 -->
<form name="formAbas" id="formAbas" method="post" action="">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="pfcid" value="<?php echo $pfcid ? $pfcid : ''; ?>" />
	<input type="hidden" name="prgid" value="<?php echo $prgid ? $prgid : ''; ?>" />
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Descrição</td>
			<td><?php echo campo_texto('pfcdescricao', 'S', 'S', '', 65, 150, '', ''); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Título</td>
			<td><?php echo campo_texto('pfctitulo', 'N', 'S', '', 65, 150, '', ''); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Qtd. de Registros</td>
			<td><?php echo campo_texto('pfclimiteregistros', 'N', 'S', '', 6, 3, '###', ''); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">Função entidade</td>
			<td>
				<?php
				$tpdid = $tpdid ? $tpdid : 41;
				
				$sql = "(select 
							'' as codigo, 
							'Selecione...' as descricao)
						union all
						(select 
							funid::varchar as codigo, 
							fundsc as descricao 
						from 
							entidade.funcao 
						where 
							funstatus = 'A'
						order by
							fundsc)";
				
				$db->monta_combo('funid', $sql, 'S', '', '', '', '', '', 'N'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Orientações</td>
			<td><?php echo campo_textarea('pfcorientacoes', 'N', 'S', '', '100%', 8, ''); ?></td>		
		</tr>		
		<tr>
			<td class="subtituloDireita">Mensagem de sucesso</td>
			<td><?php echo campo_textarea('pfcmsgsucesso', 'N', 'S', '', '100%', 2, 4000); ?></td>		
		</tr>
		<?php 
		// Função atual
		$stSql = "select
					pffid as codigo,
					pffdescricao as descricao
				from 
					par.pffuncao
				where 
					pffstatus = 'A'
				order by 
					pffdescricao";

		$stSqlCarregados = "";
		if($pfcidsfuncaoatual){
			
			$stSqlCarregados = "select
									pffid as codigo,
									pffdescricao as descricao
								from 
									par.pffuncao
								where 
									pffstatus = 'A'
								and
									pffid in ({$pfcidsfuncaoatual})
								order by 
									pffdescricao";
		}
		
		mostrarComboPopup( 'Função atual:', 'pfcidsfuncaoatual',  $stSql, $stSqlCarregados, 'Selecione a(s) Função(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		
		// Formação
		$stSql = "select
					tfoid as codigo,
					tfodsc as descricao
				from 
					public.tipoformacao
				where 
					tfostatus = 'A'
				order by 
					tfodsc";

		$stSqlCarregados = "";
		if($pfcidsformacao){
			
			$stSqlCarregados = "select
									tfoid as codigo,
									tfodsc as descricao
								from 
									public.tipoformacao
								where 
									tfostatus = 'A'
								and 
									tfoid in ({$pfcidsformacao})";
		}
		
		mostrarComboPopup( 'Formação:', 'pfcidsformacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Formação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		?>
		<tr>
			<td class="subtituloDireita">Scripts</td>
			<td><?php echo campo_textarea('pfcscripts', 'N', 'S', '', '100%', 2, 4000); ?></td>		
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btnSalvarAba" />
				<input type="button" value="Fechar" id="btnFechar"/>
			</td>
		</tr>
	</table>
</form>
